#include "geometry.hpp"
#include <glm/gtc/constants.hpp>

namespace vr
{
    std::unique_ptr<Model> createCubeModel(Device& device, glm::vec3 offset)
    {
        Model::MeshData meshData{};
        meshData.vertices = {
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f,  .5f,  .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f,  .5f}, {.9f, .9f, .9f}},
            {{-.5f,  .5f, -.5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{ .5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{ .5f,  .5f,  .5f}, {.8f, .8f, .1f}},
            {{ .5f, -.5f,  .5f}, {.8f, .8f, .1f}},
            {{ .5f,  .5f, -.5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{ .5f, -.5f,  .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f,  .5f}, {.9f, .6f, .1f}},
            {{ .5f, -.5f, -.5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{ .5f, .5f,  .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f,  .5f}, {.8f, .1f, .1f}},
            {{ .5f, .5f, -.5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{ .5f, .5f,  0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f,  0.5f}, {.1f, .1f, .8f}},
            {{ .5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{ .5f, .5f,  -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f,  -0.5f}, {.1f, .8f, .1f}},
            {{ .5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        };

        for (auto& v : meshData.vertices) {
            v.position += offset;
        }

        meshData.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
            12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

        return std::make_unique<Model>(device, meshData);
    }


    std::unique_ptr<Model> vr::createSquareModel(Device& device, glm::vec3 offset)
    {
        Model::MeshData meshData{};
        meshData.vertices = {
            {{-0.5f, -0.5f, 0.f}},
            {{ 0.5f,  0.5f, 0.f}},
            {{-0.5f,  0.5f, 0.f}},
            {{-0.5f, -0.5f, 0.f}},
            {{ 0.5f, -0.5f, 0.f}},
            {{ 0.5f,  0.5f, 0.f}},
        };

        for (auto& v : meshData.vertices) {
            v.position += offset;
        }

        return std::make_unique<Model>(device, meshData);
    }


    std::unique_ptr<Model> vr::createCircleModel(Device& device, uint32_t numSides, float radius)
    {
        std::vector<Model::Vertex> uniqueVertices{};
        for (uint32_t i = 0; i < numSides; i++) {
            float angle = i * glm::two_pi<float>() / numSides;
            uniqueVertices.push_back({ {glm::cos(angle), glm::sin(angle), 0.f} });
        }
        uniqueVertices.push_back({});  // adds center vertex at 0, 0

        Model::MeshData meshData{};
        for (uint32_t i = 0; i < numSides; i++) {
            meshData.vertices.push_back(uniqueVertices[i]);
            meshData.vertices.push_back(uniqueVertices[(i + 1) % numSides]);
            meshData.vertices.push_back(uniqueVertices[numSides]);
        }

        return std::make_unique<Model>(device, meshData);
    }
}
