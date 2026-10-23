# 架构说明

## 总览

```text
opengl/
  framework/     → ogl_framework（静态库）
  libs/          → （规划）与 GL 解耦的算法/程序生成网格，如 procmesh、德劳内核心
  samples/       → 各篇博文 demo（可执行文件）
  assets/        → 共享资源（着色器、纹理、字体）
  external/imgui → Dear ImGui（编入 framework）
  cmake/         → 依赖（GLFW FetchContent）、可选 CUDA 工具链脚本
```

每个 sample 链接 `ogl_framework`，只保留本篇场景逻辑（几何、uniform、ImGui 面板）。窗口、输入、资源路径、ImGui 生命周期由 `ogl::Application` 统一处理。GLFW 不作为 vendored 头文件/预编译库，由根 CMake 通过 `cmake/Dependencies.cmake` 拉取源码并链接目标 `glfw`（详见 [build.md](build.md)）。

## Application 生命周期

1. `GlfwLifetime` — 进程内 `glfwInit` / `glfwTerminate`
2. `Window` — 创建 OpenGL 3.3 Core 上下文并加载 GLAD
3. `ImGuiLayer::Init`
4. `OnInit()` — sample 创建 VAO、Shader、纹理等
5. 主循环：
   - 计算 `dt` / FPS
   - `ProcessInput`（如 `Escape` 关闭窗口）
   - 鼠标回调驱动 `Camera`：左键轨道、中键平移、滚轮缩放（`WantCaptureMouse` 为真时不交给相机，见 [ui.md](ui.md)）
   - `ImGuiLayer::BeginFrame`
   - `OnUpdate` → `UpdateSceneBounds`（`OnGatherSceneBounds` + 可选首帧 `FrameScene`）→ `OnRender` → `OnImGui`
   - `ImGuiLayer::EndFrameAndRender`
   - `SwapBuffers` + `PollEvents`
6. `OnShutdown()` → `ImGuiLayer::Shutdown`

## 相机与场景包围盒

- `ogl::Camera` 为轨道相机：主状态为 `Target`、`OrbitRotation`（四元数）、`OrbitRestOffset` / `OrbitRestRight`（初始机位在 rest 系下的偏移与右轴）、`Distance`。每帧拖动：`OrbitRotation = q_yaw(WorldUp) * OrbitRotation * q_pitch(局部右轴)`（先世界 yaw，再 rest 系右轴上的局部 pitch），`OrbitOffsetDirection = OrbitRotation * OrbitRestOffset`。**勿**用 `cross(WorldUp, offset)` 当俯仰轴——过极点时该切向会翻转 180°，会导致 `lookAt` 的 up 突变；局部 pitch + 由四元数推出的 `Right()` 与常见 DCC/引擎轨道一致。`Yaw` / `Pitch` 仅为读数，过极点时可能跳变。
- Sample 通过 `OnGatherSceneBounds` 向 `SceneBounds` 上报几何包围盒；`Application` 在首帧（或调用 `RequestSceneFrame()` 后下一帧）用 `Camera::FrameScene` 将观察中心对准包围盒中心并调整距离与近远裁剪面。
- `Camera::ResetOrbitViewToNegativeZ` 将轨道旋转归零、视线沿世界 \f$-Z\f$（机位在目标 \f$+Z\f$ 侧）；常与 `FrameScene` 联用以把焦点拉回包围盒中心并重新拟合距离。
- `Camera::MakePickRay` 供日后其他主题（如物体拾取）使用；**德劳内系列** 在程序内生成点与插入序列，不依赖拾取。详见 [roadmap.md](roadmap.md)。

## 着色器管线（`ogl::Shader`）

- 两阶段：`Shader(vertPath, fragPath)`，与现有 sample 兼容。
- 三阶段：`Shader(vertPath, geomPath, fragPath)`；`geomPath` 为空时与两阶段等价。几何着色器文件与 vert/frag 同目录，命名 `*.geom`（粗线系列 `20-thick-lines-gs` 再新增具体 shader）。
- `Use`、`SetBool` / `SetInt` / `SetFloat` / `SetVec3f` / `SetVec4f` / `SetMat4`；程序对象在析构与移动时通过 `glDeleteProgram` 释放（`ID` 仍公开，`IsValid()` 可检查是否链接成功）。

## 网格与缓冲

| 类型 | 头文件 | 说明 |
|------|--------|------|
| `BufferUsage` | `ogl/BufferUsage.h` | `Static` / `Dynamic` / `Stream` → 对应 `GL_*_DRAW` |
| `VertexLayout` | `ogl/VertexLayout.h` | stride + attribute；`Position3()`、`Position3TexCoord2()`；在已绑定 VAO + `ARRAY_BUFFER` 上 `Apply()` |
| `VertexBuffer` | `ogl/VertexBuffer.h` | `SetData` / `SetSubData`，目标 `GL_ARRAY_BUFFER` |
| `IndexBuffer` | `ogl/IndexBuffer.h` | `SetData` / `SetSubData`，目标 `GL_ELEMENT_ARRAY_BUFFER`；默认索引类型 `GL_UNSIGNED_INT` |
| `VertexArray` | `ogl/VertexArray.h` | `SetVertexBuffer`、`SetIndexBuffer`；`SetInstanceBuffer`（在 `SetVertexBuffer` **之后**调用，`glVertexAttribDivisor`）；`DrawArrays` / `DrawElements` / `DrawElementsInstanced` |

**动态网格上传**（德劳内等）：拓扑与顶点数不变时优先 `SetSubData`；容量变大时用 `SetData` 重新分配。`01` / `02` 使用静态 `SetData`；实例化 API 已预留，大规模系列再改用。

## 规划中的 framework 能力

见 [roadmap.md](roadmap.md)：

- **调试与性能**：CPU/GPU 计时与 ImGui 统计面板。
- **程序生成网格**：共享模块生成球面、带孔面、立方体及劣质三角网输入，供德劳内与大规模 sample 使用。

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
