// Copyright 2026 The Autoware Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Force-included into every source of this package on ROCm builds: maps the CUDA runtime
// names used here to HIP.

#ifndef PROBABILISTIC_OCCUPANCY_GRID_MAP_HIP_COMPAT_HPP_
#define PROBABILISTIC_OCCUPANCY_GRID_MAP_HIP_COMPAT_HPP_

#include <hip/hip_runtime.h>

// Types
#ifndef cudaStream_t
#define cudaStream_t hipStream_t
#endif

// Constants
#ifndef cudaMemcpyHostToDevice
#define cudaMemcpyHostToDevice hipMemcpyHostToDevice
#endif
#ifndef cudaMemcpyDeviceToHost
#define cudaMemcpyDeviceToHost hipMemcpyDeviceToHost
#endif
#ifndef cudaStreamNonBlocking
#define cudaStreamNonBlocking hipStreamNonBlocking
#endif

// Memory
#ifndef cudaMemcpyAsync
#define cudaMemcpyAsync hipMemcpyAsync
#endif
#ifndef cudaMemsetAsync
#define cudaMemsetAsync hipMemsetAsync
#endif

// Streams
#ifndef cudaStreamCreateWithFlags
#define cudaStreamCreateWithFlags hipStreamCreateWithFlags
#endif
#ifndef cudaStreamSynchronize
#define cudaStreamSynchronize hipStreamSynchronize
#endif

#endif  // PROBABILISTIC_OCCUPANCY_GRID_MAP_HIP_COMPAT_HPP_
