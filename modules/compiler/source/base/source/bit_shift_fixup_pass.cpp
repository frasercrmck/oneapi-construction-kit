// Copyright (C) Codeplay Software Limited
//
// Licensed under the Apache License, Version 2.0 (the "License") with LLVM
// Exceptions; you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://github.com/codeplaysoftware/oneapi-construction-kit/blob/main/LICENSE.txt
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations
// under the License.
//
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <base/bit_shift_fixup_pass.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <multi_llvm/multi_llvm.h>

namespace {
/// @brief       Try to extract an integer constant from a value and return it.
/// This works for both scalar constants and splatted vector constants.
/// @type        Static function.
/// @param[in]   val - Value to extract an integer constant from.
/// @param[out]  outVal - Extracted integer constant on success.
/// @return      true on success, false otherwise.
static bool ExtractIntegerConstant(llvm::Value *val, uint64_t &outVal) {
  // If the value is a splat vector, use the splat value
  if (llvm::ConstantVector *VC = llvm::dyn_cast<llvm::ConstantVector>(val)) {
    if (llvm::Constant *splatVal = VC->getSplatValue()) {
      return ExtractIntegerConstant(splatVal, outVal);
    }
  }

  // Extract the numeric value
  if (llvm::ConstantInt *V = llvm::dyn_cast<llvm::ConstantInt>(val)) {
    outVal = V->getZExtValue();
    return true;
  }

  outVal = 0;
  return false;
}
}  // namespace

llvm::PreservedAnalyses compiler::BitShiftFixupPass::run(
    llvm::Function &F, llvm::FunctionAnalysisManager &) {
  bool modified = false;
  for (auto &BB : F) {
    for (auto &I : BB) {
      // Skip non-shift instructions
      if (!llvm::Instruction::isShift(I.getOpcode())) {
        continue;
      }

      // Get each operand of the shift instruction
      llvm::Value *src = I.getOperand(0);
      llvm::Value *amount = I.getOperand(1);

      // Figure out the width of the first operand and the mask for the modulo
      unsigned srcWidth = src->getType()->getScalarSizeInBits();
      unsigned srcMask = srcWidth - 1;

      // Skip non-oversized constant shift amounts.
      if (llvm::ConstantInt *V = llvm::dyn_cast<llvm::ConstantInt>(amount)) {
        if (V->getZExtValue() < srcWidth) {
          continue;
        }
      }

      // Skip shift instructions that already have the modulo applied.
      // This happens if the SPIR module was generated by Clang.
      if (llvm::BinaryOperator *amountOp =
              llvm::dyn_cast<llvm::BinaryOperator>(amount)) {
        if (amountOp->getOpcode() == llvm::Instruction::And) {
          // Extract the mask value from the modulo operation
          uint64_t andMask = 0;
          bool extracted =
              ExtractIntegerConstant(amountOp->getOperand(1), andMask);

          // Skip the shift instruction if the modulo uses the correct mask
          if (extracted && (andMask == srcMask)) {
            continue;
          }
        }
      }

      // Apply the mask to perform the modulo operation.
      llvm::IRBuilder<> builder(&I);
      llvm::Constant *srcMaskVal =
          llvm::ConstantInt::get(amount->getType(), srcMask);
      llvm::Value *modulo = builder.CreateAnd(amount, srcMaskVal);

      // Replace the second shift operand with the masked value.
      I.setOperand(1, modulo);
      modified = true;
    }
  }

  return modified ? llvm::PreservedAnalyses::allInSet<llvm::CFGAnalyses>()
                  : llvm::PreservedAnalyses::all();
}
