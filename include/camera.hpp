#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace vr
{
    class Camera
    {
    public:
        void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
        void setPerspectiveProjection(float fovY, float aspect, float near, float far);
        void setViewYXZ(glm::vec3 position, glm::vec3 rotation);
        const glm::mat4& getProjection() const { return projectionMatrix; }
        const glm::mat4& getView() const { return viewMatrix; }

    private:
        glm::mat4 projectionMatrix{ 1.f };
        glm::mat4 viewMatrix{ 1.f };
    };
}