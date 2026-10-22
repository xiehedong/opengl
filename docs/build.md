# 构建说明

## 目录约定

- `framework/` — `ogl_framework` 静态库（GLFW/GLAD、Shader、纹理、相机、Application、ImGui）
- `samples/` — 每篇博文对应的可执行目标
- `assets/` — 着色器、纹理、UI 字体（构建后复制到各 sample 输出目录）
- `external/imgui/` — Dear ImGui 源码（vendor）
- `include/` — GLAD、GLFW 头文件、GLM
- `lib/glfw3.lib` — Phase 1 预编译 GLFW（MSVC x64）

## CMake 选项

| 选项 | 默认 | 说明 |
|------|------|------|
| `BUILD_SAMPLES` | ON | 是否构建 samples |
| `OGL_COPY_ASSETS` | ON | 构建后复制 `assets/` 到 exe 目录 |

## Visual Studio 2022

```powershell
cmake -S . -B build/vs -G "Visual Studio 17 2022" -A x64
cmake --build build/vs --config Debug
```

Debug / Release 均需指定 `--config`。

## Ninja（CMakeSettings.json）

`CMakeSettings.json` 中 `buildRoot` 为 `${projectDir}\build\${name}`。需已安装 Ninja 且 CMake 能发现编译器（通常通过「x64 Native Tools」或 VS 的 CMake 集成）。

```powershell
cmake -S . -B build/x64-Debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build/x64-Debug
```

若提示找不到 Ninja，请安装 [Ninja](https://ninja-build.org/) 或改用 Visual Studio 生成器。

## 资源与环境变量

解析顺序（`ogl::FileSystem::AssetPath`）：

1. `OGL_ASSETS_ROOT` + 相对路径（若文件存在）
2. `<exe 目录>/assets/` + 相对路径
3. 编译时记录的源码树 `assets/`（便于在 IDE 中直接运行未复制资源的情况）

设置示例（PowerShell）：将 `OGL_ASSETS_ROOT` 设为仓库内 **`assets` 目录** 的绝对路径：

```powershell
$env:OGL_ASSETS_ROOT = "C:\path\to\opengl\assets"
```

代码中仍使用 `FileSystem::AssetPath("shaders/basic.vert")` 这类相对路径。

修正：读 FileSystem.cpp - env is `root / rel` where root is OGL_ASSETS_ROOT. So OGL_ASSETS_ROOT should be the assets folder path, and relative is "shaders/basic.vert". Good.

## 常见错误

| 现象 | 处理 |
|------|------|
| `Failed to load texture` / 着色器读文件失败 | 确认 exe 旁存在 `assets/`；重新构建以触发 POST_BUILD 复制 |
| `Failed to initialize GLAD` | 显卡驱动、是否误用软件 GL |
| 链接 `glfw3.lib` 失败 | 仅验证过 **MSVC x64**；勿用 MinGW 链同一 lib |
| ImGui 中文方块 | 确认 `assets/fonts/ui-cn.ttf` 存在；见 [ui.md](ui.md) |

## Phase 2（可选）

计划改用 FetchContent 构建 GLFW，可删除 `lib/glfw3.lib` 依赖。届时会更新本文件。
