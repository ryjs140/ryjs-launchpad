# RYJS Launchpad

由 [ryjs140](https://github.com/ryjs140) 维护的独立 Windows 11 / Windhawk 全屏应用启动器。基于 [DavidHiFi 的 Seamless Launchpad](https://github.com/DavidHiFi/davids-windhawk-mods/tree/main/mods/local/seamless-launchpad)，更早基础为 [Rafael Mendes 的 Fullscreen Start](https://windhawk.net/mods/fullscreen-start)。保留原作者署名，沿用 MIT 许可。

这个仓库仅包含启动器源码、构建工具和需求说明，不包含个人桌面布局、Windhawk 用户配置或旧开发仓库历史。

## 构建与安装

需要 Windows x64、Windhawk 编译器及 Python（`pip install -r dev/requirements.txt`）。

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File dev/Build.ps1 -Verify -Smoke
```

Windhawk 中新建模组，使用根目录的 `ryjs-launchpad.wh.cpp` 编译。开发源文件在 `mods/local/seamless-launchpad/`；修改后执行上述命令重新生成单文件。不要同时启用旧版 Launchpad 和 RYJS 来接管开始菜单。

## 数据兼容

新 ID 为 `ryjs-launchpad`，首次启动将旧 Custom 数据复制到独立存储 `local@ryjs-launchpad/data-v1`。旧存储保持只读，已有 RYJS 数据不会被重新导入。布局读取失败或遇到未知版本时停止保存，不以默认布局覆盖。

布局 TXT 导入、导出位于启动器“…”菜单。可选完整备份工具为 `dev/layout_backup.py`，仅在用户主动调用时使用。开发和测试不需要备份或改写实际桌面布局。

## 验证与开发

`dev/README.md` 说明隔离预览；`dev/Test-Phase1.ps1` 验证存储兼容及输入处理。官方元数据和编译检查使用 `dev/Check-Library.ps1`。构建测试与实际任务栏、多显示器效果分别验收，不把离屏测试视为实机验证。

当前版本 **0.2.0** 保留中文、搜索、字体和 GPU 合成，并实现动画时长滑块、手动入口管理、分组自由布局及主题设置。更改和验证记录见 [CHANGELOG.md](CHANGELOG.md)。右键菜单点击穿透和开始按钮状态复位已加入代码，实际 StartAllBack 外圈效果仍需加载后确认。

## 操作

- 点击搜索栏“…”→“添加入口”：浏览 `.exe`、`.lnk`、`.rdp` 文件，或填写 HTTP/HTTPS 网址；也可选择商店应用。填写显示名、参数、工作目录及可选图标，并选择目标分组。
- 右键应用→“编辑入口”：修改名称、路径或图标。重新扫描保留修改；原名称仍可搜索。路径失效时入口保留并显示标记，可以再次编辑修复。手动入口可单独删除，不删除目标文件。
- 网页图标异步读取网站自己的图标声明，失败回退 `/favicon.ico`，支持 ICO、PNG 和 Windows Direct2D 支持的 SVG；本地缓存，不定期后台抓取。失败保留地球占位图，可手动换图或右键重新获取。
- “…”或分组标题右键→“编辑分组布局”：拖标题移动，拖右下角调整宽高。位置和尺寸按网格吸附，红色预览表示重叠、松手不会提交。Esc 取消当前拖动，再按一次退出编辑。
- 缩小后，组内滚轮查看剩余应用；按住 Shift 使用滚轮滚动整体画布。分组标题右键可切换常驻分组与传统文件夹。所有切换、拖动都由用户主动触发，未编辑时保持旧布局。
- 设置→“应用布局”调整打开、关闭及文件夹动画时长；“外观”选择浅色、深色、跟随系统或自定义，并调整名称底衬、阴影、颜色和背景压暗。

新布局几何保存于独立 `group-geometry-layout-*.txt`，入口属性保存于 `entries-v1.txt`。原分组成员和序位文件继续兼容。新发现的应用只出现在搜索中，须主动添加/固定才进入现有布局。

## 当前验证边界

已通过预览及 Windhawk x64 编译、官方元数据校验、隔离回归和离屏渲染。没有自动切换实际运行的 Windhawk 模组、打开真实远程连接或修改桌面布局。需实际确认 StartAllBack 点击状态、混合刷新率显示器、网站网络访问及启动参数对应程序的真实行为。
