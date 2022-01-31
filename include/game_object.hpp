#pragma once
#include "model.hpp"
#include <memory>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>

namespace vr
{
    struct TransformComponent
    {
        glm::vec3 translation{};
        glm::vec3 scale{ 1.f, 1.f, 1.f };
        glm::vec3 rotation{};

        glm::mat4 mat4();
        glm::mat3 normalMatrix();
    };

    struct RigidBodyComponent
    {
        glm::vec3   velocity;
        float       mass;
        float       radius = 1.0f;
    };


    class GameObject
    {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, GameObject>;

        static GameObject createGameObject()
        {
            static id_t currentId = 0;
            return GameObject{ currentId++ };
        }

        id_t getId() { return id; }

        GameObject(const GameObject&)               = delete;
        void operator=(const GameObject&)           = delete;
        GameObject(GameObject&&)                    = default;
        GameObject& operator=(GameObject&&)         = default;

        std::shared_ptr<Model> model{};
        glm::vec3              color{};
        TransformComponent     transform{};
        RigidBodyComponent     rigidBody2D{};


    private:
        GameObject(id_t objId) : id{ objId } {}
        id_t id;
    };
}