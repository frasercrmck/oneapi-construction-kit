; Copyright (C) Codeplay Software Limited
;
; Licensed under the Apache License, Version 2.0 (the "License") with LLVM
; Exceptions; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
;     https://github.com/codeplaysoftware/oneapi-construction-kit/blob/main/LICENSE.txt
;
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
; WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
; License for the specific language governing permissions and limitations
; under the License.
;
; SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

; RUN: %veczc -vecz-scalable -w 4 -S < %s | %filecheck %s

target triple = "spir64-unknown-unknown"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

declare spir_func i64 @_Z13get_global_idj(i32)

declare spir_func i32 @_Z28sub_group_scan_inclusive_muli(i32)
declare spir_func float @_Z28sub_group_scan_inclusive_mulf(float)

declare spir_func i32 @_Z28sub_group_scan_exclusive_muli(i32)
declare spir_func float @_Z28sub_group_scan_exclusive_mulf(float)

declare spir_func i32 @_Z28sub_group_scan_inclusive_andi(i32)
declare spir_func i32 @_Z27sub_group_scan_inclusive_ori(i32)
declare spir_func i32 @_Z28sub_group_scan_inclusive_xori(i32)
declare spir_func i1 @_Z36sub_group_scan_inclusive_logical_andb(i1)
declare spir_func i1 @_Z35sub_group_scan_inclusive_logical_orb(i1)
declare spir_func i1 @_Z36sub_group_scan_inclusive_logical_xorb(i1)

; CHECK-LABEL: @__vecz_nxv4_reduce_scan_incl_mul_i32(
; CHECK: call <vscale x 4 x i32> @__vecz_b_sub_group_scan_inclusive_mul_u5nxv4j(<vscale x 4 x i32> %{{.*}})
define spir_kernel void @reduce_scan_incl_mul_i32(ptr addrspace(1) %in, ptr addrspace(1) %out) {
entry:
  %call = tail call spir_func i64 @_Z13get_global_idj(i32 0)
  %arrayidx = getelementptr inbounds i32, ptr addrspace(1) %in, i64 %call
  %0 = load i32, ptr addrspace(1) %arrayidx, align 4
  %call1 = tail call spir_func i32 @_Z28sub_group_scan_inclusive_muli(i32 %0)
  %arrayidx2 = getelementptr inbounds i32, ptr addrspace(1) %out, i64 %call
  store i32 %call1, ptr addrspace(1) %arrayidx2, align 4
  ret void
}

; CHECK-LABEL: @__vecz_nxv4_reduce_scan_excl_mul_i32(
; CHECK: call <vscale x 4 x i32> @__vecz_b_sub_group_scan_exclusive_mul_u5nxv4j(<vscale x 4 x i32> %{{.*}})
define spir_kernel void @reduce_scan_excl_mul_i32(ptr addrspace(1) %in, ptr addrspace(1) %out) {
entry:
  %call = tail call spir_func i64 @_Z13get_global_idj(i32 0)
  %arrayidx = getelementptr inbounds i32, ptr addrspace(1) %in, i64 %call
  %0 = load i32, ptr addrspace(1) %arrayidx, align 4
  %call1 = tail call spir_func i32 @_Z28sub_group_scan_exclusive_muli(i32 %0)
  %arrayidx2 = getelementptr inbounds i32, ptr addrspace(1) %out, i64 %call
  store i32 %call1, ptr addrspace(1) %arrayidx2, align 4
  ret void
}

; CHECK-LABEL: @__vecz_nxv4_reduce_scan_incl_mul_f32(
; CHECK: call <vscale x 4 x float> @__vecz_b_sub_group_scan_inclusive_mul_u5nxv4f(<vscale x 4 x float> %{{.*}})
define spir_kernel void @reduce_scan_incl_mul_f32(ptr addrspace(1) %in, ptr addrspace(1) %out) {
entry:
  %call = tail call spir_func i64 @_Z13get_global_idj(i32 0)
  %arrayidx = getelementptr inbounds float, ptr addrspace(1) %in, i64 %call
  %0 = load float, ptr addrspace(1) %arrayidx, align 4
  %call1 = tail call spir_func float @_Z28sub_group_scan_inclusive_mulf(float %0)
  %arrayidx2 = getelementptr inbounds float, ptr addrspace(1) %out, i64 %call
  store float %call1, ptr addrspace(1) %arrayidx2, align 4
  ret void
}

; CHECK-LABEL: @__vecz_nxv4_reduce_scan_excl_mul_f32(
; CHECK: call <vscale x 4 x float> @__vecz_b_sub_group_scan_exclusive_mul_u5nxv4f(<vscale x 4 x float> %{{.*}})
define spir_kernel void @reduce_scan_excl_mul_f32(ptr addrspace(1) %in, ptr addrspace(1) %out) {
entry:
  %call = tail call spir_func i64 @_Z13get_global_idj(i32 0)
  %arrayidx = getelementptr inbounds float, ptr addrspace(1) %in, i64 %call
  %0 = load float, ptr addrspace(1) %arrayidx, align 4
  %call1 = tail call spir_func float @_Z28sub_group_scan_exclusive_mulf(float %0)
  %arrayidx2 = getelementptr inbounds float, ptr addrspace(1) %out, i64 %call
  store float %call1, ptr addrspace(1) %arrayidx2, align 4
  ret void
}

; CHECK-LABEL: @__vecz_nxv4_reduce_scan_incl_and_i32(
; CHECK: call <vscale x 4 x i32> @__vecz_b_sub_group_scan_inclusive_and_u5nxv4j(<vscale x 4 x i32> %{{.*}})
define spir_kernel void @reduce_scan_incl_and_i32(ptr addrspace(1) %in, ptr addrspace(1) %out) {
entry:
  %call = tail call spir_func i64 @_Z13get_global_idj(i32 0)
  %arrayidx = getelementptr inbounds i32, ptr addrspace(1) %in, i64 %call
  %0 = load i32, ptr addrspace(1) %arrayidx, align 4
  %call1 = tail call spir_func i32 @_Z28sub_group_scan_inclusive_andi(i32 %0)
  %arrayidx2 = getelementptr inbounds i32, ptr addrspace(1) %out, i64 %call
  store i32 %call1, ptr addrspace(1) %arrayidx2, align 4
  ret void
}

; CHECK-LABEL: @__vecz_nxv4_reduce_scan_incl_or_i32(
; CHECK: call <vscale x 4 x i32> @__vecz_b_sub_group_scan_inclusive_or_u5nxv4j(<vscale x 4 x i32> %{{.*}})
define spir_kernel void @reduce_scan_incl_or_i32(ptr addrspace(1) %in, ptr addrspace(1) %out) {
entry:
  %call = tail call spir_func i64 @_Z13get_global_idj(i32 0)
  %arrayidx = getelementptr inbounds i32, ptr addrspace(1) %in, i64 %call
  %0 = load i32, ptr addrspace(1) %arrayidx, align 4
  %call1 = tail call spir_func i32 @_Z27sub_group_scan_inclusive_ori(i32 %0)
  %arrayidx2 = getelementptr inbounds i32, ptr addrspace(1) %out, i64 %call
  store i32 %call1, ptr addrspace(1) %arrayidx2, align 4
  ret void
}

; CHECK-LABEL: @__vecz_nxv4_reduce_scan_incl_xor_i32(
; CHECK: call <vscale x 4 x i32> @__vecz_b_sub_group_scan_inclusive_xor_u5nxv4j(<vscale x 4 x i32> %{{.*}})
define spir_kernel void @reduce_scan_incl_xor_i32(ptr addrspace(1) %in, ptr addrspace(1) %out) {
entry:
  %call = tail call spir_func i64 @_Z13get_global_idj(i32 0)
  %arrayidx = getelementptr inbounds i32, ptr addrspace(1) %in, i64 %call
  %0 = load i32, ptr addrspace(1) %arrayidx, align 4
  %call1 = tail call spir_func i32 @_Z28sub_group_scan_inclusive_xori(i32 %0)
  %arrayidx2 = getelementptr inbounds i32, ptr addrspace(1) %out, i64 %call
  store i32 %call1, ptr addrspace(1) %arrayidx2, align 4
  ret void
}

; CHECK-LABEL: @__vecz_nxv4_reduce_scan_incl_logical_and(
; CHECK: call <vscale x 4 x i1> @__vecz_b_sub_group_scan_inclusive_and_u5nxv4b(<vscale x 4 x i1> %{{.*}})
define spir_kernel void @reduce_scan_incl_logical_and(ptr addrspace(1) %in, ptr addrspace(1) %out) {
entry:
  %call = tail call spir_func i64 @_Z13get_global_idj(i32 0)
  %arrayidx = getelementptr inbounds i32, ptr addrspace(1) %in, i64 %call
  %0 = load i32, ptr addrspace(1) %arrayidx, align 4
  %1 = trunc i32 %0 to i1
  %call1 = tail call spir_func i1 @_Z36sub_group_scan_inclusive_logical_andb(i1 %1)
  %arrayidx2 = getelementptr inbounds i32, ptr addrspace(1) %out, i64 %call
  %2 = zext i1 %call1 to i32
  store i32 %2, ptr addrspace(1) %arrayidx2, align 4
  ret void
}

; CHECK-LABEL: @__vecz_nxv4_reduce_scan_incl_logical_or(
; CHECK: call <vscale x 4 x i1> @__vecz_b_sub_group_scan_inclusive_or_u5nxv4b(<vscale x 4 x i1> %{{.*}})
define spir_kernel void @reduce_scan_incl_logical_or(ptr addrspace(1) %in, ptr addrspace(1) %out) {
entry:
  %call = tail call spir_func i64 @_Z13get_global_idj(i32 0)
  %arrayidx = getelementptr inbounds i32, ptr addrspace(1) %in, i64 %call
  %0 = load i32, ptr addrspace(1) %arrayidx, align 4
  %1 = trunc i32 %0 to i1
  %call1 = tail call spir_func i1 @_Z35sub_group_scan_inclusive_logical_orb(i1 %1)
  %arrayidx2 = getelementptr inbounds i32, ptr addrspace(1) %out, i64 %call
  %2 = zext i1 %call1 to i32
  store i32 %2, ptr addrspace(1) %arrayidx2, align 4
  ret void
}

; CHECK-LABEL: @__vecz_nxv4_reduce_scan_incl_logical_xor(
; CHECK: call <vscale x 4 x i1> @__vecz_b_sub_group_scan_inclusive_xor_u5nxv4b(<vscale x 4 x i1> %{{.*}})
define spir_kernel void @reduce_scan_incl_logical_xor(ptr addrspace(1) %in, ptr addrspace(1) %out) {
entry:
  %call = tail call spir_func i64 @_Z13get_global_idj(i32 0)
  %arrayidx = getelementptr inbounds i32, ptr addrspace(1) %in, i64 %call
  %0 = load i32, ptr addrspace(1) %arrayidx, align 4
  %1 = trunc i32 %0 to i1
  %call1 = tail call spir_func i1 @_Z36sub_group_scan_inclusive_logical_xorb(i1 %1)
  %arrayidx2 = getelementptr inbounds i32, ptr addrspace(1) %out, i64 %call
  %2 = zext i1 %call1 to i32
  store i32 %2, ptr addrspace(1) %arrayidx2, align 4
  ret void
}

!opencl.ocl.version = !{!0}

!0 = !{i32 3, i32 0}
