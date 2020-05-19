#include <GLFW/glfw3.h>

#include "Mantaray/Core/Window.hpp"
#include "Mantaray/Core/InputManager.hpp"
#include "Mantaray/OpenGL/ObjectChain.hpp"
#include "Mantaray/OpenGL/Context.hpp"
#include "Mantaray/OpenGL/Canvas.hpp"

using namespace MR;

Window* Window::Instance = nullptr;

Window::Window(std::string title, Vector2u size, bool shouldKeepAspectRatio) {
    Context::Create(&m_Window, title, size);
    glfwSetFramebufferSizeCallback(m_Window, Window::OnWindowResized);
    InputManager::SetWindowHandle(m_Window);
    ObjectChain::Initialize();
    m_Canvas = new Canvas(size);
    m_Timer = Timer();
    m_Timer.start();
    m_ViewPort = Rectanglei(0, 0, size.x, size.y);
    m_ShouldKeepAspectRatio = shouldKeepAspectRatio;
    m_PrefferedAspectRatio = (float)size.x / (float)size.y;
    calculateViewDestination(size.x, size.y);
    Window::Instance = this;
}

Window::Window(std::string title, Vector2u size, Vector2u resolution, bool shouldKeepAspectRatio) {
    Context::Create(&m_Window, title, size);
    glfwSetFramebufferSizeCallback(m_Window, Window::OnWindowResized);
    InputManager::SetWindowHandle(m_Window);
    ObjectChain::Initialize();
    m_Canvas = new Canvas(resolution);
    m_Timer = Timer();
    m_Timer.start();
    m_ViewPort = Rectanglei(0, 0, size.x, size.y);
    m_ShouldKeepAspectRatio = shouldKeepAspectRatio;
    m_PrefferedAspectRatio = (float)resolution.x / (float)resolution.y;
    calculateViewDestination(size.x, size.y);
    Window::Instance = this;
}

Window::Window(std::string title, Vector2u size, Vector2u resolution, Vector2f coordinateScale, bool shouldKeepAspectRatio) {
    Context::Create(&m_Window, title, size);
    glfwSetFramebufferSizeCallback(m_Window, Window::OnWindowResized);
    InputManager::SetWindowHandle(m_Window);
    ObjectChain::Initialize();
    m_Canvas = new Canvas(resolution, coordinateScale);
    m_Timer = Timer();
    m_Timer.start();
    m_ViewPort = Rectanglei(0, 0, size.x, size.y);
    m_ShouldKeepAspectRatio = shouldKeepAspectRatio;
    m_PrefferedAspectRatio = (float)resolution.x / (float)resolution.y;
    calculateViewDestination(size.x, size.y);
    Window::Instance = this;
}

Window::~Window() {
    delete m_Canvas;
    ObjectChain::TearDown();
    Context::Destroy();
    Window::Instance = nullptr;
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(m_Window);
}

float Window::update() {
    float deltaTime = m_Timer.getDelta();
    MR::InputManager::Update(deltaTime);
    return deltaTime;
}

void Window::beginFrame() {
    m_Canvas->bind(Color(0x00));
}

void Window::endFrame() {
    m_Canvas->display(m_ViewPort, m_ViewDestination);
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

void Window::draw(Sprite& sprite) {
    m_Canvas->draw(sprite);
}

void Window::draw(Polygon& polygon) {
    m_Canvas->draw(polygon);
}

void Window::OnWindowResized(GLFWwindow* window, int width, int height) {
    Window::Instance->calculateViewDestination(width, height);
}

void Window::calculateViewDestination(int windowWidth, int windowHeight) {
    if (m_ShouldKeepAspectRatio) {
        float aspectRatio = (float)windowWidth / (float)windowHeight;
        if (aspectRatio > m_PrefferedAspectRatio) {
            int viewDestinationWidth = windowHeight * m_PrefferedAspectRatio;
            int xOffset = (windowWidth - viewDestinationWidth) / 2.0f;
            m_ViewDestination = Rectanglef(xOffset, 0, viewDestinationWidth, windowHeight);
        }
        else {
            int viewDestinationHeight = windowWidth / m_PrefferedAspectRatio;
            int yOffset = (windowHeight - viewDestinationHeight) / 2.0f;
            m_ViewDestination = Rectanglef(0, yOffset, windowWidth, viewDestinationHeight);
        }
    }
    else {
        m_PrefferedAspectRatio = (float)windowWidth / (float)windowHeight;
    }
    m_ViewPort = Rectanglei(0, 0, windowWidth, windowHeight);
}
