# autoware_ground_segmentation_cuda

## Purpose

The `autoware_ground_segmentation` algorithms have been thoroughly tested with Autoware. However, due to latency and high computational cost when processing large pointcloud, the input pointcloud range has been limited by the `crop_box_filter` based on the ego-vehicle's `base_link`. This can cause unwanted object loss, especially before a sloped road.

![ground_segmentation_pipeline issue](./docs/image/ground_segmentation_issue.png)

Recently, GPU and CUDA-supported libraries such as [cuda_blackboard](https://github.com/autowarefoundation/cuda_blackboard/blob/1837689df2891f6223f07c178c21aed252566ede/README.md) and accelerated versions of [`autoware_pointcloud_preprocessor`](../../sensing/autoware_cuda_pointcloud_preprocessor/README.md) have been implemented. These can be leveraged to improve the performance of ground segmentation filter algorithms using CUDA/GPU.

This package reimplements the current scan_ground_filter of the ground_segmentation package to reduce latency and avoid the bottleneck caused by processing a large number of point clouds.

## AMD GPUs (ROCm)

When CUDA is not found, the package builds with HIP on ROCm instead, with hipCUB for CUB. `hip/include` maps `cub/cub.cuh` to hipCUB, and a header force-included on ROCm builds maps the CUDA runtime names to HIP. The sector kernel works in groups of 32 threads; on GPUs with 64-wide wavefronts two groups share a wavefront, and the shuffles and ballots stay within each group. `autoware_cuda_utils`, `cuda_blackboard` and `autoware_cuda_pointcloud_preprocessor` must be built for ROCm as well.

```bash
colcon build --packages-up-to autoware_ground_segmentation_cuda \
  --cmake-args -DCMAKE_PREFIX_PATH=/opt/rocm -DCMAKE_HIP_ARCHITECTURES=gfx942
```

Pass `-DAUTOWARE_CUDA_USE_HIP=ON` to build with HIP on a machine that also has CUDA. ROCm installed outside `/opt/rocm` needs its prefix in `CMAKE_PREFIX_PATH` or `ROCM_PATH`.

## Inner-workings / Algorithm

The detailed algorithm is available in [scan-ground-filter.md](../autoware_ground_segmentation/docs/scan-ground-filter.md).

## Parameters

{{ json_to_markdown("perception/autoware_ground_segmentation_cuda/schema/cuda_scan_ground_segmentation_filter.schema.json") }}
