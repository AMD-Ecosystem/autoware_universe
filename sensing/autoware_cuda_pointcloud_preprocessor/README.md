# autoware_cuda_pointcloud_preprocessor

## Purpose

The pointcloud preprocessing implemented in `autoware_pointcloud_preprocessor` has been thoroughly tested in autoware. However, the latency it introduces does not scale well with modern LiDAR devices due to the high number of points they introduce.

To alleviate this issue, this package reimplements most of the pipeline presented in `autoware_pointcloud_preprocessor` leveraging the use of GPGPUs. In particular, this package makes use of CUDA to provide accelerated versions of the already established implementations, while also maintaining compatibility with normal ROS nodes/topics. <!-- cSpell: ignore GPGPUs -->

## Inner-workings / Algorithms

A detailed description of each filter's algorithm is available in the following links.

| Filter Name                         | Description                                                                                                                                  | Detail                                          |
| ----------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------- |
| cuda_pointcloud_preprocessor        | Implements the cropping, distortion correction, and outlier filtering (ring-based) of the `autoware_pointcloud_preprocessor`'s CPU versions. | [link](docs/cuda-pointcloud-preprocessor.md)    |
| cuda_concatenate_and_time_sync_node | Implements pointcloud concatenation an synchronization following `autoware_pointcloud_preprocessor`'s CPU implementation.                    | [link](docs/cuda-concatenate-data.md)           |
| cuda_voxel_grid_downsample_filter   | Implements voxel downsample filtering of the `autoware_pointcloud_preprocessor`'s CPU version                                                | [link](docs/cuda-voxel-grid-downsample.md)      |
| cuda_polar_voxel_outlier_filter     | Implements polar voxel outlier filtering of the `autoware_pointcloud_preprocessor`'s CPU version                                             | [link](docs/cuda-polar-voxel-outlier-filter.md) |

## AMD GPUs (ROCm)

When CUDA is not found, the package builds with HIP on ROCm instead, with hipCUB for CUB, rocThrust for Thrust and libhipcxx for libcu++. The sources need no HIP-specific code: `hip/include` maps the CUB headers to hipCUB, and a header force-included on ROCm builds maps the CUDA runtime names to HIP. `autoware_cuda_utils` and `cuda_blackboard` must be built for ROCm as well.

```bash
colcon build --packages-up-to autoware_cuda_pointcloud_preprocessor \
  --cmake-args -DCMAKE_PREFIX_PATH=/opt/rocm -DCMAKE_HIP_ARCHITECTURES=gfx942
```

Pass `-DAUTOWARE_CUDA_USE_HIP=ON` to build with HIP on a machine that also has CUDA. ROCm installed outside `/opt/rocm` needs its prefix in `CMAKE_PREFIX_PATH` or `ROCM_PATH`. `CMAKE_HIP_ARCHITECTURES` is the GPU target (`gfx942` for MI300, `gfx1100` for Radeon RX 7900); without it CMake builds for the GPUs it detects.

## (Optional) Future extensions / Unimplemented parts

The subsample filters implemented in `autoware_pointcloud_preprocessor` will have similar counterparts in this package.
