# 构建说明

## 目录约定

- `framework/` — `ogl_framework` 静态库（GLFW/GLAD、Shader、纹理、相机、Application、ImGui）
- `samples/` — 每篇博文对应的可执行目标
- `assets/` — 着色器、纹理、UI 字体（构建后复制到各 sample 输出目录）
- `external/imgui/` — Dear ImGui 源码（vendor）
- `include/` — GLAD、GLM（GLFW 由 CMake FetchContent 提供，见下文）
- `cmake/Dependencies.cmake` — 第三方依赖（当前为 GLFW 3.4）
- `tests/` — 可选 CPU 测试（`BUILD_TESTS=ON`）

## CMake 选项

| 选项 | 默认 | 说明 |
|------|------|------|
| `BUILD_SAMPLES` | ON | 是否构建 samples |
| `OGL_COPY_ASSETS` | ON | 构建后复制 `assets/` 到 exe 目录 |
| `BUILD_CUDA` | OFF | 启用 CUDA 工具链（为大规模 / CUDA 篇预留；无 CUDA 编译器时配置失败） |
| `BUILD_TESTS` | OFF | 构建 `tests/` 并注册 CTest（德劳内、程序网格等将在此扩展） |

## GLFW（FetchContent）

首次配置时 CMake 会从 GitHub 拉取 [GLFW 3.4](https://github.com/glfw/glfw) 并在本地构建，产物位于构建目录下的 `_deps/`（已随 `/build` 忽略，不入库）。

- 需能访问 GitHub；离线或内网可将本地克隆路径设为 `FETCHCONTENT_SOURCE_DIR_GLFW`（指向含 `CMakeLists.txt` 的 glfw 源码根目录）。
- 升级 GLFW 版本：修改 `cmake/Dependencies.cmake` 中的 `GIT_TAG`，然后 **重新配置** 构建目录。
- 从旧版预编译 `lib/glfw3.lib` 迁移后，请删除或清空旧的 `build/` 再 `cmake -S . -B ...`，避免混用旧缓存。

Linux 等非 Windows 环境需自行安装 OpenGL 与窗口系统开发包（如 `libgl-dev`、`libx11-dev` 等）；本期未在 CI 上验证 Linux 全链路。

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

## CPU 测试（可选）

```powershell
cmake -S . -B build/vs -G "Visual Studio 17 2022" -A x64 -DBUILD_TESTS=ON
cmake --build build/vs --config Debug --target ogl_test_smoke
ctest --test-dir build/vs -C Debug
```

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

## OpenGL 版本

- 上下文：当前由 `framework/src/Window.cpp` 请求 **OpenGL 3.3 Core**。
- Loader：`include/glad/` 按 **3.3 Core** 生成；未列入 GLAD 的更高版本 API 不可调用。
- 仅提高 GLFW 请求的主/次版本（如 4.5）时，现有 3.3 sample 在常见驱动上通常仍可运行，但 **不会** 自动获得 4.3+ 函数；若需 `MultiDrawIndirect`、compute 等，须重新生成 GLAD 并更新 [roadmap.md](roadmap.md) 与本文。
- 几何着色器粗线、实例化绘制在 **3.3** 即可；大规模优化中的 **CUDA** 为可选构建，见 [roadmap.md](roadmap.md)。

## 大规模网格与资源

- 优先 **程序生成** 三角网（分辨率由 sample / ImGui 参数控制），避免将千万级网格提交进 git。
- 性能对比请在 **Release** 下运行，并可在 ImGui 中关闭 vsync（若 sample 提供）；具体以各篇 demo 说明为准。
- 固定数据集若必须使用，采用 Git LFS 或博文外链，并在 `samples/README.md` 注明下载步骤。

## 常见错误

| 现象 | 处理 |
|------|------|
| `Failed to load texture` / 着色器读文件失败 | 确认 exe 旁存在 `assets/`；重新构建以触发 POST_BUILD 复制 |
| `Failed to initialize GLAD` | 显卡驱动、是否误用软件 GL |
| FetchContent / GLFW 下载失败 | 检查网络与 GitHub；或设置 `FETCHCONTENT_SOURCE_DIR_GLFW` |
| `BUILD_CUDA=ON` 配置失败 | 安装 NVIDIA CUDA Toolkit，或保持 `BUILD_CUDA=OFF` |
| ImGui 中文方块 | 确认 `assets/fonts/ui-cn.ttf` 存在；见 [ui.md](ui.md) |

## CI（下一阶段）

[roadmap.md](roadmap.md) 中规划的 GitHub Actions（配置 + 构建 `01`/`02` + 可选 `ctest`）依赖 Phase 2 GLFW FetchContent，**尚未接入**；接入后在本节补充工作流说明。
