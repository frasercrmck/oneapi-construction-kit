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

#include "common.h"

struct muxDestroyCommandBufferTest : public DeviceTest {};

INSTANTIATE_DEVICE_TEST_SUITE_P(muxDestroyCommandBufferTest);

TEST_P(muxDestroyCommandBufferTest, Default) {
  mux_command_buffer_t command_buffer;

  ASSERT_SUCCESS(
      muxCreateCommandBuffer(device, callback, allocator, &command_buffer));
  muxDestroyCommandBuffer(device, command_buffer, allocator);
}

TEST_P(muxDestroyCommandBufferTest, InvalidDevice) {
  mux_command_buffer_t command_buffer;

  ASSERT_SUCCESS(
      muxCreateCommandBuffer(device, callback, allocator, &command_buffer));
  muxDestroyCommandBuffer(nullptr, command_buffer, allocator);
  // Actually destroy the command buffer.
  muxDestroyCommandBuffer(device, command_buffer, allocator);
}

TEST_P(muxDestroyCommandBufferTest, InvalidCommandBuffer) {
  muxDestroyCommandBuffer(device, nullptr, allocator);
}

TEST_P(muxDestroyCommandBufferTest, InvalidAllocator) {
  mux_command_buffer_t command_buffer;

  ASSERT_SUCCESS(
      muxCreateCommandBuffer(device, callback, allocator, &command_buffer));
  muxDestroyCommandBuffer(device, command_buffer, {nullptr, nullptr, nullptr});
  // Actually destroy the command buffer.
  muxDestroyCommandBuffer(device, command_buffer, allocator);
}
