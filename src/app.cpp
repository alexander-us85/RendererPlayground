#include <stdexcept>
#include <chrono>
#include "app.hpp"
#include "simple_render_system.hpp"
#include "sierpinski.hpp"
#include "gravity.hpp"
#include "cube.hpp"
#include "camera.hpp"
#include "keyboard.hpp"

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
        simpleRenderSystem.demoKind = static_cast<int32_t>(demo->getDemoKind());
        Camera camera{};
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto viewerObject = GameObject::createGameObject(); // Stores the camera state for now
        Keyboard cameraController{};

        while (!window.shouldClose()) {
            glfwPollEvents();
            
            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(window.getGLFWWindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);
            
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