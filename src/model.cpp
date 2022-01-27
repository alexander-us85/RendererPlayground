#include "model.hpp"
#include "utils.hpp"
#include <cassert>
#define TINYOBJLOADER_IMPLEMENTATION

#pragma warning( push )
#pragma warning( disable : 26495 )
#pragma warning( disable : 26451 )
#pragma warning( disable : 26812 )
#pragma warning( disable : 26498 )
#include <tiny_obj_loader.h>
#pragma warning( pop )
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#pragma warning(disable:26812)

namespace std
{
    template <>
    struct hash<vr::Model::Vertex>
    {
        size_t operator()(vr::Model::Vertex const& vertex) const
        {
            size_t seed = 0;
            vr::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
            return seed;
        }
    };
}

namespace vr
{
    Model::Model(Device& device, const MeshData& meshData) : device{ device }
    {
        createVertexBuffer(meshData.vertices);
        createIndexBuffer(meshData.indices);
    }

    void Model::createVertexBuffer(const std::vector<Vertex>& vertices)
    {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be at least 3.");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
        uint32_t vertexSize = sizeof(vertices[0]);

        GfxBuffer stagingBuffer {
            device,
            vertexSize,
            vertexCount,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        };
        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void*)vertices.data());

        vertexBuffer = std::make_unique<GfxBuffer>(
            device,
            vertexSize,
            vertexCount,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );
        device.copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);
    }

    void Model::createIndexBuffer(const std::vector<uint32_t>& indices)
    {
        indexCount = static_cast<uint32_t>(indices.size());
        hasIndexBuffer = indexCount > 0;

        if (!hasIndexBuffer) {
            return;
        }

        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
        uint32_t indexSize = sizeof(indices[0]);

        GfxBuffer stagingBuffer(
            device,
            indexSize,
            indexCount,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );
        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void*)indices.data());

        indexBuffer = std::make_unique<GfxBuffer>(
            device,
            indexSize,
            indexCount,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );
        device.copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), bufferSize);
    }

    void Model::bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer buffers[] = { vertexBuffer->getBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

        // 2^16 = 65535 indices
        // 2^32 = 4.294.967.295 indices
        if (hasIndexBuffer) {
            vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
        }
    }

    void Model::draw(VkCommandBuffer commandBuffer)
    {
        if (hasIndexBuffer) {
            vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        } else {
            vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
        }
    }

    std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> attrDesc;
        attrDesc.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position) });
        attrDesc.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color) });
        attrDesc.push_back({ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal) });
        attrDesc.push_back({ 3, 0, VK_FORMAT_R32G32_SFLOAT,    offsetof(Vertex, uv) });
        return attrDesc;
    }

    std::unique_ptr<Model> Model::createModelFromFile(Device& device, const std::string& filePath)
    {
        MeshData meshData{};
        meshData.loadModel(filePath);
        std::cout << "Loaded model: " << filePath << std::endl;
        std::cout << "Vertex count: " << meshData.vertices.size() << ". Index count: "
            << meshData.indices.size() << "." << std::endl;
        return std::make_unique<Model>(device, meshData);
    }

    void Model::MeshData::loadModel(const std::string& filePath)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str())) {
            throw std::runtime_error(warn + err);
        }

        vertices.clear();
        indices.clear();

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};
        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex{};

                if (index.vertex_index >= 0) {
                    vertex.position = {
                        attrib.vertices[3 * static_cast<size_t>(index.vertex_index) + 0],
                        attrib.vertices[3 * static_cast<size_t>(index.vertex_index) + 1],
                        attrib.vertices[3 * static_cast<size_t>(index.vertex_index) + 2]
                    };

                    vertex.color = {
                        attrib.colors[3 * static_cast<size_t>(index.vertex_index) + 0],
                        attrib.colors[3 * static_cast<size_t>(index.vertex_index) + 1],
                        attrib.colors[3 * static_cast<size_t>(index.vertex_index) + 2]
                    };
                }

                if (index.normal_index >= 0) {
                    vertex.normal = {
                        attrib.normals[3 * static_cast<size_t>(index.normal_index) + 0],
                        attrib.normals[3 * static_cast<size_t>(index.normal_index) + 1],
                        attrib.normals[3 * static_cast<size_t>(index.normal_index) + 2]
                    };
                }

                if (index.texcoord_index >= 0) {
                    vertex.uv = {
                        attrib.texcoords[2 * static_cast<size_t>(index.texcoord_index) + 0],
                        attrib.texcoords[2 * static_cast<size_t>(index.texcoord_index) + 1]
                    };
                }

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }
                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }
}