#include <unordered_map>
#include <vector>

#include <GLFW/glfw3.h>

#include "Mantaray/Core/Vector.h"

namespace MR {
class InputManager{
    public:
        static void SetWindowHandle(GLFWwindow* windowHandle);
        static GLFWwindow* GetWindowHandle();

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
        static GLFWwindow* m_WindowHandle;

        static std::vector<int> m_WatchedKeys;
        static std::unordered_map<int, float> m_WatchedKeysElapsedTime;
        static std::unordered_map<int, bool> m_WatchedKeysDown;
        static std::unordered_map<int, bool> m_WatchedKeysUp;
        static Vector2d m_LastMousePosition;
        static Vector2d m_DeltaMousePosition;
};
}
