#include "sierpinski.hpp"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

static void sierpinski(std::vector<vr::Model::Vertex>& vertices, int depth,
    glm::vec2 left, glm::vec2 right, glm::vec2 top)
{
    if (depth <= 0) {
        vertices.push_back({ top,   glm::vec3(1.0, 0.0, 0.0) });
        vertices.push_back({ right, glm::vec3(0.0, 1.0, 0.0) });
        vertices.push_back({ left,  glm::vec3(0.0, 0.0, 1.0) });
    }
    else {
        auto leftTop = 0.5f * (left + top);
        auto rightTop = 0.5f * (right + top);
        auto leftRight = 0.5f * (left + right);
        sierpinski(vertices, depth - 1, left, leftRight, leftTop);
        sierpinski(vertices, depth - 1, leftRight, right, rightTop);
        sierpinski(vertices, depth - 1, leftTop, rightTop, top);
    }
}

namespace vr
{
    void vr::Sierpinski::terminate()
    {
        gameObjects.clear();
    }

    void Sierpinski::init(Device& device)
    {
        std::vector<Model::Vertex> vertices{};
        sierpinski(vertices, 4, { -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.0f, -0.5f });
        auto model = std::make_shared<Model>(device, vertices);

        std::vector<glm::vec3> colors{
            { 0.0f, 0.39f, 0.49f},
            {0.41f, 0.44f, 0.07f},
            {0.96f, 0.75f,  0.0f},
            {0.72f, 0.66f, 0.60f},
            { 0.0f, 0.61f, 0.87f}
        };

        for (auto& color : colors) {
            color = glm::pow(color, glm::vec3{ 2.2f });
        }

        for (int i = 0; i < 40; i++) {
            auto triangle = GameObject::createGameObject();
            triangle.model = model;
            triangle.color = colors[i % colors.size()];
            triangle.transform2d.scale = glm::vec2(.5f) + i * 0.025f;
            triangle.transform2d.rotation = i * glm::pi<float>() * .025f;
            gameObjects.push_back(std::move(triangle));
        }
    }

    void Sierpinski::update()
    {
        int i = 0;
        for (auto& obj : gameObjects) {
            i += 1;
            obj.transform2d.rotation = glm::mod<float>(obj.transform2d.rotation + 0.00005f * i, 2.f * glm::pi<float>());
        }
    }
}