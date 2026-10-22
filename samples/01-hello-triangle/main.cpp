#include <ogl/Application.h>
#include <ogl/FileSystem.h>
#include <ogl/Shader.h>

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

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        return m_shader->ID != 0;
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

        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void OnImGui() override
    {
        ImGui::Begin("Hello Triangle");
        ImGui::ColorEdit4("Clear color", m_clearColor);
        ImGui::ColorEdit4("Triangle color", m_triColor);
        ImGui::Checkbox("Wireframe", &m_wireframe);
        ImGui::Text("FPS: %.1f", Fps());
        ImGui::End();
    }

    void OnShutdown() override
    {
        if (m_ebo != 0)
        {
            glDeleteBuffers(1, &m_ebo);
        }
        if (m_vbo != 0)
        {
            glDeleteBuffers(1, &m_vbo);
        }
        if (m_vao != 0)
        {
            glDeleteVertexArrays(1, &m_vao);
        }
    }

private:
    std::unique_ptr<ogl::Shader> m_shader;
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;
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
