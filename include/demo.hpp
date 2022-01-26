#pragma once
#include "game_object.hpp"
#include <vector>

#pragma warning(disable:26812)

namespace vr
{
    enum DemoKind
    {
        kDemoKindSierpinski,
        kDemoKindGravity,
        kDemoKindCube,

        kDemoKindCount
    };

    struct Demo
    {
    public:
        virtual void init(Device& device) = 0;
        virtual std::vector<GameObject>& getGameObjects() = 0;
        virtual void update(const float dt) = 0;
        virtual ~Demo() {};
        virtual DemoKind getDemoKind() = 0;
    };
}