#include <ogl/Application.h>
#include <ogl/FileSystem.h>
#include <ogl/Shader.h>
#include <ogl/Texture2D.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

#include <cmath>
#include <cstdlib>
#include <memory>
#include <vector>

namespace
{

class TexturedCubesApp : public ogl::Application
{
public:
    TexturedCubesApp()
        : Application("02-textured-cubes")
    {
    }

protected:
    bool OnInit() override
    {
        m_shader = std::make_unique<ogl::Shader>(
            ogl::FileSystem::AssetPath("shaders/basic.vert"),
            ogl::FileSystem::AssetPath("shaders/basic.frag"));

        m_texture1 = ogl::Texture2D::FromFile("textures/container.jpg");
        m_texture2 = ogl::Texture2D::FromFile("textures/awesomeface.png");

        const float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        };

        m_cubePositions = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f),
        };

        std::srand(0);
        m_cubeAngles.resize(m_cubePositions.size());
        for (size_t i = 0; i < m_cubeAngles.size(); ++i)
        {
            m_cubeAngles[i] = static_cast<float>((std::rand() % 150) + 1);
        }

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        m_shader->Use();
        m_shader->SetInt("texture1", 0);
        m_shader->SetInt("texture2", 1);

        glEnable(GL_DEPTH_TEST);
        return m_shader->ID != 0 && m_texture1.Id() != 0 && m_texture2.Id() != 0;
    }

    void OnUpdate(float dt) override
    {
        if (m_animateInterpolation)
        {
            const float timeValue = static_cast<float>(glfwGetTime());
            m_interpolation = std::sin(timeValue) / 2.0f + 1.0f;
        }
    }

    void OnRender() override
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_texture1.Bind(0);
        m_texture2.Bind(1);

        m_shader->Use();
        m_shader->SetFloat("interpolation", m_interpolation);

        const int width = GetWindow().Width();
        const int height = GetWindow().Height();
        const float aspect = height > 0 ? static_cast<float>(width) / static_cast<float>(height) : 1.0f;

        m_shader->SetMat4("view", GetCamera().GetViewMatrix());
        const glm::mat4 proj = glm::perspective(glm::radians(GetCamera().Zoom), aspect, 0.1f, 100.0f);
        m_shader->SetMat4("proj", proj);

        glBindVertexArray(m_vao);
        const float timeValue = static_cast<float>(glfwGetTime());
        for (size_t i = 0; i < m_cubePositions.size(); ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m_cubePositions[i]);
            const float angle = m_cubeAngles[i] * timeValue;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            m_shader->SetMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
    }

    void OnImGui() override
    {
        ImGui::Begin("Textured Cubes");
        ImGui::SliderFloat("interpolation", &m_interpolation, 0.0f, 1.0f);
        ImGui::Checkbox("Animate interpolation", &m_animateInterpolation);
        ImGui::SliderFloat("Camera zoom", &GetCamera().Zoom, 1.0f, 90.0f);
        ImGui::Text("FPS: %.1f", Fps());
        ImGui::Text("WASD: move camera");
        ImGui::End();
    }

    void OnShutdown() override
    {
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
    ogl::Texture2D m_texture1;
    ogl::Texture2D m_texture2;

    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;

    std::vector<glm::vec3> m_cubePositions;
    std::vector<float> m_cubeAngles;

    float m_interpolation = 0.2f;
    bool m_animateInterpolation = true;
};

} // namespace

int main()
{
    TexturedCubesApp app;
    return app.Run();
}
