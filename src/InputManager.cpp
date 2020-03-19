#include <GLFW/glfw3.h>

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

void InputManager::SetWindowHandle(GLFWwindow* windowHandle) {
    InputManager::m_WindowHandle = windowHandle;
}

GLFWwindow* InputManager::GetWindowHandle() {
    if (InputManager::m_WindowHandle == nullptr) {
        Logger::Log("InputManager", "Window handle not set!", Logger::LOG_ERROR);
    }
    return InputManager::m_WindowHandle;
}

void InputManager::Update(float deltaTime) {
    GLFWwindow* windowHandle = InputManager::GetWindowHandle();
    if (windowHandle == nullptr)
        return;
    
    for (unsigned int i = 0; i < InputManager::m_WatchedKeys.size(); i++) {
        int keyCode = InputManager::m_WatchedKeys[i];
        if (InputManager::GetKey(keyCode)) {
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
    GetMousePosition(currentMousePosition);
    InputManager::m_DeltaMousePosition = currentMousePosition - InputManager::m_LastMousePosition;
    GetMousePosition(InputManager::m_LastMousePosition); 
}

void InputManager::AddKeyToWatch(int keyCode) {
    InputManager::m_WatchedKeys.push_back(keyCode);

    InputManager::m_WatchedKeysElapsedTime[keyCode] =  1.0f;
    InputManager::m_WatchedKeysDown[keyCode] =  false;
    InputManager::m_WatchedKeysUp[keyCode] =  false;
}

bool InputManager::GetKey(int keyCode) {
    return (glfwGetKey(InputManager::GetWindowHandle(), keyCode) == GLFW_PRESS);
}

bool InputManager::GetKeyDown(int keyCode) {
    return InputManager::m_WatchedKeysDown[keyCode];
}

bool InputManager::GetKeyUp(int keyCode) {
    return InputManager::m_WatchedKeysUp[keyCode];
}

void InputManager::GetMousePosition(Vector2d &mousePos) {
    glfwGetCursorPos(InputManager::GetWindowHandle(), &mousePos.x, &mousePos.y);
}

Vector2d InputManager::GetMousePosition() {
    Vector2d mousePos;
    GetMousePosition(mousePos);
    return mousePos;
}

void InputManager::GetMouseDelta(Vector2d &mouseDelta) {
    mouseDelta.x = InputManager::m_DeltaMousePosition.x;
    mouseDelta.y = InputManager::m_DeltaMousePosition.y;
}

Vector2d InputManager::GetMouseDelta() {
    return InputManager::m_DeltaMousePosition;
}

bool InputManager::GetMouseButton(int mouseButtonCode) {
    return (glfwGetMouseButton(InputManager::GetWindowHandle(), mouseButtonCode) == GLFW_PRESS);
}
