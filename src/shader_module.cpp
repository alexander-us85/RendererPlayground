#include "shader_module.hpp"
#include <StandAlone/ResourceLimits.h>

namespace vr
{
    std::string ReadShaderFile(const char* fileName)
    {
        FILE* file = fopen(fileName, "r");
        if (!file) {
            printf("I/O error. Cannot open '%s'.\n", fileName);
        }
        fseek(file, 0L, SEEK_END);
        const auto bytesInFile = ftell(file);
        fseek(file, 0L, SEEK_SET);
        char* buffer = (char*)alloca(bytesInFile + 1);
        const size_t bytesRead = fread(buffer, 1, bytesInFile, file);
        fclose(file);
        buffer[bytesRead] = 0;

        // Eliminate UTF byte-order marker (BOM).
        static constexpr unsigned char BOM[] = {0xef, 0xbb, 0xbf};
        if (bytesRead > 3) {
            if (!memcmp(buffer, BOM, 3)) {
                memset(buffer, ' ', 3);
            }
        }

        std::string code(buffer);

        while (code.find("#include ") != code.npos) {
            const auto pos = code.find("#include ");
            const auto p1 = code.find('<', pos);
            const auto p2 = code.find('>', pos);
            if (p1 == code.npos || p2 == code.npos || p2 <= p1) {
                printf("Error while loading shader program: %s.\n", code.c_str());
                return std::string();
            }
            const std::string name = code.substr(p1 + 1, p2 - p1 - 1);
            const std::string include = ReadShaderFile(name.c_str());
            code.replace(pos, p2 - pos + 1, include.c_str());
        }

        return code;
    }

    size_t CompileShader(glslang_stage_t stage, const char* shaderSource, ShaderModule& shaderModule)
    {
        glslang_input_t input{};
        input.language = GLSLANG_SOURCE_GLSL;
        input.stage = stage;
        input.client = GLSLANG_CLIENT_VULKAN;
        input.client_version = GLSLANG_TARGET_VULKAN_1_2;
        input.target_language = GLSLANG_TARGET_SPV;
        input.target_language_version = GLSLANG_TARGET_SPV_1_0;
        input.code = shaderSource;
        input.default_version = 100;
        input.default_profile = GLSLANG_NO_PROFILE;
        input.force_default_version_and_profile = false;
        input.forward_compatible = false;
        input.messages = GLSLANG_MSG_DEFAULT_BIT;
        input.resource = (const glslang_resource_t*)&glslang::DefaultTBuiltInResource;

        glslang_shader_t* shd = glslang_shader_create(&input);

        if (!glslang_shader_preprocess(shd, &input)) {
            fprintf(stderr, "GLSL preprocessing failed.\n");
            fprintf(stderr, "\n%s", glslang_shader_get_info_log(shd));
            fprintf(stderr, "\n%s", glslang_shader_get_info_debug_log(shd));
            fprintf(stderr, "Code:\n%s", input.code);
            return 0;
        }

        if (!glslang_shader_parse(shd, &input)) {
            fprintf(stderr, "GLSL parsing failed\n");
            fprintf(stderr, "\n%s", glslang_shader_get_info_log(shd));
            fprintf(stderr, "\n%s", glslang_shader_get_info_debug_log(shd));
            fprintf(stderr, "%s", glslang_shader_get_preprocessed_code(shd));
            return 0;
        }

        glslang_program_t* prog = glslang_program_create();
        glslang_program_add_shader(prog, shd);
        int msgs = GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT;
        if (!glslang_program_link(prog, msgs)) {
            fprintf(stderr, "GLSL linking failed.\n");
            fprintf(stderr, "\n%s", glslang_program_get_info_log(prog));
            fprintf(stderr, glslang_program_get_info_debug_log(prog));
            return 0;
        }

        glslang_program_SPIRV_generate(prog, stage);
        shaderModule.SPIRV.resize(glslang_program_SPIRV_get_size(prog));
        glslang_program_SPIRV_get(prog, shaderModule.SPIRV.data());

        const char* spirv_messages = glslang_program_SPIRV_get_messages(prog);
        if (spirv_messages) {
            fprintf(stderr, "%s", spirv_messages);
        }

        // Cleanup
        glslang_program_delete(prog);
        glslang_shader_delete(shd);
        return shaderModule.SPIRV.size();
    }

    int EndsWith(const char* s, const char* part)
    {
        return (strstr(s, part) - s) == (strlen(s) - strlen(part));
    }

    glslang_stage_t GlslangShaderStageFromFileName(const char* fileName)
    {
        if (EndsWith(fileName, ".vert")) {
            return GLSLANG_STAGE_VERTEX;
        }

        if (EndsWith(fileName, ".frag")) {
            return GLSLANG_STAGE_FRAGMENT;
        }

        if (EndsWith(fileName, ".geom")) {
            return GLSLANG_STAGE_GEOMETRY;
        }

        if (EndsWith(fileName, ".comp")) {
            return GLSLANG_STAGE_COMPUTE;
        }

        if (EndsWith(fileName, ".tesc")) {
            return GLSLANG_STAGE_TESSCONTROL;
        }

        if (EndsWith(fileName, ".tese")) {
            return GLSLANG_STAGE_TESSEVALUATION;
        }

        return GLSLANG_STAGE_VERTEX;
    }

    size_t CompileShaderFile(const char* file, ShaderModule& shaderModule)
    {
        auto shaderSource = ReadShaderFile(file);
        if (!shaderSource.empty()) {
            return CompileShader(GlslangShaderStageFromFileName(file), shaderSource.c_str(), shaderModule);
        }

        return 0;
    }
}