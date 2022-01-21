#pragma once
#include "game_object.hpp"
#include <vector>

namespace vr
{
    struct Demo
    {
    public:
        virtual void init(Device& device) = 0;
        virtual std::vector<GameObject>& getGameObjects() = 0;
        virtual void update() = 0;
        virtual ~Demo() {};
    };
}