#pragma once
#include "window.hpp"
#include "gfx_device.hpp"
#include "swap_chain.hpp"
#include <memory>
#include <cassert>

namespace vr
{
    class Renderer
    {
    public:
        Renderer(Window& wind, Device& dev);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        void operator=(const Renderer&) = delete;

        bool isFrameInProgress() const { return isFrameStarted; }
        VkRenderPass getSwapChainRenderPass() const { return swapChain->getRenderPass(); }
        float getAspectRatio() const { return swapChain->extentAspectRatio(); }

        VkCommandBuffer getCurrentCommandBuffer() const
        {
            assert(isFrameStarted && "Cannot get command buffer when frame is not in progress.");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const
        {
            assert(isFrameStarted && "Cannot get frame index when frame is not in progress.");
            return currentFrameIndex;
        }
        
        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);


    private:
        Window&                      window;
        Device&                      device;
        std::unique_ptr<SwapChain>   swapChain;
        std::vector<VkCommandBuffer> commandBuffers;
        uint32_t                     currentImageIndex;
        int                          currentFrameIndex;
        bool                         isFrameStarted;

        void createCommandBuffers();
        void recreateSwapChain();
        void freeCommandBuffers();
    };
}