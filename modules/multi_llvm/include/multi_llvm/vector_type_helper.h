// Copyright (C) Codeplay Software Limited. All Rights Reserved.
#ifndef MULTI_LLVM_VECTOR_TYPE_HELPER_H_INCLUDED
#define MULTI_LLVM_VECTOR_TYPE_HELPER_H_INCLUDED

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/TypeSize.h>
#include <multi_llvm/llvm_version.h>

namespace multi_llvm {

// The functions defined below are common functions to allow us to generically
// get VectorType information from a base Type class, due to either deprecation
// or removal of these in LLVM 11 (result of scalable/fixed vectors separation)

inline llvm::Type *getVectorElementType(llvm::Type *ty) {
  assert(llvm::isa<llvm::VectorType>(ty) && "Not a vector type");
  return llvm::cast<llvm::VectorType>(ty)->getElementType();
}
inline llvm::Type *getVectorElementType(const llvm::Type *ty) {
  assert(llvm::isa<llvm::VectorType>(ty) && "Not a vector type");
  return llvm::cast<llvm::VectorType>(ty)->getElementType();
}

inline unsigned getVectorNumElements(llvm::Type *ty) {
  assert(ty->getTypeID() == llvm::Type::FixedVectorTyID &&
         "Not a fixed vector type");
  return llvm::cast<llvm::FixedVectorType>(ty)
      ->getElementCount()
      .getFixedValue();
}
inline unsigned getVectorNumElements(const llvm::Type *ty) {
  assert(ty->getTypeID() == llvm::Type::FixedVectorTyID &&
         "Not a fixed vector type");
  return llvm::cast<llvm::FixedVectorType>(ty)
      ->getElementCount()
      .getFixedValue();
}

inline llvm::ElementCount getVectorElementCount(llvm::Type *ty) {
  return llvm::cast<llvm::VectorType>(ty)->getElementCount();
}
inline llvm::ElementCount getVectorElementCount(const llvm::Type *ty) {
  return llvm::cast<llvm::VectorType>(ty)->getElementCount();
}
}  // namespace multi_llvm

#endif  // MULTI_LLVM_VECTOR_TYPE_HELPER_H_INCLUDED
