cmake_minimum_required(VERSION 3.12)

include(build/CommonMacros.txt)

project(VulkanRenderer CXX C)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")

set(VULKAN_SDK "C:/VulkanSDK/1.2.198.1/")
link_directories(${VULKAN_SDK}/Lib)

include_directories(build/src/glm)
include_directories(build/src/glfw/include)
include_directories(build/src/vulkan/include)
include_directories(build/src/tiny_obj_loader)
include_directories(include)

add_subdirectory(build/src/glfw)

SETUP_APP(VulkanRenderer)

target_link_libraries(VulkanRenderer glfw vulkan-1)