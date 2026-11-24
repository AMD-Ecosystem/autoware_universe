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

#ifndef CUDA_GROUND_SEGMENTATION_HIP_COMPAT_HPP_
#define CUDA_GROUND_SEGMENTATION_HIP_COMPAT_HPP_

#include <hip/hip_runtime.h>

// Types
#ifndef cudaError_t
#define cudaError_t hipError_t
#endif
#ifndef cudaStream_t
#define cudaStream_t hipStream_t
#endif
#ifndef cudaMemPool_t
#define cudaMemPool_t hipMemPool_t
#endif
#ifndef cudaMemPoolProps
#define cudaMemPoolProps hipMemPoolProps
#endif

// Constants
#ifndef cudaSuccess
#define cudaSuccess hipSuccess
#endif
#ifndef cudaErrorInvalidValue
#define cudaErrorInvalidValue hipErrorInvalidValue
#endif
#ifndef cudaErrorLaunchFailure
#define cudaErrorLaunchFailure hipErrorLaunchFailure
#endif
#ifndef cudaMemcpyHostToDevice
#define cudaMemcpyHostToDevice hipMemcpyHostToDevice
#endif
#ifndef cudaMemcpyDeviceToHost
#define cudaMemcpyDeviceToHost hipMemcpyDeviceToHost
#endif
#ifndef cudaMemcpyDeviceToDevice
#define cudaMemcpyDeviceToDevice hipMemcpyDeviceToDevice
#endif
#ifndef cudaMemAllocationTypePinned
#define cudaMemAllocationTypePinned hipMemAllocationTypePinned
#endif
#ifndef cudaMemLocationTypeDevice
#define cudaMemLocationTypeDevice hipMemLocationTypeDevice
#endif
#ifndef cudaMemPoolAttrReleaseThreshold
#define cudaMemPoolAttrReleaseThreshold hipMemPoolAttrReleaseThreshold
#endif

// Errors and devices
#ifndef cudaGetLastError
#define cudaGetLastError hipGetLastError
#endif
#ifndef cudaGetDevice
#define cudaGetDevice hipGetDevice
#endif

// Memory
#ifndef cudaMalloc
#define cudaMalloc hipMalloc
#endif
#ifndef cudaMallocAsync
#define cudaMallocAsync hipMallocAsync
#endif
#ifndef cudaMemcpyAsync
#define cudaMemcpyAsync hipMemcpyAsync
#endif
#ifndef cudaMemsetAsync
#define cudaMemsetAsync hipMemsetAsync
#endif
#ifndef cudaMemPoolCreate
#define cudaMemPoolCreate hipMemPoolCreate
#endif
#ifndef cudaMemPoolDestroy
#define cudaMemPoolDestroy hipMemPoolDestroy
#endif
#ifndef cudaMemPoolSetAttribute
#define cudaMemPoolSetAttribute hipMemPoolSetAttribute
#endif
#ifndef cudaMallocFromPoolAsync
#define cudaMallocFromPoolAsync hipMallocFromPoolAsync
#endif
#ifndef cudaFreeAsync
#define cudaFreeAsync hipFreeAsync
#endif

// Streams
#ifndef cudaStreamCreate
#define cudaStreamCreate hipStreamCreate
#endif
#ifndef cudaStreamDestroy
#define cudaStreamDestroy hipStreamDestroy
#endif
#ifndef cudaStreamSynchronize
#define cudaStreamSynchronize hipStreamSynchronize
#endif

#endif  // CUDA_GROUND_SEGMENTATION_HIP_COMPAT_HPP_
