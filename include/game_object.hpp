#pragma once
#include "model.hpp"
#include <memory>

namespace vr
{
    struct Transform2DComponent
    {
        glm::vec2 translation{};
        glm::vec2 scale{ 1.f, 1.f };
        float rotation;

        glm::mat2 mat2()
        {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMat{ {c, s}, {-s, c} };

            glm::mat2 scaleMat{ {scale.x, .0f}, {.0f, scale.y} };
            return rotMat * scaleMat;
        }
    };


    class GameObject
    {
    public:
        using id_t = unsigned int;

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
        Transform2DComponent   transform2d{};


    private:
        GameObject(id_t objId) : id{ objId } {}
        id_t id;
    };
}