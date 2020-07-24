#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>

#include "Mantaray/Core/Window.hpp"
#include "Mantaray/Core/InputManager.hpp"
#include "Mantaray/OpenGL/Objects/Canvas.hpp"
#include "Mantaray/OpenGL/ObjectChain.hpp"
#include "Mantaray/OpenGL/ObjectLibrary.hpp"
#include "Mantaray/OpenGL/Context.hpp"

using namespace MR;

Window* Window::Instance = nullptr;

Window::Window() {}

Window*& Window::CreateWindow(std::string title, Vector2u size, bool shouldKeepAspectRatio) {
    return Window::CreateWindow(title, size, size, Vector2f(size.x, size.y), shouldKeepAspectRatio);
}

Window*& Window::CreateWindow(std::string title, Vector2u size, Vector2u resolution, bool shouldKeepAspectRatio) {
    return Window::CreateWindow(title, size, resolution, Vector2f(size.x, size.y), shouldKeepAspectRatio);
}

Window*& Window::CreateWindow(std::string title, Vector2u size, Vector2u resolution, Vector2f coordinateScale, bool shouldKeepAspectRatio) {
    if (Window::GetInstance() != nullptr) {
        return Window::GetInstance();
    }

    Window* newWindow = new Window();
    newWindow->initialize(title, size, resolution, coordinateScale, shouldKeepAspectRatio);
    Window::Instance = newWindow;
    return Window::Instance;
}

void Window::initialize(std::string title, Vector2u size, Vector2u resolution, Vector2f coordinateScale, bool shouldKeepAspectRatio) {
    Context::Create(&m_Window, title, size);
    glfwSetFramebufferSizeCallback(m_Window, Window::OnWindowResized);
    InputManager::SetWindowHandle(m_Window);
    ObjectChain::Initialize();
    m_Timer = Timer();
    m_ShouldKeepAspectRatio = shouldKeepAspectRatio;
    m_PrefferedAspectRatio = (float)resolution.x / (float)resolution.y;
    m_lastWindowedSize = getSize();
    m_lastWindowedPosition = getPosition();
    calculateViewDestination(size.x, size.y);
    m_DisplayBuffer = new Canvas(resolution, coordinateScale);
    ObjectLibrary::FindObject("DefaultTexturedShader", m_DisplayShader);
    m_Timer.start();
}

Window*& Window::GetInstance() {
    return Window::Instance;
}

Window::~Window() {
    delete m_DisplayBuffer;
    if (m_DisplayShader != ObjectLibrary::DefaultTexturedShader) {
        delete m_DisplayShader;
    }
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

float Window::update() {
    glfwPollEvents();
    float deltaTime = m_Timer.getDelta();
    MR::InputManager::Update(deltaTime);
    return deltaTime;
}

void Window::beginFrame() {
    m_DisplayBuffer->clear(m_ClearColor);
}

void Window::endFrame() {
    display();
    glfwSwapBuffers(m_Window);
}

void Window::display() {    
    m_DisplayBuffer->unbind();
    glViewport(0, 0, getSize().x, getSize().y);
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 projection = glm::ortho(
        static_cast<float>(0),
        static_cast<float>(getSize().x),
        static_cast<float>(0),
        static_cast<float>(getSize().y),
        -1.0f, 1.0f
    );
    m_DisplayShader->setUniformMatrix4("u_projectionMatrix", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(m_ViewportRect.x(), m_ViewportRect.y(), 0.0f));

    model = glm::scale(model, glm::vec3(m_ViewportRect.width(), m_ViewportRect.height(), 1.0f));

    m_DisplayShader->setUniformMatrix4("u_modelMatrix", model);
    m_DisplayShader->setUniformVector4f("u_textureSource", Vector4f(0, 0, 1, 1));
    m_DisplayShader->setRenderTexture("u_texture0", 0, *m_DisplayBuffer);
    m_DisplayShader->setUniformVector4f("u_color", Vector4f(1, 1, 1, 1));
    m_DisplayShader->setupForDraw();
    RenderTexture::DefaultVertexArray->draw();
}

void Window::draw(Sprite& sprite) {
    m_DisplayBuffer->draw(sprite);
}

void Window::draw(Polygon& polygon) {
    m_DisplayBuffer->draw(polygon);
}

void Window::draw(Canvas*& canvas) {
    m_DisplayBuffer->draw(canvas);
}

void Window::drawLine(Vector2f p1, Vector2f p2, float thickness, Color color) {
    m_DisplayBuffer->drawLine(p1, p2, thickness, color);
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

Vector2f Window::getMouseWorldPosition() {
    return m_DisplayBuffer->getMousePosition();
}

bool Window::getShouldClose() {
    return glfwWindowShouldClose(m_Window);
}

void Window::setShouldClose(bool shouldClose) {
    glfwSetWindowShouldClose(m_Window, (shouldClose) ? 1 : 0);
}

Vector2f Window::getOffset() {
    return m_DisplayBuffer->getOffset();
}

void Window::setOffset(Vector2f offset) {
    m_DisplayBuffer->setOffset(offset);
}

void Window::addOffset(Vector2f deltaOffset) {
    m_DisplayBuffer->addOffset(deltaOffset);
}

Vector2f Window::getCoordinateScale() {
    return m_DisplayBuffer->getCoordinateScale();
}

void Window::setCoordinateScale(Vector2f coordinateScale) {
    m_DisplayBuffer->setCoordinateScale(coordinateScale);
}

float Window::getScale() {
    return m_DisplayBuffer->getScale();
}

void Window::setScale(float scale) {
    m_DisplayBuffer->setScale(scale);
}

Vector2f Window::getScaleCenter() {
    return m_DisplayBuffer->getScaleCenter();
}

void Window::setScaleCenter(Vector2f scaleCenter) {
    m_DisplayBuffer->setScaleCenter(scaleCenter);
}

Shader* Window::getScreenShader() {
    return m_DisplayShader;
}

void Window::setScreenShader(class Shader* screenShader) {
    if (screenShader == nullptr) {
        m_Logger.Log("nullptr cannot be set as screenshader", Logger::LOG_WARNING);
        return;
    }
    if (m_DisplayShader != ObjectLibrary::DefaultTexturedShader) {
        delete m_DisplayShader;
    }
    m_DisplayShader = screenShader;
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

Rectanglei Window::getViewportRect() {
    return m_ViewportRect;
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
            m_ViewportRect = Rectanglei(xOffset, 0, viewDestinationWidth, windowHeight);
        }
        else {
            int viewDestinationHeight = windowWidth / m_PrefferedAspectRatio;
            int yOffset = (windowHeight - viewDestinationHeight) / 2.0f;
            m_ViewportRect = Rectanglei(0, yOffset, windowWidth, viewDestinationHeight);
        }
    }
    else {
        m_PrefferedAspectRatio = (float)windowWidth / (float)windowHeight;
        m_ViewportRect = Rectanglei(0, 0, windowWidth, windowHeight);
    }
}
