#pragma once

struct GLFWwindow;

namespace ogl
{

class GlfwLifetime
{
public:
    GlfwLifetime();
    ~GlfwLifetime();

    GlfwLifetime(const GlfwLifetime&) = delete;
    GlfwLifetime& operator=(const GlfwLifetime&) = delete;

    static bool IsInitialized();
};

class Window
{
public:
    Window(int width, int height, const char* title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool IsValid() const { return m_window != nullptr; }
    GLFWwindow* GetNative() const { return m_window; }

    int Width() const { return m_width; }
    int Height() const { return m_height; }
    void SetSize(int width, int height);

    bool ShouldClose() const;
    void SwapBuffers();
    void PollEvents();

    void SetFramebufferSizeCallback(void (*callback)(GLFWwindow*, int, int));
    void SetMouseButtonCallback(void (*callback)(GLFWwindow*, int, int, int));
    void SetCursorPosCallback(void (*callback)(GLFWwindow*, double, double));
    void SetScrollCallback(void (*callback)(GLFWwindow*, double, double));

private:
    GLFWwindow* m_window = nullptr;
    int m_width = 0;
    int m_height = 0;
};

} // namespace ogl
