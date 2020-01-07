#include "Mantaray/Core/InputManager.h"
#include "Mantaray/Core/Logger.h"

using namespace MR;

GLFWwindow* InputManager::m_WindowHandle = nullptr;

std::vector<int> InputManager::m_WatchedKeys = std::vector<int>();
std::unordered_map<int, float> InputManager::m_WatchedKeysElapsedTime = std::unordered_map<int, float>();
std::unordered_map<int, bool> InputManager::m_WatchedKeysDown = std::unordered_map<int, bool>();
std::unordered_map<int, bool> InputManager::m_WatchedKeysUp = std::unordered_map<int, bool>();
Vector2d InputManager::m_LastMousePosition = Vector2d(0, 0);
Vector2d InputManager::m_DeltaMousePosition = Vector2d(0, 0);

void InputManager::setWindowHandle(GLFWwindow* windowHandle) {
    InputManager::m_WindowHandle = windowHandle;
}

GLFWwindow* InputManager::getWindowHandle() {
    if (InputManager::m_WindowHandle == nullptr) {
        Logger::Log("InputManager", "Window handle not set!", Logger::LOG_ERROR);
    }
    return InputManager::m_WindowHandle;
}

void InputManager::update(float deltaTime) {
    GLFWwindow* windowHandle = InputManager::getWindowHandle();
    if (windowHandle == nullptr)
        return;
    
    for (unsigned int i = 0; i < InputManager::m_WatchedKeys.size(); i++) {
        int keyCode = InputManager::m_WatchedKeys[i];
        if (InputManager::getKey(keyCode)) {
            InputManager::m_WatchedKeysDown[keyCode] = (InputManager::m_WatchedKeysElapsedTime[keyCode] > 0);
            InputManager::m_WatchedKeysElapsedTime[keyCode] = 0;
            InputManager::m_WatchedKeysUp[keyCode] = false;
        } else {
            InputManager::m_WatchedKeysUp[keyCode] = (InputManager::m_WatchedKeysElapsedTime[keyCode] == 0);
            InputManager::m_WatchedKeysElapsedTime[keyCode] += deltaTime;
            InputManager::m_WatchedKeysDown[keyCode] = false;
        }
    }

    Vector2d currentMousePosition;
    getMousePosition(currentMousePosition);
    InputManager::m_DeltaMousePosition = currentMousePosition - InputManager::m_LastMousePosition;
    getMousePosition(InputManager::m_LastMousePosition); 
}

void InputManager::addKeyToWatch(int keyCode) {
    InputManager::m_WatchedKeys.push_back(keyCode);

    InputManager::m_WatchedKeysElapsedTime[keyCode] =  1.0f;
    InputManager::m_WatchedKeysDown[keyCode] =  false;
    InputManager::m_WatchedKeysUp[keyCode] =  false;
}

bool InputManager::getKey(int keyCode) {
    return (glfwGetKey(InputManager::getWindowHandle(), keyCode) == GLFW_PRESS);
}

bool InputManager::getKeyDown(int keyCode) {
    return InputManager::m_WatchedKeysDown[keyCode];
}

bool InputManager::getKeyUp(int keyCode) {
    return InputManager::m_WatchedKeysUp[keyCode];
}

void InputManager::getMousePosition(Vector2d &mousePos) {
    glfwGetCursorPos(InputManager::getWindowHandle(), &mousePos.x, &mousePos.y);
}

Vector2d InputManager::getMousePosition() {
    Vector2d mousePos;
    getMousePosition(mousePos);
    return mousePos;
}

void InputManager::getMouseDelta(Vector2d &mouseDelta) {
    mouseDelta.x = InputManager::m_DeltaMousePosition.x;
    mouseDelta.y = InputManager::m_DeltaMousePosition.y;
}

Vector2d InputManager::getMouseDelta() {
    return InputManager::m_DeltaMousePosition;
}

bool InputManager::getMouseButton(int mouseButtonCode) {
    return (glfwGetMouseButton(InputManager::getWindowHandle(), mouseButtonCode) == GLFW_PRESS);
}
