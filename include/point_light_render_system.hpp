#pragma once

#include "camera.hpp"
#include "pipeline.hpp"
#include "gfx_device.hpp"
#include "game_object.hpp"
#include "frame_info.hpp"
#include <memory>

namespace vr
{
    class PointLightRenderSystem
    {
    public:
        PointLightRenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~PointLightRenderSystem();

        PointLightRenderSystem(const PointLightRenderSystem&) = delete;
        void operator=(const PointLightRenderSystem&) = delete;

        void render(FrameInfo& frameInfo);


    private:
        Device& device;
        std::unique_ptr<Pipeline>    pipeline;
        VkPipelineLayout             pipelineLayout;

        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);
    };
}