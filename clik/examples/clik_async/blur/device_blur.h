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

#ifndef _CLIK_EXAMPLES_CLIK_ASYNC_BLUR_H
#define _CLIK_EXAMPLES_CLIK_ASYNC_BLUR_H

#include "constants.h"
#include "kernel_if.h"

__kernel void copy_and_pad_hor(__global uint *src, __global uint *dst,
                               exec_state_t *item);
__kernel void pad_vert(__global uint *dst, exec_state_t *item);
__kernel void blur(__global uint *src, __global uint *dst, exec_state_t *item);

// Argument structs for kernels

typedef struct {
  __global uint *src;
  __global uint *dst;
} copy_and_pad_hor_args;

typedef struct {
  __global uint *buf;
} pad_vert_args;

typedef struct {
  __global uint *src;
  __global uint *dst;
} blur_args;

#endif  // _CLIK_EXAMPLES_CLIK_ASYNC_BLUR_H
