#pragma once

#include <vector>
#include <string>
#include <vulkan/vulkan.h>
#include <glslang_c_interface.h>

namespace vr
{
    struct ShaderModule
    {
        std::vector<uint32_t> SPIRV;
        VkShaderModule shaderModule = nullptr;
    };

    std::string ReadShaderFile(const char* fileName);
    size_t CompileShader(glslang_stage_t stage, const char* shaderSource, ShaderModule& shaderModule);
    size_t CompileShaderFile(const char* file, ShaderModule& shaderModule);
}