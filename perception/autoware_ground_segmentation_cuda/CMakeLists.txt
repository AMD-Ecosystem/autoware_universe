cmake_minimum_required(VERSION 3.8)
project(autoware_ground_segmentation_cuda)

find_package(ament_cmake_auto REQUIRED)

option(AUTOWARE_CUDA_USE_HIP "Build with HIP for AMD GPUs even if CUDA is found" OFF)
if(NOT AUTOWARE_CUDA_USE_HIP)
  find_package(CUDA)
endif()

if(NOT CUDA_FOUND)
  find_package(hip CONFIG QUIET PATHS $ENV{ROCM_PATH} /opt/rocm)
  find_package(hipcub CONFIG QUIET PATHS $ENV{ROCM_PATH} /opt/rocm)
  if(NOT hip_FOUND OR NOT hipcub_FOUND)
    if(AUTOWARE_CUDA_USE_HIP)
      message(FATAL_ERROR "AUTOWARE_CUDA_USE_HIP is ON, but HIP or hipCUB is not found. "
        "Add the ROCm prefix to CMAKE_PREFIX_PATH or set ROCM_PATH.")
    endif()
    message(WARNING "cuda was not found, so the autoware_ground_segmentation_cuda package will not be built.")
    return()
  endif()
  message(STATUS "cuda was not found, building autoware_ground_segmentation_cuda with HIP ${hip_VERSION}")
  enable_language(HIP)
  set(USE_HIP TRUE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CUDA_FLAGS ${CMAKE_CUDA_FLAGS} "-g -G")
  set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} "-g -G")
endif()

ament_auto_find_build_dependencies()

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wunused-function)
endif()

if(BUILD_TESTING)
    list(APPEND AMENT_LINT_AUTO_EXCLUDE ament_cmake_uncrustify)
endif()

if(USE_HIP)
  # hip/include holds cub/cub.cuh mapped to hipCUB and a header, force-included into every
  # source, that maps the CUDA runtime names used here to HIP.
  get_target_property(hipcub_INCLUDE_DIRS hip::hipcub INTERFACE_INCLUDE_DIRECTORIES)
  include_directories(
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/hip/include
    SYSTEM
    ${hipcub_INCLUDE_DIRS}
  )
  add_compile_options(
    "$<$<COMPILE_LANGUAGE:HIP>:-include;${CMAKE_CURRENT_SOURCE_DIR}/hip/include/cuda_ground_segmentation_hip_compat.hpp>"
  )
else()
  include_directories(
    include
    SYSTEM
    ${CUDA_INCLUDE_DIRS}
  )
endif()

# cSpell: ignore expt
list(APPEND CUDA_NVCC_FLAGS "--expt-relaxed-constexpr -diag-suppress 20012")
list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_75,code=sm_75")
list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_86,code=sm_86")
list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_87,code=sm_87")
list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_89,code=sm_89")
if(CUDA_VERSION VERSION_GREATER_EQUAL "12.8")
  if(CUDA_VERSION VERSION_LESS "13.0")
    list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_101,code=sm_101")
  else()  # CUDA 13.0 renamed SM101 to SM110
    list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_110,code=sm_110")
  endif()
  list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_120,code=sm_120")
  list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_120,code=compute_120")
else()
  list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_89,code=compute_89")
endif()

#################### cuda_ground_segmentation ##################
if(USE_HIP)
  set_source_files_properties(
    src/cuda_scan_ground_segmentation/cuda_scan_ground_segmentation_filter.cu
    src/cuda_scan_ground_segmentation/cuda_scan_ground_segmentation_filter_node.cpp
    PROPERTIES LANGUAGE HIP)
  add_library(cuda_ground_segmentation_lib SHARED
    src/cuda_scan_ground_segmentation/cuda_scan_ground_segmentation_filter.cu
  )
  target_link_libraries(cuda_ground_segmentation_lib autoware_cuda_utils::autoware_cuda_utils)
else()
  cuda_add_library(cuda_ground_segmentation_lib SHARED
    src/cuda_scan_ground_segmentation/cuda_scan_ground_segmentation_filter.cu
  )
endif()

target_link_libraries(cuda_ground_segmentation_lib
  ${autoware_pointcloud_preprocessor_TARGETS}
  ${autoware_cuda_pointcloud_preprocessor_TARGETS}
)

target_include_directories(cuda_ground_segmentation_lib SYSTEM PRIVATE
  ${autoware_pointcloud_preprocessor_INCLUDE_DIRS}
  ${autoware_cuda_pointcloud_preprocessor_INCLUDE_DIRS}
  ${autoware_point_types_INCLUDE_DIRS}
  ${cuda_blackboard_INCLUDE_DIRS}
  ${diagnostic_msgs_INCLUDE_DIRS}
  ${geometry_msgs_INCLUDE_DIRS}
  ${rclcpp_INCLUDE_DIRS}
  ${rclcpp_components_INCLUDE_DIRS}
  ${rcl_interfaces_INCLUDE_DIRS}
  ${sensor_msgs_INCLUDE_DIRS}
  ${tf2_INCLUDE_DIRS}
  ${tf2_msgs_INCLUDE_DIRS}
  ${autoware_cuda_utils_INCLUDE_DIRS}
)

# Targets
ament_auto_add_library(cuda_ground_segmentation SHARED
  src/cuda_scan_ground_segmentation/cuda_scan_ground_segmentation_filter_node.cpp
)

target_link_libraries(cuda_ground_segmentation
  ${CUDA_LIBRARIES}
  ${diagnostic_msgs_LIBRARIES}
  cuda_ground_segmentation_lib
)

# TODO(mojomex): remove after migrating to the stream-ordered cuda_blackboard API,
# see autowarefoundation/cuda_blackboard#22 (cf. example migrations #12797, #12798, #12799)
target_compile_options(cuda_ground_segmentation PRIVATE -Wno-error=deprecated-declarations)

#=========== ScanGround Segmentation filter ========
rclcpp_components_register_node(cuda_ground_segmentation
  PLUGIN "autoware::cuda_ground_segmentation::CudaScanGroundSegmentationFilterNode"
  EXECUTABLE cuda_scan_ground_segmentation_filter_node)

################################################################################
# Install
install(DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}
)

install(
  TARGETS cuda_ground_segmentation_lib
  LIBRARY DESTINATION lib
)

ament_auto_package()

# Set ROS_DISTRO macros
set(ROS_DISTRO $ENV{ROS_DISTRO})
if(${ROS_DISTRO} STREQUAL "rolling")
    add_compile_definitions(ROS_DISTRO_ROLLING)
elseif(${ROS_DISTRO} STREQUAL "foxy")
    add_compile_definitions(ROS_DISTRO_FOXY)
elseif(${ROS_DISTRO} STREQUAL "galactic")
    add_compile_definitions(ROS_DISTRO_GALACTIC)
elseif(${ROS_DISTRO} STREQUAL "humble")
    add_compile_definitions(ROS_DISTRO_HUMBLE)
endif()
