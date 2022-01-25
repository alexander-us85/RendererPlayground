#include "geometry.hpp"
#include <glm/gtc/constants.hpp>

namespace vr
{
    std::unique_ptr<Model> createCubeModel(Device& device, glm::vec3 offset)
    {
        std::vector<Model::Vertex> vertices {
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f,  .5f,  .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f,  .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f,  .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f,  .5f,  .5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{ .5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{ .5f,  .5f,  .5f}, {.8f, .8f, .1f}},
            {{ .5f, -.5f,  .5f}, {.8f, .8f, .1f}},
            {{ .5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{ .5f,  .5f, -.5f}, {.8f, .8f, .1f}},
            {{ .5f,  .5f,  .5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{ .5f, -.5f,  .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f,  .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{ .5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{ .5f, -.5f,  .5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{ .5f, .5f,  .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f,  .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{ .5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{ .5f, .5f,  .5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{ .5f, .5f,  0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f,  0.5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{ .5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{ .5f, .5f,  0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{ .5f,  .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f,  .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{ .5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{ .5f,  .5f, -0.5f}, {.1f, .8f, .1f}},

        };

        for (auto& v : vertices) {
            v.position += offset;
        }

        return std::make_unique<Model>(device, vertices);
    }


    std::unique_ptr<Model> vr::createSquareModel(Device& device, glm::vec3 offset)
    {
        std::vector<Model::Vertex> vertices = {
            {{-0.5f, -0.5f, 0.f}},
            {{ 0.5f,  0.5f, 0.f}},
            {{-0.5f,  0.5f, 0.f}},
            {{-0.5f, -0.5f, 0.f}},
            {{ 0.5f, -0.5f, 0.f}},
            {{ 0.5f,  0.5f, 0.f}},
        };

        for (auto& v : vertices) {
            v.position += offset;
        }

        return std::make_unique<Model>(device, vertices);
    }


    std::unique_ptr<Model> vr::createCircleModel(Device& device, uint32_t numSides, float radius)
    {
        std::vector<Model::Vertex> uniqueVertices{};
        for (uint32_t i = 0; i < numSides; i++) {
            float angle = i * glm::two_pi<float>() / numSides;
            uniqueVertices.push_back({ {glm::cos(angle), glm::sin(angle), 0.f} });
        }
        uniqueVertices.push_back({});  // adds center vertex at 0, 0

        std::vector<Model::Vertex> vertices{};
        for (uint32_t i = 0; i < numSides; i++) {
            vertices.push_back(uniqueVertices[i]);
            vertices.push_back(uniqueVertices[(i + 1) % numSides]);
            vertices.push_back(uniqueVertices[numSides]);
        }
        return std::make_unique<Model>(device, vertices);
    }
}
