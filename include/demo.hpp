#pragma once
#include "game_object.hpp"
#include <vector>

#pragma warning(disable:26812)

namespace vr
{
    struct Demo
    {
    public:
        Demo() {}
        Demo(const Demo&) = delete;
        void operator=(const Demo&) = delete;

        virtual void init(Device& device) = 0;
        virtual GameObject::Map& getGameObjects() = 0;
        virtual void update(const float dt) = 0;
        virtual ~Demo() {};
    };
}