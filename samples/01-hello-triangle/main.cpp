#include <ogl/Application.h>
#include <ogl/FileSystem.h>
#include <ogl/IndexBuffer.h>
#include <ogl/Shader.h>
#include <ogl/VertexArray.h>
#include <ogl/VertexBuffer.h>
#include <ogl/VertexLayout.h>

#include <glad/glad.h>

#include <imgui.h>

#include <memory>

namespace
{

class HelloTriangleApp : public ogl::Application
{
public:
    HelloTriangleApp()
        : Application("01-hello-triangle")
    {
    }

protected:
    bool OnInit() override
    {
        m_shader = std::make_unique<ogl::Shader>(
            ogl::FileSystem::AssetPath("shaders/hello.vert"),
            ogl::FileSystem::AssetPath("shaders/hello.frag"));

        const float vertices[] = {
            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
        };

        const unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
        };

        m_vbo.SetData(sizeof(vertices), vertices);
        m_ibo.SetData(sizeof(indices), indices);
        m_mesh.SetVertexBuffer(m_vbo, ogl::VertexLayout::Position3());
        m_mesh.SetIndexBuffer(m_ibo);

        return m_shader->IsValid();
    }

    void OnGatherSceneBounds(ogl::SceneBounds& bounds) override
    {
        bounds.Include(glm::vec3(-0.5f, 0.5f, 0.0f));
        bounds.Include(glm::vec3(0.5f, 0.5f, 0.0f));
        bounds.Include(glm::vec3(0.5f, -0.5f, 0.0f));
        bounds.Include(glm::vec3(-0.5f, -0.5f, 0.0f));
    }

    void OnRender() override
    {
        glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        if (m_wireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        m_shader->Use();
        m_shader->SetVec4f("uColor", m_triColor[0], m_triColor[1], m_triColor[2], m_triColor[3]);

        m_mesh.DrawElements(GL_TRIANGLES, 6);
    }

    void OnImGui() override
    {
        ImGui::Begin("Hello Triangle");
        ImGui::ColorEdit4("Clear color", m_clearColor);
        ImGui::ColorEdit4("Triangle color", m_triColor);
        ImGui::Checkbox("Wireframe", &m_wireframe);
        ImGui::Text("FPS: %.1f", Fps());
        ImGui::Text("LMB: orbit, MMB: pan, wheel: zoom");
        ImGui::End();
    }

private:
    std::unique_ptr<ogl::Shader> m_shader;
    ogl::VertexBuffer m_vbo;
    ogl::IndexBuffer m_ibo;
    ogl::VertexArray m_mesh;
    float m_clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    float m_triColor[4] = {1.0f, 0.5f, 0.2f, 1.0f};
    bool m_wireframe = false;
};

} // namespace

int main()
{
    HelloTriangleApp app;
    return app.Run();
}
