#include "cube.hpp"
#include "geometry.hpp"

namespace vr
{
    void vr::Cube::init(Device& device)
    {
        std::shared_ptr<Model> model = createCubeModel(device, { .0f, .0f, .0f });
        auto cube = GameObject::createGameObject();
        cube.model = model;
        cube.transform.translation = { .0f, .0f, .5f };
        cube.transform.scale = { .5f, .5f, .5f };
        gameObjects.push_back(std::move(cube));
    }


    void vr::Cube::update(const float dt)
    {
    }
}