#include <ogl/FileSystem.h>

#include <cstdlib>
#include <fstream>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

namespace ogl
{

std::filesystem::path FileSystem::SourceAssetsRoot()
{
#ifdef OGL_PROJECT_SOURCE_DIR
    return std::filesystem::path(OGL_PROJECT_SOURCE_DIR) / "assets";
#else
    return std::filesystem::path("assets");
#endif
}

std::filesystem::path FileSystem::ExecutableDirectory()
{
#ifdef _WIN32
    wchar_t buffer[MAX_PATH];
    const DWORD length = GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    if (length == 0 || length == MAX_PATH)
    {
        return std::filesystem::current_path();
    }
    return std::filesystem::path(buffer).parent_path();
#else
    return std::filesystem::current_path();
#endif
}

std::filesystem::path FileSystem::AssetPath(const std::string& relative)
{
    const std::filesystem::path rel(relative);

    if (const char* envRoot = std::getenv("OGL_ASSETS_ROOT"))
    {
        const auto root = std::filesystem::path(envRoot);
        const auto candidate = root / rel;
        if (std::filesystem::exists(candidate))
        {
            return candidate;
        }
    }

    const auto besideExe = ExecutableDirectory() / "assets" / rel;
    if (std::filesystem::exists(besideExe))
    {
        return besideExe;
    }

    const auto sourceAssets = SourceAssetsRoot() / rel;
    if (std::filesystem::exists(sourceAssets))
    {
        return sourceAssets;
    }

    return besideExe;
}

} // namespace ogl
