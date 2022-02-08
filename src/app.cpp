#include <stdexcept>
#include <chrono>
#include "app.hpp"
#include "simple_render_system.hpp"
#include "point_light_render_system.hpp"
#include "cube.hpp"
#include "camera.hpp"
#include "keyboard.hpp"
#include "gfx_buffer.hpp"
#include "shader_module.hpp"

namespace vr
{
    App::App()
    {
        globalDescriptorPool = DescriptorPool::Builder(device).setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT).build();

        demo = std::make_unique<Cube>();
        demo->init(device);
    }

    App::~App() 
    {
    }

    void App::run()
    {
        glslang_initialize_process();
        ShaderModule shaderModule{};
        if (CompileShaderFile("shaders/diffuse.vert", shaderModule) < 1) {
            throw std::exception("Failed to compile vertex shader.\n");
        } else {
            fprintf(stderr, "Vertex shader compiled successfully.\n");
        }
        if (CompileShaderFile("shaders/diffuse.frag", shaderModule) < 1) {
            throw std::exception("Failed to compile fragment shader.\n");
        } else {
            fprintf(stderr, "Fragment shader compiled successfully.\n");
        }
        glslang_finalize_process();

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
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
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
        PointLightRenderSystem pointLightRenderSystem{ device, renderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout() };
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
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);

            demo->update(1.f / 6000.f);
            if (auto commandBuffer = renderer.beginFrame()) {
                int frameIndex = renderer.getFrameIndex();
                FrameInfo frameInfo {
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex],
                    demo->getGameObjects()
                };

                // Update
                GlobalUBO ubo{};
                ubo.projection = camera.getProjection();
                ubo.view = camera.getView();
                pointLightRenderSystem.update(frameInfo, ubo);
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // Render
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(frameInfo);
                pointLightRenderSystem.render(frameInfo);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }
}