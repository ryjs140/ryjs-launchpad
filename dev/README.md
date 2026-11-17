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

展开分组默认开启，在应用布局中可切回旧文件夹模式。标题右键管理分组，标题拖动排序，应用拖动或右键跨组移动。组布局使用独立 `groups-v1-*.txt`，不会替换旧 `layout-*.txt`。“…”提供导入与备份恢复。正式模块内置设置使用私有 `ui-settings.ini`，无需 CLI；后续 Windhawk 同键修改会优先采用。预览仍使用自身隔离的 `settings.ini`。

Esc/点击空白区域隐藏网格，托盘右键可重新打开；网格 `…` 菜单或托盘右键中的“退出预览”完全退出进程。布局与设置不会因重新编译删除。备份时先退出预览，再复制 `build/preview-data` 中的 `settings.ini`、`state.ini` 和所有 `.txt` 文件；恢复时退出预览再放回同一目录。不要将 `validation-state` 中的测试数据当作个人布局。
