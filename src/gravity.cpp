#include "gravity.hpp"
#include "geometry.hpp"

namespace vr
{
    void Gravity::init(Device& device)
    {
        std::shared_ptr<Model> squareModel = createSquareModel(device, { .5f, .0f, .0f });
        std::shared_ptr<Model> circleModel = createCircleModel(device, 64);
        std::shared_ptr<Model> bigCircleModel = createCircleModel(device, 32, 3.f);

        auto red = GameObject::createGameObject();
        red.transform.scale = glm::vec3{.05f, .05f, 1.f };
        red.transform.translation = { .5f, .5f, 0.f };
        red.color = { 1.f, 0.f, 0.f };
        red.rigidBody2D.velocity = { -.5f, .0f, .0f };
        red.rigidBody2D.mass = 1.f;
        red.model = circleModel;
        gameObjects.push_back(std::move(red));

        auto blue = GameObject::createGameObject();
        blue.transform.scale = glm::vec3{.05f, .05f, 1.f };
        blue.transform.translation = { -.45f, -.25f, 0.f };
        blue.color = { 0.f, 0.f, 1.f };
        blue.rigidBody2D.velocity = { .5f, .0f, .0f };
        blue.rigidBody2D.mass = 1.f;
        blue.model = circleModel;
        gameObjects.push_back(std::move(blue));

        auto green = GameObject::createGameObject();
        green.transform.scale = glm::vec3{ .1f, .1f, 1.f };
        green.transform.translation = { 0.f, 0.f, 0.f };
        green.color = { 0.f, 1.f, 0.f };
        green.rigidBody2D.velocity = { .0f, .0f, .0f };
        green.rigidBody2D.mass = 10.f;
        green.rigidBody2D.radius = 3.f;
        green.model = bigCircleModel;
        gameObjects.push_back(std::move(green));

        gravityBodiesBeginIndex = 0;
        vectorFieldBeginIndex = gameObjects.size();

        int gridCount = 40;
        for (int i = 0; i < gridCount; i++) {
            for (int j = 0; j < gridCount; j++) {
                auto vf = GameObject::createGameObject();
                vf.transform.scale = glm::vec3(0.005f, 0.005f, 1.f);
                vf.transform.translation = {
                    -1.0f + (i + 0.5f) * 2.0f / gridCount,
                    -1.0f + (j + 0.5f) * 2.0f / gridCount,
                    0.f
                };
                vf.color = glm::vec3(1.0f);
                vf.model = squareModel;
                vf.rigidBody2D.mass = 1.0f;
                gameObjects.push_back(std::move(vf));
            }
        }
    }

    glm::vec3 Gravity::computeForce(GameObject& fromObj, GameObject& toObj) const
    {
        auto offset = fromObj.transform.translation - toObj.transform.translation;
        float distanceSquared = glm::dot(offset, offset);

        auto collisionDistance = fromObj.rigidBody2D.radius + toObj.rigidBody2D.radius;
        collisionDistance = collisionDistance * collisionDistance;
        // Return 0 if objects are too close to each other.
        if (glm::abs(distanceSquared) < 0.01f) {
            return { .0f, .0f, .0f };
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
                gameObjects[i].transform.translation += dt * gameObjects[i].rigidBody2D.velocity;
            }
        }

        // For each field line calculate the net gravitation force for that point in space
        for (size_t vfi = vectorFieldBeginIndex; vfi < gameObjects.size(); vfi++) {
            glm::vec3 direction{};
            for (size_t obji = gravityBodiesBeginIndex; obji < vectorFieldBeginIndex; obji++) {
                direction += computeForce(gameObjects[obji], gameObjects[vfi]);
            }

            // Scale the length of the field line based on the log of the length values were
            // chosen just through trial and error and then the field line is rotated to point
            // in the direction of the field.
            gameObjects[vfi].transform.scale.x = 0.001f + 0.03f * glm::clamp(glm::log(glm::length(direction) + 1) / 3.f, 0.f, 1.f);
            gameObjects[vfi].transform.scale.y = 0.003f; 
            gameObjects[vfi].transform.rotation = glm::vec3(0.f, 0.f, atan2(direction.y, direction.x));
        }
    }
}