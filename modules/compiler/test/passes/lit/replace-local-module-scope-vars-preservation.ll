; Copyright (C) Codeplay Software Limited. All Rights Reserved.

; RUN: %muxc --passes replace-module-scope-vars,verify -S %s  | %filecheck %s

target triple = "spir64-unknown-unknown"
target datalayout = "e-p:64:64:64-m:e-i64:64-f80:128-n8:16:32:64-S128"

; CHECK: %localVarTypes = type { i16 }

@a = internal addrspace(3) global i16 undef, align 2
@b = internal addrspace(3) global [4 x float] undef, align 4
@c = internal addrspace(3) global i32 addrspace(3)* undef
@d = internal addrspace(3) global i32 undef

; CHECK: define internal spir_kernel void @add(ptr addrspace(1) readonly %in, ptr byval(i32) %out, ptr [[STRUCTPTR:%.*]]) #[[ATTRS:[0-9]+]] !dummy [[MD:\![0-9]+]] {
; CHECK: [[GEP:%.*]] = getelementptr inbounds %localVarTypes, ptr [[STRUCTPTR]], i32 0, i32 0
; CHECK: [[ADDR:%.*]] = addrspacecast ptr [[GEP]] to ptr addrspace(3)
; CHECK: %ld = load i16, ptr addrspace(3) [[ADDR]], align 2
; CHECK: ret void
; CHECK: }

; Check we've carried over parameter attributes and metadata
; CHECK: define spir_kernel void @foo.mux-local-var-wrapper(ptr addrspace(1) readonly %in, ptr byval(i32) %out) #[[WRAPPER_ATTRS:[0-9]+]] !dummy [[MD]] {
; The alignment of this alloca must be the maximum alignment of the new struct
; CHECK: [[ALLOCA:%.*]] = alloca %localVarTypes, align 2
; Check that when we call the original kernel we preserve the various attributes
; CHECK: call spir_kernel void @add(ptr addrspace(1) readonly %in, ptr byval(i32) %out, ptr [[ALLOCA]])
define spir_kernel void @add(ptr addrspace(1) readonly %in, ptr byval(i32) %out) #0 !dummy !0 {
  %ld = load i16, ptr addrspace(3) @a, align 2
  ret void
}

; CHECK: define spir_kernel void @baz.mux-local-var-wrapper(ptr addrspace(1) readonly %in, ptr byval(i32) %out) #[[BAZ_WRAPPER_ATTRS:[0-9]+]] {
define spir_kernel void @baz(ptr addrspace(1) readonly %in, ptr byval(i32) %out) #1 {
  %ld = load i16, ptr addrspace(3) @a, align 2
  ret void
}

attributes #0 = { noinline "mux-base-fn-name"="foo" "mux-kernel" "test"="2" }
attributes #1 = { "mux-kernel" }

!0 = !{!"dummy"}

; Check we haven't added alwaysinline, given that the kernel is marked
; noinline.
; Check also that we've preserved the original function's attributes
; ... but check that only the new kernel has the 'kernel' attribute
; CHECK-DAG: attributes #[[ATTRS]] = { noinline "mux-base-fn-name"="foo" "test"="2" }
; CHECK-DAG: attributes #[[WRAPPER_ATTRS]] = { noinline nounwind "mux-base-fn-name"="foo" "mux-kernel" "test"="2" }
; CHECK-DAG: attributes #[[BAZ_WRAPPER_ATTRS]] = { nounwind "mux-base-fn-name"="baz" "mux-kernel" }

; Check that both kernels have the dummy metadata
; CHECK-DAG: [[MD]] = !{!"dummy"}
