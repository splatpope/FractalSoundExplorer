#ifndef RENDERER_INCLUDED
#define RENDERER_INCLUDED
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <libconfig.h++>
#include "Settings.hpp"

class Renderer
{
    public:
        int frame_counter;
        int max_iters;
        sf::Vector2<float> window_res;
        sf::Vector2<float> window_res_tr;
        sf::Vector2<float> cam_world;
        sf::Vector2<int> cam_dest_screen;
        sf::Vector2<float> cam_dest_world;
        sf::Vector2<float> julia_offset;
        bool julia_drag;
        float cam_zoom;
        float cam_zoom_dest;
        bool use_color = false;
        bool is_fullscreen;
        struct state_info {

        } state;

    private:
        static const sf::BlendMode BlendAlpha;
        static const sf::BlendMode BlendIgnoreAlpha;

        sf::ContextSettings context_settings;
        sf::RenderWindow window;
        sf::RenderTexture render_target;
        sf::Shader shader;
        sf::Font font;

        sf::RectangleShape viewport;
    public:
        void Init(Settings);
        void Render();
        void MakeWindow(Settings);
        void ApplyZoom();
        void ResetCam();
        void SetWindowRes(const int&, const int&);
        void ResizeWindow(const int&, const int&);
        
        void ScreenToWorld(sf::Vector2<int>, sf::Vector2<float>&);
        void WorldToScreen(sf::Vector2<float>, sf::Vector2<int>&);
        void GrabJuliaOffset();
        sf::Shader& GetShader();
        void CloseWindow();
        bool IsWindowOpen();
        bool PollWindowForEvents(sf::Event&);
        sf::Vector2<int> GetMousePosition();
};
#endif