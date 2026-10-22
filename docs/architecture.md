# 架构说明

## 总览

```text
opengl/
  framework/     → ogl_framework（静态库）
  samples/       → 各篇博文 demo（可执行文件）
  assets/        → 共享资源（着色器、纹理、字体）
  external/imgui → Dear ImGui（编入 framework）
```

每个 sample 链接 `ogl_framework`，只保留本篇场景逻辑（几何、uniform、ImGui 面板）。窗口、输入、资源路径、ImGui 生命周期由 `ogl::Application` 统一处理。

## Application 生命周期

1. `GlfwLifetime` — 进程内 `glfwInit` / `glfwTerminate`
2. `Window` — 创建 OpenGL 3.3 Core 上下文并加载 GLAD
3. `ImGuiLayer::Init`
4. `OnInit()` — sample 创建 VAO、Shader、纹理等
5. 主循环：
   - 计算 `dt` / FPS
   - 键盘移动（若 ImGui 未抢占）
   - `ImGuiLayer::BeginFrame`
   - `OnUpdate` → `OnRender` → `OnImGui`
   - `ImGuiLayer::EndFrameAndRender`
   - `SwapBuffers` + `PollEvents`
6. `OnShutdown()` → `ImGuiLayer::Shutdown`

## 与 Hexo 博客仓库的关系

- **博文 Markdown** 只放在 `xiehedong.github.io` 的 `source` 分支（`source/_posts/`）。
- **本仓库** 提供可编译源码与 `samples/README.md` 索引。
- 建议在文章 front matter 中注明 `sample` 路径与 `code_tag`，发文后在本仓库打同名 git tag，便于读者 checkout。

本仓库 **不** 存放 Hexo 文稿或部署后的 `master` 分支内容。

## 扩展新 sample

1. 在 `samples/` 下新建 `NN-topic/` 与 `CMakeLists.txt`，调用 `ogl_add_sample`。
2. 继承 `ogl::Application`，实现 `OnInit` / `OnRender` / 可选 `OnImGui`。
3. 资源放入根目录 `assets/`（或日后 `samples/NN/assets/` overlay）。
4. 更新 `samples/README.md` 与博客 front matter。
