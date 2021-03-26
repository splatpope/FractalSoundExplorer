#include "App.hpp"
//List of fractal equations
const Fractal all_fractals[] = {
  mandelbrot,
  burning_ship,
  feather,
  sfx,
  henon,
  duffing,
  ikeda,
  chirikov,
};
//Change the fractal
void FSE::SetFractal(const int& type) {
    // Reset renderer fields to proper values
    renderer.GetShader().setUniform("iType", type);
    renderer.julia_offset.x = 1e8;
    renderer.julia_offset.y = 1e8;
    renderer.frame_counter = 0;
    // Reset fractal type and app state
    fractal = all_fractals[type];
    state.normalized = (type == 0);
    //synth.audio_pause = true;
    state.hide_orbit = true;

}

void FSE::PollEvents() {
    sf::Event event;
    while(renderer.PollWindowForEvents(event)) {
        switch(event.type) {
            case sf::Event::Closed:
                renderer.CloseWindow();
                break;
            case sf::Event::Resized:
                renderer.ResizeWindow(event.size.width, event.size.height);
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::R) {
                    renderer.ResetCam();
                }
            case sf::Event::KeyReleased:
                if(event.key.code == sf::Keyboard::J) {
                    state.julia_drag = false;
                    renderer.frame_counter = 0;
                }
                break;
            case sf::Event::MouseWheelMoved:
                renderer.cam_zoom_dest *= std::pow(1.1f, event.mouseWheel.delta);
                renderer.cam_dest_screen = renderer.GetMousePosition();
                renderer.ApplyZoom();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mouse_pos = renderer.GetMousePosition();
                    sf::Vector2f mouse_pos_world;
                    renderer.ScreenToWorld(mouse_pos, mouse_pos_world);
                    printf("\nClicked on (screen) : %d, %d | (world) %f, %f", mouse_pos.x, mouse_pos.y, mouse_pos_world.x, mouse_pos_world.y);
                }
                break;
        }
    }
}

int FSE::Init(Settings app_settings){
    // Initialize the renderer singleton
    renderer.Init(app_settings);
    // Setup the initial fractal
    SetFractal(app_settings.starting_fractal);

    while(renderer.IsWindowOpen()) {
        PollEvents();
        renderer.Render();
    }
    return EXIT_SUCCESS;
}