#include <GLFW/glfw3.h>

#include "Mantaray/Core/InputManager.hpp"
#include "Mantaray/Core/Logger.hpp"

using namespace MR;

GLFWwindow* InputManager::WindowHandle = nullptr;

std::vector<int> InputManager::WatchedKeys = std::vector<int>();
std::unordered_map<int, float> InputManager::WatchedKeysElapsedTime = std::unordered_map<int, float>();
std::unordered_map<int, bool> InputManager::WatchedKeysDown = std::unordered_map<int, bool>();
std::unordered_map<int, bool> InputManager::WatchedKeysUp = std::unordered_map<int, bool>();
Vector2d InputManager::LastMousePosition = Vector2d(0, 0);
Vector2d InputManager::DeltaMousePosition = Vector2d(0, 0);

void InputManager::SetWindowHandle(GLFWwindow* windowHandle) {
    InputManager::WindowHandle = windowHandle;
}

GLFWwindow* InputManager::GetWindowHandle() {
    if (InputManager::WindowHandle == nullptr) {
        Logger::Log("InputManager", "Window handle not set!", Logger::LOG_ERROR);
    }
    return InputManager::WindowHandle;
}

void InputManager::Update(float deltaTime) {
    GLFWwindow* windowHandle = InputManager::GetWindowHandle();
    if (windowHandle == nullptr)
        return;
    
    for (unsigned int i = 0; i < InputManager::WatchedKeys.size(); i++) {
        int keyCode = InputManager::WatchedKeys[i];
        if (InputManager::GetKey(keyCode)) {
            InputManager::WatchedKeysDown[keyCode] = (InputManager::WatchedKeysElapsedTime[keyCode] > 0);
            InputManager::WatchedKeysElapsedTime[keyCode] = 0;
            InputManager::WatchedKeysUp[keyCode] = false;
        } else {
            InputManager::WatchedKeysUp[keyCode] = (InputManager::WatchedKeysElapsedTime[keyCode] == 0);
            InputManager::WatchedKeysElapsedTime[keyCode] += deltaTime;
            InputManager::WatchedKeysDown[keyCode] = false;
        }
    }

    Vector2d currentMousePosition;
    GetMousePosition(currentMousePosition);
    InputManager::DeltaMousePosition = currentMousePosition - InputManager::LastMousePosition;
    GetMousePosition(InputManager::LastMousePosition); 
}

void InputManager::AddKeyToWatch(int keyCode) {
    InputManager::WatchedKeys.push_back(keyCode);

    InputManager::WatchedKeysElapsedTime[keyCode] =  1.0f;
    InputManager::WatchedKeysDown[keyCode] =  false;
    InputManager::WatchedKeysUp[keyCode] =  false;
}

bool InputManager::GetKey(int keyCode) {
    return (glfwGetKey(InputManager::GetWindowHandle(), keyCode) == GLFW_PRESS);
}

bool InputManager::GetKeyDown(int keyCode) {
    return InputManager::WatchedKeysDown[keyCode];
}

bool InputManager::GetKeyUp(int keyCode) {
    return InputManager::WatchedKeysUp[keyCode];
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
    mouseDelta.x = InputManager::DeltaMousePosition.x;
    mouseDelta.y = InputManager::DeltaMousePosition.y;
}

Vector2d InputManager::GetMouseDelta() {
    return InputManager::DeltaMousePosition;
}

bool InputManager::GetMouseButton(int mouseButtonCode) {
    return (glfwGetMouseButton(InputManager::GetWindowHandle(), mouseButtonCode) == GLFW_PRESS);
}
