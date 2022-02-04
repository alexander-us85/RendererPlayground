#pragma once

#include "camera.hpp"
#include "game_object.hpp"
#include <vulkan/vulkan.h>

namespace vr
{
    #define MAX_LIGHTS 10

    struct PointLight
    {
        glm::vec4 position{}; // ignore w
        glm::vec4 color{};
    };

    struct GlobalUBO
    {
        glm::mat4 projection{ 1.f };
        glm::mat4 view{ 1.f };
        glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f }; // w is the intensity of light
        PointLight pointLights[MAX_LIGHTS];
        int numLights;
    };

    struct FrameInfo
    {
        int              frameIndex;
        float            frameTime;
        VkCommandBuffer  commandBuffer;
        Camera&          camera;
        VkDescriptorSet  globalDescriptorSet;
        GameObject::Map& gameObjects;
    };
}