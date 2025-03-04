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

#include <UnitVK.h>

// https://www.khronos.org/registry/vulkan/specs/1.0/xhtml/vkspec.html#vkEnumerateDeviceLayerProperties

class EnumerateDeviceLayerPropertiesTest : public uvk::PhysicalDeviceTest {
  // Empty class
};

TEST_F(EnumerateDeviceLayerPropertiesTest, Default) {
  uint32_t propertyCount;

  ASSERT_EQ_RESULT(VK_SUCCESS,
              vkEnumerateDeviceLayerProperties(physicalDevice, &propertyCount,
                                               nullptr));
}

// VK_INCOMPLETE
// Is a possible return from this function, but is untestable as
// the result may change even with the same parameters.
//
// VK_ERROR_OUT_OF_HOST_MEMORY
// Is a possible return from this function but is untestable
// as it doesn't take an allocator as a parameter.
//
// VK_ERROR_OUT_OF_DEVICE_MEMORY
// Is a possible return from this function, but is untestable
// due to the fact that we can't currently access device memory
// allocators to mess with.
//
// VK_ERROR_INITIALIZATION_FAILED
// Is a possible return from this function, but is untestable
// because it can't actually be generated using only api calls.
