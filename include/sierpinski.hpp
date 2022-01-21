#pragma once

#include "demo.hpp"

namespace vr
{
    class Sierpinski : public Demo
    {
    public:
        Sierpinski() {}
        ~Sierpinski() override;

        Sierpinski(const Sierpinski&) = delete;
        void operator=(const Sierpinski&) = delete;

        void init(Device& device);
        std::vector<GameObject>& getGameObjects() { return gameObjects; }
        void update(const float dt);

    private:
        std::vector<GameObject> gameObjects;
    };
}