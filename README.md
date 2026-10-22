# opengl-blog

OpenGL 3.3 Core 博文配套源码：可复用的 `ogl_framework` 静态库 + 按篇划分的 `samples/` 可执行示例。

- 博客站点：[https://xiehedong.github.io/](https://xiehedong.github.io/)
- 代码仓库：[xiehedong/opengl](https://github.com/xiehedong/opengl)（若已发布）

## 环境

- Windows 10/11 x64
- Visual Studio 2022（MSVC x64）
- CMake 3.16+
- 可选：Ninja（Visual Studio 的「C++ CMake 工具」可安装）

**GLFW** 由 CMake **FetchContent** 在配置时拉取并构建（见 `docs/build.md`）；首次配置需网络访问 GitHub。Linux 等环境需自行安装 OpenGL / 窗口系统开发包。

## 构建

### Visual Studio（推荐）

1. 用 Visual Studio 打开文件夹 `opengl`，或打开已生成的 `build/vs` 解决方案。
2. 配置 CMake（若首次）：生成器选 **Visual Studio 17 2022**，架构 **x64**。
3. 将启动项设为 `01-hello-triangle` 或 `02-textured-cubes`，构建并运行。

命令行示例：

```powershell
cmake -S . -B build/vs -G "Visual Studio 17 2022" -A x64
cmake --build build/vs --config Debug
```

可执行文件与 `assets/` 位于各 sample 的输出目录，例如：

`build/vs/samples/02-textured-cubes/Debug/02-textured-cubes.exe`

构建后会自动将仓库根目录 `assets/` 复制到 exe 同级的 `assets/`。

### CMakeSettings（VS 打开文件夹）

默认 `CMakeSettings.json` 使用 Ninja，输出在 `build/x64-Debug`。若本机未安装 Ninja，请改用上文 Visual Studio 生成器，或安装 Ninja 后重试。

## Samples

| 可执行文件 | 目录 | 说明 |
|-----------|------|------|
| `01-hello-triangle` | `samples/01-hello-triangle` | 索引绘制矩形 / 着色器文件 |
| `02-textured-cubes` | `samples/02-textured-cubes` | 多立方体纹理混合 + ImGui 调参 |

索引表见 [samples/README.md](samples/README.md)。

## 文档

- [docs/build.md](docs/build.md) — 构建、资源路径、OpenGL 版本、常见错误
- [docs/architecture.md](docs/architecture.md) — framework 与 samples、与 Hexo 双仓关系
- [docs/roadmap.md](docs/roadmap.md) — 博文系列规划、待建设能力与工程升级
- [docs/ui.md](docs/ui.md) — Dear ImGui、输入抢占、中文字体

**维护约定**：功能、构建或示例行为有变动时，须在同一轮改动中同步更新上表及相关文档；Cursor 侧见 [.cursor/rules/docs-sync.mdc](.cursor/rules/docs-sync.mdc)（`alwaysApply`）。

## 资源路径

运行时代码通过 `ogl::FileSystem::AssetPath("相对路径")` 解析资源，例如 `shaders/basic.vert`。勿再使用 `../../../` 相对路径。

可选环境变量 `OGL_ASSETS_ROOT` 指向 `assets` 的父目录或等价根路径（见 `docs/build.md`）。
