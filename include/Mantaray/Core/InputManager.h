#pragma once

#include <unordered_map>
#include <vector>

#include "Mantaray/Core/Vector.h"

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
        static void GetMousePosition(Vector2<double> &mousePos);
        static Vector2<double> GetMousePosition();
        static void GetMouseDelta(Vector2<double> &mouseDelta);
        static Vector2<double> GetMouseDelta();
        static bool GetMouseButton(int mouseButtonCode);

    private:
        static class GLFWwindow* m_WindowHandle;

        static std::vector<int> m_WatchedKeys;
        static std::unordered_map<int, float> m_WatchedKeysElapsedTime;
        static std::unordered_map<int, bool> m_WatchedKeysDown;
        static std::unordered_map<int, bool> m_WatchedKeysUp;
        static Vector2<double> m_LastMousePosition;
        static Vector2<double> m_DeltaMousePosition;
};
}
