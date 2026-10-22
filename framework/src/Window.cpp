#include <ogl/Window.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace ogl
{

static bool s_glfwInitialized = false;

GlfwLifetime::GlfwLifetime()
{
    if (!s_glfwInitialized)
    {
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW\n";
            return;
        }
        s_glfwInitialized = true;
    }
}

GlfwLifetime::~GlfwLifetime()
{
    if (s_glfwInitialized)
    {
        glfwTerminate();
        s_glfwInitialized = false;
    }
}

bool GlfwLifetime::IsInitialized()
{
    return s_glfwInitialized;
}

Window::Window(int width, int height, const char* title)
    : m_width(width)
    , m_height(height)
{
    if (!GlfwLifetime::IsInitialized())
    {
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window)
    {
        std::cerr << "Failed to create GLFW window\n";
        return;
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        return;
    }

}

Window::~Window()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

bool Window::ShouldClose() const
{
    return m_window && glfwWindowShouldClose(m_window);
}

void Window::SwapBuffers()
{
    if (m_window)
    {
        glfwSwapBuffers(m_window);
    }
}

void Window::PollEvents()
{
    glfwPollEvents();
}

void Window::SetFramebufferSizeCallback(void (*callback)(GLFWwindow*, int, int))
{
    if (m_window)
    {
        glfwSetFramebufferSizeCallback(m_window, callback);
    }
}

void Window::SetMouseButtonCallback(void (*callback)(GLFWwindow*, int, int, int))
{
    if (m_window)
    {
        glfwSetMouseButtonCallback(m_window, callback);
    }
}

void Window::SetCursorPosCallback(void (*callback)(GLFWwindow*, double, double))
{
    if (m_window)
    {
        glfwSetCursorPosCallback(m_window, callback);
    }
}

void Window::SetScrollCallback(void (*callback)(GLFWwindow*, double, double))
{
    if (m_window)
    {
        glfwSetScrollCallback(m_window, callback);
    }
}

void Window::SetSize(int width, int height)
{
    m_width = width;
    m_height = height;
}

} // namespace ogl
