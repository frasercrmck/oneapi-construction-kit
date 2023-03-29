// Copyright (C) Codeplay Software Limited. All Rights Reserved.

#ifndef COMPILER_UTILS_METADATA_H_INCLUDED
#define COMPILER_UTILS_METADATA_H_INCLUDED

#include <llvm/ADT/Optional.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/Metadata.h>
#include <multi_llvm/optional_helper.h>

#include "vectorization_factor.h"

namespace llvm {
class Function;
class Module;
}  // namespace llvm

namespace compiler {
namespace utils {

/// @brief Describes the state of vectorization on a function/loop.
struct VectorizationInfo {
  /// @brief The VectorizationFactor. A scalar value if unvectorized.
  VectorizationFactor vf;
  /// @brief The dimension along which vectorization took place.
  unsigned simdDimIdx;
  /// @brief Whether or not the function/loop was vector-predicated.
  bool IsVectorPredicated;
};

/// @brief Encodes metadata indicating vectorization failure to a kernel, along
/// with the the vectorization factor and dimension that failed.
///
/// @param[in] f Function in which to encode the link.
/// @param[in] info Vectorization info serving as the key.
void encodeVectorizationFailedMetadata(llvm::Function &f,
                                       const VectorizationInfo &info);

/// @brief Encodes the vectorization metadata linking the original kernel to a
/// vectorized one, using the vectorization factor and dimension as the key.
///
/// @param[in] origF Original function in which to encode the link.
/// @param[in] vectorizedF Vectorized function to link.
/// @param[in] info Vectorization factor serving as the key.
void linkOrigToVeczFnMetadata(llvm::Function &origF,
                              llvm::Function &vectorizedF,
                              const VectorizationInfo &info);

/// @brief Encodes the vectorization metadata linking a vectorized kernel back
/// to its original one, using the vectorization factor and dimension as the
/// key.
///
/// @param[in] vectorizedF Vectorized function in which to encode the link.
/// @param[in] origF Original function to link.
/// @param[in] info Vectorization factor serving as the key.
void linkVeczToOrigFnMetadata(llvm::Function &vectorizedF,
                              llvm::Function &origF,
                              const VectorizationInfo &info);

using LinkMetadataResult = std::pair<llvm::Function *, VectorizationInfo>;

/// @brief Decodes the metadata linking a kernel to its vectorized variant.
///
/// @param[in] f Function for which to decode the metadata.
/// @param[out] factors unordered vector of recovered vectorization links.
///
/// @return true on success, false if there is no vectorization metadata for the
/// function.
bool parseOrigToVeczFnLinkMetadata(
    llvm::Function &f, llvm::SmallVectorImpl<LinkMetadataResult> &factors);

/// @brief Decodes the metadata linking a vectorized kernel back to its
/// original one.
///
/// @param[in] f Function for which to decode the metadata.
///
/// @return On success, a pair containing a pointer to the original kernel
/// function and the vectorization factor used as the key. The original
/// function may be null. On decoding failure, llvm::None.
multi_llvm::Optional<LinkMetadataResult> parseVeczToOrigFnLinkMetadata(
    llvm::Function &f);

/// @brief Drops "base" vectorization metadata from a function, if present.
///
/// @param[in] f Function to drop metadata from.
void dropVeczOrigMetadata(llvm::Function &f);

/// @brief Drops "derived" vectorization metadata from a function, if present.
///
/// @param[in] f Function to drop metadata from.
void dropVeczDerivedMetadata(llvm::Function &f);

/// @brief Encodes metadata indicating the various components that constitute a
/// kernel function wrapped with the HandleBarriers pass.
///
/// @param[in] f Function in which to encode the metadata.
/// @param[in] simdDimIdx The dimension (0,1,2) along which vectorization took
/// place.
/// @param[in] mainInfo VectorizationInfo used on the 'main' work-item
/// iterations.
/// @param[in] tailInfo VectorizationInfo used on the tail iterations, if
/// applicable.
///
/// Note that a 'tail' is defined as the work done to execute work-items not
/// covered by the 'main' body. Therefore an unvectorized kernel should expect
/// a scalar 'main' vectorization factor and no 'tail' (rather than the other
/// way round).

/// Some examples of *typical* usage:
/// 1. An unvectorized kernel will encode a scalar VF for the main iterations
/// and nothing for the tail ones.
/// 2. A vectorized kernel will encode vectorization factor for its main
/// iterations. If it handles the case in which the local work-group size does
/// not evenly divide the vectorization factor, it will encode how it manages
/// the tail iterations. This is *typically* with a series of scalar
/// iterations, encoded in tailVF.
/// 3. Vector-predicated kernels with no tails will encode the *maximum* VF used
/// for the main loop, with no tail iterations.
///
/// This metadata is encoded as:
/// define void @foo() !codeplay_ca_wrapper !X
/// !X = { !Main, !Tail }
/// !Main = { i32 mKnownMin, i32 mIsScalable, i32 simdDimIdx, i32 mIsVP }
/// if tailVF is None:
///   !Tail = {}
/// else
///   !Tail = { i32 tKnownMin, i32 tIsScalable, i32 simdDimIdx, i32 tIsVP }
void encodeWrapperFnMetadata(llvm::Function &f,
                             const VectorizationInfo &mainInfo,
                             multi_llvm::Optional<VectorizationInfo> tailInfo);

/// @brief Decodes the metadata describing a wrapped kernel's loop structure.
///
/// @param[in] f Function for which to decode the metadata.
///
/// @return On success, a pair containing the VectorizationInfo for the main
/// loop(s) and the (optional) VectorizationInfo info for the tail loop(s). On
/// decoding failure, llvm::None.
multi_llvm::Optional<
    std::pair<VectorizationInfo, multi_llvm::Optional<VectorizationInfo>>>
parseWrapperFnMetadata(llvm::Function &f);

/// @brief Copies function metadata from one function to another.
///
/// @param[in] fromF Function from which to copy the metadata.
/// @param[in] toF Function onto which to copy the metadata.
/// @param[in] includeDebug Whether or not to copy debug function metadata.
void copyFunctionMetadata(llvm::Function &fromF, llvm::Function &toF,
                          bool includeDebug = false);

/// @brief Encodes information about a function's local work group size as
/// metadata.
///
/// @param[in] f Function in which to encode the metadata.
/// @param[in] localSizes array of size information to encode.
void encodeLocalSizeMetadata(llvm::Function &f,
                             const std::array<uint64_t, 3> &localSizes);

/// @brief Retrieves information about a function's local sizes via metadata.
///
/// @param[in] f Function from which to decode the metadata
/// @returns The local size array if presernt, else `llvm::None`
multi_llvm::Optional<std::array<uint64_t, 3>> getLocalSizeMetadata(
    const llvm::Function &f);

/// @brief Drops all !mux_scheduled_fn metadata from a function.
void dropSchedulingParameterMetadata(llvm::Function &f);

/// @brief Retrieves the indices of scheduling parameters from the function.
llvm::SmallVector<int, 4> getSchedulingParameterFunctionMetadata(
    const llvm::Function &f);

/// @brief Sets scheduling-parameter metadata on the given function
void setSchedulingParameterFunctionMetadata(llvm::Function &f,
                                            llvm::ArrayRef<int> idxs);

/// @brief Sets module-level metadata describing the set of scheduling
/// parameters.
void setSchedulingParameterModuleMetadata(llvm::Module &m,
                                          llvm::ArrayRef<std::string> names);

/// @brief Retrieves module-level metadata describing the set of scheduling
/// parameters or nullptr.
llvm::NamedMDNode *getSchedulingParameterModuleMetadata(const llvm::Module &m);

/// @brief If the given function parameter index is considered a scheduling
/// parameter, it returns the corresponding index into the target's list of
/// scheduling parameters.
///
/// It uses !mux_scheduled_fn metadata for this check.
multi_llvm::Optional<unsigned> isSchedulingParameter(const llvm::Function &f,
                                                     unsigned idx);

/// @brief Extracts the required work group size from a kernel's function
/// metadata.
///
/// @param[in] f Kernel for extraction.
///
/// @return The work group size or llvm::None if there is no such metadata.
multi_llvm::Optional<std::array<uint64_t, 3>> parseRequiredWGSMetadata(
    const llvm::Function &f);

/// @brief Extracts the required work group size from an opencl.kernels subnode,
/// which is similar to the function metadata, but the size is stored under
/// different indices than on a function.
///
/// @param[in] node Kernel's subnode for extraction.
///
/// @return The work group size or llvm::None if there is no such metadata.
multi_llvm::Optional<std::array<uint64_t, 3>> parseRequiredWGSMetadata(
    const llvm::MDNode &node);

/// @brief Extracts the maximum work dimension from a kernel's function
/// metadata
///
/// @param[in] f Kernel for extraction.
///
/// @return The maximum work dimension or llvm::None if there is no such
/// metadata.
multi_llvm::Optional<uint32_t> parseMaxWorkDimMetadata(const llvm::Function &f);

/// @brief Describes the state of vectorization on a function/loop.
struct KernelInfo {
  explicit KernelInfo(llvm::StringRef name) : Name(name) {}
  /// @brief The function name
  std::string Name;
  /// @brief The required work-group size. Optional.
  multi_llvm::Optional<std::array<uint64_t, 3>> ReqdWGSize;
};

/// @brief Helper function to populate a list of kernels and associated
/// information from a module.
///
/// @param m Module to retrieve kernels from
/// @param results List of kernel info parsed from metadata or taken from the
/// module.
void populateKernelList(llvm::Module &m,
                        llvm::SmallVectorImpl<KernelInfo> &results);

}  // namespace utils
}  // namespace compiler

#endif  // COMPILER_UTILS_METADATA_H_INCLUDED
