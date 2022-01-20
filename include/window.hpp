#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <vulkan/vulkan.h>

namespace vr
{
    class Window
    {
    public:
        Window(uint32_t w, uint32_t h, std::string name);
        ~Window();

        bool shouldClose()              { return glfwWindowShouldClose(window); }
        VkExtent2D getExtent()          { return { width, height }; }
        bool wasWindowResized()         { return framebufferResized; }
        void resetWindowResizedFlag()   { framebufferResized = false; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

    private:
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        void initWindow();

        GLFWwindow*    window;
        uint32_t       width;
        uint32_t       height;
        bool           framebufferResized = false;
        std::string    windowName;
    };
}