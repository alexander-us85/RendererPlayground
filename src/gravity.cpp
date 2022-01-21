#include <glm/gtc/constants.hpp>
#include "gravity.hpp"

namespace vr
{
    void Gravity::init(Device& device)
    {
        std::shared_ptr<Model> squareModel = createSquareModel(device, { .5f, .0f });
        std::shared_ptr<Model> circleModel = createCircleModel(device, 64);

        auto red = GameObject::createGameObject();
        red.transform2d.scale = glm::vec2{ .05f };
        red.transform2d.translation = { .5f, .5f };
        red.color = { 1.f, 0.f, 0.f };
        red.rigidBody2D.velocity = { -.5f, .0f };
        red.rigidBody2D.mass = 1.f;
        red.model = circleModel;
        gameObjects.push_back(std::move(red));

        auto blue = GameObject::createGameObject();
        blue.transform2d.scale = glm::vec2{ .05f };
        blue.transform2d.translation = { -.45f, -.25f };
        blue.color = { 0.f, 0.f, 1.f };
        blue.rigidBody2D.velocity = { .5f, .0f };
        blue.rigidBody2D.mass = 1.f;
        blue.model = circleModel;
        gameObjects.push_back(std::move(blue));

        gravityBodiesBeginIndex = 0;
        vectorFieldBeginIndex = gameObjects.size();

        int gridCount = 40;
        for (int i = 0; i < gridCount; i++) {
            for (int j = 0; j < gridCount; j++) {
                auto vf = GameObject::createGameObject();
                vf.transform2d.scale = glm::vec2(0.005f);
                vf.transform2d.translation = {
                    -1.0f + (i + 0.5f) * 2.0f / gridCount,
                    -1.0f + (j + 0.5f) * 2.0f / gridCount
                };
                vf.color = glm::vec3(1.0f);
                vf.model = squareModel;
                vf.rigidBody2D.mass = 1.0f;
                gameObjects.push_back(std::move(vf));
            }
        }
    }

    glm::vec2 Gravity::computeForce(GameObject& fromObj, GameObject& toObj) const
    {
        auto offset = fromObj.transform2d.translation - toObj.transform2d.translation;
        float distanceSquared = glm::dot(offset, offset);

        // Return 0 if objects are too close to each other.
        if (glm::abs(distanceSquared) < 1e-10f) {
            return { .0f, .0f };
        }

        float force = strengthGravity * fromObj.rigidBody2D.mass * toObj.rigidBody2D.mass / distanceSquared;
        return force * offset / glm::sqrt(distanceSquared);
    }

    void Gravity::update(const float dt)
    {
        const int simulationSubsteps = 5;
        for (int i = 0; i < simulationSubsteps; i++) {
            for (auto iterA = gravityBodiesBeginIndex; iterA != vectorFieldBeginIndex; ++iterA) {
                auto& objA = gameObjects[iterA];
                for (auto iterB = iterA; iterB != vectorFieldBeginIndex; ++iterB) {
                    auto& objB = gameObjects[iterB];
                    auto force = computeForce(objA, objB);
                    objA.rigidBody2D.velocity += dt * -force / objA.rigidBody2D.mass;
                    objB.rigidBody2D.velocity += dt * force / objB.rigidBody2D.mass;
                }
            }

            for (size_t i = gravityBodiesBeginIndex; i < vectorFieldBeginIndex; i++) {
                gameObjects[i].transform2d.translation += dt * gameObjects[i].rigidBody2D.velocity;
            }
        }

        // For each field line calculate the net gravitation force for that point in space
        for (size_t vfi = vectorFieldBeginIndex; vfi < gameObjects.size(); vfi++) {
            glm::vec2 direction{};
            for (size_t obji = gravityBodiesBeginIndex; obji < vectorFieldBeginIndex; obji++) {
                direction += computeForce(gameObjects[obji], gameObjects[vfi]);
            }

            // Scale the length of the field line based on the log of the length values were
            // chosen just through trial and error and then the field line is rotated to point
            // in the direction of the field.
            gameObjects[vfi].transform2d.scale.x = 0.005f + 0.045f * glm::clamp(glm::log(glm::length(direction) + 1) / 3.f, 0.f, 1.f);
            gameObjects[vfi].transform2d.rotation = atan2(direction.y, direction.x);
        }
    }

    std::unique_ptr<Model> Gravity::createSquareModel(Device& device, glm::vec2 offset)
    {
        std::vector<Model::Vertex> vertices = {
            {{-0.5f, -0.5f}},
            {{ 0.5f,  0.5f}},
            {{-0.5f,  0.5f}},
            {{-0.5f, -0.5f}},
            {{ 0.5f, -0.5f}},
            {{ 0.5f,  0.5f}},
        };
        for (auto& v : vertices) {
            v.position += offset;
        }
        return std::make_unique<Model>(device, vertices);
    }

    std::unique_ptr<Model> vr::Gravity::createCircleModel(Device& device, unsigned int numSides)
    {
        std::vector<Model::Vertex> uniqueVertices{};
        for (uint32_t i = 0; i < numSides; i++) {
            float angle = i * glm::two_pi<float>() / numSides;
            uniqueVertices.push_back({ {glm::cos(angle), glm::sin(angle)} });
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