#include "cube.hpp"
#include "geometry.hpp"

namespace vr
{
    void vr::Cube::init(Device& device)
    {
        std::shared_ptr<Model> model1 = Model::createModelFromFile(device, "models/Winged_Victory.obj");
        std::shared_ptr<Model> model2 = Model::createModelFromFile(device, "models/Winged_Victory_Smooth.obj");
        std::shared_ptr<Model> model3 = Model::createModelFromFile(device, "models/quad.obj");

        auto obj1 = GameObject::createGameObject();
        obj1.model = model1;
        obj1.transform.translation = { .0f, 0.0f, 2.5f };
        obj1.transform.scale = { 1.f, -1.f, 1.f }; // Scale negative to invert Y axis.

        auto obj2 = GameObject::createGameObject();
        obj2.model = model2;
        obj2.transform.translation = { 1.f, 0.0f, 2.5f };
        obj2.transform.scale = { 1.f, -1.f, 1.f };

        auto plane = GameObject::createGameObject();
        plane.model = model3;
        plane.transform.translation = { 0.f, 0.f, 2.5f };
        plane.transform.scale = { 3.f, 1.f, 3.f };

        gameObjects.emplace(obj1.getId(), std::move(obj1));
        gameObjects.emplace(obj2.getId(), std::move(obj2));
        gameObjects.emplace(plane.getId(), std::move(plane));

        std::vector<glm::vec3> lightColors{
            {1.f, .1f, .1f},
            {.1f, .1f, 1.f},
            {.1f, 1.f, .1f},
            {1.f, 1.f, .1f},
            {.1f, 1.f, 1.f},
            {1.f, 1.f, 1.f}
        };

        for (int i = 0; i < lightColors.size(); i++) {
            auto pointLight = GameObject::makePointLight(0.2f);
            pointLight.color = lightColors[i];
            auto rotateLight = glm::rotate(
                glm::mat4(1.f),
                (i * glm::two_pi<float>()) / lightColors.size(),
                { 0.f, -1.f, 0.f }
            );
            pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
            pointLight.transform.translation -= glm::vec3(-0.4f, 0.f, -2.5f);
            gameObjects.emplace(pointLight.getId(), std::move(pointLight));
        }
    }


    void vr::Cube::update(const float dt)
    {
        auto rotate = glm::rotate(glm::mat4(1.f), dt, { 0.f, -1.f, 2.5f });
        for (auto& obj : gameObjects) {
            if (obj.second.pointLight == nullptr) {
                continue;
            }
            obj.second.transform.translation = glm::vec3(rotate * glm::vec4(obj.second.transform.translation, 1.f));
            //obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.00005f, glm::two_pi<float>());
        }
    }
}