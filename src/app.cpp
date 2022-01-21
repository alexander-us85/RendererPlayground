#include <stdexcept>
#include "app.hpp"
#include "simple_render_system.hpp"
#include "sierpinski.hpp"

namespace vr
{
    App::App()
    {
        demo = std::make_unique<Sierpinski>();
        demo->init(device);
    }

    App::~App() 
    {
        demo->terminate();
    }

    void App::run()
    {
        SimpleRenderSystem simpleRenderSystem{ device, renderer.getSwapChainRenderPass() };

        while (!window.shouldClose()) {
            glfwPollEvents();
            demo->update();
            if (auto commandBuffer = renderer.beginFrame()) {
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, demo->getGameObjects());
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }
}