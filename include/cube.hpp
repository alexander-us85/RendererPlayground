#pragma once

#include "demo.hpp"

namespace vr
{
    class Cube : public Demo
    {
    public:
        ~Cube() override { gameObjects.clear(); }

        void init(Device& device);
        void update(const float dt);
        GameObject::Map& getGameObjects() { return gameObjects; }


    private:
        GameObject::Map gameObjects;
    };
}