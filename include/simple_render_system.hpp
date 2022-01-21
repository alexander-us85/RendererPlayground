#pragma once
#include "pipeline.hpp"
#include "gfx_device.hpp"
#include "game_object.hpp"
#include <memory>

namespace vr
{
    class SimpleRenderSystem
    {
    public:
        SimpleRenderSystem(Device& device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem&) = delete;
        SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects);


    private:
        Device&                      device;
        std::unique_ptr<Pipeline>    pipeline;
        VkPipelineLayout             pipelineLayout;

        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);
    };
}