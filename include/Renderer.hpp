#ifndef RENDERER_INCLUDED
#define RENDERER_INCLUDED
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <libconfig.h++>
#include "Fractals.hpp"
#include "Settings.hpp"

namespace FSE {
    
class HelpMenu {
public:
    sf::RectangleShape background;
    sf::Font font;
    sf::Text help_text;
public:
    void Reset (const sf::Vector2<float>& window_res, const std::string& path);
    void SetSize(const sf::Vector2<float>& window_res);
};

class Renderer {
public:
    bool is_fullscreen {false};
    bool help_enabled {false};
    bool orbit_enabled {false};
    bool julia_drag {false};
    bool use_color {false};
    unsigned int frame_counter {0};
    unsigned int max_iters {1000};
    unsigned int orbit_iters {200};
    float cam_zoom {200.0f};
    float cam_zoom_dest {200.0f};
    sf::Vector2<float> window_res;
    sf::Vector2<float> cam_world;
    sf::Vector2<int> cam_dest_screen;
    sf::Vector2<float> cam_dest_world;
    sf::Vector2<float> julia_offset {1e8f, 1e8f};
    sf::Vector2<float> orbit {0.0, 0.0};
    sf::Vector2<float> orbit_c {0.0, 0.0};
    struct state_info {

    } state;
private:
    static const sf::BlendMode BlendAlpha;
    static const sf::BlendMode BlendIgnoreAlpha;
    sf::ContextSettings context_settings;
    sf::RenderWindow window;
    sf::RenderTexture render_target;
    sf::Shader shader;
    sf::RectangleShape viewport;
    HelpMenu help;
public:
    Renderer (const Settings& app_settings);

    void DrawOrbit(Fractal);
    void Fractal_Render(Fractal);
    void MakeWindow(Settings);
    // camera manipulation
    void ApplyZoom();
    void ResetCam();
    // window manipulation
    void SetWindowRes(const int&, const int&);
    void ResizeWindow(const int&, const int&);
    void CloseWindow();
    bool IsWindowOpen();
    bool PollWindowForEvents(sf::Event&);
    sf::Vector2<int> GetMousePosition();
    // coordinate conversion
    void ScreenToWorld(sf::Vector2<int>, sf::Vector2<float>&);
    void WorldToScreen(sf::Vector2<float>, sf::Vector2<int>&);
    // misc
    void GrabJuliaOffset();
    sf::Shader& GetShader();
    void TakeScreenshot();
};

}
#endif
