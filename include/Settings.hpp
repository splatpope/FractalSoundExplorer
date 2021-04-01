#ifndef SETTINGS_INCLUDED
#define SETTINGS_INCLUDED
#include <iostream>

struct Settings {
    int target_fps;
    int sample_rate;
    int max_freq;
    int window_width;
    int window_height;
    int starting_fractal;
    int max_iters;
    int escape_radius;
    bool fullscreen;
    std::string app_name = "Fractal Sound Explorer";
};
#endif
