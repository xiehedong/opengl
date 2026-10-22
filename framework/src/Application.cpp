#include <ogl/Application.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>

namespace ogl
{

Application::Application(const char* title, int width, int height)
    : m_window(width, height, title)
    , m_camera(glm::vec3(0.0f, 0.0f, 10.0f))
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

void Application::ProcessInput()
{
    GLFWwindow* window = m_window.GetNative();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (m_imguiLayer.WantsCaptureKeyboard())
    {
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_camera.ProcessKeyboard(CameraMovement::Forward, m_deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_camera.ProcessKeyboard(CameraMovement::Backward, m_deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_camera.ProcessKeyboard(CameraMovement::Left, m_deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_camera.ProcessKeyboard(CameraMovement::Right, m_deltaTime);
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

void Application::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app)
    {
        return;
    }

    if (ImGui::GetIO().WantCaptureMouse)
    {
        app->m_firstMouse = true;
        return;
    }

    const float xf = static_cast<float>(xpos);
    const float yf = static_cast<float>(ypos);

    if (app->m_firstMouse)
    {
        app->m_lastMouseX = xf;
        app->m_lastMouseY = yf;
        app->m_firstMouse = false;
    }

    const float xoffset = xf - app->m_lastMouseX;
    const float yoffset = app->m_lastMouseY - yf;
    app->m_lastMouseX = xf;
    app->m_lastMouseY = yf;

    app->m_camera.ProcessMouseMovement(xoffset, yoffset);
}

void Application::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app || ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }
    app->m_camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

} // namespace ogl
