#include <ogl/Texture2D.h>

#include <ogl/FileSystem.h>

#include <iostream>

#include "stb_image.h"

namespace ogl
{

Texture2D::~Texture2D()
{
    if (m_id != 0)
    {
        glDeleteTextures(1, &m_id);
        m_id = 0;
    }
}

Texture2D::Texture2D(Texture2D&& other) noexcept
    : m_id(other.m_id)
    , m_width(other.m_width)
    , m_height(other.m_height)
{
    other.m_id = 0;
    other.m_width = 0;
    other.m_height = 0;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
    if (this != &other)
    {
        if (m_id != 0)
        {
            glDeleteTextures(1, &m_id);
        }
        m_id = other.m_id;
        m_width = other.m_width;
        m_height = other.m_height;
        other.m_id = 0;
        other.m_width = 0;
        other.m_height = 0;
    }
    return *this;
}

Texture2D Texture2D::FromFile(const std::string& relativeAssetPath, bool flipY)
{
    Texture2D texture;
    stbi_set_flip_vertically_on_load(flipY ? 1 : 0);

    const auto path = FileSystem::AssetPath(relativeAssetPath);
    int channels = 0;
    unsigned char* data = stbi_load(path.string().c_str(), &texture.m_width, &texture.m_height, &channels, 0);
    if (!data)
    {
        std::cout << "Failed to load texture: " << path << std::endl;
        return texture;
    }

    GLenum format = GL_RGB;
    GLenum internal = GL_RGB;
    if (channels == 4)
    {
        format = GL_RGBA;
        internal = GL_RGBA;
    }

    glGenTextures(1, &texture.m_id);
    glBindTexture(GL_TEXTURE_2D, texture.m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, internal, texture.m_width, texture.m_height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return texture;
}

void Texture2D::Bind(unsigned int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

} // namespace ogl
