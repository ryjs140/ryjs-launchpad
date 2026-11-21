# 独立预览开发

预览程序直接编译上游模块的绘制、设置、应用枚举和布局代码。数据仅保存在工作区 `build/preview-data`。

在仓库根目录执行：

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File dev/Build.ps1 -Verify
powershell -NoProfile -ExecutionPolicy Bypass -File dev/Preview.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File dev/Preview.ps1 -Rebuild
powershell -NoProfile -ExecutionPolicy Bypass -File dev/Preview.ps1 -Settings -Language zh-CN
```

VS Code 的“运行任务”中有相同入口，默认编译任务可用 `Ctrl+Shift+B`。最终仅交付代码，不提供一键启动包；这些脚本用于开发验证。

开发依赖：Windows 10/11 x64、Windhawk 自带的 Clang/Windows 头文件与导入库、Python 3.11+、`dev/requirements.txt`。本机这些构建依赖已可用。官方库检查额外使用 Python 3.12。

修改 `dev/strings.zh-CN.json` 或 `dev/settings-extension.json` 后编译即可更新资源；生成器同时更新 Windhawk 设置页的简体中文元数据，并输出可以直接交给 Windhawk 编辑器的根目录单文件源码 `ryjs-launchpad.wh.cpp`（副本在 build）。此单文件包含所有自定义头文件，无需部署附属文件。原作者署名、MIT 许可证和 Rafael Mendes 的致谢保留在源码/README 中。

预览 EXE 直接 `#include` 工作分支的 `.wh.cpp`。`SEAMLESS_PREVIEW` 只替换存储和入口边界：预览使用本地 INI，不进入 Windhawk 生命周期或键盘钩子；电源操作显示提示。真实应用枚举、搜索、布局、文件夹、拖放、图标与 Direct2D/Direct3D 绘制使用同一份代码。`--lang zh-CN` / `--lang en` 只设置本次启动的语言；在设置页手动保存语言会结束该临时覆盖。

```powershell
# 应用自身的离屏渲染验证：不操作桌面，不模拟输入。
powershell -NoProfile -ExecutionPolicy Bypass -File dev/Build.ps1 -Verify -Smoke
# 官方源码检查与编译检查；只在 build 中下载和暂存文件。
powershell -NoProfile -ExecutionPolicy Bypass -File dev/Check-Library.ps1
```

生成的网格 PNG 使用固定深色背景以便比较布局，不截取桌面。正常打开预览时走原来的屏幕快照模糊代码。日志与 `render-baseline.csv` 位于 `build/preview-data`；CSV 对比兼容路径和 GPU 路径的绘制/提交耗时，不代表显示帧率。运行时的 `performance.csv` 另行记录阶段延迟、帧间隔和可用的呈现统计。

展开分组默认开启，可切回旧文件夹模式。标题右键管理分组；进入编辑模式后拖动标题移动、右下角调整尺寸。原成员文件 `groups-v1-*.txt` 与可选几何文件 `group-geometry-layout-*.txt` 分离。“…”提供入口添加及布局导入/导出。正式模块内置设置使用私有 `ui-settings.ini`，无需 CLI；后续 Windhawk 同键修改会优先采用。预览仍使用自身隔离的 `settings.ini`。

`dev/Test-Features.ps1` 检查新入口、动画、几何和主题；`dev/Test-Phase1.ps1 -SkipBuild` 默认生成虚构布局夹具验证存储和输入，不需要用户数据快照。新增验证图包括 `free-group-edit-zh-CN.png`、`readability-white-background.png` 和 `readability-dark-background.png`，都使用隔离数据与合成背景。

`dev/Test-FullBackup.ps1` 验证 GUI 调用链、内嵌 Windows PowerShell 5.1 导出/导入、旧 ZIP 兼容、设置及图标还原、原数据保留，以及损坏/不支持的备份被拒绝时数据不变。只写 `build/` 下的虚构数据。编辑 `dev/full_backup_worker.ps1` 后重新构建，生成器会将脚本和设置默认值一并内嵌到单文件源码。

搜索时，按 Esc 或点击空白区域先返回主网格，再操作一次才会隐藏启动器。搜索结果和主网格切换有独立过渡，遵循启动器和 Windows 的动画开关。托盘右键可重新打开；网格 `…` 菜单或托盘右键中的“退出预览”完全退出进程。布局与设置不会因重新编译删除。备份时先退出预览，再复制 `build/preview-data` 中的 `settings.ini`、`state.ini` 和所有 `.txt` 文件；恢复时退出预览再放回同一目录。不要将 `validation-state` 中的测试数据当作个人布局。

本轮整改验证见 `FOUR_FIXES_REPORT.md`。专项测试覆盖四档 DPI、吸附阈值、碰撞、编辑取消、溢出访问及名称恢复。新增灰色和明暗混合背景离屏图，直接测试 GPU 读回与局部黑白文字。`dev/inspect_taskbar_symbols.cpp` 仅离线审计本机任务栏符号，不修改 Explorer。


应用悬停将共享的窗口鼠标位置转换成绘制坐标，并使用完整单元作为光照范围。动态边缘遮罩参考 [Meapri PointerField](https://github.com/Meapri/liquid-glass-web/blob/main/src/core/PointerField.ts) 和其 [边缘样式](https://github.com/Meapri/liquid-glass-web/blob/main/src/styles/liquid-glass.css)（MIT，Copyright 2026 Meapri）。轻微的表面反射和边缘层次参考 [rdev/liquid-glass-react](https://github.com/rdev/liquid-glass-react/blob/master/src/index.tsx)（MIT，Copyright 2025 Max Rovensky）和 [ybouane/liquidglass 的着色器](https://github.com/ybouane/liquidglass/blob/main/src/shaders.ts)（项目声明 MIT）。Direct2D 版本根据这些思路独立实现，使用两支缓存的径向画刷，无第三方源码或纹理。
