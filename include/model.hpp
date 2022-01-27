#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "gfx_device.hpp"
#include "gfx_buffer.hpp"

namespace vr
{
    class Model
    {
    public:

        struct Vertex
        {
            glm::vec3 position{};
            glm::vec3 color{};
            glm::vec3 normal{};
            glm::vec2 uv{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex& other) const {
                return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
            }
        };

        struct MeshData
        {
            std::vector<Vertex>     vertices{};
            std::vector<uint32_t>   indices{};

            void loadModel(const std::string& filePath);
        };

        Model(Device& device, const MeshData& meshData);
        ~Model() {};

        Model(const Model&) = delete;
        void operator=(const Model&) = delete;

        static std::unique_ptr<Model> createModelFromFile(Device& device, const std::string& filePath);

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);


    private:
        void createVertexBuffer(const std::vector<Vertex>& vertices);
        void createIndexBuffer(const std::vector<uint32_t>& indices);

        std::unique_ptr<GfxBuffer> vertexBuffer;
        std::unique_ptr<GfxBuffer> indexBuffer;

        Device&                    device;
        uint32_t                   vertexCount;
        uint32_t                   indexCount;
        bool                       hasIndexBuffer = false;
    };
}