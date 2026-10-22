# Dear ImGui 集成

## 渲染顺序

场景在 `OnRender()` 中绘制；ImGui 在 `OnImGui()` 中构建 UI，并在同一帧末尾由 `ImGuiLayer::EndFrameAndRender()` 叠加到颜色缓冲上。因此 UI 始终画在 3D 画面之上。

ImGui 使用 OpenGL3 后端，GLSL `#version 130`；场景着色器可为 `#version 330 core`，二者可共存。

## 输入抢占（WantCapture）

`Application` 在以下情况 **不** 把输入交给相机：

- `ImGui::GetIO().WantCaptureKeyboard` 为真时，忽略 WASD
- `WantCaptureMouse` 为真时，忽略鼠标旋转与滚轮缩放

在 ImGui 面板外拖动鼠标仍可旋转相机。在滑条、按钮上操作时不应带动视角。

## 在 sample 中加控件

重写 `OnImGui()`，在 `ImGui::Begin` / `End` 之间写控件即可（见 `samples/02-textured-cubes/main.cpp`）。

## 中文字体

构建时从本机 `C:\Windows\Fonts\` 拷贝字体到 `assets/fonts/ui-cn.ttf`（仓库内统一文件名）。`ImGuiLayer::Init` 会加载该文件并启用 `GetGlyphRangesChineseFull()`。

若 clone 后缺少字体文件，可从系统拷贝例如：

```powershell
Copy-Item $env:WINDIR\Fonts\simhei.ttf .\assets\fonts\ui-cn.ttf
```

使用 `.ttc`（如 `msyh.ttc`）时需改 `ImGuiLayer` 中的路径并在 `ImFontConfig` 中设置 `FontNo`（通常为 `0`）。

字体文件约数 MB，默认纳入仓库以便 clone 即可运行。
