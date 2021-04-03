#ifndef APP_INCLUDED
#define APP_INCLUDED
#include <iostream>
#include <SFML/Graphics.hpp>
#include <libconfig.h++>
#include "Settings.hpp"
#include "Fractals.hpp"
#include "Renderer.hpp"
#include "Synth.hpp"

class Renderer;

namespace FSE {
    
class App {
public:
    struct state_info {
        bool normalized {true}; //used by sound synth, must be true for mandelbrot
        bool sustain {true}; //should probly move to sound synth class
        bool hide_orbit {false};
        bool left_pressed {false};
        bool dragging  {false};
        bool should_take_screenshot {false};
        bool should_show_help {false};
        sf::Vector2<int> prevDrag;
    } state;
private:
    Settings settings;
    Fractal fractal;
    FractalSynth synth;
    Renderer renderer;
public:
    App (Settings app_settings) :
    settings {app_settings},
    synth{48000},
    renderer {app_settings}
    {
        SetFractal(app_settings.starting_fractal);
    }
    int Start();
    void SetFractal(const int& type);
    void PollEvents();
};

}
#endif
