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

当前版本保留中文、搜索、文件夹、展开分组、字体和 GPU 合成。右键菜单点击穿透和开始按钮状态复位已加入代码，需在实际 StartAllBack 环境确认。新增需求及后续实现范围见 [REQUIREMENTS_ADDENDUM.md](REQUIREMENTS_ADDENDUM.md)。
