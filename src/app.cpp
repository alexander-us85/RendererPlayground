#include <stdexcept>
#include "app.hpp"
#include "simple_render_system.hpp"
//#include "sierpinski.hpp"
#include "gravity.hpp"

namespace vr
{
    App::App()
    {
        //demo = std::make_unique<Sierpinski>();
        demo = std::make_unique<Gravity>(9.81f);
        demo->init(device);
    }

    App::~App() 
    {
    }

    void App::run()
    {
        SimpleRenderSystem simpleRenderSystem{ device, renderer.getSwapChainRenderPass() };

        while (!window.shouldClose()) {
            glfwPollEvents();
            demo->update(1.f / 6000.f);
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