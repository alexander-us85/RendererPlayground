#pragma once

#include "demo.hpp"

namespace vr
{
    class Sierpinski : public Demo
    {
    public:
        ~Sierpinski() override;

        void init(Device& device);
        std::vector<GameObject>& getGameObjects() { return gameObjects; }
        void update(const float dt);
        DemoKind getDemoKind() { return kDemoKindSierpinski; }

    private:
        std::vector<GameObject> gameObjects;
    };
}