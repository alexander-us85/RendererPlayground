#include "cube.hpp"
#include "geometry.hpp"

namespace vr
{
    void vr::Cube::init(Device& device)
    {
        std::shared_ptr<Model> model1 = Model::createModelFromFile(device, "models/Winged_Victory.obj");
        std::shared_ptr<Model> model2 = Model::createModelFromFile(device, "models/Winged_Victory_Smooth.obj");

        auto obj1 = GameObject::createGameObject();
        obj1.model = model1;
        obj1.transform.translation = { .0f, .0f, 2.5f };
        obj1.transform.scale = { 1.f, -1.f, 1.f }; // Scale negative to invert Y axis.

        auto obj2 = GameObject::createGameObject();
        obj2.model = model2;
        obj2.transform.translation = { 1.f, .0f, 2.5f };
        obj2.transform.scale = { 1.f, -1.f, 1.f };

        gameObjects.push_back(std::move(obj1));
        gameObjects.push_back(std::move(obj2));
    }


    void vr::Cube::update(const float dt)
    {
        for (auto& obj : gameObjects) {
            obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.0001f, glm::two_pi<float>());
            //obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.00005f, glm::two_pi<float>());
        }
    }
}