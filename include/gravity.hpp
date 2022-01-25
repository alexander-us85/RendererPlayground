#pragma once

#include "demo.hpp"

namespace vr
{
    class Gravity : public Demo
    {
    public:
        Gravity(float strength) : strengthGravity{ strength }, gravityBodiesBeginIndex{ 0 }, vectorFieldBeginIndex{ 0 } {}
        ~Gravity() override { gameObjects.clear(); }

        Gravity(const Gravity&) = delete;
        void operator=(const Gravity&) = delete;

        void init(Device& device);
        std::vector<GameObject>& getGameObjects() { return gameObjects; }
        void update(const float dt);


    private:
        std::vector<GameObject> gameObjects;
        const float             strengthGravity;
        size_t                  gravityBodiesBeginIndex;
        size_t                  vectorFieldBeginIndex;

        glm::vec3 computeForce(GameObject& fromObj, GameObject& toObj) const;
    };
}