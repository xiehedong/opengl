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

    void Use() const;

    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3f(const std::string& name, float r, float g, float b) const;
    void SetVec4f(const std::string& name, float r, float g, float b, float a) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;

    unsigned int ID = 0;

private:
    void CheckCompileErrors(unsigned int shader, const std::string& type) const;
};

} // namespace ogl
