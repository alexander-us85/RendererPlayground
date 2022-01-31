#include "point_light_render_system.hpp"
#include <stdexcept>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#pragma warning(disable:26812)
namespace vr
{
    PointLightRenderSystem::PointLightRenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
        : device{ device }
    {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }

    PointLightRenderSystem::~PointLightRenderSystem()
    {
        vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
    }

    void PointLightRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
    {
        //VkPushConstantRange pushConstantRange{};
        //pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        //pushConstantRange.offset = 0;
        //pushConstantRange.size = sizeof(SimplePushConstantData);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout.");
        }
    }

    void PointLightRenderSystem::createPipeline(VkRenderPass renderPass)
    {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout.");

        PipelineConfigInfo pipelineConfig{};
        Pipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.attributeDescriptions.clear();
        pipelineConfig.bindingDescriptions.clear();
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipeline = std::make_unique<Pipeline>(
            device,
            "shaders/point_light.vert.spv",//"shaders/simple_shader.vert.spv",
            "shaders/point_light.frag.spv",//"shaders/simple_shader.frag.spv",
            pipelineConfig
            );
    }
     
    void PointLightRenderSystem::render(FrameInfo& frameInfo)
    {
        pipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout,
            0, 1, &frameInfo.globalDescriptorSet, 0, nullptr);
        vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
    }
}