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
    m_lastWindowedSize = getSize();
    m_lastWindowedPosition = getPosition();
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
    m_lastWindowedSize = getSize();
    m_lastWindowedPosition = getPosition();
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
    m_lastWindowedSize = getSize();
    m_lastWindowedPosition = getPosition();
    calculateViewDestination(size.x, size.y);
    Window::Instance = this;
}

Window::~Window() {
    delete m_Canvas;
    ObjectChain::TearDown();
    Context::Destroy();
    Window::Instance = nullptr;
}

void Window::iconify() {
    glfwIconifyWindow(m_Window);
}

void Window::maximize() {
    if (glfwGetWindowMonitor(m_Window) != nullptr) {
        return;
    }
    
    m_lastWindowedSize = getSize();
    m_lastWindowedPosition = getPosition();
    const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowMonitor(m_Window, glfwGetPrimaryMonitor(), 0, 0, videoMode->width, videoMode->height, GLFW_DONT_CARE);
}

void Window::restore() {
    glfwSetWindowMonitor(m_Window, NULL, m_lastWindowedPosition.x, m_lastWindowedPosition.y, m_lastWindowedSize.x, m_lastWindowedSize.y, GLFW_DONT_CARE);
    glfwRestoreWindow(m_Window);
}

void Window::setTitle(std::string title) {
    glfwSetWindowTitle(m_Window, title.c_str());
}

Vector2i Window::getSize() {
    int width, height;
    glfwGetWindowSize(m_Window, &width, &height);
    return Vector2i(width, height);
}

void Window::setSize(Vector2i size) {
    glfwSetWindowSize(m_Window, size.x, size.y);
}

Vector2i Window::getPosition() {
    int xPos, yPos;
    glfwGetWindowPos(m_Window, &xPos, &yPos);
    return Vector2i(xPos, yPos);
}

void Window::setPosition(Vector2i position) {
    glfwSetWindowPos(m_Window, position.x, position.y);
}

bool Window::getShouldClose() {
    return glfwWindowShouldClose(m_Window);
}

void Window::setShouldClose(bool shouldClose) {
    glfwSetWindowShouldClose(m_Window, (shouldClose) ? 1 : 0);
}

float Window::update() {
    float deltaTime = m_Timer.getDelta();
    MR::InputManager::Update(deltaTime);
    return deltaTime;
}

void Window::beginFrame() {
    m_Canvas->clear(m_ClearColor);
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

Vector2f Window::getOffset() {
    return m_Canvas->getOffset();
}

void Window::setOffset(Vector2f offset) {
    m_Canvas->setOffset(offset);
}

void Window::addOffset(Vector2f deltaOffset) {
    m_Canvas->addOffset(deltaOffset);
}

float Window::getScale() {
    return m_Canvas->getScale();
}

void Window::setScale(float scale) {
    m_Canvas->setScale(scale);
}

Vector2f Window::getScaleCenter() {
    return m_Canvas->getScaleCenter();
}

void Window::setScaleCenter(Vector2f scaleCenter) {
    m_Canvas->setScaleCenter(scaleCenter);
}

Shader* Window::getScreenShader() {
    return m_Canvas->getShader();
}

void Window::setScreenShader(class Shader* screenShader) {
    if (screenShader == nullptr) {
        m_Logger.Log("nullptr cannot be set as screenshader", Logger::LOG_WARNING);
        return;
    }
    m_Canvas->setShader(screenShader);
}

Color Window::getClearColor() {
    return m_ClearColor;
}

void Window::setClearColor(Color clearColor) {
    m_ClearColor = clearColor;
}

void Window::setClearColor(unsigned char clearColor) {
    m_ClearColor = Color(clearColor);
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
        m_ViewDestination = Rectanglef(0, 0, windowWidth, windowHeight);
    }
    m_ViewPort = Rectanglei(0, 0, windowWidth, windowHeight);
}
