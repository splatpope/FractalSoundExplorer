#include "Renderer.hpp"
using namespace FSE;

void HelpMenu::Reset(const sf::Vector2<float>& window_res, const std::string& path)
{
    SetSize(window_res);
    if (!font.loadFromFile(path)) {
        std::cerr << "Failed to load font" << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }
    background.setFillColor(sf::Color{0, 0, 0, 128});
    help_text.setFont(font);
    help_text.setCharacterSize(24);
    help_text.setFillColor(sf::Color::White);
    help_text.setString(
    "  H - Toggle Help Menu                Left Mouse - Click or drag to hear orbits\n"
    "  D - Toggle Audio Dampening        Middle Mouse - Drag to pan view\n"
    "  C - Toggle Color                   Right Mouse - Stop orbit and sound\n"
    "F11 - Toggle Fullscreen             Scroll Wheel - Zoom in and out\n"
    "  S - Save Snapshot\n"
    "  R - Reset View\n"
    "  J - Hold down, move mouse, and\n"
    "      release to make Julia sets.\n"
    "      Press again to switch back.\n"
    "  1 - Mandelbrot Set\n"
    "  2 - Burning Ship\n"
    "  3 - Feather Fractal\n"
    "  4 - SFX Fractal\n"
    "  5 - Hénon Map\n"
    "  6 - Duffing Map\n"
    "  7 - Ikeda Map\n"
    "  8 - Chirikov Map\n"
    );
    help_text.setPosition(20.0F, 20.0F);
}

void HelpMenu::SetSize(const sf::Vector2<float>& window_res) {
    background.setSize(window_res);
}

void load_shader(sf::Shader& shader) {
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
}

void set_ogl_context(sf::ContextSettings& settings) {
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;
}

const sf::BlendMode Renderer::BlendAlpha = sf::BlendMode(sf::BlendMode::SrcAlpha, sf::BlendMode::OneMinusSrcAlpha, sf::BlendMode::Add,
                               sf::BlendMode::Zero, sf::BlendMode::One, sf::BlendMode::Add);

const sf::BlendMode Renderer::BlendIgnoreAlpha = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero, sf::BlendMode::Add,
                                     sf::BlendMode::Zero, sf::BlendMode::One, sf::BlendMode::Add);

Renderer::Renderer(const Settings& app_settings) :
is_fullscreen {app_settings.fullscreen},
max_iters {app_settings.max_iters} 
{
    load_shader(shader);

    viewport.setPosition(0, 0);
    set_ogl_context(context_settings);

    MakeWindow(app_settings);
    // Initial shader uniform values
    shader.setUniform("iCam", cam_world);
    shader.setUniform("iZoom", cam_zoom);
    // Create help menu view
    help.Reset(window_res, "static/fonts/RobotoMono-Medium.ttf");
}

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

// Screen utilities
void Renderer::ScreenToWorld(sf::Vector2<int> screen, sf::Vector2<float>& point) {
    point = (sf::Vector2<float> {screen} - window_res_tr / 2.0f) / cam_zoom - cam_world;
}
void Renderer::WorldToScreen(sf::Vector2<float> point, sf::Vector2<int>& screen) {
    screen = sf::Vector2<int> {cam_zoom * (point + cam_world) + window_res_tr / 2.0f};
}

void Renderer::GrabJuliaOffset() {
    ScreenToWorld(GetMousePosition(), julia_offset);
}

// Used whenever the window is created or resized
void Renderer::ResizeWindow(const int& w, const int& h) {
    SetWindowRes(w, h);
    render_target.create(w, h);
    window.setView(sf::View {sf::FloatRect {0.0F, 0.0F, window_res.x, window_res.y}});
    frame_counter = 0;
}

void Renderer::MakeWindow(Settings app_settings) {
    window.close();

    sf::VideoMode screenSize;
    unsigned int window_w_init {app_settings.window_width};
    unsigned int window_h_init {app_settings.window_height};

    Renderer::SetWindowRes(window_h_init, window_w_init);

    if (is_fullscreen) {
        screenSize = sf::VideoMode::getDesktopMode();
        window.create(screenSize, app_settings.app_name, sf::Style::Fullscreen, context_settings);
    } else {
        screenSize = sf::VideoMode(window_w_init, window_h_init, 24);
        window.create(screenSize, app_settings.app_name, sf::Style::Resize | sf::Style::Close, context_settings);
    }
    Renderer::ResizeWindow(screenSize.width, screenSize.height);
    window.setFramerateLimit(app_settings.target_fps);
    //window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    window.requestFocus();
}

void Renderer::TakeScreenshot() {
    window.display();
    const time_t t {std::time(0)};
    const tm* now {std::localtime(&t)};
    char buffer[128];
    std::strftime(buffer, sizeof(buffer), "pic_%m-%d-%y_%H-%M-%S.png", now);
    const sf::Vector2u windowSize {window.getSize()};
    sf::Texture texture;
    texture.create(windowSize.x, windowSize.y);
    texture.update(window);
    texture.copyToImage().saveToFile(buffer);
}

// need to decouple cam translation from cam scaling
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

void Renderer::Fractal_Render() {
    ApplyZoom();
    const bool hasJulia = (julia_offset.x < 1e8);
    const bool drawMSet = (julia_drag || !hasJulia);
    const bool drawJSet = (julia_drag || hasJulia);
    const int flags = (drawMSet ? 0x01 : 0) | (drawJSet ? 0x02 : 0) | (use_color ? 0x04 : 0);
    //std::cout << "in render loop, max iter : " << max_iters << std::endl;
    shader.setUniform("iResolution", window_res);
    shader.setUniform("iCam", cam_world);
    shader.setUniform("iZoom", cam_zoom);
    shader.setUniform("iFlags", flags); // for now
    shader.setUniform("iJulia", julia_offset);
    shader.setUniform("iIters", static_cast<int>(max_iters));
    shader.setUniform("iTime", static_cast<int>(frame_counter));

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

    if (help_enabled) {
        window.draw(help.background, sf::RenderStates(BlendAlpha));
        window.draw(help.help_text);
    };

    window.display();
    //Update shader time if frame blending is needed
    const double xSpeed {std::abs(cam_world.x - cam_dest_world.x) * cam_zoom_dest};
    const double ySpeed {xSpeed};
    const double zoomSpeed {std::abs(cam_zoom / cam_zoom_dest - 1.0)};
    if (xSpeed < 0.2 && ySpeed < 0.2 && zoomSpeed < 0.002) {
      frame_counter += 1;
    } else {
      frame_counter = 1;
    }

}
