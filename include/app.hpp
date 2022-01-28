#pragma once
#include "window.hpp"
#include "gfx_device.hpp"
#include "renderer.hpp"
#include "game_object.hpp"
#include "descriptors.hpp"
#include "demo.hpp"
#include <memory>

namespace vr
{
    class App
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        App();
        ~App();

        App(const App&) = delete;
        void operator=(const App&) = delete;

        void run();

    private:
        Window                           window{ WIDTH, HEIGHT, "Output window" };
        Device                           device{ window };
        Renderer                         renderer{ window, device };
        std::unique_ptr<DescriptorPool>  globalDescriptorPool{};
        std::unique_ptr<Demo>            demo;
    };
}