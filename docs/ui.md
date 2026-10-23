# Dear ImGui 集成

## 渲染顺序

场景在 `OnRender()` 中绘制；ImGui 在 `OnImGui()` 中构建 UI，并在同一帧末尾由 `ImGuiLayer::EndFrameAndRender()` 叠加到颜色缓冲上。因此 UI 始终画在 3D 画面之上。

ImGui 使用 OpenGL3 后端，GLSL `#version 130`；场景着色器可为 `#version 330 core`，二者可共存。

## 输入抢占（WantCapture）

相机**仅使用鼠标**（左键轨道、中键平移、滚轮缩放）。`Application` 在 `ImGui::GetIO().WantCaptureMouse` 为真时 **不** 把上述操作交给相机（滚轮缩放同样被忽略）；在 ImGui 控件上按下鼠标时，释放按键会结束轨道/平移拖动状态。

左键水平拖动：鼠标向右时，场景绕观察中心**从上往下看为逆时针**（右手系、绕 `+Y` 的机位运动为顺时针）。

在 3D 视图区域（ImGui 未抢占鼠标）拖动或滚轮即可操作相机；在滑条、按钮等控件上操作时不应带动视角或缩放。

键盘：`Escape` 关闭窗口（`ProcessInput`，不依赖 WantCapture）。其余按键由 ImGui 控件使用（`WantCaptureKeyboard` 为真时由 ImGui 消费）；当前 framework **不** 用键盘驱动相机。

## 在 sample 中加控件

重写 `OnImGui()`，在 `ImGui::Begin` / `End` 之间写控件即可（见 `samples/02-textured-cubes/main.cpp`）。该 sample 提供 **Reset camera**（`ResetOrbitViewToNegativeZ` + 当前帧 `FrameScene`：焦点回包围盒中心、视线沿 \f$-Z\f$）与 **Frame scene**（仅重新拟合距离与裁剪，不改变当前轨道朝向）。

## 中文字体

构建时从本机 `C:\Windows\Fonts\` 拷贝字体到 `assets/fonts/ui-cn.ttf`（仓库内统一文件名）。`ImGuiLayer::Init` 会加载该文件并启用 `GetGlyphRangesChineseFull()`。

若 clone 后缺少字体文件，可从系统拷贝例如：

```powershell
Copy-Item $env:WINDIR\Fonts\simhei.ttf .\assets\fonts\ui-cn.ttf
```

使用 `.ttc`（如 `msyh.ttc`）时需改 `ImGuiLayer` 中的路径并在 `ImFontConfig` 中设置 `FontNo`（通常为 `0`）。

字体文件约数 MB，默认纳入仓库以便 clone 即可运行。
