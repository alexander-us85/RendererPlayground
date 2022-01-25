#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "gfx_device.hpp"
#include "model.hpp"

namespace vr
{
    // temporary helper function, creates a 1x1x1 cube centered at offset
    std::unique_ptr<Model> createCubeModel(Device& device, glm::vec3 offset);
    std::unique_ptr<Model> createSquareModel(Device& device, glm::vec3 offset);
    std::unique_ptr<Model> createCircleModel(Device& device, uint32_t numSides, float radius = 1.f);
}
