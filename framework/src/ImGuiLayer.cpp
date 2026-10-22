#include <ogl/ImGuiLayer.h>
#include <ogl/FileSystem.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <filesystem>
#include <iostream>

namespace ogl
{

void ImGuiLayer::Init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    const auto fontPath = FileSystem::AssetPath("fonts/ui-cn.ttf");
    if (std::filesystem::exists(fontPath))
    {
        io.Fonts->AddFontFromFileTTF(fontPath.string().c_str(), 19.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    }
    else
    {
        std::cout << "ImGui: Chinese font not found at " << fontPath << ", using default font.\n";
    }

    m_initialized = true;
}

void ImGuiLayer::Shutdown()
{
    if (!m_initialized)
    {
        return;
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    m_initialized = false;
}

void ImGuiLayer::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::EndFrameAndRender()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool ImGuiLayer::WantsCaptureMouse() const
{
    return ImGui::GetIO().WantCaptureMouse;
}

bool ImGuiLayer::WantsCaptureKeyboard() const
{
    return ImGui::GetIO().WantCaptureKeyboard;
}

} // namespace ogl
