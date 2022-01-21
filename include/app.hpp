#pragma once
#include "window.hpp"
#include "gfx_device.hpp"
#include "renderer.hpp"
#include "game_object.hpp"
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
        App& operator=(const App&) = delete;

        void run();

    private:
        Window                       window{ WIDTH, HEIGHT, "Output window" };
        Device                       device{ window };
        Renderer                     renderer{ window, device };
        std::vector<GameObject>      gameObjects;

        void loadGameObjects();
    };
}