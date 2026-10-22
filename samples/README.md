# Samples 索引

与 Hexo 博文一一对应（文稿在 [xiehedong.github.io](https://github.com/xiehedong/xiehedong.github.io) `source` 分支）。

## 已发布

| Sample | 目录 | 知识点 | 建议 `code_tag` | 博文 URL |
|--------|------|--------|-----------------|----------|
| `01-hello-triangle` | `samples/01-hello-triangle` | VAO/VBO/EBO、着色器文件、ImGui 清屏色 | `post-01-hello-triangle` | （待填） |
| `02-textured-cubes` | `samples/02-textured-cubes` | 纹理、MVP、多物体、轨道相机（鼠标）、场景包围盒、ImGui 调参 | `post-02-textured-cubes` | （待填） |

## 规划中

实现后移入上表；系列说明与 framework 待办见 [docs/roadmap.md](../docs/roadmap.md)。

| Sample（占位） | 目录（建议） | 系列 | 知识点（概要） |
|----------------|--------------|------|----------------|
| `10-delaunay-2d` | `samples/10-delaunay-2d` | 德劳内 | 2D 德劳内可视化；程序生成点集 |
| `11-delaunay-insert` | `samples/11-delaunay-insert` | 德劳内 | 逐点插入、加速结构 |
| `12-surface-remesh` | `samples/12-surface-remesh` | 德劳内 | 球面 / 带孔面 / 立方体等曲面重剖分，均匀三角网 |
| `20-thick-lines-gs` | `samples/20-thick-lines-gs` | 粗线 | 几何着色器扩线宽、miter |
| `30-massive-mesh` | `samples/30-massive-mesh` | 大规模 | 程序生成超大网格；排序、空间加速、实例化；可选 CUDA |

## Front matter 示例

```yaml
series: opengl
repo: https://github.com/xiehedong/opengl
sample: samples/02-textured-cubes
code_tag: post-02-textured-cubes
```

德劳内 / 粗线 / 大规模子系列可在 Hexo 文稿中增加 `subseries`（如 `delaunay`、`thick-lines`、`massive-mesh`），与 [roadmap.md](../docs/roadmap.md) 保持一致。

发文后在本仓库打与 `code_tag` 同名的 git tag。
