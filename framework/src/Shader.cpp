#include <ogl/Shader.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

namespace ogl
{

Shader::Shader(const std::filesystem::path& vertPath, const std::filesystem::path& fragPath)
    : Shader(vertPath, std::filesystem::path{}, fragPath)
{
}

Shader::Shader(const std::filesystem::path& vertPath,
               const std::filesystem::path& geomPath,
               const std::filesystem::path& fragPath)
{
    const std::string vertexCode = LoadFile(vertPath);
    const std::string fragmentCode = LoadFile(fragPath);

    const unsigned int vertex = CompileStage(GL_VERTEX_SHADER, vertexCode);
    unsigned int geometry = 0;
    if (!geomPath.empty())
    {
        const std::string geometryCode = LoadFile(geomPath);
        geometry = CompileStage(GL_GEOMETRY_SHADER, geometryCode);
    }
    const unsigned int fragment = CompileStage(GL_FRAGMENT_SHADER, fragmentCode);

    LinkProgram(vertex, geometry, fragment);

    glDeleteShader(vertex);
    if (geometry != 0)
    {
        glDeleteShader(geometry);
    }
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    if (ID != 0)
    {
        glDeleteProgram(ID);
        ID = 0;
    }
}

Shader::Shader(Shader&& other) noexcept
    : ID(other.ID)
{
    other.ID = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        if (ID != 0)
        {
            glDeleteProgram(ID);
        }
        ID = other.ID;
        other.ID = 0;
    }
    return *this;
}

std::string Shader::LoadFile(const std::filesystem::path& path)
{
    std::string code;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file.open(path);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        code = stream.str();
    }
    catch (const std::ifstream::failure&)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << path << std::endl;
    }
    return code;
}

unsigned int Shader::CompileStage(unsigned int type, const std::string& source)
{
    const char* sourceCStr = source.c_str();
    const unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);

    std::string typeName;
    switch (type)
    {
    case GL_VERTEX_SHADER:
        typeName = "VERTEX";
        break;
    case GL_GEOMETRY_SHADER:
        typeName = "GEOMETRY";
        break;
    case GL_FRAGMENT_SHADER:
        typeName = "FRAGMENT";
        break;
    default:
        typeName = "UNKNOWN";
        break;
    }
    CheckCompileErrors(shader, typeName);
    return shader;
}

void Shader::LinkProgram(unsigned int vert, unsigned int geom, unsigned int frag)
{
    ID = glCreateProgram();
    glAttachShader(ID, vert);
    if (geom != 0)
    {
        glAttachShader(ID, geom);
    }
    glAttachShader(ID, frag);
    glLinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");
}

void Shader::CheckCompileErrors(unsigned int shader, const std::string& type)
{
    int success = 0;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Shader::Use() const
{
    glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec3f(const std::string& name, float r, float g, float b) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), r, g, b);
}

void Shader::SetVec4f(const std::string& name, float r, float g, float b, float a) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), r, g, b, a);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

} // namespace ogl
