#pragma once

#include <filesystem>
#include <string>

namespace ogl
{

class FileSystem
{
public:
    static std::filesystem::path AssetPath(const std::string& relative);
    static std::filesystem::path ExecutableDirectory();

private:
    static std::filesystem::path SourceAssetsRoot();
};

} // namespace ogl
