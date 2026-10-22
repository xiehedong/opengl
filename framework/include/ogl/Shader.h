#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <filesystem>
#include <string>

namespace ogl
{

class Shader
{
public:
    Shader(const std::filesystem::path& vertPath, const std::filesystem::path& fragPath);
    /// Pipeline order: vert → geom (optional) → frag. Empty @p geomPath skips geometry stage.
    Shader(const std::filesystem::path& vertPath,
           const std::filesystem::path& geomPath,
           const std::filesystem::path& fragPath);

    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    void Use() const;

    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3f(const std::string& name, float r, float g, float b) const;
    void SetVec4f(const std::string& name, float r, float g, float b, float a) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;

    bool IsValid() const { return ID != 0; }

    unsigned int ID = 0;

private:
    static std::string LoadFile(const std::filesystem::path& path);
    static unsigned int CompileStage(unsigned int type, const std::string& source);
    void LinkProgram(unsigned int vert, unsigned int geom, unsigned int frag);
    static void CheckCompileErrors(unsigned int shader, const std::string& type);
};

} // namespace ogl
