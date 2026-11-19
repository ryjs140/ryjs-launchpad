# RYJS Launchpad

A fullscreen app launcher for Windows 11 that replaces the Start menu with a
grid of all your apps, in the style of the macOS Launchpad: instant search, a
blurred background, folders, and groups whose tiles sit directly on the grid.

Developed and maintained by **Fangyu Guo (ryjs)**, version 1.0.0. A derivative
work, not an independent rewrite: the direct base is
[Seamless Launchpad](https://github.com/DavidHiFi/davids-windhawk-mods/tree/main/mods/local/seamless-launchpad)
by [DavidHiFi](https://github.com/DavidHiFi), which is based on
[Fullscreen Start (Launchpad style)](https://windhawk.net/mods/fullscreen-start)
by [Rafael Mendes](https://github.com/RafaelMendes1407). All three are MIT.
See [Credits](#credits), [Copyright notices](#copyright-notices) and
[LICENSE](LICENSE).

---

## Requirements

- Windows 11, x64.
- [Windhawk](https://windhawk.net/) 1.7.3 or newer (2.0 supported).
- No other Start replacement enabled. RYJS refuses to load while Seamless
  Launchpad, Fullscreen Start Plus or Fullscreen Start (Launchpad style) is
  running, and it never disables them for you.

## Install

1. Install Windhawk.
2. In Windhawk choose **Create a new mod**.
3. Paste the whole of [`ryjs-launchpad.wh.cpp`](../ryjs-launchpad.wh.cpp) and
   click **Compile**.
4. Enable the mod, then press the Windows key.

The first load downloads the symbols of `twinui.pcshell.dll`, which can take a
minute. Once published it will also be installable from the Windhawk catalog
as `ryjs-launchpad`.

## Open and close

| Action | Result |
| --- | --- |
| Windows key, Ctrl+Esc | Opens the launcher |
| Start button (taskbar or Standalone Dock) | Opens the launcher |
| Shift + Windows key | Opens the original Start by default; configurable |
| Esc | Clears the search, then closes |
| Click outside the icons | Closes |
| Switch to another window | Closes |

Each of the three Start entry points can independently open the grid, the
original Start, or a program of your choice.

## Groups and layout

- Apps are shown inside **expanded groups**; every tile is directly clickable.
  The original scrolling list and paged folder modes are still available under
  **Layout**.
- Right-click empty space, or the `...` button, to create a group.
- Right-click a group title to rename it, move it, delete it, or turn it into a
  traditional folder.
- **Edit group layout**: drag a title to move the group freely, drag the
  lower-right handle to resize it by whole icon cells. A red preview means the
  groups would overlap and the move is refused. Enter or **Finish** saves;
  Escape cancels the current gesture, then the edit session.
- Drag apps inside a group to reorder them, or onto another group to move them.
  Search results have the same **Move to group** action.
- Deleting a group moves its apps to **Ungrouped**; it never uninstalls
  anything.

## Entries and search

- **Add entry** accepts a program, a shortcut, a Remote Desktop file, a Store
  app or an http/https address. You can set a display name, arguments, working
  directory and icon, and choose the destination group.
- **Edit entry** changes those fields later; **Rename** only changes the name
  shown in the launcher, and **Restore original name** undoes it.
- A missing target keeps its entry and is marked, so it can be repaired instead
  of disappearing.
- Search matches both the original application name and your alias; the
  original name always keeps working.
- Icons for web entries are read from the page once, when you add or refresh
  the entry, then cached locally.

## Settings

The `...` button opens the mod's own settings window: layout, appearance
(light/dark/system/custom, colours, blur, opacity, dimming), icon and label
sizes, spacing, group width and flow, monitor, animation durations and easing,
reduced motion, frame cap, GPU composition, and performance logging.

These settings are stored in the mod's private storage, so they need neither
administrator rights nor the Windhawk CLI. If the same setting is later changed
in Windhawk's own settings page, that newer value wins.

## Backups

- Every layout write keeps a `.bak` of the previous file.
- `...` → **Import layout file** / **Restore layout backup**.
- `...` → **Complete backup** writes a `.ryjs.zip` with layouts, groups,
  geometry, entry names and arguments, icon cache, custom icons and settings.
- **Import complete backup** validates the manifest, hashes, paths and schema
  versions, asks for confirmation, keeps the replaced data in a sibling
  directory, and rolls back on failure. It restores data only: code, DLLs and
  registry exports inside an archive are never installed or executed.

## Compatibility

- **Windhawk**: settings use only `$name`, `$description` and `$options`, so the
  same file parses on 1.7.3 and 2.0.
- **Windows**: developed and tested on Windows 11. The Start hook targets
  `ShowStartView` in `twinui.pcshell.dll`; the taskbar button hooks target the
  Windows 11 XAML `ExperienceToggleButton` and are skipped when absent.
  Windows 10 is not tested.
- **Injection**: `explorer.exe` only.

## Known issues

- On a StartAllBack taskbar, or with the native XAML taskbar, clearing the
  Start button's "pressed" look is isolated-tested but not yet confirmed by a
  live click on every configuration. If the button keeps a highlight, the
  launcher still works; report the Windows build and taskbar tool.
- Mixed-refresh multi-monitor setups (for example 60 Hz and 144 Hz) are not
  measured; animation pacing follows the monitor the grid opens on.
- Shift + Windows key cannot reach the grid while an elevated app has focus,
  because Windows keeps those keys away from Explorer. The optional
  `helper/` program in the source repository works around it, but it is not
  installed by the mod.
- Groups and entries are per user; roaming profiles are not synchronised.
- Performance figures in `performance.csv` measure submission, not what the
  monitor physically shows.

## Data and privacy

- No telemetry, no analytics, no accounts.
- The only network requests happen when you add a web entry or ask to refresh
  its icon. They have time limits, a 2 MB cap, cookies disabled, no autologon
  and no HTTPS→HTTP redirects; results are cached locally.
- Layouts, entries, icons and settings stay in Windhawk's per-mod storage.
- The only registry access is a one-time **read** of a previous Launchpad mod's
  settings, to inherit them.

## Build from source

```powershell
pip install -r dev/requirements.txt
powershell -NoProfile -ExecutionPolicy Bypass -File dev/Build.ps1 -Verify -Smoke
```

`dev/Build.ps1` regenerates the root single file from `mods/local/ryjs-launchpad/`.
Official metadata and compile checks:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File dev/Check-Library.ps1
```

## Credits

| Project | Author | Role |
| --- | --- | --- |
| [Fullscreen Start (Launchpad style)](https://windhawk.net/mods/fullscreen-start) | [Rafael Mendes](https://github.com/RafaelMendes1407) | Original launcher, its rendering and the Start hook |
| [Seamless Launchpad](https://github.com/DavidHiFi/davids-windhawk-mods/tree/main/mods/local/seamless-launchpad) | [DavidHiFi](https://github.com/DavidHiFi) | Direct base: grid, search, folders, power dock, settings |
| RYJS Launchpad | Fangyu Guo (ryjs) | The changes in [CHANGELOG.md](CHANGELOG.md) |
| Catppuccin Mocha palette | Catppuccin | Colours of the original search bar, folder glass and power bar |

All three code projects are MIT licensed.

## Copyright notices

Reproduced as the upstream projects state them; no year is invented where the
upstream declares none.

- **Seamless Launchpad** — `Copyright (c) 2026 DavidHiFi`, from the upstream
  `LICENSE` file.
- **Fullscreen Start (Launchpad style)** — `Copyright (c) Rafael Mendes`. The
  upstream mod declares `@license MIT` in its metadata but has no `LICENSE`
  file and no copyright line, so no year is asserted.
- **RYJS Launchpad changes** — `Copyright (c) 2026 Fangyu Guo (ryjs)`.

See [LICENSE](LICENSE) for the full MIT text.

---

# 中文说明

## 这是什么

RYJS Launchpad 是 Windows 11 的全屏应用启动器，用来替代开始菜单：所有应用以
Launchpad 风格的网格铺满屏幕，支持即时搜索、毛玻璃背景、文件夹，以及图标直接
铺在网格上的分组。

由 **Fangyu Guo (ryjs)** 开发与维护，当前版本 **1.0.0**。这是**衍生作品**，不是从零重写：
直接基础是 [DavidHiFi 的 Seamless Launchpad](https://github.com/DavidHiFi/davids-windhawk-mods/tree/main/mods/local/seamless-launchpad)，
更早来源为 [Rafael Mendes 的 Fullscreen Start](https://windhawk.net/mods/fullscreen-start)。
三方均为 MIT 许可，原始版权与后续修改版权分别保留。

## 安装

1. 安装 [Windhawk](https://windhawk.net/)（1.7.3 及以上，含 2.0）。
2. 在 Windhawk 里选择"创建新模组"。
3. 粘贴 [`ryjs-launchpad.wh.cpp`](../ryjs-launchpad.wh.cpp) 的全部内容，点击"编译"。
4. 启用模组，按 Windows 键。

首次加载会下载 `twinui.pcshell.dll` 的符号，可能需要一分钟。

**先禁用** Seamless Launchpad、Fullscreen Start Plus、Fullscreen Start
（Launchpad style）。它们都要接管开始菜单，同时启用会冲突；RYJS 检测到同类
启动器在运行时会拒绝加载，但不会替你禁用对方。

## 使用

- **打开/关闭**：Windows 键、Ctrl+Esc、开始按钮打开；Esc 先清搜索再关闭；点击
  图标以外的区域关闭。三个入口可分别设为打开启动器、系统开始菜单或指定程序。
- **分组**：右键空白处或"…"新建分组；右键标题可重命名、前移、后移、删除，或
  切换为传统文件夹。删除分组只把应用移到"未分组"，不会卸载应用。
- **编辑分组布局**："…"或标题右键进入编辑，拖动标题移动分组，拖动右下角按整格
  缩放；重叠会显示红色预览并拒绝移动。Enter/"保存并完成编辑"写盘，Esc 先取消
  当前拖动、再按一次取消整次编辑。
- **入口**：可添加程序、快捷方式、RDP、商店应用和网址，设置显示名、参数、工作
  目录和图标。"重命名"只改显示名，"恢复原始名称"可撤销；目标丢失时入口保留并
  标记，可再修好。搜索同时匹配原名与别名。
- **设置**："…"打开模组自带设置窗口，可调布局、外观（浅色/深色/跟随系统/自定义、
  颜色、模糊、透明度、压暗）、图标与名称大小、间距、分组宽度与排列、显示器、
  动画时长与缓动、减少动画、帧率上限、GPU 合成和性能记录。设置保存在模组私有
  目录，不需要管理员权限；之后在 Windhawk 设置页改动同一项时以新值为准。
- **备份**："…"支持导入布局、恢复备份，以及"完整备份"生成 `.ryjs.zip`（含布局、
  分组、几何、入口、图标和设置）。导入会先校验清单、哈希与版本并二次确认，替换
  前的数据保留在旁侧目录，失败可回退；只恢复数据，不执行压缩包内的代码。

## 兼容性与已知问题

- Windhawk 1.7.3 与 2.0 均可解析本模组的设置块。
- 在 Windows 11 上开发与测试；Windows 10 未测试。
- 只注入 `explorer.exe`。
- StartAllBack 或原生任务栏上"开始按钮残留选中外圈"的修复已做隔离测试，尚未在
  每种真实配置上逐一点击确认；即使残留也不影响启动器本身。
- 混合刷新率多显示器、`.rdp` 连接与带参数启动尚未实机验收。
- `performance.csv` 记录的是提交节奏，不等于显示器实际呈现帧率。

## 隐私

无遥测、无账号、不上传任何数据。唯一的网络访问发生在你添加网页入口或手动刷新
其图标时：读取该页面声明的图标，失败回退 `/favicon.ico`，有超时与 2 MB 上限，
禁用 Cookie，不允许 HTTPS 降级到 HTTP，结果只缓存在本机。布局、入口、图标和
设置都保存在 Windhawk 的模组私有目录。

## 致谢与版权

| 项目 | 作者 | 角色 |
| --- | --- | --- |
| [Fullscreen Start (Launchpad style)](https://windhawk.net/mods/fullscreen-start) | [Rafael Mendes](https://github.com/RafaelMendes1407) | 原始启动器、渲染与开始菜单钩子 |
| [Seamless Launchpad](https://github.com/DavidHiFi/davids-windhawk-mods/tree/main/mods/local/seamless-launchpad) | [DavidHiFi](https://github.com/DavidHiFi) | 直接基础：网格、搜索、文件夹、电源栏、设置 |
| RYJS Launchpad | Fangyu Guo (ryjs) | [CHANGELOG.md](CHANGELOG.md) 中所列的修改 |

版权声明按上游原文保留：DavidHiFi 为 `Copyright (c) 2026 DavidHiFi`（取自其 LICENSE）；
Rafael Mendes 为 `Copyright (c) Rafael Mendes`，**不写年份**，因为上游只有
`@license MIT`、没有版权行，本项目不代为推测。三方均为 MIT 许可，全文见
[LICENSE](LICENSE)。
