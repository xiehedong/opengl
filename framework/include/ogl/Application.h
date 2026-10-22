#pragma once

#include <ogl/Camera.h>
#include <ogl/ImGuiLayer.h>
#include <ogl/Window.h>

#include <string>

struct GLFWwindow;

namespace ogl
{

class Application
{
public:
    explicit Application(const char* title, int width = 1280, int height = 720);
    virtual ~Application() = default;

    int Run();

protected:
    virtual bool OnInit() { return true; }
    virtual void OnUpdate(float dt) {}
    virtual void OnRender() {}
    virtual void OnImGui() {}
    virtual void OnShutdown() {}

    Window& GetWindow() { return m_window; }
    Camera& GetCamera() { return m_camera; }
    ImGuiLayer& GetImGuiLayer() { return m_imguiLayer; }

    float DeltaTime() const { return m_deltaTime; }
    float Fps() const { return m_fps; }

private:
    void ProcessInput();
    void UpdateTiming();

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    GlfwLifetime m_glfwLifetime;
    Window m_window;
    Camera m_camera;
    ImGuiLayer m_imguiLayer;

    std::string m_title;
    float m_deltaTime = 0.0f;
    float m_lastFrame = 0.0f;
    float m_fps = 0.0f;

    bool m_firstMouse = true;
    float m_lastMouseX = 0.0f;
    float m_lastMouseY = 0.0f;
};

} // namespace ogl
