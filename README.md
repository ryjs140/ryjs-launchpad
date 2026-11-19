# RYJS Launchpad

由 **Fangyu Guo (ryjs)** 维护的 Windows 11 / Windhawk 全屏应用启动器。当前版本 **1.0.0**。

这是一个**衍生作品**，不是从零重写：直接基础是 [DavidHiFi 的 Seamless Launchpad](https://github.com/DavidHiFi/davids-windhawk-mods/tree/main/mods/local/seamless-launchpad)，其更早来源为 [Rafael Mendes 的 Fullscreen Start (Launchpad style)](https://windhawk.net/mods/fullscreen-start)。三方均为 MIT 许可，原始版权与后续修改的版权分别保留，详见 [LICENSE](LICENSE) 与 [release/SOURCE_PROVENANCE.md](release/SOURCE_PROVENANCE.md)。

![RYJS Launchpad](media/preview.png)

这个仓库仅包含启动器源码、构建工具和发布资料，不包含个人桌面布局、Windhawk 用户配置或旧开发仓库历史。上游源码快照放在 `reference/upstream/`，仅用于来源核对。

## 构建与安装

需要 Windows x64、Windhawk 编译器及 Python（`pip install -r dev/requirements.txt`）。

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File dev/Build.ps1 -Verify -Smoke
```

Windhawk 中新建模组，使用根目录的 `ryjs-launchpad.wh.cpp` 编译。开发源文件在 `mods/local/ryjs-launchpad/`；修改后执行上述命令重新生成单文件。不要同时启用旧版 Launchpad 和 RYJS 来接管开始菜单。

## 数据兼容

新 ID 为 `ryjs-launchpad`，首次启动将旧 Custom 数据复制到独立存储 `local@ryjs-launchpad/data-v1`。旧存储保持只读，已有 RYJS 数据不会被重新导入。布局读取失败或遇到未知版本时停止保存，不以默认布局覆盖。

启动器“…”菜单提供“完整备份…”和“导入完整备份…”，使用 Windows 自带组件，无需安装 Python。备份为 `.ryjs.zip`，包括所有布局、分组尺寸、入口名称与启动参数、图标缓存和自定义图标、以及经 Windhawk API 读取的设置。不再包含安装源码、DLL 或注册表导出，因此便携版 Windhawk 也能完整备份。也兼容此前旧版工具生成的 `.ryjs.zip`（旧包中的源码、DLL 与注册表文件只会被忽略）。

导入时先校验清单、哈希、文件路径和数据版本，再显示确认提示；只恢复布局、图标和设置，不执行或安装备份内的源码、DLL 或注册表文件。确认后暂时关闭启动器，完成后重新载入。替换前的数据保留在 `data-v1.before-import-*` 同级目录，提交失败时回退。原 ZIP 保持不变。原有单个布局 TXT 导入/导出仍保留。

设置有两个入口：启动器内置设置窗口（“…”按钮）和 Windhawk 设置页。内置窗口保存的值优先于 Windhawk 设置页；之后在 Windhawk 设置页再次修改同一项时，该项以 Windhawk 的值为准。Windhawk 设置页不会自动刷新内置窗口保存的值。同一项请只用一个入口修改。

临时文件（`Export-StartLayout` 输出、完整备份工作目录、商店应用提权用的临时快捷方式）都位于存储目录相邻的 `tmp` 子目录，完成后删除；不再写入 `%TEMP%` 或用户配置目录。

存储目录存在但校验失败（损坏文件、未知版本）时，启动器以只读方式打开并保留原文件，不再拒绝加载；首次迁移被中断留下的 `data-v1.pending` 会先清理再重试。空的白名单布局（例如取消固定最后一个应用）视为有效空列表。

可选命令行工具为 `dev/layout_backup.py`。开发和测试使用隔离夹具，不备份或改写实际桌面布局。

## 验证与开发

`dev/README.md` 说明隔离预览；`dev/Test-Phase1.ps1` 验证存储兼容及输入处理。官方元数据和编译检查使用 `dev/Check-Library.ps1`。构建测试与实际任务栏、多显示器效果分别验收，不把离屏测试视为实机验证。

当前版本 **1.0.0** 是首次提交 Windhawk 官方库的发布候选：功能与 0.2.1 相同（GUI 完整备份与导入、中文、搜索、字体、GPU 合成、动画滑块、手动入口、分组自由布局、主题），差异仅在版本号、署名、版权声明与目录结构。更改和验证记录见 [CHANGELOG.md](CHANGELOG.md)；来源与许可审计见 [release/SOURCE_PROVENANCE.md](release/SOURCE_PROVENANCE.md)。右键菜单点击穿透和开始按钮状态复位已加入代码，实际 StartAllBack 外圈效果仍需按 [release/MANUAL_ACCEPTANCE.md](release/MANUAL_ACCEPTANCE.md) 人工确认。

## 操作

- 点击搜索栏“…”→“添加入口”：浏览 `.exe`、`.lnk`、`.rdp` 文件，或填写 HTTP/HTTPS 网址；也可选择商店应用。填写显示名、参数、工作目录及可选图标，并选择目标分组。
- 右键应用→“重命名”：仅修改启动台显示名称，Enter 确认、Esc 取消；“恢复原始名称”撤销名称覆盖。所有支持的应用类型均可使用，搜索同时匹配原名和自定义名。鼠标停留可查看完整名称。
- 右键应用→“编辑入口”：修改名称、路径或图标。重新扫描保留修改；原名称仍可搜索。路径失效时入口保留并显示标记，可以再次编辑修复。手动入口可单独删除，不删除目标文件。
- 网页图标异步读取网站自己的图标声明，失败回退 `/favicon.ico`，支持 ICO、PNG 和 Windows Direct2D 支持的 SVG；本地缓存，不定期后台抓取。失败保留地球占位图，可手动换图或右键重新获取。
- “…”或分组标题右键→“编辑分组布局”：拖标题移动，拖右下角调整宽高。位置连续自由移动，尺寸按完整图标节距吸附到整数行列；标题和内边距单独计算。红色预览表示重叠，松手保持原位，不移动其他分组。Enter/“保存并完成编辑”统一写盘；Esc 取消当前拖动，再按一次取消整次编辑。关闭启动台也取消未保存编辑。
- 缩小后，组内滚轮查看剩余应用；按住 Shift 使用滚轮滚动整体画布。分组标题右键可切换常驻分组与传统文件夹。所有切换、拖动都由用户主动触发，未编辑时保持旧布局。
- 设置→“应用布局”调整打开、关闭及文件夹动画时长；“外观”选择浅色、深色、跟随系统或自定义，并调整背景颜色、模糊强度与不透明度。

新布局几何保存于独立 `group-geometry-layout-*.txt`，入口属性保存于 `entries-v1.txt`。原分组成员和序位文件继续兼容。显示名称单独保存于 `display-names-v1.txt`。几何 schema 2 额外存储整数行列，兼容 schema 1；旧分组未主动拉伸时保留原有尺寸，首次转换前保留 `.before-v2.bak`。新发现的应用只出现在搜索中，须主动添加/固定才进入现有布局。

## 当前验证边界

已通过预览及 Windhawk x64 编译、官方元数据校验、隔离回归和离屏渲染。没有自动切换实际运行的 Windhawk 模组、打开真实远程连接或修改桌面布局。需实际确认 StartAllBack 点击状态、混合刷新率显示器、网站网络访问及启动参数对应程序的真实行为。

文字直接绘制在毛玻璃上，只使用纯黑或纯白；按文字实际占用区域采样同一张无文字 GPU 模糊图，叠加 tint、背景调暗及分组面板，sRGB 线性化后比较亮度，并用滞回稳定颜色。每次打开只读取一次背景，不逐帧截屏。默认自定义模糊模式与当前背景绘制同源；DWM 亚克力/系统模糊的最终材质不在本模块 GPU 位图内，使用打开前的屏幕模糊估计，不能视为最终合成像素检测。开关动画的透明度和时序保持原实现。

原生 Windows 11 开始按钮使用 XAML `ExperienceToggleButton`。RYJS 在其 WinRT 可见状态设置入口中，仅对 AutomationId 为 `StartButton` 且当前由 RYJS 接管的状态阻止错误的“原生开始菜单可见”值。原生开始菜单操作仍透传；不改任务栏插件尺寸、样式或悬停反馈。该路径需要本机真实点击验证，模拟 Win32 按钮测试不代表 XAML 验收通过。
