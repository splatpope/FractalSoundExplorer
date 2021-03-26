#include "Renderer.hpp"

const sf::BlendMode Renderer::BlendAlpha = sf::BlendMode(sf::BlendMode::SrcAlpha, sf::BlendMode::OneMinusSrcAlpha, sf::BlendMode::Add,
                               sf::BlendMode::Zero, sf::BlendMode::One, sf::BlendMode::Add);

const sf::BlendMode Renderer::BlendIgnoreAlpha = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero, sf::BlendMode::Add,
                                     sf::BlendMode::Zero, sf::BlendMode::One, sf::BlendMode::Add);

void Renderer::SetWindowRes(const int& w, const int& h){
    window_res = sf::Vector2<float>(static_cast<float>(w), static_cast<float>(h));
    window_res_tr.x = window_res.x;
    window_res_tr.y = window_res.y;
}

sf::Shader& Renderer::GetShader() {
    return shader;
}

void Renderer::CloseWindow() {
    window.close();
}

bool Renderer::IsWindowOpen() {
    return window.isOpen();
}

bool Renderer::PollWindowForEvents(sf::Event& event) {
    return window.pollEvent(event);
}

sf::Vector2<int> Renderer::GetMousePosition() {
    return sf::Mouse::getPosition(window);
}

//Screen utilities
void Renderer::ScreenToWorld(sf::Vector2<int> screen, sf::Vector2<float>& point) {
    point = (sf::Vector2<float>(screen) - window_res_tr / 2.0f) / cam_zoom - cam_world;
    //point.y = -point.y;
}
void Renderer::WorldToScreen(sf::Vector2<float> point, sf::Vector2<int>& screen) {
    screen = sf::Vector2<int>(cam_zoom * (point + cam_world) + window_res_tr / 2.0f);
}

void set_ogl_context(sf::ContextSettings& settings) {
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;
}


//Used whenever the window is created or resized
void Renderer::ResizeWindow(const int& w, const int& h) {
    SetWindowRes(w, h);
    render_target.create(w, h);
    window.setView(sf::View(sf::FloatRect(0, 0, window_res.x, window_res.y)));
    frame_counter = 0;
}

void Renderer::MakeWindow(Settings app_settings) {
    window.close(); // is this really needed ?

    sf::VideoMode screenSize;
    int window_w_init = app_settings.window_width;
    int window_h_init = app_settings.window_height;

    Renderer::SetWindowRes(window_h_init, window_w_init);

    if (is_fullscreen) {
        screenSize = sf::VideoMode::getDesktopMode();
        window.create(screenSize, app_settings.app_name, sf::Style::Fullscreen, context_settings);
    } else {
        screenSize = sf::VideoMode(window_w_init, window_h_init, 24);
        window.create(screenSize, app_settings.app_name, sf::Style::Resize | sf::Style::Close, context_settings);
    }
    Renderer::ResizeWindow(screenSize.width, screenSize.height); // probably only need to create the rt and reset frame
    window.setFramerateLimit(app_settings.target_fps);
    //window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    window.requestFocus();
}

void Renderer::Init(Settings app_settings)
{
    //Make sure shader is supported
    if (!sf::Shader::isAvailable()) {
        std::cerr << "Graphics card does not support shaders" << std::endl;
        
        exit(EXIT_FAILURE);
    }
    //Load the vertex shader
    if (!shader.loadFromFile("static/shaders/vert.glsl", sf::Shader::Vertex)) {
        std::cerr << "Failed to compile vertex shader" << std::endl;
        system("pause");

        exit(EXIT_FAILURE);
    }
    //Load the fragment shader
    if (!shader.loadFromFile("static/shaders/frag.glsl", sf::Shader::Fragment)) {
        std::cerr << "Failed to compile fragment shader" << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }
    //Load the font
    if (!font.loadFromFile("static/fonts/RobotoMono-Medium.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    frame_counter = 0;
    cam_world = sf::Vector2<float>(0.0, 0.0);
    cam_dest_screen = sf::Vector2<int>(0.0, 0.0);
    cam_dest_world = sf::Vector2<float>(0.0, 0.0);
    cam_zoom = 100.0;
    cam_zoom_dest = cam_zoom;
    julia_offset = sf::Vector2<double>(1e8, 1e8);

    viewport.setPosition(0, 0);
    set_ogl_context(context_settings);
    is_fullscreen = app_settings.fullscreen;
    max_iters = app_settings.max_iters;
    MakeWindow(app_settings);
      //Setup the shader
    shader.setUniform("iCam", cam_world);
    shader.setUniform("iZoom", cam_zoom);
}

void Renderer::ApplyZoom() {
    sf::Vector2<float> fp, delta_cam;
    ScreenToWorld(cam_dest_screen, fp); // fp == cam_dest_World ??
    cam_zoom = cam_zoom*0.8 + cam_zoom_dest * 0.2;
    ScreenToWorld(cam_dest_screen, delta_cam); 
    cam_dest_world += delta_cam - fp;
    cam_world += delta_cam - fp;
    cam_world = cam_world * float(0.8) + cam_dest_world * float(0.2);
}

void Renderer::ResetCam() {
    cam_world = cam_dest_world = sf::Vector2<float>(0.0, 0.0);
    cam_dest_screen = sf::Vector2<int>(window_res / 2.0f);
    cam_zoom = cam_zoom_dest = 100.0;
    frame_counter = 0;
    ApplyZoom();
}

void Renderer::Render() {
    ApplyZoom();
    //std::cout << "in render loop, max iter : " << max_iters << std::endl;
    shader.setUniform("iResolution", window_res);
    shader.setUniform("iCam", cam_world);
    shader.setUniform("iZoom", cam_zoom);
    shader.setUniform("iFlags", (0x01 | (use_color ? 0x04 : 0))); // for now
    shader.setUniform("iJulia", sf::Vector2f(julia_offset));
    shader.setUniform("iIters", max_iters);
    shader.setUniform("iTime", frame_counter);

    sf::RenderStates states = sf::RenderStates::Default;
    states.blendMode = (frame_counter > 0 ? BlendAlpha : BlendIgnoreAlpha);
    states.shader = &shader;
    viewport.setSize(window_res);
    render_target.draw(viewport, states);
    render_target.display();

    //Draw the render texture to the window
    sf::Sprite sprite(render_target.getTexture());
    window.clear();
    window.draw(sprite, sf::RenderStates(BlendIgnoreAlpha));

    window.display();
    //Update shader time if frame blending is needed
    const double xSpeed = std::abs(cam_world.x - cam_dest_world.x) * cam_zoom_dest;
    const double ySpeed = xSpeed;
    const double zoomSpeed = std::abs(cam_zoom / cam_zoom_dest - 1.0);
    if (xSpeed < 0.2 && ySpeed < 0.2 && zoomSpeed < 0.002) {
      frame_counter += 1;
    } else {
      frame_counter = 1;
    }

}