#ifndef APP_INCLUDED
#define APP_INCLUDED
#include <iostream>
#include <SFML/Graphics.hpp>
#include <libconfig.h++>
#include "Settings.hpp"
#include "Fractals.hpp"
#include "Renderer.hpp"

class Renderer;

class FSE
{
    public:
        struct state_info {
            bool normalized = true; //used by sound synth, must be true for mandelbrot
            bool sustain = true; //should probly move to sound synth class
            bool hide_orbit = false;
            bool left_pressed = false;
            bool dragging = false;
            sf::Vector2<int> prevDrag;
            bool should_take_screenshot = false;
            bool should_show_help = false;
        } state;
    private:
        Settings settings;
        Fractal fractal;
        Renderer renderer;
    public:
        int Init(Settings);
        void SetFractal(const int& type);
        void PollEvents();
};
#endif