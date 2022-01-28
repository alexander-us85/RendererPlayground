#include <stdexcept>
#include <chrono>
#include "app.hpp"
#include "simple_render_system.hpp"
#include "sierpinski.hpp"
#include "gravity.hpp"
#include "cube.hpp"
#include "camera.hpp"
#include "keyboard.hpp"
#include "gfx_buffer.hpp"

namespace vr
{
    struct GlobalUBO
    {
        glm::mat4 projectionView{ 1.f };
        glm::vec3 lightDirection = glm::normalize(glm::vec3{ 1.f, -3.f, -1.f });
    };


    App::App()
    {
        globalDescriptorPool = DescriptorPool::Builder(device).setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT).build();

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
        std::vector<std::unique_ptr<GfxBuffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<GfxBuffer>(
                device,
                sizeof(GlobalUBO),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            );
            uboBuffers[i]->map();
        }

        auto globalSetLayout = DescriptorSetLayout::Builder(device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
            .build();
        std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            DescriptorWriter(*globalSetLayout, *globalDescriptorPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        SimpleRenderSystem simpleRenderSystem { device, renderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout() };
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
                int frameIndex = renderer.getFrameIndex();
                FrameInfo frameInfo{ frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex] };

                // Update
                GlobalUBO ubo{};
                ubo.projectionView = camera.getProjection() * camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // Render
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(frameInfo, demo->getGameObjects());
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }
}