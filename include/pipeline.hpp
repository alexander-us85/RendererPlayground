#pragma once
#include <string>
#include <vector>
#include "gfx_device.hpp"

namespace vr
{
    struct PipelineConfigInfo
    {
        PipelineConfigInfo() = default;
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

        std::vector<VkVertexInputBindingDescription>     bindingDescriptions{};
        std::vector<VkVertexInputAttributeDescription>   attributeDescriptions{};
        VkPipelineViewportStateCreateInfo                viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo           inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo           rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo             multisampleInfo;
        VkPipelineColorBlendAttachmentState              colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo              colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo            depthStencilInfo;
        std::vector<VkDynamicState>                      dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo                 dynamicStateInfo;
        VkPipelineLayout                                 pipelineLayout = nullptr;
        VkRenderPass                                     renderPass = nullptr;
        uint32_t                                         subpass = 0;
    };


    class Pipeline
    {
    public:
        Pipeline(
            Device&                   device,
            const std::string&        vertFilePath,
            const std::string&        fragFilePath,
            const PipelineConfigInfo& configInfo
        );
        ~Pipeline();
        Pipeline(const Pipeline&) = delete;
        void operator=(const Pipeline&) = delete;
        void bind(VkCommandBuffer commandBuffer);
        static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

    private:
        static std::vector<char> readFile(const std::string& filePath);
        void createGraphicsPipeline(
            const std::string&        vertFilePath, 
            const std::string&        fragFilePath,
            const PipelineConfigInfo& configInfo
        );
        void createShaderModule(const std::vector<uint32_t>& code, VkShaderModule* shaderModule);

        Device&        device;
        VkPipeline     graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}