// Copyright (C) Codeplay Software Limited. All Rights Reserved.
// CL_STD: 3.0
__kernel void exchange_local_float(__global float *in_out_buffer,
                             __global float *desired_buffer,
                             __global float *output_buffer,
                             volatile __local atomic_float *local_buffer) {
  uint gid = get_global_id(0);
  uint lid = get_local_id(0);

  atomic_init(local_buffer + lid, in_out_buffer[gid]);
  output_buffer[gid] =
      atomic_exchange_explicit(local_buffer + lid, desired_buffer[gid],
                               memory_order_relaxed, memory_scope_work_item);
  in_out_buffer[gid] = atomic_load_explicit(
      local_buffer + lid, memory_order_relaxed, memory_scope_work_item);
}
