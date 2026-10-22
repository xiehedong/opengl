# 路线图与待建设能力

本文记录后续博文系列对仓库的规划，以及 **尚未实现** 的 framework / 构建升级项。实现某一项时，须在同一轮改动中更新本文件状态、`docs/architecture.md`、`docs/build.md` 与 `samples/README.md`（见 [.cursor/rules/docs-sync.mdc](../.cursor/rules/docs-sync.mdc)）。

## 博文系列（规划）

| 系列 | 编号区间（建议） | 主题概要 | 与仓库的约定 |
|------|------------------|----------|--------------|
| 德劳内三角剖分 | `10–19` | 2D 德劳内；逐点插入及加速结构；**曲面网格重剖分**（球面、带孔矩形面、立方体等），将狭长三角等不规则网格处理为更均匀的三角网格 | 点集与插入序列在 **程序内生成**，不依赖屏幕拾取加点；算法与 GL 解耦，可视化用 sample |
| 几何着色器粗线 | `20–29` | 线宽、带宽扩展、斜接（miter）限幅等 | **仅几何着色器（GS）** 路径，不做 CPU 扩线 / tess 对照 demo |
| 大规模网格渲染 | `30–39` | 数千万级三角网：瓶颈分析、深度排序、空间加速、实例化、合批等 | 网格 **程序生成**；大文件不进 git（见 [build.md](build.md)） |
| CUDA（子主题） | 归入 `30–39` 中对应篇 | 作为大规模优化的一种 **可选策略**（如视锥裁剪、compact、与 GL 缓冲互操作） | 独立 CMake 目标，`BUILD_CUDA=OFF` 默认；无 NVIDIA 时 CPU 路径可跑通小规模 |

### 曲面德劳内（术语）

本仓库所说的 **3D 德劳内** 指：在 **物体表面**（参数域或已知曲面嵌入）上对采样点做德劳内（或约束德劳内）三角剖分 / 重网格化，改善狭长三角与密度不均，**不是** 三维体网格四面体 Delaunay。

## Framework 待补充（规划）

下列能力计划在 `ogl_framework` 或紧邻的 `libs/` 中落地；具体 API 以实现时的头文件为准。

### 1. 管线与着色器

- 扩展 `ogl::Shader`（或等价类型）：支持 **vert + 可选 geom + frag**，与 `assets/shaders/` 命名约定（如 `*.geom`）。
- 粗线系列：输入拓扑为 **线段 + adjacency**（`GL_LINES_ADJACENCY`），GS 扩带宽与 miter 策略在着色器中完成。
- 现有 vert/frag 两阶段 sample **保持兼容**。

### 2. 网格与缓冲抽象

- 轻量封装：顶点布局、`VertexArray` / `VertexBuffer` / `IndexBuffer`（静态与动态更新）。
- 支持索引绘制、实例化（`glDrawElementsInstanced` 等）；大规模系列再视 **OpenGL 版本策略** 引入 indirect / MDI。
- 德劳内系列：动态三角网频繁改索引与顶点，需要 **动态 VBO/EBO** 与明确的 upload 策略（文档化）。

### 3. 调试与性能计量

- CPU：帧时间、各阶段耗时（排序、BVH 构建、德劳内插入等），ImGui 只读面板。
- GPU：`GL_TIME_ELAPSED` 或 timestamp query，区分上传与绘制。
- 大规模 sample：提供可开关的优化项，便于博文对比截图（排序 on/off、实例化 on/off、CUDA on/off）。

### 4. 程序生成网格

- 共享模块（建议 `libs/procmesh/` 或 `framework` 内 `ogl::ProcMesh`）：规则网格、细分曲面、带孔参数面、球面/立方体等 **可配置分辨率** 的三角网。
- 德劳内：生成 **故意劣质** 三角网（狭长三角）作为输入，再展示重剖分结果。
- 大规模：通过参数 `NxM` 控制规模，默认小规模便于 Debug；Release 跑分说明见 [build.md](build.md)。

### 不在规划内（明确排除）

- 通过 **鼠标拾取** 交互式逐点插入德劳内（`Camera::MakePickRay` 仍保留给未来其他主题，德劳内系列不使用）。

## 构建与工程升级

| 项 | 状态 | 说明 |
|----|------|------|
| Phase 1 预编译 `lib/glfw3.lib` | 已移除 | 由 FetchContent GLFW 替代 |
| **Phase 2** FetchContent 构建 GLFW | **已完成** | 见 [build.md](build.md)、`cmake/Dependencies.cmake` |
| `BUILD_CUDA` 可选目标 | 骨架已接入 | `cmake/Cuda.cmake`；大规模 / CUDA 篇再 `add_subdirectory` 目标；默认 OFF |
| `BUILD_TESTS` CPU 测试 | 骨架已接入 | `tests/` 含 smoke；德劳不变量、程序生成网格拓扑待 `libs/` 落地 |
| CI | 待做 | 依赖 Phase 2；配置生成 + 构建 `01`/`02` + 可选 tests |
| 大资源 | 约定 | 程序生成为主；外链或 Release 跑分文档 |

## OpenGL 版本策略

### 当前

- `Window` 请求 **OpenGL 3.3 Core**；GLAD 按 **gl=3.3 core** 生成（`include/glad/`）。
- 几何着色器、实例化等 **3.3 已包含** 的能力可直接使用。

### 能否「少改动」提高到更高版本？

- **仅提高上下文版本**（修改 `glfwWindowHint` 主次版本）：在多数驱动上 **现有 sample 通常仍可运行**，因为 3.3 API 仍是子集；**不必**为「申请 4.5 上下文」而改每个 sample。
- **限制**：GLAD 未生成的 **4.3+ 入口**（如 `glMultiDrawIndirect`、compute shader）在链接期/运行期 **不可用**；调高 hint **不等于** 自动获得新 API。
- **若大规模系列需要 MDI / compute（非 CUDA）**：需要 **重新生成 GLAD**（建议 4.5 或 4.6 Core）、更新最低驱动说明，并在一轮工程升级中与 Phase 2、文档同步。

### 建议目标（工程升级时）

- **公开基线**：读者文档仍写「OpenGL 3.3 Core 起」；实际上下文可申请 **4.5+** 以预留 MDI 等，由 CMake 选项或 `docs/build.md` 统一说明。
- **CUDA 路径**：依赖 CUDA Toolkit 与驱动，与是否 4.3 compute **无强制绑定**（互操作以 buffer 为主）。

## Sample 索引（占位）

发文后在 [samples/README.md](../samples/README.md) 填 `code_tag` 与博文 URL；实现 demo 时新增 `samples/NN-.../` 并调用 `ogl_add_sample`。

| 占位 Sample | 目录（建议） | 系列 |
|-------------|--------------|------|
| `10-delaunay-2d` | `samples/10-delaunay-2d` | 德劳内 |
| `11-delaunay-insert` | `samples/11-delaunay-insert` | 德劳内 |
| `12-surface-remesh` | `samples/12-surface-remesh` | 曲面重剖分 |
| `20-thick-lines-gs` | `samples/20-thick-lines-gs` | 粗线 GS |
| `30-massive-mesh` | `samples/30-massive-mesh` | 大规模（含可选 CUDA 开关） |

## 建议实施顺序

1. ~~构建升级（Phase 2 GLFW、CMake 选项骨架、文档）~~ **已完成**
2. 着色器多阶段 + 缓冲抽象 + 计时工具
3. 程序生成网格库 + 德劳内 2D sample
4. 曲面重剖分 sample
5. 粗线 GS sample
6. 大规模 mesh sample → 可选 CUDA 策略
