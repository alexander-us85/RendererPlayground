#pragma once

#include "demo.hpp"

namespace vr
{
    class Cube : public Demo
    {
    public:
        Cube() {}
        ~Cube() override { gameObjects.clear(); }

        Cube(const Cube&) = delete;
        void operator=(const Cube&) = delete;

        void init(Device& device);
        void update(const float dt);
        std::vector<GameObject>& getGameObjects() { return gameObjects; }
        DemoKind getDemoKind() { return kDemoKindCube; }


    private:
        std::vector<GameObject> gameObjects;
    };
}