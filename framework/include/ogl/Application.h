#pragma once

#include <ogl/Camera.h>
#include <ogl/ImGuiLayer.h>
#include <ogl/SceneBounds.h>
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
    virtual void OnGatherSceneBounds(SceneBounds& bounds) {}

    Window& GetWindow() { return m_window; }
    Camera& GetCamera() { return m_camera; }
    const SceneBounds& GetSceneBounds() const { return m_sceneBounds; }
    ImGuiLayer& GetImGuiLayer() { return m_imguiLayer; }

    void RequestSceneFrame() { m_autoFrameScene = true; }

    float DeltaTime() const { return m_deltaTime; }
    float Fps() const { return m_fps; }

private:
    void ProcessInput();
    void UpdateTiming();
    void UpdateSceneBounds();

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    GlfwLifetime m_glfwLifetime;
    Window m_window;
    Camera m_camera;
    SceneBounds m_sceneBounds;
    ImGuiLayer m_imguiLayer;

    std::string m_title;
    float m_deltaTime = 0.0f;
    float m_lastFrame = 0.0f;
    float m_fps = 0.0f;

    bool m_autoFrameScene = true;
    bool m_orbitDragActive = false;
    bool m_panDragActive = false;
    bool m_skipNextMouseDelta = false;
    float m_lastMouseX = 0.0f;
    float m_lastMouseY = 0.0f;
};

} // namespace ogl
