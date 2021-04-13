#include "App.hpp"
#include "Complex.hpp"
using namespace FSE;

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
void App::SetFractal(const int& type) {
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
    synth.fractal = fractal;
}

void App::PollEvents() {
    sf::Event event;
    while(renderer.PollWindowForEvents(event)) {
        switch(event.type) {
            case sf::Event::Closed: {
                renderer.CloseWindow();
                break;
            }
            case sf::Event::Resized: {
                renderer.ResizeWindow(event.size.width, event.size.height);
                break;
            }
            case sf::Event::KeyPressed:
                switch(event.key.code) {
                    case sf::Keyboard::Escape: {
                        renderer.CloseWindow();
                        break;
                    }
                    case sf::Keyboard::R: {
                        renderer.ResetCam();
                        break;
                    }
                    case sf::Keyboard::C: {
                        renderer.use_color = !renderer.use_color;
                        renderer.frame_counter = 0;
                        break;
                    }
                    case sf::Keyboard::J: {
                        if (renderer.julia_offset.x < 1e8) {
                            renderer.julia_offset = sf::Vector2<float>(1e8f, 1e8f);
                        } else {
                            renderer.julia_drag = true;
                            renderer.GrabJuliaOffset();
                        }
                        renderer.frame_counter = 0;
                        break;
                    }
                    case sf::Keyboard::S: {
                        renderer.TakeScreenshot();
                        break;
                    }
                    case sf::Keyboard::H: {
                        renderer.help_enabled = !renderer.help_enabled;
                        break;
                    }
                    case sf::Keyboard::F11: {
                        renderer.is_fullscreen = !renderer.is_fullscreen;
                        renderer.MakeWindow(settings);
                        break;
                    }
                    default: {
                        if (event.key.code == sf::Keyboard::Quote) event.key.code = sf::Keyboard::Num4; // Dirty hack because for some reason num4 is borked
                        if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num8)
                            SetFractal(event.key.code - sf::Keyboard::Num1);
                        break;
                    }
                }
            case sf::Event::KeyReleased: {
                if(event.key.code == sf::Keyboard::J && !sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
                    renderer.julia_drag = false;
                    renderer.frame_counter = 0;
                }
                break;
            }
            case sf::Event::MouseWheelMoved: {
                renderer.cam_zoom_dest *= std::pow(1.1f, event.mouseWheel.delta);
                renderer.cam_dest_screen = renderer.GetMousePosition();
                //renderer.ApplyZoom();
                break;
            }
            case sf::Event::MouseButtonPressed:
                switch (event.mouseButton.button) {
                    case sf::Mouse::Left: {
                        sf::Vector2<int> mouse_pos {renderer.GetMousePosition()};
                        sf::Vector2<float> mouse_pos_world;
                        renderer.ScreenToWorld(mouse_pos, mouse_pos_world);
                        renderer.orbit_c = mouse_pos_world;
                        renderer.orbit = renderer.orbit_c;
                        renderer.orbit_enabled = true;
                        //printf("\nClicked on (screen) : %d, %d | (world) %f, %f", mouse_pos.x, mouse_pos.y, mouse_pos_world.x, mouse_pos_world.y);
                        synth.orbit_start = Complex<> {mouse_pos_world.x, mouse_pos_world.y};
                        synth.start();
                        break;
                    }
                    case sf::Mouse::Middle: {
                        state.prevDrag = sf::Vector2<int>(event.mouseButton.x, event.mouseButton.y);
                        state.dragging = true;
                        break;
                    }
                    case sf::Mouse::Right: {
                        renderer.orbit_enabled = false;
                        synth.stop();
                        break;
                    }
                }
                break;
            case sf::Event::MouseButtonReleased: {
                switch (event.mouseButton.button) {
                    case sf::Mouse::Middle: {
                        state.dragging = false;
                    }
                }
            }
            case sf::Event::MouseMoved: {
                if (state.dragging) {
                    sf::Vector2<int> curDrag {sf::Vector2<int>(event.mouseMove.x, event.mouseMove.y)};
                    renderer.cam_dest_world += sf::Vector2<float>(curDrag - state.prevDrag) / renderer.cam_zoom;
                    state.prevDrag = curDrag;
                    renderer.frame_counter = 0;
                }
                if (renderer.julia_drag == true) {
                    renderer.ScreenToWorld(sf::Vector2<int>(event.mouseMove.x, event.mouseMove.y), renderer.julia_offset);
                    renderer.frame_counter = 0;
                }
                break;
            }
            default:
                break;
        }
    }
}

int App::Start(){
    while(renderer.IsWindowOpen()) {
        PollEvents();
        renderer.Fractal_Render(fractal);
    }
    return EXIT_SUCCESS;
}
