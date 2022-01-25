#include <stdexcept>
#include "app.hpp"
#include "simple_render_system.hpp"
#include "sierpinski.hpp"
#include "gravity.hpp"
#include "cube.hpp"
#include "camera.hpp"

namespace vr
{
    App::App()
    {
        //demo = std::make_unique<Sierpinski>();
        //demo = std::make_unique<Gravity>(1.f);
        demo = std::make_unique<Cube>();
        demo->init(device);
    }

    App::~App() 
    {
    }

    void App::run()
    {
        SimpleRenderSystem simpleRenderSystem{ device, renderer.getSwapChainRenderPass() };
        Camera camera{};

        while (!window.shouldClose()) {
            glfwPollEvents();
            float aspect = renderer.getAspectRatio();
            //camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

            demo->update(1.f / 6000.f);
            if (auto commandBuffer = renderer.beginFrame()) {
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, demo->getGameObjects(), camera);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }
}