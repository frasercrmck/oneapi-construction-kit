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

/// @file
/// Host's LLVM pass machinery interface.

#ifndef HOST_PASSES_MACHINERY_H_INCLUDED
#define HOST_PASSES_MACHINERY_H_INCLUDED

#include <base/base_pass_machinery.h>

namespace llvm {
class TargetMachine;
}

namespace host {

class HostPassMachinery final : public compiler::BaseModulePassMachinery {
 public:
  HostPassMachinery(llvm::LLVMContext &Ctx, llvm::TargetMachine *TM,
                    const compiler::utils::DeviceInfo &Info,
                    compiler::utils::BuiltinInfoAnalysis::CallbackFn BICallback,
                    bool verifyEach, compiler::utils::DebugLogging debugLogging,
                    bool timePasses)
      : compiler::BaseModulePassMachinery(Ctx, TM, Info, BICallback, verifyEach,
                                          debugLogging, timePasses) {}
  ~HostPassMachinery() = default;
  void addClassToPassNames() override;

  void registerPassCallbacks() override;

  void registerPasses() override;

  void printPassNames(llvm::raw_ostream &) override;
};

}  // namespace host

#endif
