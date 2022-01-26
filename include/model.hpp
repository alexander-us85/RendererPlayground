#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>
#include "gfx_device.hpp"

namespace vr
{
    class Model
    {
    public:

        struct Vertex
        {
            glm::vec3 position;
            glm::vec3 color;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        struct MeshData
        {
            std::vector<Vertex>     vertices{};
            std::vector<uint32_t>   indices{};
        };

        Model(Device& device, const MeshData& meshData);
        ~Model();

        Model(const Model&) = delete;
        void operator=(const Model&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);


    private:
        void createVertexBuffer(const std::vector<Vertex>& vertices);
        void createIndexBuffer(const std::vector<uint32_t>& indices);

        Device&         device;
        VkBuffer        vertexBuffer;
        VkDeviceMemory  vertexBufferMemory;
        uint32_t        vertexCount;
        VkBuffer        indexBuffer;
        VkDeviceMemory  indexBufferMemory;
        uint32_t        indexCount;
        bool            hasIndexBuffer = false;
    };
}