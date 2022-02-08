cmake_minimum_required(VERSION 3.12)

include(build/CommonMacros.txt)

project(VulkanRenderer CXX C)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

set(VULKAN_SDK "C:/VulkanSDK/1.2.198.1/")
link_directories(${VULKAN_SDK}/Lib)

set(ENABLE_GLSLANG_BINARIES OFF CACHE BOOL "")
set(ENABLE_HLSL             OFF CACHE BOOL "")
set(ENABLE_CTEST            OFF CACHE BOOL "")
set(ENABLE_OPT              OFF CACHE BOOL "")
set(SKIP_GLSLANG_INSTALL    ON  CACHE BOOL "")
add_subdirectory(build/src/glslang)

include_directories(build/src/glm)
include_directories(build/src/glfw/include)
include_directories(build/src/vulkan/include)
include_directories(build/src/tiny_obj_loader)
include_directories(build/src/glslang/glslang/Include)
include_directories(build/src/glslang)
include_directories(build/src/glslang/StandAlone)
include_directories(build/src/glslang/src)
include_directories(include)

add_subdirectory(build/src/glfw)

SETUP_APP(VulkanRenderer)
target_sources(VulkanRenderer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/build/src/glslang/StandAlone/ResourceLimits.cpp)
target_link_libraries(VulkanRenderer glfw glslang SPIRV vulkan-1)