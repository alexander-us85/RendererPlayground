#pragma once

#include "demo.hpp"

namespace vr
{
    class Sierpinski : public Demo
    {
    public:
        Sierpinski() {}
        ~Sierpinski() {}

        Sierpinski(const Sierpinski&) = delete;
        void operator=(const Sierpinski&) = delete;

        void init(Device& device);
        std::vector<GameObject>& getGameObjects() { return gameObjects; }
        void update();
        void terminate();

    private:
        std::vector<GameObject> gameObjects;
    };
}