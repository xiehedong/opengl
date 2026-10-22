#pragma once

struct GLFWwindow;

namespace ogl
{

class ImGuiLayer
{
public:
    void Init(GLFWwindow* window);
    void Shutdown();

    void BeginFrame();
    void EndFrameAndRender();

    bool WantsCaptureMouse() const;
    bool WantsCaptureKeyboard() const;

private:
    bool m_initialized = false;
};

} // namespace ogl
