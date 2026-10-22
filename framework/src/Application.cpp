#include <ogl/Application.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>

namespace ogl
{

Application::Application(const char* title, int width, int height)
    : m_window(width, height, title)
    , m_title(title)
    , m_lastMouseX(static_cast<float>(width) * 0.5f)
    , m_lastMouseY(static_cast<float>(height) * 0.5f)
{
}

int Application::Run()
{
    if (!m_window.IsValid())
    {
        return -1;
    }

    glfwSetWindowUserPointer(m_window.GetNative(), this);

    m_window.SetFramebufferSizeCallback(&Application::FramebufferSizeCallback);
    m_window.SetMouseButtonCallback(&Application::MouseButtonCallback);
    m_window.SetCursorPosCallback(&Application::MouseCallback);
    m_window.SetScrollCallback(&Application::ScrollCallback);

    m_imguiLayer.Init(m_window.GetNative());

    if (!OnInit())
    {
        m_imguiLayer.Shutdown();
        return -1;
    }

    while (!m_window.ShouldClose())
    {
        UpdateTiming();
        ProcessInput();

        m_imguiLayer.BeginFrame();
        OnUpdate(m_deltaTime);
        UpdateSceneBounds();
        OnRender();
        OnImGui();
        m_imguiLayer.EndFrameAndRender();

        m_window.SwapBuffers();
        m_window.PollEvents();
    }

    OnShutdown();
    m_imguiLayer.Shutdown();
    return 0;
}

void Application::UpdateTiming()
{
    const float currentFrame = static_cast<float>(glfwGetTime());
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;
    if (m_deltaTime > 0.0f)
    {
        m_fps = 1.0f / m_deltaTime;
    }
}

void Application::UpdateSceneBounds()
{
    m_sceneBounds.Reset();
    OnGatherSceneBounds(m_sceneBounds);

    if (!m_autoFrameScene || !m_sceneBounds.IsValid())
    {
        return;
    }

    const int width = m_window.Width();
    const int height = m_window.Height();
    const float aspect = height > 0 ? static_cast<float>(width) / static_cast<float>(height) : 1.0f;
    m_camera.FrameScene(m_sceneBounds.Box(), aspect);
    m_autoFrameScene = false;
}

void Application::ProcessInput()
{
    GLFWwindow* window = m_window.GetNative();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    if (auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window)))
    {
        app->m_window.SetSize(width, height);
    }
}

void Application::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app)
    {
        return;
    }

    if (ImGui::GetIO().WantCaptureMouse)
    {
        if (action == GLFW_RELEASE)
        {
            app->m_orbitDragActive = false;
            app->m_panDragActive = false;
        }
        return;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            app->m_orbitDragActive = true;
            app->m_skipNextMouseDelta = true;
        }
        else if (action == GLFW_RELEASE)
        {
            app->m_orbitDragActive = false;
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        if (action == GLFW_PRESS)
        {
            app->m_panDragActive = true;
            app->m_skipNextMouseDelta = true;
        }
        else if (action == GLFW_RELEASE)
        {
            app->m_panDragActive = false;
        }
    }
}

void Application::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app)
    {
        return;
    }

    if (ImGui::GetIO().WantCaptureMouse)
    {
        app->m_skipNextMouseDelta = true;
        return;
    }

    if (!app->m_orbitDragActive && !app->m_panDragActive)
    {
        return;
    }

    const float xf = static_cast<float>(xpos);
    const float yf = static_cast<float>(ypos);

    if (app->m_skipNextMouseDelta)
    {
        app->m_lastMouseX = xf;
        app->m_lastMouseY = yf;
        app->m_skipNextMouseDelta = false;
        return;
    }

    const float xoffset = xf - app->m_lastMouseX;
    const float yoffset = app->m_lastMouseY - yf;
    app->m_lastMouseX = xf;
    app->m_lastMouseY = yf;

    if (app->m_orbitDragActive)
    {
        app->m_camera.ProcessOrbit(xoffset, yoffset);
    }
    if (app->m_panDragActive)
    {
        app->m_camera.ProcessPan(xoffset, yoffset);
    }
}

void Application::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app || ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }
    app->m_camera.ProcessScroll(static_cast<float>(yoffset));
}

} // namespace ogl
