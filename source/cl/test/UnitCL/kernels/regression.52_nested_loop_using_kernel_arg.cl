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

__kernel void nested_loop_using_kernel_arg(__global int *src, __global int *dst) {
  int result = 0;
  for (int i = 0; i < 1; i++) {
    for (int j = 0; j < 1; j++) {
      result = *src;
    }
    src++;
  }

  barrier(CLK_LOCAL_MEM_FENCE);
  dst[get_global_id(0)] = result;
}
