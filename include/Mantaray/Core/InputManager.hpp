#pragma once

#include <unordered_map>
#include <vector>

#include "Mantaray/Core/Vector.hpp"

namespace MR {
class InputManager{
    public:
        static void SetWindowHandle(class GLFWwindow* windowHandle);
        static class GLFWwindow* GetWindowHandle();

        static void Update(float deltaTime);

        static void AddKeyToWatch(int keyCode);

        static bool GetKey(int keyCode);
        static bool GetKeyDown(int keyCode);
        static bool GetKeyUp(int keyCode);
        static void GetMousePosition(Vector2d &mousePos);
        static Vector2d GetMousePosition();
        static void GetMouseDelta(Vector2d &mouseDelta);
        static Vector2d GetMouseDelta();
        static bool GetMouseButton(int mouseButtonCode);

    private:
        static class GLFWwindow* WindowHandle;

        static std::vector<int> WatchedKeys;
        static std::unordered_map<int, float> WatchedKeysElapsedTime;
        static std::unordered_map<int, bool> WatchedKeysDown;
        static std::unordered_map<int, bool> WatchedKeysUp;
        static Vector2d LastMousePosition;
        static Vector2d DeltaMousePosition;
};
}
