#pragma once

#include "camera.hpp"
#include "pipeline.hpp"
#include "gfx_device.hpp"
#include "game_object.hpp"
#include "frame_info.hpp"
#include <memory>

namespace vr
{
    class SimpleRenderSystem
    {
    public:
        SimpleRenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem&) = delete;
        void operator=(const SimpleRenderSystem&) = delete;

        void renderGameObjects(FrameInfo& frameInfo, std::vector<GameObject>& gameObjects);
        int demoKind;


    private:
        Device&                      device;
        std::unique_ptr<Pipeline>    pipeline;
        VkPipelineLayout             pipelineLayout;

        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);
    };
}