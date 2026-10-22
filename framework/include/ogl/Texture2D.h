#pragma once

#include <glad/glad.h>

#include <string>

namespace ogl
{

class Texture2D
{
public:
    Texture2D() = default;
    ~Texture2D();

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(Texture2D&& other) noexcept;

    static Texture2D FromFile(const std::string& relativeAssetPath, bool flipY = true);

    void Bind(unsigned int unit = 0) const;

    unsigned int Id() const { return m_id; }
    int Width() const { return m_width; }
    int Height() const { return m_height; }

private:
    unsigned int m_id = 0;
    int m_width = 0;
    int m_height = 0;
};

} // namespace ogl
