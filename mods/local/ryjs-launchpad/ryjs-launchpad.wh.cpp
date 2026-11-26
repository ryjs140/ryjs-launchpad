// ==WindhawkMod==
// @id              ryjs-launchpad
// @name            Lumiflow
// @name:zh-CN      流光启动台
// @description     A fullscreen Start replacement with expanded groups, free group layout, manual and web entries, themes, bilingual UI and instant search
// @description:zh-CN 全屏应用启动器：展开分组、自由分组布局、手动与网址入口、主题、中英双语与即时搜索
// @version         1.5.9
// @author          Fangyu Guo (ryjs)
// @github          https://github.com/ryjs140
// @homepage        https://github.com/ryjs140/ryjs-launchpad
// @include         explorer.exe
// @architecture    x86-64
// @compilerOptions -lgdi32 -lshell32 -lshcore -ldwmapi -ld2d1 -ldwrite -ld3d11 -ldxgi -ldcomp -lole32 -luuid -lcomdlg32 -luxtheme -lpropsys -lpowrprof -lwindowscodecs -limm32 -lcomctl32 -lwinhttp -lshlwapi -lruntimeobject -loleaut32
// @license         MIT
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# Lumiflow

Developed and maintained by **Fangyu Guo (ryjs)**. This is a derivative work,
not an independent rewrite:

| Project | Author | Role |
| --- | --- | --- |
| [Fullscreen Start (Launchpad style)](https://windhawk.net/mods/fullscreen-start) | [Rafael Mendes](https://github.com/RafaelMendes1407) | Original launcher, its rendering and the Start hook |
| [Seamless Launchpad](https://github.com/DavidHiFi/davids-windhawk-mods/tree/main/mods/local/seamless-launchpad) | [DavidHiFi](https://github.com/DavidHiFi) | Direct base: grid, search, folders, power dock, settings |
| Lumiflow | Fangyu Guo (ryjs) | The changes listed under "What Lumiflow changes" below |

All three are MIT licensed. The original copyright notices and the MIT text
are kept verbatim in the License section at the end of this readme.

**Settings precedence.** The built-in settings window (the `...` button) and
Windhawk's settings page edit the same settings. A value saved in the built-in
window overrides the Windhawk page; changing that same setting on the Windhawk
page again takes precedence. The Windhawk page does not refresh to show values
saved in the built-in window. Use either page, not both for the same setting.

![Lumiflow](https://raw.githubusercontent.com/ryjs140/ryjs-launchpad/main/media/preview.png)

## What Lumiflow changes
- Expanded groups show their apps directly. Choose Layout > Expanded groups;
  the original scrolling and paged folder modes remain available.
- Right-click empty space to create a group. Right-click a group title to
  rename it or enter Edit group layout. In edit mode drag a title to move
  the group, or its lower-right handle to resize by whole icon cells.
  Ctrl-click groups or draw a selection rectangle, then drag any selected
  group to move them together.
  Enter/Finish saves; Escape cancels the gesture, then the edit session.
  Drag apps between groups,
  or use Move to group (also available in search).
- Ctrl-click or drag a rectangle to select apps. Drag a selected icon to move
  the selection, press Enter to open it, or press Delete to remove it from the grid.
- Add entry accepts programs, shortcuts, Remote Desktop files, Store apps
  and web addresses. Edit entry saves aliases, arguments and working folders.
  Rename/Restore original name change only the Lumiflow display name.
  Website icons are fetched when missing or manually refreshed and cached locally.
  Pictures you choose are saved inside the mod's storage.
- Launcher labels and menus use white text with independently adjustable shadow and outline.
  New discoveries never insert themselves into an existing arrangement.
- Lumiflow menus share a rounded surface tinted from the launcher's whole background, while
  retaining native submenu, keyboard and accessibility behavior.
- Interface language follows Windows, with manual Chinese/English selection.
  The Interface font picker lists local fonts without changing icon glyphs.
- Icon/name sizes, spacing, group width/flow, blur, opacity, taskbar coverage,
  paging, durations, easing, reduced motion and frame caps are configurable.
- The built-in settings window saves to private per-mod storage without
  administrator access or windhawk-cli. Later changes to the same setting
  in Windhawk override its private value.
- Layout updates keep a .bak file. Import layout file and Restore layout
  backup are available in the ... menu. Group files are separate from the
  original folder layout; unknown newer schemas are never overwritten.
- performance.csv distinguishes first-visible latency, render cost, frame
  intervals, submission cadence and transition duration. Submission FPS is
  not a measurement of physical monitor presentation.

### 中文操作
在“应用布局”中选择“展开分组”，应用会直接显示在分组内；原文件夹模式仍可选。
右键空白处或点击搜索栏的“…”可新建分组，右键标题可重命名、前移、后移或删除分组。
进入“编辑分组布局”后拖动标题可自由移动分组，拖动右下角可调整尺寸；重叠时显示红色预览并拒绝提交。
“添加入口”支持程序、快捷方式、RDP、商店应用和网址；右键“编辑入口”可改名、路径、参数和图标。
缩小分组后在组内滚动查看应用，按住 Shift 滚动整体画布；未进入编辑模式时保留已有排列。
删除分组只会把应用移出该分组，不会卸载应用；未分组的内容不会单独显示。字体、名称字号、图标间距及分组布局可在设置中调整。
流光启动台自有菜单采用圆角亚克力玻璃效果，并跟随当前主题调整文字与悬停对比度；原生子菜单和键盘操作保持不变。
设置保存在模块私有目录中；修改 Windhawk 设置页的同名选项后，以该次修改为准。
布局自动保留上一次备份；“…”菜单提供导入及恢复。导入前会保留当前布局。

---

Replaces the Start menu with a fullscreen grid of all your apps, in the style
of a macOS app grid, with instant search and a blurred background.

## Features
- **Fullscreen.** Fills the monitor the mouse is on, or one monitor you pick.
  It leaves the taskbar, side bars and the Standalone Dock visible, or covers
  the whole screen if you turn that on.
- **Your choice of apps.** The "All apps" list of Start, your desktop icons,
  both together, or only the apps you pin. With only pinned apps, search still
  finds every app, and right-click pins or unpins one.
- **Scroll or pages.** One list that scrolls, or pages that flip left and
  right with dots below, like a paged launcher.
- **Instant search.** Just type. Case and accents don't matter, and names that
  start with your text come first. The search box is a real text field, with a
  blinking caret and click-and-drag selection, and Ctrl+A, copy, cut and paste
  work as usual.
- **GPU rendered.** The grid, the blur and the folder glass all rasterize on
  the GPU (Direct2D over Direct3D), so it stays snappy at any resolution.
- **Wiggle edit mode.** Grab and hold any icon and every icon starts to
  wiggle, like iOS, while you arrange them.
- **Adjustable material.** Choose the blur radius, saturation, brightness,
  contrast, tint, luminosity fog, grain and dim of the background, or use Mica,
  system acrylic or a solid/transparent panel. Named presets can be saved and
  exported. The search bar, folder glass, dots and power bar use the
  Catppuccin Mocha palette.
- **Opens like Start.** The Windows key, Ctrl+Esc and the Start button of the
  taskbar or the Standalone Dock all open it.
- **Your own Windows key.** The Windows key, Shift + Windows key and the Start
  button can each open this grid, the original Start, or a program of your
  choice, such as a launcher. The program starts even when it isn't running,
  so this replaces the same feature of Open-Shell.
- **Your own order.** Drag icons to move them, across pages and in and out
  of folders. Drop an app on another to make a folder.
- **Expanded groups and free layout.** Groups can show their apps directly,
  and Edit group layout moves or resizes each group freely; apps can be moved
  between groups. Ctrl-click or drag a rectangle to select several apps, then
  drag, open or remove them together.
- **Entries beyond Start.** Add programs, shortcuts, Remote Desktop files,
  Store apps and web addresses, with your own name, arguments, working folder
  and icon. Website icons are fetched once and cached locally.
- **Right-click menu.** Open, run as administrator, open the file location
  or the shortcut location, change the icon, pin or unpin, or remove an app
  from the grid.
- **Power buttons.** A small glass bar at the bottom with Lock, Sign out,
  Sleep, Hibernate, Restart and Shut down. Pick which ones show, or hide the
  bar. The first click asks ("Click again to restart") and a second click on
  the same button does it. Turn on **No confirmation** to act on the first
  click. Restart, shut down and sign out can show a red warning light that
  follows the pointer across the dock.
- **Built-in settings window.** A rounded glass window with light, dark,
  system and custom themes; layout, typography, material, power and key pages;
  a live material preview; and complete backup/import of layouts, icons,
  entries and settings.
- **Works over administrator windows.** Windows keeps keys pressed in an app
  running as administrator away from Explorer, so Shift + Windows key can't
  open the grid there. The optional helper in the `helper` folder of this
  mod's repository runs as administrator and fixes that (see its README).

## Keyboard and mouse
| Key | Action |
| --- | --- |
| Type | Search |
| Backspace, Ctrl+Backspace, Delete | Delete the selection, a letter, or the last word |
| Ctrl+A, Ctrl+C, Ctrl+X, Ctrl+V | Select all, copy, cut, paste |
| Left, Right, Home, End | Move the caret in the search; hold Shift to select. With an empty search, Left and Right pick an app |
| Up, Down, Enter | Pick an app and open it. While searching, the first result is already picked |
| Esc | Clear the search, or close when the search is empty |
| Mouse wheel, Page Up, Page Down | Scroll, or flip pages |
| Sideways scroll, page dots | Flip pages |
| The ... button, top right | Open this mod's own settings window (private settings), manage groups, restore/import layouts, bring back removed apps, or sort A to Z |
| Grab and hold an icon | All icons wiggle; drag to move it. Hold it at the screen's edge to flip the page |
| Drop an app on another | Make a folder. Drop more apps on it to add them |
| Hold an app over a folder | The folder opens, so you can place it inside |
| Drag an app out of an open folder | Take it out of the folder |
| Type in an open folder | Rename it |
| Right-click an icon | Its menu. Right-click empty space for the ... menu |
| Power buttons, bottom | Lock, sign out, sleep, hibernate, restart or shut down. Click twice, or once with No confirmation on |

Your order, folders, removed apps and icons are kept in Windhawk's storage
folder for this mod. New apps go at the end.

Clicking outside the icons or switching to another window closes it.

## Background material
- **Adjustable** (default): a snapshot of the screen taken when Start opens,
  then blurred and graded by this mod. Blur radius, saturation, brightness,
  contrast, tint, luminosity fog, grain and dim are independent, and the built-in
  settings window includes a live preview plus named presets you can export and
  import.
- **Solid** and **Transparent**: a tinted panel without a snapshot, or only the
  tint layer.
- **Windows 11 system acrylic, Mica and Mica Alt**, plus the older accent
  **acrylic** and **blur**: drawn by DWM (or the accent API) behind the window.
  They follow what changes behind them, ignore blur/grade/grain, and fall back
  to the adjustable material when the OS build does not support them.
- The tint colour, tint opacity and dim apply on top of every backend.

## Notes
- Disable Seamless Launchpad, Fullscreen Start Plus and Fullscreen Start
  (Launchpad style) before you enable this mod. All of them replace Start, so
  only one should run. Lumiflow refuses to load while another launcher window is
  present and never disables or unloads that mod for you.
- On its first run, Lumiflow copies the storage folder of Seamless Launchpad
  Custom, Seamless Launchpad or Fullscreen Start Plus, if one of them is
  installed, into its own. The copy is what that folder holds — the layout,
  group, pin and icon files, and its old `ui-settings.ini` — but only the
  layout, group, pin and icon files are used: the old settings file is kept in
  the new folder as `migration-original-ui-settings.ini` for reference rather
  than applied, so no settings are inherited silently. Nothing in the old
  folder is modified or deleted. If the old folder cannot be validated, Lumiflow
  starts with an empty layout instead of staying read-only, leaves that folder
  untouched, and writes `migration-note.txt` next to the new data to say so.
  Set the appearance you want once in the settings window.
- The power buttons never force apps to close. If an app has unsaved work,
  Windows asks about it as usual. Hibernate shows only when hibernation is on
  in Windows.
- Set **Windows key**, **Shift + Windows key** or **Start button** to "Open
  the original Start" to get the normal Start menu for that action. Shift +
  Windows key opens it by default.
- If Open-Shell or another Start replacement also takes the Windows key, turn
  that feature off there, or it gets the key first.
- The first time the mod loads, Windhawk downloads the symbols of
  `twinui.pcshell.dll`, which can take a minute.
- Setting changes apply the next time Start opens.

## Compatibility
- Windhawk 1.7.3 and later, including 2.0. The settings block uses only the
  `$name`, `$description` and `$options` keys, so it parses on both.
- Windows 11. The Start replacement hooks `ShowStartView` in
  `twinui.pcshell.dll` and, when present, the `ExperienceToggleButton` of the
  Windows 11 taskbar. Windows 10 was not tested by the maintainer.
- Explorer only (`@include explorer.exe`). Nothing is injected into other
  processes.

## Data and network
- No telemetry and no analytics. Lumiflow does not send anything anywhere except
  the requests described next.
- Website icon requests run after an app-list load whenever the launcher finds a
  web entry without a cached icon, including the first automatic load after
  Explorer starts and even if you never open the launcher. This includes direct
  web entries and eligible `.url` / browser-shortcut entries discovered on the
  Desktop and Public Desktop. Saving or manually refreshing an entry also
  requests its icon. **Background website icon download** in Settings > Help
  turns the automatic app-list requests off; saving and manual refresh still
  download, and cached icons still work. The default is on.
  The page is read to find its declared icon, with `/favicon.ico` as the
  fallback. Declared icons can be hosted on a different server than the page
  you entered (a CDN, a third party, or a private/intranet host). Requests have
  time limits and a size cap, are sent without cookies or automatic credentials,
  and the result is cached in the mod's storage. Nothing is uploaded. Set a
  custom icon for the entry and no request is made for it.
- Layouts, groups, entries, icons and settings stay in Windhawk's per-mod
  storage folder on this PC. Scratch files (the exported Start layout and the
  backup worker) live in a `tmp` folder next to it and are removed when the
  work is done. The optional full backup writes a `.ryjs.zip`
  file only where you choose. Import never executes archive code or binaries;
  it does restore app entries with their targets and arguments, and the
  confirmation lists every imported launch target. The Start-button and Win-key
  actions and the configured program command remain as they are on this PC.
  Running a Store app as administrator uses a temporary shortcut in that same
  `tmp` folder; it is deleted after the launch.

## Credits
- [Fullscreen Start (Launchpad style)](https://windhawk.net/mods/fullscreen-start)
  by [Rafael Mendes](https://github.com/RafaelMendes1407): the origin of the
  launcher, its rendering and its Start hook.
- [Seamless Launchpad](https://github.com/DavidHiFi/davids-windhawk-mods/tree/main/mods/local/seamless-launchpad)
  by [DavidHiFi](https://github.com/DavidHiFi): the direct base of this mod.
  Most of the grid, search, folder, power-dock and settings code comes from it.
- Lumiflow changes by Fangyu Guo (ryjs).
- The Catppuccin Mocha palette used by the original search bar, folder glass
  and power bar.

All three projects are MIT licensed.

## Copyright notices

The notices below are reproduced as the upstream projects state them. No year
is invented for a project that does not declare one.

- **Seamless Launchpad** — `Copyright (c) 2026 DavidHiFi`, from the `LICENSE`
  file of https://github.com/DavidHiFi/davids-windhawk-mods.
- **Fullscreen Start (Launchpad style)** — `Copyright (c) Rafael Mendes`. The
  upstream mod declares `@license MIT` in its metadata but ships no separate
  `LICENSE` file and no copyright line, so no year is stated here.
- **Lumiflow changes** — `Copyright (c) 2026 Fangyu Guo (ryjs)`.

## License

```text
MIT License

Copyright (c) 2026 DavidHiFi
Copyright (c) Rafael Mendes
Copyright (c) 2026 Fangyu Guo (ryjs)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- interfaceLanguage: auto
  $name: Interface language
  $description: Follow Windows display language. Save to apply; app names stay unchanged.
  $options:
  - auto: Automatic (Windows)
  - zh-CN: Simplified Chinese
  - en: English
  $name:zh-CN: 界面语言
  $description:zh-CN: 跟随 Windows 显示语言，保存后生效。应用名称保持原样。
  $options:zh-CN:
  - auto: 自动（跟随 Windows）
  - zh-CN: 简体中文
  - en: English（英语）
- uiFont: auto
  $name: Interface font
  $description: Choose a locally installed font. Applies to text only; symbol icons use their own font.
  $name:zh-CN: 界面字体
  $description:zh-CN: 选择本机已安装字体，用于所有界面文字。功能图标使用独立的符号字体。
- winKeyAction: grid
  $name: Windows key
  $description: Also Ctrl+Esc.
  $options:
  - grid: Open this grid
  - start: Open the original Start
  - program: Run the program below
  $name:zh-CN: Windows 键
  $description:zh-CN: 同时适用于 Ctrl+Esc。
  $options:zh-CN:
  - grid: 打开启动器
  - start: 打开系统开始菜单
  - program: 运行下方程序
- shiftWinKeyAction: start
  $name: Shift + Windows key
  $options:
  - grid: Open this grid
  - start: Open the original Start
  - program: Run the program below
  $name:zh-CN: Shift + Windows 键
  $options:zh-CN:
  - grid: 打开启动器
  - start: 打开系统开始菜单
  - program: 运行下方程序
- startButtonAction: grid
  $name: Start button
  $description: The taskbar Start button, the Standalone Dock's, and every other way Start opens that is not the
    keyboard.
  $options:
  - grid: Open this grid
  - start: Open the original Start
  - program: Run the program below
  $name:zh-CN: 开始按钮
  $description:zh-CN: 适用于任务栏、独立 Dock 的开始按钮及其他非键盘打开方式。
  $options:zh-CN:
  - grid: 打开启动器
  - start: 打开系统开始菜单
  - program: 运行下方程序
- program: ''
  $name: Program
  $description: What "Run the program below" starts. A command line such as "C:\Program Files\App\app.exe" --flag,
    or a shortcut, file, folder or web address
  $name:zh-CN: 程序
  $description:zh-CN: 选择“运行下方程序”时执行的命令。可填写带参数的程序路径、快捷方式、文件、文件夹或网址。
- monitor: mouse
  $name: Monitor
  $description: Where the grid opens. Monitors 2 to 4 are the other monitors, counted left to right. If the picked
    monitor is not connected, it opens where the mouse is.
  $options:
  - mouse: The monitor with the mouse
  - '1': Main monitor
  - '2': Monitor 2
  - '3': Monitor 3
  - '4': Monitor 4
  $name:zh-CN: 显示器
  $description:zh-CN: 选择启动器显示的位置。显示器 2 至 4 按从左到右排列；所选显示器未连接时，使用鼠标所在的显示器。
  $options:zh-CN:
  - mouse: 鼠标所在显示器
  - '1': 主显示器
  - '2': 显示器 2
  - '3': 显示器 3
  - '4': 显示器 4
- appSource: allApps
  $name: Apps to show
  $description: With pinned apps, search still finds every app. Right-click an app to pin or unpin it.
  $options:
  - allApps: Start menu apps (the All apps list)
  - desktop: Desktop icons
  - both: Start menu apps and desktop icons
  - pinned: Only the apps you pin
  $name:zh-CN: 显示的应用
  $description:zh-CN: 仅显示固定应用时仍可搜索全部应用。右键可固定或取消固定。
  $options:zh-CN:
  - allApps: 开始菜单中的所有应用
  - desktop: 桌面图标
  - both: 开始菜单应用与桌面图标
  - pinned: 仅显示固定的应用
- gridLayout: groups
  $name: Layout
  $description: How the grid is arranged.
  $options:
  - scroll: One list that scrolls up and down
  - pages: Pages that flip left and right
  - groups: Expanded groups
  $name:zh-CN: 布局方式
  $description:zh-CN: 选择应用排列和浏览方式。
  $options:zh-CN:
  - scroll: 上下滚动列表
  - pages: 左右翻页
  - groups: 展开分组
- gridColumns: 0
  $name: Columns
  $description: '0 to 30. 0 picks a number that fits the screen (Suggested: 0).'
  $name:zh-CN: 每行图标数量
  $description:zh-CN: 范围 0–30。0 表示根据屏幕自动调整（建议：0）。
- globalColumns: 0
  $name: Columns per row
  $description: Global grid columns, 1–30. 0 keeps the saved density or estimates it for a new layout. Preview changes
    in the layout editor.
  $name:zh-CN: 每行列数（全局）
  $description:zh-CN: 全局列数为 1–30。0 沿用已保存的密度，新布局则自动估算。进入布局编辑可预览列数变化。
- coverScreen: false
  $name: Cover the whole screen
  $description: On, the grid also covers the taskbar, side bars and the Standalone Dock. Off, they stay visible.
  $name:zh-CN: 覆盖整个屏幕
  $description:zh-CN: 开启后覆盖任务栏、侧边栏和独立 Dock；关闭后保留它们的可见区域。
- gridIconSize: 56
  $name: Icon size
  $description: '24 to 192 (Suggested: 56). Pixels at 100% Windows scaling. The grid uses fewer columns when needed.'
  $name:zh-CN: 图标大小
  $description:zh-CN: 范围为 24–192（建议值为 56），单位为 Windows 100% 缩放下的像素。空间不足时会减少显示列数。
- showLabels: true
  $name: Icon labels
  $description: On, the app name shows under each icon. Off, icons only.
  $name:zh-CN: 显示应用名称
  $description:zh-CN: 开启后在图标下显示应用名称；关闭后仅显示图标。
- normalizeIcons: true
  $name: Fill small icons
  $description: Scale small or padded icons to full size.
  $name:zh-CN: 放大小图标
  $description:zh-CN: 将尺寸较小或留白较多的图标放大至统一大小。
- blurMode: custom
  $name: Background material
  $description: How the backdrop is drawn. The adjustable material is the one the controls below shape; the Windows
    materials are drawn by DWM and ignore blur, grade and grain.
  $options:
  - custom: Adjustable material
  - acrylicSystem: Windows 11 system acrylic
  - mica: Windows 11 Mica
  - micaAlt: Windows 11 Mica Alt
  - acrylic: Windows acrylic (accent)
  - blur: Windows blur (accent)
  - transparent: Transparent
  - solid: Solid colour
  $name:zh-CN: 背景材质
  $description:zh-CN: 背景的绘制方式。可调材质由下面的参数塑造；Windows 材质由 DWM 绘制，会忽略模糊、调色和颗粒。
  $options:zh-CN:
  - custom: 可调材质
  - acrylicSystem: Windows 11 系统亚克力
  - mica: Windows 11 云母
  - micaAlt: Windows 11 云母 Alt
  - acrylic: Windows 亚克力（强调色）
  - blur: Windows 模糊（强调色）
  - transparent: 透明
  - solid: 纯色
- blurAmount: 60
  $name: Legacy blur strength
  $description: 0 to 100. Kept for configurations upgraded from 1.0; it is read only when Blur radius (DIP) is not
    set. New installs use Blur radius below.
  $name:zh-CN: 旧版模糊强度
  $description:zh-CN: 范围 0–100。仅为 1.0 升级配置保留；仅在“模糊半径（DIP）”未设置时读取。新安装请使用下面的模糊半径。
- backgroundOpacity: 30
  $name: Tint opacity (%)
  $description: 0 shows only the material, 100 covers it with the tint colour.
  $name:zh-CN: 着色不透明度（%）
  $description:zh-CN: 0 只显示材质，100 完全覆盖为着色色。
- tintColor: '#000000'
  $name: Tint colour
  $description: 'Hexadecimal, like #1E1E2E. Mixed over the material at the tint opacity.'
  $name:zh-CN: 着色
  $description:zh-CN: '十六进制，如 #1E1E2E。按着色不透明度混合在材质上。'
- glowStrength: 60
  $name: Glow strength
  $description: '0 to 100 (Suggested: 60). How strongly hovered buttons glow.'
  $name:zh-CN: 光晕强度
  $description:zh-CN: 范围 0–100（建议：60）。控制鼠标悬停时按钮的光晕强度。
- chromeOpacity: 90
  $name: Search bar and dock opacity (%)
  $description: '0 to 100 (Suggested: 90). How solid the search bar and the power dock are.'
  $name:zh-CN: 搜索栏与电源栏不透明度（%）
  $description:zh-CN: 范围 0–100（建议：90）。控制搜索栏与电源栏的不透明度。
- powerBar: true
  $name: Power dock
  $description: The pill of power buttons at the bottom of the grid.
  $name:zh-CN: 显示电源栏
  $description:zh-CN: 在启动器底部显示电源操作按钮。
- dockAlignment: center
  $name: Dock position
  $description: Where the power dock sits along the bottom edge.
  $options:
  - left: Left
  - center: Center
  - right: Right
  $name:zh-CN: 电源栏位置
  $description:zh-CN: 选择电源栏在屏幕底部的对齐位置。
  $options:zh-CN:
  - left: 靠左
  - center: 居中
  - right: 靠右
- powerButtons:
  - lock: true
    $name: Lock
    $name:zh-CN: 锁定
  - signOut: true
    $name: Sign out
    $name:zh-CN: 注销
  - sleep: true
    $name: Sleep
    $name:zh-CN: 睡眠
  - hibernate: true
    $name: Hibernate
    $description: Shows only when hibernation is turned on in Windows.
    $name:zh-CN: 休眠
    $description:zh-CN: 仅在 Windows 已启用休眠时显示。
  - restart: true
    $name: Restart
    $name:zh-CN: 重启
  - shutDown: true
    $name: Shut down
    $name:zh-CN: 关机
  $name: Power dock buttons
  $name:zh-CN: 电源栏按钮
- powerNoConfirm: false
  $name: Skip confirmation
  $description: On, a power button acts on the first click. Off, the first click arms it and a second click on the
    same button confirms.
  $name:zh-CN: 跳过二次确认
  $description:zh-CN: 开启后点击一次即执行。关闭后需再次点击同一按钮确认。
- powerWarning: true
  $name: Warning light
  $description: One shared red environment light inside the power capsule. It marks only the actions ticked below.
  $name:zh-CN: 警示光效
  $description:zh-CN: 电源胶囊内共用的一束红色环境光，仅标记下方勾选的操作。
- powerWarningActions:
  - lock: false
    $name: Lock
    $name:zh-CN: 锁定
  - signOut: true
    $name: Sign out
    $name:zh-CN: 注销
  - sleep: false
    $name: Sleep
    $name:zh-CN: 睡眠
  - hibernate: false
    $name: Hibernate
    $name:zh-CN: 休眠
  - restart: true
    $name: Restart
    $name:zh-CN: 重启
  - shutDown: true
    $name: Shut down
    $name:zh-CN: 关机
  $name: Warning actions
  $name:zh-CN: 警示操作
- powerWarningFollow: mouse
  $name: Light follows
  $description: 'Mouse: the light responds slightly to the pointer within each button. Button: it stays centred
    on the icon.'
  $name:zh-CN: 光晕跟随方式
  $description:zh-CN: 鼠标：光晕在按钮内轻微跟随指针。按钮：光晕停在图标中心。
  $options:
  - mouse: Follow mouse
  - button: Follow button
  $options:zh-CN:
  - mouse: 跟随鼠标
  - button: 跟随按钮
- powerWarningColor: '#A91223'
  $name: Warning colour
  $description: 'Hexadecimal, like #A91223. Only the environment light and its gentle reflection on the label above
    use it.'
  $name:zh-CN: 警示颜色
  $description:zh-CN: '十六进制，如 #A91223。仅用于环境光及上方文字胶囊的轻微透染。'
- powerWarningStrength: 78
  $name: Warning strength (%)
  $description: Centre concentration of the warning light. 78 is the reference value.
  $name:zh-CN: 警示强度 (%)
  $description:zh-CN: 光晕中心浓度，78 为参考值。
- powerWarningSpread: 100
  $name: Diffusion range (%)
  $description: Percent of the 48 DIP reference radius, scaled with the capsule. 100 keeps the reference spread.
  $name:zh-CN: 扩散范围 (%)
  $description:zh-CN: 以 48 DIP 基准半径的百分比，并随胶囊缩放。100 保持原扩散。
- powerWarningEdge: 65
  $name: Edge tint (%)
  $description: Relative strength of the red along the bottom glass edge.
  $name:zh-CN: 边缘着色 (%)
  $description:zh-CN: 玻璃底边红色相对中心浓度的强度。
- powerWarningBleed: 15
  $name: Label bleed (%)
  $description: Very weak reflection on the text capsule above, relative to the warning light.
  $name:zh-CN: 标签透染 (%)
  $description:zh-CN: 上方文字胶囊的极弱透染，相对主光源的百分比。
- wiggle: true
  $name: Edit mode wiggle
  $description: While you drag an icon, all icons wiggle like on iOS.
  $name:zh-CN: 编辑时图标轻晃
  $description:zh-CN: 拖动图标时，其他图标轻微晃动以提示编辑状态。
- holdTime: 420
  $name: Drag hold time
  $description: '150 to 1500 (Suggested: 420). Milliseconds to hold an icon before dragging starts.'
  $name:zh-CN: 长按拖动等待时间
  $description:zh-CN: 范围 150–1500 毫秒（建议：420）。长按图标达到此时间后开始拖动。
- animations: true
  $name: Animations
  $description: Off, everything snaps instantly. Opening, folders, hover fades, tile moves and wiggle all turn off.
  $name:zh-CN: 启用动画
  $description:zh-CN: 关闭后立即完成切换，同时停用打开、文件夹、悬停、图标移动和轻晃动画。
- settingsGlass: true
  $name: Blurred settings window
  $description: On, the settings window draws its own rounded glass over a blurred snapshot of the desktop, with
    transparent corners on Windows 11 24H2 and later. Off, it is solid. Applies when you save.
  $name:zh-CN: 设置窗口毛玻璃
  $description:zh-CN: 开启后设置窗口在模糊的桌面快照上绘制自己的圆角玻璃，Windows 11 24H2 及更高版本显示透明圆角；关闭后使用不透明背景。保存后生效。
- nativeSettingsTitleBar: false
  $name: Native Windows title bar
  $description: Use the Windows title bar and window buttons for this settings window. Applies when you save.
  $name:zh-CN: 使用系统标题栏
  $description:zh-CN: 为设置窗口使用 Windows 原生标题栏和窗口按钮。保存后生效。
- settingsTrayIcon: true
  $name: Tray icon
  $description: A Lumiflow icon in the system tray. Click it to open these settings, right-click it for a menu.
  $name:zh-CN: 显示托盘图标
  $description:zh-CN: 在系统托盘显示流光启动台图标。点击打开设置，右键打开菜单。
- gpuPresenter: true
  $name: GPU composition
  $description: Use GPU composition for smoother animation. Turn off for the compatibility renderer.
  $name:zh-CN: GPU 合成呈现
  $description:zh-CN: 使用 GPU 合成呈现以改善动画流畅度；关闭后使用兼容渲染路径。
- performanceLogging: false
  $name: Record performance
  $description: Record opening latency, frame intervals and rendering cost in the mod storage folder.
  $name:zh-CN: 记录性能
  $description:zh-CN: 在模块存储目录记录打开延迟、帧间隔和渲染耗时。
- frameRateLimit: 0
  $name: Frame rate limit
  $description: 0 follows the selected monitor. Other values cap animation updates without changing the monitor.
  $name:zh-CN: 动画帧率上限
  $description:zh-CN: 0 跟随所选显示器刷新率。其他值限制动画更新频率，不改变显示器设置。
- openDuration: 280
  $name: Open duration (ms)
  $description: Duration of the opening transition.
  $name:zh-CN: 打开动画时长（毫秒）
  $description:zh-CN: 控制启动器打开过渡的持续时间。
- closeDuration: 180
  $name: Close duration (ms)
  $description: Duration of the closing transition.
  $name:zh-CN: 关闭动画时长（毫秒）
  $description:zh-CN: 控制启动器关闭过渡的持续时间。
- animationEasing: settle
  $name: Animation easing
  $description: Choose how movement slows down. These curves do not overshoot.
  $options:
  - settle: Gentle settle
  - cubic: Cubic ease-out
  - quint: Quintic ease-out
  $name:zh-CN: 动画缓动
  $description:zh-CN: 选择动画减速收尾的方式，所有曲线均无过度回弹。
  $options:zh-CN:
  - settle: 柔和收尾
  - cubic: 三次方减速
  - quint: 五次方减速
- reducedMotion: false
  $name: Reduce motion
  $description: Use immediate transitions. Windows animation accessibility settings are also respected.
  $name:zh-CN: 减少动画
  $description:zh-CN: 立即完成过渡，同时遵循 Windows 的动画辅助功能设置。
- labelFontSize: 13
  $name: Application name size
  $description: Text size in DIP. Display scaling is applied automatically.
  $name:zh-CN: 应用名称字号
  $description:zh-CN: 使用设备无关像素 DIP，自动适应显示缩放。
- horizontalSpacing: 56
  $name: Horizontal icon spacing
  $description: Preferred horizontal space between icons, in DIP.
  $name:zh-CN: 图标水平间距
  $description:zh-CN: 图标之间的首选水平间距，单位为 DIP。
- verticalSpacing: 16
  $name: Vertical icon spacing
  $description: Additional space between icon rows, in DIP.
  $name:zh-CN: 图标垂直间距
  $description:zh-CN: 图标行之间的额外间距，单位为 DIP。
- groupWidth: 0
  $name: Group width
  $description: Preferred group width in DIP. 0 adapts to the screen and number of groups.
  $name:zh-CN: 分组宽度
  $description:zh-CN: 首选分组宽度，单位为 DIP。0 根据屏幕和分组数量自动调整。
- groupSpacing: 32
  $name: Group spacing
  $description: Minimum horizontal and vertical space between groups, in DIP.
  $name:zh-CN: 分组间距
  $description:zh-CN: 分组之间水平和垂直方向的最小间距，单位为 DIP。
- groupFlow: columns
  $name: Group arrangement
  $description: Arrange groups in columns or use one full-width group per row.
  $options:
  - columns: Responsive columns
  - rows: Full-width rows
  $name:zh-CN: 分组排列方式
  $description:zh-CN: 将分组排列成多列，或每行使用一个全宽分组。
  $options:zh-CN:
  - columns: 自适应多列
  - rows: 每行一个全宽分组
- groupNavigation: scroll
  $name: Group navigation
  $description: Large groups continue on the next page when paging is selected.
  $options:
  - scroll: Vertical scrolling
  - pages: Horizontal pages
  $name:zh-CN: 分组浏览方式
  $description:zh-CN: 选择分页时，大分组的其余应用会在后续页面继续显示。
  $options:zh-CN:
  - scroll: 上下滚动
  - pages: 左右分页
- folderDuration: 180
  $name: Folder duration (ms)
  $description: Duration of folder expansion and collapse.
  $name:zh-CN: 文件夹动画时长（毫秒）
  $description:zh-CN: 文件夹展开与收起的动画时长。
- themeMode: custom
  $name: Theme mode
  $description: Choose a preset, follow Windows, or keep your own colors.
  $options:
  - custom: Custom
  - light: Light
  - dark: Dark
  - system: Follow Windows
  $name:zh-CN: 主题模式
  $description:zh-CN: 选择浅色、深色、跟随 Windows，或保留自定义颜色。
  $options:zh-CN:
  - custom: 自定义
  - light: 浅色
  - dark: 深色
  - system: 跟随 Windows
- backgroundDim: 0
  $name: Dim (%)
  $description: 0 to 80. Darkens the finished material for readability.
  $name:zh-CN: 压暗（%）
  $description:zh-CN: 0–80，压暗成品材质以提高可读性。
- launchpadAnimation: iconFlyIn
  $name: Startup animation style
  $description: Choose how the launcher opens and closes.
  $options:
  - iconFlyIn: iOS icon fly-in
  - classic: Classic zoom and fade
  $name:zh-CN: 启动动画风格
  $description:zh-CN: 选择启动器展开和收起的动画风格。
  $options:zh-CN:
  - iconFlyIn: iOS 图标飞入
  - classic: 经典缩放与淡入淡出
- powerOrder: lock,signOut,sleep,hibernate,restart,shutDown
  $name: Power button order
  $description: 'Comma-separated action keys in left-to-right order: lock, signOut, sleep, hibernate, restart, shutDown.'
  $name:zh-CN: 电源按钮顺序
  $description:zh-CN: 按从左到右的顺序填写操作键，以逗号分隔：lock、signOut、sleep、hibernate、restart、shutDown。
- textShadowOpacity: 72
  $name: Text shadow opacity
  $description: 0 turns off the shadow. 100 makes it fully opaque.
  $name:zh-CN: 文字阴影不透明度
  $description:zh-CN: 0 关闭阴影，100 为完全不透明。
- textShadowBlur: 2
  $name: Text shadow blur
  $description: Blur radius at 100% display scaling. 0 gives the shadow a hard edge.
  $name:zh-CN: 文字阴影模糊
  $description:zh-CN: 显示缩放为 100% 时的模糊半径。0 使用硬边阴影。
- textOutlineOpacity: 75
  $name: Text outline opacity
  $description: 0 turns off the outline. 100 makes it fully opaque.
  $name:zh-CN: 文字描边不透明度
  $description:zh-CN: 0 关闭描边，100 为完全不透明。
- textOutlineWidth: 1
  $name: Text outline width
  $description: Outline width at 100% display scaling. 0 turns off the outline.
  $name:zh-CN: 文字描边宽度
  $description:zh-CN: 显示缩放为 100% 时的描边宽度。0 关闭描边。
- materialPreset: Default
  $name: Material preset
  $description: Changes blur, transparency and texture only; your tint colour stays as it is. Custom keeps the controls
    as they are.
  $options:
  - Default: Default
  - Clear Glass: Clear Glass
  - Light Glass: Light Glass
  - Soft Frost: Soft Frost
  - Light Acrylic: Light Acrylic
  - Dark Acrylic: Dark Acrylic
  - macOS Style: macOS Style
  - Heavy Frost: Heavy Frost
  - Vivid Glass: Vivid Glass
  - Minimal Grey: Minimal Grey
  - custom: Custom
  $name:zh-CN: 材质预设
  $description:zh-CN: 只改模糊、透明度与质感，不改当前着色；选“自定义”则保持当前值。
  $options:zh-CN:
  - Default: 默认
  - Clear Glass: 清透玻璃
  - Light Glass: 轻薄玻璃
  - Soft Frost: 柔和毛玻璃
  - Light Acrylic: 浅色亚克力
  - Dark Acrylic: 深色亚克力
  - macOS Style: macOS 风格
  - Heavy Frost: 重度磨砂
  - Vivid Glass: 浓郁玻璃
  - Minimal Grey: 极简灰白
  - custom: 自定义
- materialPresetName: ''
  $name: Preset name
  $description: Name the Save preset button uses. Leave empty for a numbered name.
  $name:zh-CN: 预设名称
  $description:zh-CN: “保存预设”按钮使用的名称；留空则自动编号。
- materialUserPresets: ''
  $name: Material presets
  $description: The presets saved from the Material page, as JSON. Leave it alone unless you move settings by hand.
  $name:zh-CN: 材质预设
  $description:zh-CN: 材质页保存的预设，JSON 格式。除非手动迁移设置，否则不要改动。
- materialBlur: 36
  $name: Blur radius (DIP)
  $description: Gaussian radius of the backdrop, 0 to 120 DIP. 0 keeps the screen sharp.
  $name:zh-CN: 模糊半径（DIP）
  $description:zh-CN: 背景的高斯半径，0–120 DIP。0 保持屏幕清晰。
- materialSaturation: 100
  $name: Saturation (%)
  $description: 0 is grey, 100 keeps the backdrop's colour, 200 doubles it.
  $name:zh-CN: 饱和度（%）
  $description:zh-CN: 0 为灰度，100 保持背景原色，200 加倍。
- materialBrightness: 100
  $name: Brightness (%)
  $description: 40 to 160. 100 keeps the backdrop as captured.
  $name:zh-CN: 亮度（%）
  $description:zh-CN: 40–160。100 保持截图原样。
- materialContrast: 100
  $name: Contrast (%)
  $description: 50 to 150, pivoting on mid grey. 100 keeps the backdrop as captured.
  $name:zh-CN: 对比度（%）
  $description:zh-CN: 50–150，以中灰为轴。100 保持截图原样。
- materialLuminosity: 0
  $name: Adaptive haze (%)
  $description: Adds a light haze over a dark backdrop and a dark one over a bright backdrop, like acrylic's luminosity
    layer.
  $name:zh-CN: 自适应雾化（%）
  $description:zh-CN: 深色背景加白雾、浅色背景加黑雾，类似亚克力的明度层。
- materialNoise: 0
  $name: Grain (%)
  $description: 0 to 15. Adds fine noise so large flat areas do not band.
  $name:zh-CN: 颗粒（%）
  $description:zh-CN: 0–15。加入细噪点，避免大面积色带。
- materialNoiseScale: 100
  $name: Grain size (%)
  $description: 50 to 200. Larger grain is coarser.
  $name:zh-CN: 颗粒尺寸（%）
  $description:zh-CN: 50–200，数值越大颗粒越粗。
- powerShadowOpacity: 72
  $name: Power icon shadow opacity
  $description: Shadow strength behind the power dock glyphs. It is separate from the text shadow.
  $name:zh-CN: 电源图标阴影不透明度
  $description:zh-CN: 电源坞图标背后的阴影强度，与文字阴影分开调整。
- powerShadowBlur: 2
  $name: Power icon shadow blur
  $description: Blur radius for the power dock glyphs, at 100% display scaling.
  $name:zh-CN: 电源图标阴影模糊
  $description:zh-CN: 电源坞图标的阴影模糊半径，按 100% 显示缩放计算。
- glowDistance: 24
  $name: Glow trigger distance
  $description: Sets how far the nearest app icon reacts outside its bounds and how quickly its rim lights as the
    pointer moves away from the icon centre. At 80, a small movement lights the facing edge.
  $name:zh-CN: 光效触发距离
  $description:zh-CN: 设置指针在应用图标外多远仍可触发光效，以及离开图标中心时边缘的响应速度。调到 80 后，稍微移动指针即可点亮对应方向的边缘。
- webIconBackgroundFetch: true
  $name: Background website icon download
  $description: Fetch missing icons for websites and desktop URL shortcuts after the app list loads. Turn off to
    allow downloads only when you save or manually refresh an entry; cached icons still work.
  $name:zh-CN: 后台下载网站图标
  $description:zh-CN: 应用列表加载后，为网站及桌面 URL 快捷方式获取缺失图标。关闭后仅在保存条目或手动刷新时联网下载；已缓存图标仍可使用。
*/
// ==/WindhawkModSettings==

#include <windows.h>
#include <wincodec.h>
#include <winhttp.h>
#include <shlwapi.h>
#include <regex>
// Original code and credits: DavidHiFi and Rafael Mendes. See the MIT readme above.
#include <windowsx.h>
#include <commctrl.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shobjidl.h>
#include <knownfolders.h>
#include <shellscalingapi.h>
#include <commdlg.h>
#include <uxtheme.h>
#include <vssym32.h>
#include <dwmapi.h>
#include <propsys.h>
#include <powrprof.h>
#include <wuapi.h>
#include <imm.h>
// Otherwise the Win32 macro renames ID2D1RenderTarget::DrawText.
#undef DrawText
#include <d2d1_1.h>
#include <d2d1_3.h>
#include <d2d1effects.h>
#include <d3d11.h>
#include <dwrite.h>
#include <wrl/client.h>
#ifdef SEAMLESS_PREVIEW
#include "preview_api.h"
#else
#include <windhawk_utils.h>
#undef GetCurrentTime
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Automation.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Controls.Primitives.h>
#include <winrt/Windows.UI.Core.h>
#endif
#include "launchpad_i18n.h"
#include "launchpad_presenter.h"
#include "launchpad_metrics.h"
#include "launchpad_settings_store.h"
#include "launchpad_fly_in.h"
#include "launchpad_material.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <deque>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_set>
#include <vector>
#include "ryjs_data.h"

using Microsoft::WRL::ComPtr;
#include "launchpad_app_icon.inc"

extern "C" IMAGE_DOS_HEADER __ImageBase;

// The supplied 3x3 ice-glass sheet is the application mark, not a texture to
// be approximated with a vector glyph. These helpers decode the embedded PNG
// once for the title bar and produce native HICONs for the window and tray.
static std::vector<BYTE> DecodeAppIconBase64(std::string_view text) {
    std::vector<BYTE> out;
    int value = 0, bits = 0;
    for (char c : text) {
        if (c == '=') break;
        int digit = c >= 'A' && c <= 'Z' ? c - 'A'
                    : c >= 'a' && c <= 'z' ? c - 'a' + 26
                    : c >= '0' && c <= '9' ? c - '0' + 52
                    : c == '+' ? 62 : c == '/' ? 63 : -1;
        if (digit < 0) continue;
        value = (value << 6) | digit;
        bits += 6;
        if (bits >= 8) {
            bits -= 8;
            out.push_back((BYTE)((value >> bits) & 0xFF));
        }
    }
    return out;
}

static ComPtr<IWICBitmapSource> DecodeAppIcon(int size, const GUID& format) {
    static const std::vector<BYTE> png = DecodeAppIconBase64(kAppIconPngBase64);
    ComPtr<IWICImagingFactory> wic;
    if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                                __uuidof(IWICImagingFactory), (void**)wic.GetAddressOf())))
        return {};
    ComPtr<IWICStream> stream;
    if (FAILED(wic->CreateStream(stream.GetAddressOf())) ||
        FAILED(stream->InitializeFromMemory(const_cast<BYTE*>(png.data()), (DWORD)png.size())))
        return {};
    ComPtr<IWICBitmapDecoder> decoder;
    if (FAILED(wic->CreateDecoderFromStream(stream.Get(), nullptr, WICDecodeMetadataCacheOnLoad,
                                            decoder.GetAddressOf())))
        return {};
    ComPtr<IWICBitmapFrameDecode> frame;
    if (FAILED(decoder->GetFrame(0, frame.GetAddressOf()))) return {};
    ComPtr<IWICBitmapSource> scaled;
    if (size > 0) {
        ComPtr<IWICBitmapScaler> scaler;
        if (FAILED(wic->CreateBitmapScaler(scaler.GetAddressOf())) ||
            FAILED(scaler->Initialize(frame.Get(), (UINT)size, (UINT)size,
                                      WICBitmapInterpolationModeCubic)))
            return {};
        scaled = scaler;
    } else {
        scaled = frame;
    }
    ComPtr<IWICFormatConverter> converter;
    if (FAILED(wic->CreateFormatConverter(converter.GetAddressOf())) ||
        FAILED(converter->Initialize(scaled.Get(), format, WICBitmapDitherTypeNone, nullptr,
                                     0, WICBitmapPaletteTypeCustom)))
        return {};
    ComPtr<IWICBitmapSource> result;
    converter.As(&result);
    return result;
}

static ComPtr<ID2D1Bitmap> CreateAppIconBitmap(ID2D1RenderTarget* rt) {
    if (!rt) return {};
    auto source = DecodeAppIcon(256, GUID_WICPixelFormat32bppPBGRA);
    if (!source) return {};
    ComPtr<ID2D1Bitmap> bitmap;
    rt->CreateBitmapFromWicBitmap(source.Get(), nullptr, bitmap.GetAddressOf());
    return bitmap;
}

static HICON CreateAppIconHicon(int size) {
    if (size <= 0) return nullptr;
    auto source = DecodeAppIcon(size, GUID_WICPixelFormat32bppBGRA);
    if (!source) return nullptr;
    std::vector<BYTE> pix((size_t)size * size * 4);
    if (FAILED(source->CopyPixels(nullptr, size * 4, (UINT)pix.size(), pix.data()))) return nullptr;
    BITMAPV5HEADER bi{};
    bi.bV5Size = sizeof(bi);
    bi.bV5Width = size;
    bi.bV5Height = -size;
    bi.bV5Planes = 1;
    bi.bV5BitCount = 32;
    bi.bV5Compression = BI_BITFIELDS;
    bi.bV5RedMask = 0x00FF0000;
    bi.bV5GreenMask = 0x0000FF00;
    bi.bV5BlueMask = 0x000000FF;
    bi.bV5AlphaMask = 0xFF000000;
    void* bits = nullptr;
    HDC dc = GetDC(nullptr);
    HBITMAP color = CreateDIBSection(dc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &bits, nullptr, 0);
    ReleaseDC(nullptr, dc);
    if (!color || !bits) return nullptr;
    memcpy(bits, pix.data(), pix.size());
    std::vector<BYTE> maskBits((size_t)((size + 15) / 16) * 2 * size, 0);
    HBITMAP mask = CreateBitmap(size, size, 1, 1, maskBits.data());
    ICONINFO ii{TRUE, 0, 0, mask, color};
    HICON icon = CreateIconIndirect(&ii);
    DeleteObject(mask);
    DeleteObject(color);
    return icon;
}

namespace {

using launchpad_i18n::Tr;

// Start-button reset messages are handled synchronously by a taskbar-window
// subclass. No module lambda is queued on another thread's dispatcher.

#ifdef SEAMLESS_PREVIEW
constexpr WCHAR kWindowClass[] = L"RYJSLaunchpadPreview";
constexpr WCHAR kBackdropClass[] = L"RYJSLaunchpadPreviewBackdrop";
#else
constexpr WCHAR kWindowClass[] = L"WindhawkRYJSLaunchpad";
constexpr WCHAR kBackdropClass[] = L"WindhawkRYJSLaunchpadBackdrop";
#endif
constexpr WCHAR kAppsFolder[] = L"shell:AppsFolder\\";

// From the Standalone Dock mod: the edge strip it takes.
constexpr WCHAR kDockClass[] = L"WindhawkMacDock";
constexpr WCHAR kDockPlateClass[] = L"WindhawkMacDockPlate";
constexpr WCHAR kDockEdgeProp[] = L"WindhawkMacDock.Edge";
constexpr WCHAR kDockReservedProp[] = L"WindhawkMacDock.Reserved";

constexpr UINT WM_APP_TOGGLE = WM_APP + 1;
constexpr UINT WM_APP_APPS = WM_APP + 2;  // lParam: new AppList*.
constexpr UINT WM_APP_RELOAD = WM_APP + 3;
constexpr UINT WM_APP_QUIT = WM_APP + 4;
constexpr UINT WM_APP_RUN = WM_APP + 5;
constexpr UINT WM_APP_SHIFT_WIN = WM_APP + 6;
// A lone Windows key tap, which the grid answers by closing (see KeyboardHook:
// once the mod answered Start's first show, Windows never asks again, so the
// second tap has to be caught at the key itself).
constexpr UINT WM_APP_WIN_TAP = WM_APP + 7;
constexpr UINT kPowerUpdateStatusReady = WM_APP + 66;

constexpr UINT_PTR kTimerFrame = 1;
constexpr UINT_PTR kTimerFirstLoad = 4;
constexpr UINT_PTR kTimerShiftWin = 5;
constexpr UINT_PTR kTimerPowerUpdate = 8;
constexpr UINT_PTR kTimerCaret = 6;  // The search caret's blink.
constexpr UINT_PTR kTimerPowerArm = 7;  // An unconfirmed power button resets.

constexpr GUID kBhidEnumItems = {
    0x94f60519,
    0x2850,
    0x4924,
    {0xaa, 0x5a, 0xd1, 0x5e, 0x84, 0x86, 0x80, 0x39}};

double Now() {
    static LARGE_INTEGER frequency = [] {
        LARGE_INTEGER f;
        QueryPerformanceFrequency(&f);
        return f;
    }();
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / frequency.QuadPart;
}

std::wstring ToLower(std::wstring text) {
    if (!text.empty()) {
        LCMapStringEx(LOCALE_NAME_USER_DEFAULT, LCMAP_LOWERCASE, text.data(),
                      (int)text.size(), text.data(), (int)text.size(),
                      nullptr, nullptr, 0);
    }
    return text;
}

bool EndsWith(const std::wstring& text, const std::wstring& suffix) {
    return text.size() >= suffix.size() &&
           text.compare(text.size() - suffix.size(), suffix.size(), suffix) ==
               0;
}

////////////////////////////////////////////////////////////////////////////////
// Settings

enum class AppSource { AllApps, Desktop, Both, Pinned };
enum class KeyAction { Grid, Start, Program };
enum class DockAlign { Left, Center, Right };

enum class PowerAction { Lock, SignOut, Sleep, Hibernate, Restart, ShutDown };

struct PowerButton {
    PCWSTR key;     // Under powerButtons. in the settings.
    PCWSTR label;
    PCWSTR verb;    // "Click again to <verb>".
    WCHAR glyph;    // Segoe Fluent Icons.
    bool danger;    // Requires confirmation before activation.
};

constexpr PowerButton kPower[] = {
    {L"lock", L"Lock", L"lock", 0xE72E, false},
    {L"signOut", L"Sign out", L"sign out", 0xF3B1, true},
    {L"sleep", L"Sleep", L"sleep", 0xE708, false},
    {L"hibernate", L"Hibernate", L"hibernate", 0xE823, false},
    {L"restart", L"Restart", L"restart", 0xE72C, true},
    {L"shutDown", L"Shut down", L"shut down", 0xE7E8, true},
};

using PowerOrder = std::array<int, std::size(kPower)>;

constexpr PowerOrder DefaultPowerOrder() {
    PowerOrder order{};
    for (int i = 0; i < (int)order.size(); ++i) order[i] = i;
    return order;
}

int PowerActionIndex(std::wstring_view key) {
    for (int i = 0; i < (int)std::size(kPower); ++i)
        if (key == kPower[i].key) return i;
    return -1;
}

PowerOrder ParsePowerOrder(std::wstring_view text) {
    PowerOrder order{};
    std::array<bool, std::size(kPower)> seen{};
    size_t count = 0;
    while (!text.empty()) {
        const auto comma = text.find(L',');
        auto key = text.substr(0, comma);
        const auto start = key.find_first_not_of(L" \t");
        if (start != std::wstring_view::npos) {
            key = key.substr(start, key.find_last_not_of(L" \t") - start + 1);
            const int action = PowerActionIndex(key);
            if (action >= 0 && !seen[action]) {
                order[count++] = action;
                seen[action] = true;
            }
        }
        if (comma == std::wstring_view::npos) break;
        text.remove_prefix(comma + 1);
    }
    for (int action : DefaultPowerOrder())
        if (!seen[action]) order[count++] = action;
    return order;
}

std::wstring FormatPowerOrder(const PowerOrder& order) {
    std::wstring text;
    for (int action : order) {
        if (!text.empty()) text += L',';
        text += kPower[action].key;
    }
    return text;
}

struct TextEffects {
    int shadowOpacity = 72;
    int shadowBlur = 2;
    int outlineOpacity = 75;
    int outlineWidth = 1;
};

bool SameTextEffects(const TextEffects& a, const TextEffects& b) {
    return a.shadowOpacity == b.shadowOpacity && a.shadowBlur == b.shadowBlur &&
           a.outlineOpacity == b.outlineOpacity && a.outlineWidth == b.outlineWidth;
}

struct Settings {
    std::wstring font;
    std::wstring theme=L"custom";
    float backgroundDim=0;
    bool gpuPresenter = true, performanceLogging = false, reducedMotion = false;
    bool iconFlyIn = true;
    int openMs = 280, closeMs = 180, folderMs = 180, frameRateLimit = 0;
    std::wstring easing = L"settle";
    bool groups = false, groupRows = false;
    int groupWidth = 0, groupGap = 32, horizontalGap = 56, verticalGap = 16;
    float labelSize = 13;
    TextEffects textEffects;
    // Power dock icons get their own shadow strength, so tuning the label
    // shadow does not wash out (or erase) the glyphs at the bottom.
    TextEffects powerEffects;
    // Background material. Blur, grade, tint and noise live in one parameter
    // block shared by the engine, the presets and the settings page.
    launchpad_material::Backend backend = launchpad_material::Backend::Custom;
    launchpad_material::Params material;
    int blurAmount = 60;  // Legacy 0..100 strength, read only when materialBlur is missing.
    float opacity = 0.3f;                 // Mirrored into material.tintOpacity.
    D2D1_COLOR_F tint{0, 0, 0, 1};        // Mirrored into material.tint.
    int iconSize = 56;
    int columns = 0, globalColumns = 0;
    bool pages = false;
    bool coverScreen = false;
    AppSource source = AppSource::AllApps;
    std::wstring monitor = L"mouse";
    std::wstring program;
    bool webIconBackgroundFetch = true;
    bool powerBar = true;
    bool powerShown[std::size(kPower)] = {true, true, true, true, true, true};  // By action ID.
    PowerOrder powerOrder = DefaultPowerOrder();
    bool powerNoConfirm = false;
    // The warning light lives inside the shared power capsule. It is one
    // source, driven by the pointer or by the hovered button, and these values
    // are the ones the settings page writes.
    bool powerWarning = true;
    bool powerWarningActions[std::size(kPower)] = {false, true, false, false, true, true};
    bool powerWarningFollowButton = false;  // false: pointer, true: button centre.
    D2D1_COLOR_F powerWarningColor{169.f / 255.f, 18.f / 255.f, 35.f / 255.f, 1.f};
    int powerWarningStrength = 78;  // Percent of the centre concentration.
    int powerWarningSpread = 100;    // Percent of the 48 DIP reference radius.
    int powerWarningEdge = 65;      // Percent of the centre concentration.
    int powerWarningBleed = 15;     // Percent of the centre concentration.
    // 3.0.0 look and feel. Defaults here; schema and parsing in LoadSettings.
    float glowStrength = 0.6f;    // 0..1, hover/focus glow alpha.
    int glowDistance = 24;        // Outside reach in DIP and directional rim response.
    float chromeOpacity = 0.9f;   // 0..1, search bar and dock fill (Mocha base).
    DockAlign dockAlign = DockAlign::Center;
    bool wiggle = true;           // Edit-mode wiggle while dragging.
    bool animations = true;       // Off: every transition snaps.
    bool trayIcon = true;         // A tray icon that opens the settings.
    int holdMs = 420;             // Press time before an icon lifts for dragging.
    bool showLabels = true;
    bool normalizeIcons = true;   // Scale small/padded icons up to the grid size, no frame.
};

Settings g_settings;
std::atomic<bool> g_systemAnimations{true};
std::atomic<bool> g_requestedAnimations{true};

void RefreshSystemAnimations() {
    BOOL enabled = TRUE;
    SystemParametersInfoW(SPI_GETCLIENTAREAANIMATION, 0, &enabled, 0);
    g_systemAnimations = enabled != FALSE;
}

// The mod's own switch plus the Windows "Animation effects" setting.
bool Animate() {
    return g_requestedAnimations.load() && g_systemAnimations.load();
}
// Also read in the ShowStartView hook, on Explorer's threads.
std::atomic<KeyAction> g_winKeyAction{KeyAction::Grid};
std::atomic<KeyAction> g_shiftWinKeyAction{KeyAction::Start};
std::atomic<KeyAction> g_startButtonAction{KeyAction::Grid};

std::wstring DataRoot();
std::wstring ModTempPath();
std::wstring_view DeclaredSettingDefault(std::wstring_view key);

std::wstring GetStringSetting(PCWSTR name) {
#ifdef SEAMLESS_PREVIEW
    return Preview::Setting(name);
#else
    std::wstring host = WindhawkUtils::StringSetting::make(name).get();
    auto root = DataRoot();
    if (root.empty()) return host;
    return launchpad_settings_store::Resolve(root + L"\\ui-settings.ini", name, host,
                                             DeclaredSettingDefault(name));
#endif
}

// Windhawk stores a number or boolean setting as REG_DWORD, and
// Wh_GetStringSetting converts a DWORD to its decimal string (see
// PortableSettings::RawItemToString in the Windhawk engine). Reading it as a
// string therefore works for both storage shapes and still distinguishes a
// missing key (empty string, use the schema default) from a real zero.
int IntSetting(PCWSTR name, int def) {
    std::wstring value = GetStringSetting(name);
    return value.empty() ? def : _wtoi(value.c_str());
}

D2D1_COLOR_F ParseColor(std::wstring text, D2D1_COLOR_F fallback) {
    text.erase(std::remove_if(text.begin(), text.end(), iswspace), text.end());
    if (!text.empty() && text[0] == L'#') {
        text.erase(0, 1);
    }
    if (text.size() != 6 ||
        !std::all_of(text.begin(), text.end(), iswxdigit)) {
        return fallback;
    }
    UINT rgb = wcstoul(text.c_str(), nullptr, 16);
    return D2D1::ColorF(((rgb >> 16) & 0xFF) / 255.0f,
                        ((rgb >> 8) & 0xFF) / 255.0f, (rgb & 0xFF) / 255.0f);
}

TextEffects ReadTextEffects() {
    return {
        std::clamp(IntSetting(L"textShadowOpacity", 72), 0, 100),
        std::clamp(IntSetting(L"textShadowBlur", 2), 0, 6),
        std::clamp(IntSetting(L"textOutlineOpacity", 75), 0, 100),
        std::clamp(IntSetting(L"textOutlineWidth", 1), 0, 3),
    };
}

TextEffects ReadPowerEffects() {
    TextEffects e = ReadTextEffects();
    e.shadowOpacity = std::clamp(IntSetting(L"powerShadowOpacity", 72), 0, 100);
    e.shadowBlur = std::clamp(IntSetting(L"powerShadowBlur", 2), 0, 6);
    return e;
}

static void MigrateLegacyPresetTint();

void LoadSettings() {
    launchpad_i18n::Select(GetStringSetting(L"interfaceLanguage"));
    Settings s;
    RefreshSystemAnimations();
    s.gpuPresenter = IntSetting(L"gpuPresenter", 1) != 0;
    s.performanceLogging = IntSetting(L"performanceLogging", 0) != 0;
    s.reducedMotion = IntSetting(L"reducedMotion", 0) != 0;
    s.iconFlyIn = GetStringSetting(L"launchpadAnimation") != L"classic";
    s.openMs = std::clamp(IntSetting(L"openDuration", 280), 60, 1500);
    s.closeMs = std::clamp(IntSetting(L"closeDuration", 180), 60, 1500);
    s.folderMs = std::clamp(IntSetting(L"folderDuration", 180), 60, 1500);
    s.frameRateLimit = std::clamp(IntSetting(L"frameRateLimit", 0), 0, 1000);
    s.easing = GetStringSetting(L"animationEasing");
    if (s.easing.empty()) s.easing = L"settle";
    s.font = GetStringSetting(L"uiFont");
    if (s.font.empty() || s.font == L"auto") s.font = launchpad_i18n::Font();
    std::wstring mode = GetStringSetting(L"blurMode");
    s.backend = launchpad_material::BackendFromKey(mode);
    s.blurAmount = std::clamp(IntSetting(L"blurAmount", 60), 0, 100);
    s.opacity =
        std::clamp(IntSetting(L"backgroundOpacity", 30), 0, 100) / 100.0f;
    s.tint = ParseColor(GetStringSetting(L"tintColor"), s.tint);
    // Material parameters. materialBlur is in DIP; an upgraded config without
    // it keeps the old 0..100 strength, whose 60 default was 36 DIP.
    std::wstring materialBlur = GetStringSetting(L"materialBlur");
    s.material.blurSigma =
        std::clamp(materialBlur.empty() ? s.blurAmount * .6f
                                        : (float)_wtoi(materialBlur.c_str()),
                   0.f, 120.f);
    s.material.saturation = std::clamp(IntSetting(L"materialSaturation", 100), 0, 200) / 100.0f;
    s.material.brightness = std::clamp(IntSetting(L"materialBrightness", 100), 40, 160) / 100.0f;
    s.material.contrast = std::clamp(IntSetting(L"materialContrast", 100), 50, 150) / 100.0f;
    s.material.luminosity = std::clamp(IntSetting(L"materialLuminosity", 0), 0, 100) / 100.0f;
    s.material.noise = std::clamp(IntSetting(L"materialNoise", 0), 0, 15) / 100.0f;
    s.material.noiseScale = std::clamp(IntSetting(L"materialNoiseScale", 100), 50, 200) / 100.0f;
    s.iconSize = std::clamp(IntSetting(L"gridIconSize", 56), 24, 192);
    s.columns = std::clamp(IntSetting(L"gridColumns", 0), 0, 30);
    s.globalColumns=std::clamp(IntSetting(L"globalColumns",0),0,30);
    s.groups = GetStringSetting(L"gridLayout").empty() || GetStringSetting(L"gridLayout") == L"groups";
    s.pages = GetStringSetting(L"gridLayout") == L"pages" || (s.groups && GetStringSetting(L"groupNavigation") == L"pages");
    s.groupRows = GetStringSetting(L"groupFlow") == L"rows";
    s.groupWidth = std::clamp(IntSetting(L"groupWidth", 0), 0, 1600);
    s.groupGap = std::clamp(IntSetting(L"groupSpacing", 32), 8, 160);
    s.horizontalGap = std::clamp(IntSetting(L"horizontalSpacing", 56), 8, 240);
    s.verticalGap = std::clamp(IntSetting(L"verticalSpacing", 16), 0, 180);
    s.labelSize = std::clamp(IntSetting(L"labelFontSize", 13), 9, 32);
    s.textEffects = ReadTextEffects();
    s.powerEffects = ReadPowerEffects();
    s.coverScreen = IntSetting(L"coverScreen", 0) != 0;
    std::wstring source = GetStringSetting(L"appSource");
    s.source = source == L"desktop"  ? AppSource::Desktop
               : source == L"both"   ? AppSource::Both
               : source == L"pinned" ? AppSource::Pinned
                                     : AppSource::AllApps;
    s.monitor = GetStringSetting(L"monitor");
    s.program = GetStringSetting(L"program");
    s.webIconBackgroundFetch = IntSetting(L"webIconBackgroundFetch", 1) != 0;
    s.powerBar = IntSetting(L"powerBar", 1) != 0;
    s.powerOrder = ParsePowerOrder(GetStringSetting(L"powerOrder"));
    for (int i : DefaultPowerOrder()) {
        s.powerShown[i] =
            IntSetting((std::wstring(L"powerButtons.") + kPower[i].key).c_str(), 1) != 0;
    }
    s.powerNoConfirm = IntSetting(L"powerNoConfirm", 0) != 0;
    s.powerWarning = IntSetting(L"powerWarning", 1) != 0;
    for (int i = 0; i < (int)std::size(kPower); ++i)
        s.powerWarningActions[i] =
            IntSetting((std::wstring(L"powerWarningActions.") + kPower[i].key).c_str(),
                       kPower[i].danger ? 1 : 0) != 0;
    s.powerWarningFollowButton = ToLower(GetStringSetting(L"powerWarningFollow")) == L"button";
    s.powerWarningColor = ParseColor(GetStringSetting(L"powerWarningColor"), D2D1::ColorF(0xA91223));
    s.powerWarningColor.a = 1.0f;
    s.powerWarningStrength = std::clamp(IntSetting(L"powerWarningStrength", 78), 0, 100);
    s.powerWarningSpread = std::clamp(IntSetting(L"powerWarningSpread", 100), 40, 200);
    s.powerWarningEdge = std::clamp(IntSetting(L"powerWarningEdge", 65), 0, 100);
    s.powerWarningBleed = std::clamp(IntSetting(L"powerWarningBleed", 15), 0, 40);
    s.glowStrength =
        std::clamp(IntSetting(L"glowStrength", 60), 0, 100) / 100.0f;
    s.glowDistance = std::clamp(IntSetting(L"glowDistance", 24), 0, 80);
    s.chromeOpacity =
        std::clamp(IntSetting(L"chromeOpacity", 90), 0, 100) / 100.0f;
    std::wstring dock = ToLower(GetStringSetting(L"dockAlignment"));
    s.dockAlign = dock == L"left"    ? DockAlign::Left
                  : dock == L"right" ? DockAlign::Right
                                     : DockAlign::Center;
    s.wiggle = IntSetting(L"wiggle", 1) != 0;
    s.animations = IntSetting(L"animations", 1) != 0;
    s.holdMs = std::clamp(IntSetting(L"holdTime", 420), 150, 1500);
    s.showLabels = IntSetting(L"showLabels", 1) != 0;
    s.normalizeIcons = IntSetting(L"normalizeIcons", 1) != 0;
    s.trayIcon = IntSetting(L"settingsTrayIcon", 1) != 0;
    s.theme=GetStringSetting(L"themeMode");
    s.backgroundDim=std::clamp(IntSetting(L"backgroundDim",0),0,80)/100.0f;
    bool light=s.theme==L"light";
    if(s.theme==L"system") {
        DWORD value=0,size=sizeof(value);
        RegGetValueW(HKEY_CURRENT_USER,L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",L"AppsUseLightTheme",RRF_RT_REG_DWORD,nullptr,&value,&size);
        light=value!=0;
    }
    if(s.theme==L"light"||s.theme==L"dark"||s.theme==L"system") {
        s.tint=D2D1::ColorF(light?0xEEEEEE:0x141414);
        s.opacity=std::max(s.opacity,light?0.55f:0.40f);
    }
    // The theme may have replaced the tint above, so mirror it last.
    s.material.tintOpacity = s.opacity;
    s.material.tint = s.tint;
    s.material.dim = s.backgroundDim;
    launchpad_material::Clamp(s.material);
    g_settings = s;
    MigrateLegacyPresetTint();
    g_requestedAnimations = s.animations && !s.reducedMotion;
    auto action = [](PCWSTR name) {
        std::wstring value = GetStringSetting(name);
        return value == L"start"     ? KeyAction::Start
               : value == L"program" ? KeyAction::Program
                                     : KeyAction::Grid;
    };
    g_winKeyAction = action(L"winKeyAction");
    g_shiftWinKeyAction = action(L"shiftWinKeyAction");
    g_startButtonAction = action(L"startButtonAction");
}

// Runs the "program" setting: a command line, or else anything ShellExecute
// opens (a shortcut, file, folder or web address).
void RunProgram() {
    std::wstring command = g_settings.program;
    command.erase(0, command.find_first_not_of(L" \t"));
    command.erase(command.find_last_not_of(L" \t") + 1);
    if (command.empty()) {
        return;
    }
    STARTUPINFOW si{sizeof(si)};
    PROCESS_INFORMATION pi{};
    std::wstring line = command;
    if (CreateProcessW(nullptr, line.data(), nullptr, nullptr, FALSE, 0,
                       nullptr, nullptr, &si, &pi)) {
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return;
    }
    if (command.size() > 1 && command.front() == L'"' &&
        command.back() == L'"') {
        command = command.substr(1, command.size() - 2);
    }
    ShellExecuteW(nullptr, nullptr, command.c_str(), nullptr, nullptr,
                  SW_SHOWNORMAL);
}

////////////////////////////////////////////////////////////////////////////////
// App list
//
// Comes from the virtual folder "shell:AppsFolder" (the "All apps" list of
// Start), the desktop, or Start's pins, depending on the setting. It's read on
// a separate thread, with the icons, and handed ready to the window.

struct App {
    std::wstring name;
    std::wstring originalName, stableKey, layoutTarget, arguments, workingDirectory, websiteUrl;
    bool manual = false, unavailable = false;
    std::wstring target;  // shell:AppsFolder\... or a path, opened with ShellExecute.
    bool desktop = false;  // Came from the desktop.
    std::vector<UINT32> pixels;  // Square icon, premultiplied BGRA.
    int iconSize = 0;
    ComPtr<ID2D1Bitmap> bitmap;
    UINT generation = 0;
};
using AppList = std::vector<std::shared_ptr<App>>;

// The globals that hold COM objects (D2D bitmaps, factories, text formats) are
// released by the Start thread before it ends (no_destroy): when Explorer
// exits, Wh_ModUninit isn't called, the thread is already gone, and static
// destructors would release them on the exiting thread under the loader lock.
[[clang::no_destroy]] AppList g_apps;
bool g_appsLoaded;
double g_appsTime;
int g_appsKey;  // AppsKey() of the loaded list.
bool g_appsNormalized = true;  // "Fill small icons" when the list was read.

HANDLE g_worker;
std::atomic<bool> g_workerStop;
std::atomic<bool> g_fullImportBusy=false,g_fullBackupBusy=false;
std::atomic<HWND> g_wnd;

// Bounds of the pixels whose alpha is over a threshold, optionally inside a
// clip box.
struct IconBox {
    int left = 0, top = 0, right = -1, bottom = -1;
    bool empty() const { return right < left || bottom < top; }
    int width() const { return right - left + 1; }
    int height() const { return bottom - top + 1; }
};

static IconBox ContentBounds(const std::vector<UINT32>& pixels, int width,
                             int height, UINT32 threshold,
                             const IconBox& clip) {
    IconBox b;
    b.left = clip.right + 1;
    b.top = clip.bottom + 1;
    for (int y = std::max(0, clip.top); y <= std::min(height - 1, clip.bottom);
         y++) {
        for (int x = std::max(0, clip.left);
             x <= std::min(width - 1, clip.right); x++) {
            if ((pixels[(size_t)y * width + x] >> 24) > threshold) {
                b.left = std::min(b.left, x);
                b.right = std::max(b.right, x);
                b.top = std::min(b.top, y);
                b.bottom = std::max(b.bottom, y);
            }
        }
    }
    return b;
}

// Some icons arrive as a small image centred on a large canvas, often inside
// a thin frame the shell drew: a near-opaque ring of one colour along the
// edge over a faint fill. Strips such a ring, but only when what is left
// inside is really smaller, so full-bleed icons keep their edges.
static void StripIconFrame(const std::vector<UINT32>& pixels, int width,
                           int height, IconBox& box) {
    // A side is a frame line when nearly all of it is opaque in one colour.
    auto frameLine = [&](bool vertical, int pos, int from, int to) {
        int dense = 0, total = 0;
        UINT minR = 255, maxR = 0, minG = 255, maxG = 0, minB = 255, maxB = 0;
        for (int i = from; i <= to; i++) {
            UINT32 p = vertical ? pixels[(size_t)i * width + pos]
                                : pixels[(size_t)pos * width + i];
            total++;
            if ((p >> 24) <= 200) {
                continue;
            }
            dense++;
            UINT r = (p >> 16) & 0xFF, g = (p >> 8) & 0xFF, b = p & 0xFF;
            minR = std::min(minR, r);
            maxR = std::max(maxR, r);
            minG = std::min(minG, g);
            maxG = std::max(maxG, g);
            minB = std::min(minB, b);
            maxB = std::max(maxB, b);
        }
        return dense && dense * 10 >= total * 9 && maxR - minR < 40 &&
               maxG - minG < 40 && maxB - minB < 40;
    };
    int strip[4] = {};  // Left, top, right, bottom.
    // Keep at least one row and one column inside the box: a small, evenly
    // opaque icon otherwise walks a side past the opposite one and reads
    // pixels outside the image.
    auto room = [&] {
        return box.top + strip[1] < box.bottom - strip[3] &&
               box.left + strip[0] < box.right - strip[2];
    };
    for (int step = 0; step < 12 && room(); step++) {
        if (frameLine(false, box.top + strip[1], box.left + strip[0],
                      box.right - strip[2])) {
            strip[1]++;
        }
        if (frameLine(false, box.bottom - strip[3], box.left + strip[0],
                      box.right - strip[2])) {
            strip[3]++;
        }
        if (frameLine(true, box.left + strip[0], box.top + strip[1],
                      box.bottom - strip[3])) {
            strip[0]++;
        }
        if (frameLine(true, box.right - strip[2], box.top + strip[1],
                      box.bottom - strip[3])) {
            strip[2]++;
        }
    }
    if (!strip[0] && !strip[1] && !strip[2] && !strip[3]) {
        return;
    }
    IconBox rest;
    rest.left = box.left + strip[0];
    rest.top = box.top + strip[1];
    rest.right = box.right - strip[2];
    rest.bottom = box.bottom - strip[3];
    if (rest.empty()) {
        return;
    }
    IconBox dense = ContentBounds(pixels, width, height, 200, rest);
    if (dense.empty()) {
        dense = ContentBounds(pixels, width, height, 8, rest);
    }
    if (dense.empty() ||
        std::max(dense.width(), dense.height()) * 4 >=
            std::max(box.width(), box.height()) * 3) {
        return;  // The icon fills the box: the edges were its own.
    }
    box = dense;
}

// Box-average resample of a region, in premultiplied pixels.
static std::vector<UINT32> Resample(const std::vector<UINT32>& pixels,
                                    int width, int height, const IconBox& box,
                                    int outW, int outH) {
    std::vector<UINT32> out((size_t)outW * outH, 0);
    for (int oy = 0; oy < outH; oy++) {
        int sy0 = box.top + (int)((double)oy * box.height() / outH);
        int sy1 = std::max(
            sy0 + 1, box.top + (int)((double)(oy + 1) * box.height() / outH));
        for (int ox = 0; ox < outW; ox++) {
            int sx0 = box.left + (int)((double)ox * box.width() / outW);
            int sx1 = std::max(sx0 + 1, box.left +
                                            (int)((double)(ox + 1) *
                                                  box.width() / outW));
            UINT a = 0, r = 0, g = 0, b = 0, n = 0;
            for (int sy = sy0; sy < std::min(sy1, height); sy++) {
                for (int sx = sx0; sx < std::min(sx1, width); sx++) {
                    UINT p = pixels[(size_t)sy * width + sx];
                    a += p >> 24;
                    r += (p >> 16) & 0xFF;
                    g += (p >> 8) & 0xFF;
                    b += p & 0xFF;
                    n++;
                }
            }
            if (!n) {
                continue;
            }
            out[(size_t)oy * outW + ox] =
                ((a / n) << 24) | ((r / n) << 16) | ((g / n) << 8) | (b / n);
        }
    }
    return out;
}

// Centres a side*side square crop of the source into the app's pixels.
static void CenterCopy(const std::vector<UINT32>& pixels, int width,
                       int height, int cx, int cy, int side, App& app) {
    app.iconSize = side;
    app.pixels.assign((size_t)side * side, 0);
    int x0 = cx - side / 2, y0 = cy - side / 2;
    for (int y = 0; y < side; y++) {
        int sy = y0 + y;
        if (sy < 0 || sy >= height) {
            continue;
        }
        for (int x = 0; x < side; x++) {
            int sx = x0 + x;
            if (sx >= 0 && sx < width) {
                app.pixels[(size_t)y * side + x] = pixels[sy * width + sx];
            }
        }
    }
}

// Normalizes the alpha, then crops the icon to its content. The shell often
// returns a small image centred in a large canvas, sometimes inside a thin
// frame. With "Normalize icons" the content is also scaled up to the icon
// size, aspect kept, so every icon reads at the same size; without it only
// the old small-icon crop applies.
void FinishIcon(std::vector<UINT32> pixels, int width, int height, App& app) {
    if (std::none_of(pixels.begin(), pixels.end(),
                     [](UINT32 p) { return (p >> 24) != 0; })) {
        for (auto& p : pixels) {
            p |= 0xFF000000;
        }
    }
    bool premultiplied =
        std::none_of(pixels.begin(), pixels.end(), [](UINT32 p) {
            UINT a = p >> 24;
            return ((p >> 16) & 0xFF) > a || ((p >> 8) & 0xFF) > a ||
                   (p & 0xFF) > a;
        });
    if (!premultiplied) {
        for (auto& p : pixels) {
            UINT a = p >> 24;
            UINT r = ((p >> 16) & 0xFF) * a / 255;
            UINT g = ((p >> 8) & 0xFF) * a / 255;
            UINT b = (p & 0xFF) * a / 255;
            p = (a << 24) | (r << 16) | (g << 8) | b;
        }
    }

    IconBox box = ContentBounds(pixels, width, height, 8,
                                {0, 0, width - 1, height - 1});
    if (box.empty()) {
        return;
    }
    int full = std::max(width, height);
    if (!g_settings.normalizeIcons) {
        // The old crop: a small icon centred in padding, no resampling.
        int content = std::max(box.width(), box.height());
        if (content < full * 0.6f) {
            CenterCopy(pixels, width, height, (box.left + box.right + 1) / 2,
                       (box.top + box.bottom + 1) / 2,
                       std::min(full, (int)std::ceil(content * 1.08f)), app);
        } else {
            CenterCopy(pixels, width, height, width / 2, height / 2, full,
                       app);
        }
        return;
    }

    // Drop a shell-drawn frame, then find the real icon in what is left: the
    // dense pixels, ignoring the faint fill a frame canvas comes with.
    StripIconFrame(pixels, width, height, box);
    IconBox dense = ContentBounds(pixels, width, height, 200, box);
    if (dense.empty()) {
        dense = box;
    }
    int content = std::max(dense.width(), dense.height());
    if (content * 20 >= full * 17) {
        // Already near full size: centre it, no resampling, no upscaling.
        CenterCopy(pixels, width, height, (dense.left + dense.right + 1) / 2,
                   (dense.top + dense.bottom + 1) / 2, full, app);
        return;
    }
    // Resample the content up to the icon size, aspect kept, centred.
    float k = (float)full / content;
    int outW = std::max(1, (int)std::lround(dense.width() * k));
    int outH = std::max(1, (int)std::lround(dense.height() * k));
    std::vector<UINT32> scaled =
        Resample(pixels, width, height, dense, outW, outH);
    app.iconSize = full;
    app.pixels.assign((size_t)full * full, 0);
    int x0 = (full - outW) / 2, y0 = (full - outH) / 2;
    for (int y = 0; y < outH; y++) {
        for (int x = 0; x < outW; x++) {
            app.pixels[(size_t)(y0 + y) * full + x0 + x] =
                scaled[(size_t)y * outW + x];
        }
    }
}

// The default flags give the item's icon; without SIIGBF_ICONONLY a picture
// gives its own image.
void LoadAppIcon(IShellItem* item,
                 int size,
                 App& app,
                 SIIGBF flags = SIIGBF_ICONONLY | SIIGBF_BIGGERSIZEOK) {
    ComPtr<IShellItemImageFactory> factory;
    HBITMAP bitmap = nullptr;
    if (FAILED(item->QueryInterface(IID_PPV_ARGS(&factory))) ||
        FAILED(factory->GetImage(SIZE{size, size}, flags, &bitmap))) {
        return;
    }
    BITMAP bm{};
    if (GetObjectW(bitmap, sizeof(bm), &bm) && bm.bmWidth > 0) {
        int width = bm.bmWidth, height = std::abs(bm.bmHeight);
        std::vector<UINT32> pixels((size_t)width * height);
        BITMAPINFO bi{};
        bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
        bi.bmiHeader.biWidth = width;
        bi.bmiHeader.biHeight = -height;
        bi.bmiHeader.biPlanes = 1;
        bi.bmiHeader.biBitCount = 32;
        bi.bmiHeader.biCompression = BI_RGB;
        HDC dc = GetDC(nullptr);
        int lines = GetDIBits(dc, bitmap, 0, height, pixels.data(), &bi,
                              DIB_RGB_COLORS);
        ReleaseDC(nullptr, dc);
        if (lines) {
            FinishIcon(std::move(pixels), width, height, app);
        }
    }
    DeleteObject(bitmap);
}

// Uninstallers, documents and website links that some programs put in the
// Start menu.
bool ShouldSkip(const std::wstring& name, const std::wstring& parsing) {
    std::wstring lowerName = ToLower(name);
    std::wstring lowerTarget = ToLower(parsing);
    if (lowerName.empty() ||
        lowerTarget.find(L"\\unins0") != std::wstring::npos ||
        lowerTarget.find(L"\\uninst.exe") != std::wstring::npos ||
        lowerTarget.find(L"\\uninstall.exe") != std::wstring::npos ||
        lowerTarget.rfind(L"http://", 0) == 0 ||
        lowerTarget.rfind(L"https://", 0) == 0) {
        return true;
    }
    for (PCWSTR ext : {L".chm", L".txt", L".pdf", L".rtf", L".htm", L".html"}) {
        if (EndsWith(lowerTarget, ext)) {
            return true;
        }
    }
    return false;
}

bool NameLess(const std::wstring& a, const std::wstring& b) {
    return CompareStringEx(LOCALE_NAME_USER_DEFAULT,
                           LINGUISTIC_IGNORECASE | SORT_DIGITSASNUMBERS,
                           a.c_str(), (int)a.size(), b.c_str(), (int)b.size(),
                           nullptr, nullptr, 0) == CSTR_LESS_THAN;
}

void AddApp(IShellItem* item,
            std::wstring target,
            int iconPixels,
            AppList& list,
            bool desktop = false) {
    PWSTR name = nullptr;
    if (SUCCEEDED(item->GetDisplayName(SIGDN_NORMALDISPLAY, &name)) && *name) {
        auto app = std::make_shared<App>();
        app->name = name;
        // With file extensions shown, shortcuts are named "Brave.lnk".
        std::wstring lower = ToLower(app->name);
        if (lower.size() > 4 &&
            (EndsWith(lower, L".lnk") || EndsWith(lower, L".url"))) {
            app->name.resize(app->name.size() - 4);
        }
        app->target = std::move(target);
        app->desktop = desktop;
        LoadAppIcon(item, iconPixels, *app);
        list.push_back(std::move(app));
    }
    CoTaskMemFree(name);
}

void AddStartApps(int iconPixels, AppList& list) {
    ComPtr<IShellItem> folder;
    ComPtr<IEnumShellItems> items;
    if (FAILED(SHCreateItemFromParsingName(L"shell:AppsFolder", nullptr,
                                           IID_PPV_ARGS(&folder))) ||
        FAILED(folder->BindToHandler(nullptr, kBhidEnumItems,
                                     IID_PPV_ARGS(&items)))) {
        return;
    }
    ComPtr<IShellItem> item;
    while (!g_workerStop &&
           items->Next(1, item.ReleaseAndGetAddressOf(), nullptr) == S_OK) {
        PWSTR name = nullptr, parsing = nullptr;
        if (SUCCEEDED(item->GetDisplayName(SIGDN_NORMALDISPLAY, &name)) &&
            SUCCEEDED(item->GetDisplayName(SIGDN_PARENTRELATIVEPARSING,
                                           &parsing)) &&
            !ShouldSkip(name, parsing)) {
            AddApp(item.Get(), std::wstring(kAppsFolder) + parsing, iconPixels,
                   list);
        }
        CoTaskMemFree(name);
        CoTaskMemFree(parsing);
    }
}

// Everything on the desktop: the user's own and the one shared by all users.
void AddDesktopItems(int iconPixels, AppList& list) {
    for (const KNOWNFOLDERID* id :
         {&FOLDERID_Desktop, &FOLDERID_PublicDesktop}) {
        ComPtr<IShellItem> folder;
        ComPtr<IEnumShellItems> items;
        if (FAILED(SHGetKnownFolderItem(*id, KF_FLAG_DEFAULT, nullptr,
                                        IID_PPV_ARGS(&folder))) ||
            FAILED(folder->BindToHandler(nullptr, kBhidEnumItems,
                                         IID_PPV_ARGS(&items)))) {
            continue;
        }
        ComPtr<IShellItem> item;
        while (!g_workerStop &&
               items->Next(1, item.ReleaseAndGetAddressOf(), nullptr) == S_OK) {
            SFGAOF attributes = 0;
            PWSTR path = nullptr;
            if (SUCCEEDED(item->GetAttributes(SFGAO_HIDDEN, &attributes)) &&
                !(attributes & SFGAO_HIDDEN) &&
                SUCCEEDED(item->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING,
                                               &path))) {
                AddApp(item.Get(), path, iconPixels, list, true);
            }
            CoTaskMemFree(path);
        }
    }
}

// Reads the JSON string whose opening quote is json[i]; i ends past the
// closing quote.
std::wstring ReadJsonString(const std::wstring& json, size_t& i) {
    std::wstring out;
    for (i++; i < json.size() && json[i] != L'"'; i++) {
        if (json[i] != L'\\' || i + 1 >= json.size()) {
            out += json[i];
            continue;
        }
        WCHAR c = json[++i];
        switch (c) {
            case L'n':
                out += L'\n';
                break;
            case L't':
                out += L'\t';
                break;
            case L'r':
                out += L'\r';
                break;
            case L'b':
                out += L'\b';
                break;
            case L'f':
                out += L'\f';
                break;
            case L'u':
                if (i + 4 < json.size()) {
                    out += (WCHAR)wcstoul(json.substr(i + 1, 4).c_str(),
                                          nullptr, 16);
                    i += 4;
                }
                break;
            default:  // \" \\ \/
                out += c;
                break;
        }
    }
    i++;
    return out;
}

// Start's pins in their order, as Windows' own Export-StartLayout writes them:
// {"pinnedList":[{"desktopAppLink":"%APPDATA%\\...\\X.lnk"},
// {"packagedAppId":"Microsoft.WindowsCalculator_8wekyb3d8bbwe!App"}]}.
// The file Start keeps them in (start2.bin) is encrypted, so the export is the
// supported way to read them.
std::wstring ExportStartLayout() {
    WCHAR system[MAX_PATH];
    auto directory = ModTempPath();
    if (directory.empty() || !GetSystemDirectoryW(system, MAX_PATH) ||
        (FAILED(SHCreateDirectoryExW(nullptr, directory.c_str(), nullptr)) &&
         GetFileAttributesW(directory.c_str()) == INVALID_FILE_ATTRIBUTES)) {
        return {};
    }
    std::wstring path = directory + L"\\pins-" +
                        std::to_wstring(GetCurrentProcessId()) + L"-" +
                        std::to_wstring(GetCurrentThreadId()) + L".json";
    DeleteFileW(path.c_str());
    std::wstring quoted;  // Inside '...' a quote is written twice.
    for (WCHAR c : path) {
        quoted += c;
        if (c == L'\'') {
            quoted += c;
        }
    }
    std::wstring command = L"\"" + std::wstring(system) +
                           L"\\WindowsPowerShell\\v1.0\\powershell.exe\" "
                           L"-NoProfile -NonInteractive -ExecutionPolicy Bypass "
                           L"-Command \"Export-StartLayout -Path '" +
                           quoted + L"'\"";
    STARTUPINFOW si{sizeof(si)};
    PROCESS_INFORMATION pi{};
    if (!CreateProcessW(nullptr, command.data(), nullptr, nullptr, FALSE,
                        CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
        return {};
    }
    CloseHandle(pi.hThread);
    for (int waited = 0;
         WaitForSingleObject(pi.hProcess, 100) == WAIT_TIMEOUT; waited++) {
        if (g_workerStop || waited > 300) {
            TerminateProcess(pi.hProcess, 1);
            break;
        }
    }
    CloseHandle(pi.hProcess);

    std::string bytes;
    HANDLE file = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ,
                              nullptr, OPEN_EXISTING, 0, nullptr);
    if (file != INVALID_HANDLE_VALUE) {
        DWORD size = GetFileSize(file, nullptr), read = 0;
        if (size != INVALID_FILE_SIZE && size < (16 << 20)) {
            bytes.resize(size);
            if (!ReadFile(file, bytes.data(), size, &read, nullptr)) {
                read = 0;
            }
            bytes.resize(read);
        }
        CloseHandle(file);
        DeleteFileW(path.c_str());
    }
    int length = MultiByteToWideChar(CP_UTF8, 0, bytes.data(),
                                     (int)bytes.size(), nullptr, 0);
    std::wstring json(length, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, bytes.data(), (int)bytes.size(),
                        json.data(), length);
    return json;
}

// False when the pins couldn't be read at all.
bool AddStartPins(int iconPixels, AppList& list) {
    std::wstring json = ExportStartLayout();
    if (json.find(L"\"pinnedList\"") == std::wstring::npos) {
        return false;
    }
    for (size_t i = json.find(L'"'); i != std::wstring::npos && !g_workerStop;
         i = json.find(L'"', i)) {
        std::wstring key = ReadJsonString(json, i);
        size_t value = json.find_first_not_of(L" \t\r\n", i);
        if (value == std::wstring::npos || json[value] != L':') {
            continue;
        }
        value = json.find_first_not_of(L" \t\r\n", value + 1);
        if (value != std::wstring::npos && json[value] == L'{' &&
            key == L"secondaryTile") {
            // A pinned website. Its packagedAppId is the browser, which would
            // show up as a plain browser tile, so the whole object is skipped.
            int depth = 0;
            for (i = value; i < json.size();) {
                if (json[i] == L'"') {
                    ReadJsonString(json, i);
                    continue;
                }
                if (json[i] == L'{') {
                    depth++;
                } else if (json[i] == L'}' && --depth == 0) {
                    i++;
                    break;
                }
                i++;
            }
            continue;
        }
        if (value == std::wstring::npos || json[value] != L'"') {
            continue;
        }
        i = value;
        std::wstring pin = ReadJsonString(json, i);
        std::wstring target;
        if (key == L"desktopAppLink") {
            WCHAR expanded[MAX_PATH * 2];
            DWORD n = ExpandEnvironmentStringsW(pin.c_str(), expanded,
                                                ARRAYSIZE(expanded));
            target = n && n <= ARRAYSIZE(expanded) ? expanded : pin;
        } else if (key == L"desktopAppId" || key == L"packagedAppId") {
            target = std::wstring(kAppsFolder) + pin;
        } else {
            continue;
        }
        ComPtr<IShellItem> item;
        if (SUCCEEDED(SHCreateItemFromParsingName(target.c_str(), nullptr,
                                                  IID_PPV_ARGS(&item)))) {
            AddApp(item.Get(), target, iconPixels, list);
        }
    }
    return true;
}

std::wstring StoragePath(const std::wstring& file);
bool WriteLines(const std::wstring& path,
                const std::vector<std::wstring>& lines,bool backup=true);

// The first time "only the apps you pin" is used, the pins start as Start's
// own, matched by name to the apps already read. Written even when Start's
// pins can't be read, so the slow export runs once.
void SeedPins(int iconPixels, AppList& list) {
    std::wstring path = StoragePath(L"pins.txt");
    if (path.empty() ||
        GetFileAttributesW(path.c_str()) != INVALID_FILE_ATTRIBUTES) {
        return;
    }
    AppList pins;
    AddStartPins(iconPixels, pins);
    std::vector<std::wstring> lines;
    for (auto& pin : pins) {
        auto same = std::find_if(list.begin(), list.end(), [&](const auto& a) {
            return ToLower(a->name) == ToLower(pin->name);
        });
        lines.push_back(L"pin\t" +
                        (same != list.end() ? (*same)->target : pin->target));
        if (same == list.end()) {
            list.push_back(std::move(pin));
        }
    }
    if (!g_workerStop) {
        WriteLines(path, lines);
    }
}

AppList* EnumerateApps(int iconPixels, AppSource source) {
    auto list = std::make_unique<AppList>();
    // Pinned: every app is read, so the search finds the ones not pinned.
    if (source != AppSource::Desktop) {
        AddStartApps(iconPixels, *list);
    }
    if (source != AppSource::AllApps) {
        AddDesktopItems(iconPixels, *list);
    }
    if (source == AppSource::Pinned) {
        SeedPins(iconPixels, *list);
    }

    std::stable_sort(list->begin(), list->end(),
                     [](const auto& a, const auto& b) {
                         return NameLess(a->name, b->name);
                     });
    // The same app may show up twice (user and all-users shortcut). A desktop
    // shortcut named like a Start app is dropped too: the stable sort keeps
    // the Start app, added first.
    list->erase(std::unique(list->begin(), list->end(),
                            [](const auto& a, const auto& b) {
                                return a->name == b->name &&
                                       (a->desktop != b->desktop ||
                                        ToLower(a->target) ==
                                            ToLower(b->target));
                            }),
                list->end());
    return list.release();
}

// What a loaded list depends on: the icon size and the source.
// Bumped when the icon pipeline changes, so a stale list with old-style
// framed icons can't be reused within a session.
constexpr int kIconPipelineVersion = 3;

int AppsKey(int iconPixels, AppSource source) {
    return iconPixels | ((int)source << 16) | (kIconPipelineVersion << 24);
}

DWORD WINAPI LoadAppsThread(void* param) {
    int key = (int)(INT_PTR)param;
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    AppList* list =
        EnumerateApps(key & 0xFFFF, (AppSource)((key >> 16) & 0xFF));
    CoUninitialize();
    HWND wnd = g_wnd;
    if (g_workerStop || !wnd ||
        !PostMessageW(wnd, WM_APP_APPS, key, (LPARAM)list)) {
        delete list;
    }
    return 0;
}

// Icons are read at the size of the monitor with the largest scaling.
int IconPixels() {
    UINT maxDpi = 96;
    // MONITORENUMPROC and WNDENUMPROC are __stdcall: the attribute keeps
    // the lambdas convertible on 32-bit builds, where the default calling
    // convention differs. It is a no-op on x64 and arm64.
    EnumDisplayMonitors(
        nullptr, nullptr,
        [](HMONITOR monitor, HDC, LPRECT, LPARAM param) __attribute__((stdcall)) -> BOOL {
            UINT dpiX = 96, dpiY = 96;
            if (SUCCEEDED(GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpiX,
                                           &dpiY))) {
                *(UINT*)param = std::max(*(UINT*)param, dpiX);
            }
            return TRUE;
        },
        (LPARAM)&maxDpi);
    return std::min(256, (int)std::ceil(g_settings.iconSize * maxDpi / 96.0));
}

int AppsKey() {
    return AppsKey(IconPixels(), g_settings.source);
}

void StartLoadingApps() {
    if (g_fullImportBusy) return;
    if (g_worker) {
        if (WaitForSingleObject(g_worker, 0) == WAIT_TIMEOUT) {
            return;  // Already reading.
        }
        CloseHandle(g_worker);
        g_worker = nullptr;
    }
    g_appsTime = Now();
    g_worker = CreateThread(nullptr, 0, LoadAppsThread,
                            (void*)(INT_PTR)AppsKey(), 0, nullptr);
}

////////////////////////////////////////////////////////////////////////////////
// Arrangement
//
// The order of the grid, its folders and the apps removed from it, saved as
// text in the mod's storage folder: one file per "Apps to show" choice, and
// one for the replaced icons. Apps are known by their target, and apps that
// aren't in the file yet go at the end.

struct Folder;

// One icon of the grid: an app or a folder.
struct Tile {
    std::shared_ptr<App> app;
    std::shared_ptr<Folder> folder;
    bool operator==(const Tile&) const = default;
};
using Tiles = std::vector<Tile>;

struct Folder {
    std::wstring name;
    Tiles apps;
    bool inbox = false;
    bool collapsed = false; // Explicit traditional-folder presentation in a free layout.
    std::vector<std::pair<size_t,std::wstring>> unavailable;
};

[[clang::no_destroy]] Tiles g_tiles;
bool g_layoutReadOnly = false;
bool g_dataReadOnly = false;
// Set when a predecessor's storage failed validation and RYJS started empty.
bool g_storageMigrationSkipped = false;
extern bool g_entriesReadOnly;
bool SaveGroupPositions(bool finishing=false);
void BeforeArrangeGeometry();
void AfterArrangeGeometry();

std::shared_ptr<Folder> InboxGroup() {
    for (const auto& tile : g_tiles) if (tile.folder && tile.folder->inbox) return tile.folder;
    auto group = std::make_shared<Folder>();
    group->name = Tr(L"Ungrouped"); group->inbox = true;
    g_tiles.push_back({nullptr, group});
    return group;
}
void NormalizeGroups() {
    if (!g_settings.groups) return;
    Tiles ungrouped;
    for (const auto& tile : g_tiles) if (tile.app) ungrouped.push_back(tile);
    std::erase_if(g_tiles, [](const Tile& tile) { return tile.app != nullptr; });
    // Only when there is something to hold: an empty "Ungrouped" container is
    // a group the user never made, and it used to come back after deletion.
    if (!ungrouped.empty()) {
        auto group = InboxGroup();
        group->apps.insert(group->apps.end(), ungrouped.begin(), ungrouped.end());
    }
}
[[clang::no_destroy]] std::set<std::wstring> g_hidden;  // Removed apps' keys.
[[clang::no_destroy]] std::map<std::wstring, std::wstring> g_customIcons;  // Key: picture.
// Keys of the pinned apps, kept in pins.txt. Only with "only the apps you pin".
[[clang::no_destroy]] std::set<std::wstring> g_pins;

bool PinnedOnly() {
    return ((g_appsKey >> 16) & 0xFF) == (int)AppSource::Pinned;
}

// Not on the grid: removed, or not pinned when only pinned apps show.
bool OffGrid(const std::wstring& key) {
    return g_hidden.count(key) || (PinnedOnly() && !g_pins.count(key));
}

std::wstring AppKey(const App& app) {
    return ToLower(app.stableKey.empty() ? app.target : app.stableKey);
}
const std::wstring& LayoutAppKey(const App& app) {
    return app.layoutTarget.empty()?app.target:app.layoutTarget;
}

// The Windhawk namespace is independent; versioned data is committed only
// after a complete, validated copy of the selected previous mod.
std::wstring DataRoot() {
    WCHAR path[32768];
    if (!Wh_GetModStoragePath(path, ARRAYSIZE(path))) return {};
#ifdef SEAMLESS_PREVIEW
    return path;
#else
    return std::wstring(path) + L"\\data-v1";
#endif
}

// Scratch files live next to data-v1, never inside it: a backup worker whose
// working directory sat inside the data folder would lock it against import,
// and  clearing the mod's storage should take them with it.
std::wstring ModTempPath() {
    auto root = DataRoot();
    if (root.empty()) return {};
    return (std::filesystem::path(root).parent_path() / L"tmp").wstring();
}

// Explains, next to the new data, why a predecessor's folder was not used.
// Written in both languages so the note is readable either way.
void WriteMigrationNote(const std::wstring& target,const std::wstring& source) {
    auto note=target+L"\\migration-note.txt";
    std::wstring text=
        L"Lumiflow started with an empty layout.\r\n"
        L"The previous launcher's folder could not be read, so nothing was copied from it:\r\n"+source+L"\r\n"
        L"That folder was not modified or deleted. To get the old layout back, restore a complete backup from the settings window, or repair and move the old folder back into place.\r\n\r\n"
        L"流光启动台以空布局启动。上一个启动器的数据目录无法通过校验，因此没有复制任何内容：\r\n"+source+L"\r\n"
        L"该目录未被修改或删除。如需恢复旧布局，可在设置窗口中导入完整备份，或在修复该目录后将其放回原位。\r\n";
    int size=WideCharToMultiByte(CP_UTF8,0,text.data(),(int)text.size(),nullptr,0,nullptr,nullptr);
    if(size<=0)return;
    std::string bytes((size_t)size,'\0');
    WideCharToMultiByte(CP_UTF8,0,text.data(),(int)text.size(),bytes.data(),size,nullptr,nullptr);
    HANDLE file=CreateFileW(note.c_str(),GENERIC_WRITE,0,nullptr,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,nullptr);
    if(file==INVALID_HANDLE_VALUE)return;
    DWORD written=0;
    WriteFile(file,bytes.data(),(DWORD)bytes.size(),&written,nullptr);
    CloseHandle(file);
}

#ifndef SEAMLESS_PREVIEW
bool PrepareStorage() {
    auto target = DataRoot();
    if (target.empty()) return false;
    if (std::filesystem::exists(target))
        return ryjs_data::Prepare(L"", target, [](const auto&) { return true; });
    // First run: inherit the layout, groups, pins and icons of a predecessor.
    // Those files are the data a user would miss. Its settings are not copied:
    // reading them means reading Windhawk's private per-mod registry layout,
    // which is not API, does not exist on portable Windhawk, and is a layout
    // the user can review and retype in the settings window anyway.
    auto parent = std::filesystem::path(target).parent_path().parent_path();
    std::wstring source;
    for (PCWSTR id : {L"local@seamless-launchpad-custom", L"seamless-launchpad-custom",
                      L"local@seamless-launchpad", L"seamless-launchpad",
                      L"local@fullscreen-start-plus", L"fullscreen-start-plus"}) {
        auto candidate = parent / id;
        if (!std::filesystem::is_directory(candidate)) continue;
        for (const auto& file : std::filesystem::directory_iterator(candidate)) {
            auto name = file.path().filename().wstring();
            if (name.starts_with(L"layout-") || name.starts_with(L"groups-")) {
                source = candidate.wstring(); break;
            }
        }
        if (!source.empty()) break;
    }
    bool copied = ryjs_data::Prepare(source, target, [](const std::wstring& stage) {
        // Keep the predecessor's settings file with the copied data instead of
        // applying it: nothing is lost, and no settings are inherited silently.
        auto original = stage + L"\\ui-settings.ini";
        if (GetFileAttributesW(original.c_str()) == INVALID_FILE_ATTRIBUTES) return true;
        return MoveFileW(original.c_str(),
                         (stage + L"\\migration-original-ui-settings.ini").c_str()) != FALSE;
    });
    if (copied) { Wh_Log(L"Lumiflow data ready, original storage untouched: %s", source.c_str()); return true; }
    // The predecessor's folder failed validation (an unknown groups-* file, a
    // layout line this version does not know, an unreadable file). Never touch
    // it: start with a fresh, empty data-v1 so the mod stays usable, and leave
    // a note that names the folder that was skipped. Staying read-only forever
    // until the user moves that folder away is worse than an empty first run.
    if (source.empty()) {
        Wh_Log(L"Lumiflow data preparation failed; no predecessor, nothing to fall back from");
        return false;
    }
    if (!ryjs_data::Prepare(L"", target, [](const auto&) { return true; })) {
        Wh_Log(L"Lumiflow data preparation failed and a fresh folder could not be created; originals kept: %s", source.c_str());
        return false;
    }
    g_storageMigrationSkipped = true;
    WriteMigrationNote(target, source);
    Wh_Log(L"Lumiflow data was unreadable; started empty, originals kept: %s", source.c_str());
    return true;
}
#endif

std::wstring StoragePath(const std::wstring& file) {
    auto directory = DataRoot();
    if (directory.empty()) return {};
    SHCreateDirectoryExW(nullptr, directory.c_str(), nullptr);
    return directory + L"\\" + file;
}

std::wstring LegacyLayoutFile() {
    static constexpr PCWSTR names[] = {L"allApps", L"desktop", L"both",
                                       L"pinned"};
    return std::wstring(L"layout-") + names[std::clamp((g_appsKey >> 16) & 0xFF, 0, 3)] +
           L".txt";
}

std::wstring LayoutFile() {
    auto file = LegacyLayoutFile();
    if (g_settings.groups) file.replace(0, 7, L"groups-v1-");
    return file;
}

// Tab-separated fields of each line of a UTF-8 file.
std::vector<std::vector<std::wstring>> ReadLines(const std::wstring& path, bool* success = nullptr) {
    std::vector<std::vector<std::wstring>> lines;
    std::wstring text;
    bool ok = ryjs_data::ReadUtf8(path, text);
    if (success) *success = ok;
    if (!ok) return lines;
    std::vector<std::wstring> fields(1);
    for (WCHAR c : text + L"\n") {
        if (c == L'\n') {
            if (fields.size() > 1 || !fields[0].empty()) {
                lines.push_back(std::move(fields));
            }
            fields.assign(1, {});
        } else if (c == L'\t') {
            fields.emplace_back();
        } else if (c != L'\r') {
            fields.back() += c;
        }
    }
    return lines;
}

// Written whole to a temporary file first, so a crash can't leave half a file.
bool WriteLines(const std::wstring& path,
                const std::vector<std::wstring>& lines,bool backup) {
    if (path.empty() || g_dataReadOnly) {
        return false;
    }
    std::wstring text;
    for (const auto& line : lines) {
        text += line + L"\r\n";
    }
    int length = WideCharToMultiByte(CP_UTF8, 0, text.data(), (int)text.size(),
                                     nullptr, 0, nullptr, nullptr);
    std::string bytes(length, '\0');
    WideCharToMultiByte(CP_UTF8, 0, text.data(), (int)text.size(),
                        bytes.data(), length, nullptr, nullptr);
    std::wstring temp = path + L".tmp";
    HANDLE file = CreateFileW(temp.c_str(), GENERIC_WRITE, 0, nullptr,
                              CREATE_ALWAYS, 0, nullptr);
    if (file == INVALID_HANDLE_VALUE) {
        Wh_Log(L"Can't write %s: %u", temp.c_str(), GetLastError());
        return false;
    }
    DWORD written = 0;
    bool ok = WriteFile(file, bytes.data(), (DWORD)bytes.size(), &written,
                        nullptr) &&
              written == bytes.size();
    CloseHandle(file);
    if (ok && backup && GetFileAttributesW(path.c_str()) != INVALID_FILE_ATTRIBUTES && !CopyFileW(path.c_str(), (path + L".bak").c_str(), FALSE)) {
        Wh_Log(L"Cannot back up %s: %u; keeping previous data", path.c_str(), GetLastError());
        DeleteFileW(temp.c_str()); return false;
    }
    if (!ok || !MoveFileExW(temp.c_str(), path.c_str(),
                            MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        Wh_Log(L"Can't save %s: %u", path.c_str(), GetLastError());
        DeleteFileW(temp.c_str());
        return false;
    }
    return true;
}

// The arrangement as the lines of its file:
// hidden<TAB>target, app<TAB>target, folder<TAB>name, in<TAB>target (an app
// of the folder above).
std::vector<std::wstring> ArrangementLines() {
    std::vector<std::wstring> lines;
    if (g_settings.groups) lines.push_back(L"schema\t1\tgroups");
    for (const auto& key : g_hidden) {
        lines.push_back(L"hidden\t" + key);
    }
    for (const auto& tile : g_tiles) {
        if (tile.app) {
            lines.push_back(L"app\t" + LayoutAppKey(*tile.app));
            continue;
        }
        lines.push_back(L"folder\t" + (tile.folder->name.empty()
                                            ? std::wstring(Tr(L"Folder"))
                                            : tile.folder->name) + (tile.folder->inbox ? L"\tinbox" : L""));
        size_t appIndex=0, missingIndex=0, position=0;
        const auto& apps=tile.folder->apps;
        const auto& missing=tile.folder->unavailable;
        while (appIndex<apps.size() || missingIndex<missing.size()) {
            if (missingIndex<missing.size() && (appIndex>=apps.size() || missing[missingIndex].first<=position))
                lines.push_back(L"in\t"+missing[missingIndex++].second);
            else lines.push_back(L"in\t"+LayoutAppKey(*apps[appIndex++].app));
            ++position;
        }
    }
    return lines;
}

// Empty folders go, and a folder with one app becomes that app.
void TidyTiles() {
    if (g_settings.groups) return; // Empty and single-app groups are intentional.
    Tiles tidy;
    for (auto& tile : g_tiles) {
        if (tile.folder && tile.folder->apps.size() < 2) {
            if (!tile.folder->apps.empty()) {
                tidy.push_back(tile.folder->apps[0]);
            }
        } else {
            tidy.push_back(std::move(tile));
        }
    }
    g_tiles.swap(tidy);
}

void ArrangeTiles(const std::vector<std::wstring>& lines,bool appendNew=false) {
    BeforeArrangeGeometry();
    std::vector<std::vector<std::wstring>> parsed;
    for (const auto& line : lines) {
        std::vector<std::wstring> fields(1);
        for (WCHAR c : line) {
            if (c == L'\t') {
                fields.emplace_back();
            } else {
                fields.back() += c;
            }
        }
        parsed.push_back(std::move(fields));
    }
    g_hidden.clear();
    for (const auto& f : parsed) {
        if (f.size() >= 2 && f[0] == L"hidden") {
            g_hidden.insert(ToLower(f[1]));
        }
    }
    std::map<std::wstring, std::shared_ptr<App>> byKey;
    for (const auto& app : g_apps) {
        byKey.emplace(AppKey(*app), app);
    }
    std::set<std::wstring> placed;
    auto take = [&](const std::wstring& target) -> std::shared_ptr<App> {
        std::wstring key = ToLower(target);
        auto it = byKey.find(key);
        if (it == byKey.end() || OffGrid(key) || !placed.insert(key).second) {
            return nullptr;
        }
        it->second->layoutTarget=target;
        return it->second;
    };
    g_tiles.clear();
    std::shared_ptr<Folder> folder;
    size_t folderPosition=0;
    for (const auto& f : parsed) {
        if (f.size() < 2) {
            continue;
        }
        if (f[0] == L"folder") {
            folder = std::make_shared<Folder>();
            folderPosition=0;
            folder->name = f[1];
            folder->inbox = f.size() >= 3 && f[2] == L"inbox";
            g_tiles.push_back({nullptr, folder});
        } else if (f[0] == L"in" && folder) {
            if (auto app = take(f[1])) {
                folder->apps.push_back({std::move(app), nullptr});
            } else if (g_settings.groups && !byKey.count(ToLower(f[1])) && !OffGrid(ToLower(f[1])) && placed.insert(ToLower(f[1])).second) {
                folder->unavailable.emplace_back(folderPosition,f[1]);
            }
            ++folderPosition;
        } else if (f[0] == L"app") {
            folder = nullptr;
            if (auto app = take(f[1])) {
                g_tiles.push_back({std::move(app), nullptr});
            }
        }
    }
    for (const auto& app : g_apps) {
        if(app->manual)continue; // Only explicit addition/pinning chooses a manual entry's place.
        if(!appendNew&&!lines.empty())continue; // Discovery never inserts into an existing arrangement.
        if (auto taken = take(AppKey(*app))) {
            g_tiles.push_back({std::move(taken), nullptr});
        }
    }
    TidyTiles();
    NormalizeGroups();
    AfterArrangeGeometry();
}

bool SaveTiles(bool explicitImport = false) {
    if (g_fullImportBusy) return false;
    if (g_entriesReadOnly) return false;
    if (g_layoutReadOnly || g_dataReadOnly) { Wh_Log(L"Layout is read-only; keeping original file unchanged"); return false; }
    auto path = StoragePath(LayoutFile());
    if (!explicitImport && GetFileAttributesW(path.c_str()) != INVALID_FILE_ATTRIBUTES) {
        std::wstring existing;
        if (!ryjs_data::ReadUtf8(path, existing) || !ryjs_data::Validate(LayoutFile(), existing)) {
            g_layoutReadOnly = g_dataReadOnly = true;
            Wh_Log(L"Layout changed to an unreadable/unsupported file; refusing to overwrite it.");
            return false;
        }
    }
    return WriteLines(path, ArrangementLines()) && SaveGroupPositions();
}

#include "launchpad_svg.inc"

// A picture (or the icon of a file) in place of an app's icon.
bool LoadIconFile(const std::wstring& path, int size, App& app, bool allowShellIcon = true) {
    ComPtr<IWICImagingFactory> factory;
    ComPtr<IWICBitmapDecoder> decoder;
    ComPtr<IWICBitmapFrameDecode> frame;
    ComPtr<IWICBitmapScaler> scaler;
    ComPtr<IWICFormatConverter> converter;
    if (SUCCEEDED(CoCreateInstance(CLSID_WICImagingFactory,nullptr,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&factory))) &&
        SUCCEEDED(factory->CreateDecoderFromFilename(path.c_str(),nullptr,GENERIC_READ,WICDecodeMetadataCacheOnLoad,&decoder)) &&
        SUCCEEDED(decoder->GetFrame(0,&frame))) {
        UINT frames=0;decoder->GetFrameCount(&frames);unsigned best=UINT_MAX;
        for(UINT i=0;i<std::min(frames,64u);i++) {
            ComPtr<IWICBitmapFrameDecode> candidate;UINT cw=0,ch=0;
            if(FAILED(decoder->GetFrame(i,&candidate))||FAILED(candidate->GetSize(&cw,&ch)))continue;
            unsigned edge=std::min(cw,ch),score=(unsigned)std::abs((int)edge-size)+(edge<(UINT)size?10000:0);
            if(score<best){best=score;frame=candidate;}
        }
        UINT w=0,h=0;frame->GetSize(&w,&h);
        UINT side=(UINT)std::clamp(size,16,512);
        UINT dw=w>=h?side:std::max(1u,(UINT)((unsigned long long)side*w/std::max(1u,h)));
        UINT dh=h>=w?side:std::max(1u,(UINT)((unsigned long long)side*h/std::max(1u,w)));
        if (w && h && w<=16384 && h<=16384 && SUCCEEDED(factory->CreateBitmapScaler(&scaler)) &&
            SUCCEEDED(scaler->Initialize(frame.Get(),dw,dh,WICBitmapInterpolationModeFant)) &&
            SUCCEEDED(factory->CreateFormatConverter(&converter)) &&
            SUCCEEDED(converter->Initialize(scaler.Get(),GUID_WICPixelFormat32bppPBGRA,WICBitmapDitherTypeNone,nullptr,0,WICBitmapPaletteTypeCustom))) {
            std::vector<UINT32> pixels((size_t)dw*dh);
            if (SUCCEEDED(converter->CopyPixels(nullptr,dw*4,(UINT)pixels.size()*4,(BYTE*)pixels.data()))) {
                if (!std::any_of(pixels.begin(), pixels.end(), [](UINT32 pixel) { return pixel >> 24; })) return false;
                app.pixels.assign((size_t)side*side,0);app.iconSize=side;
                for(UINT y=0;y<dh;y++)std::copy_n(pixels.data()+y*dw,dw,app.pixels.data()+(y+(side-dh)/2)*side+(side-dw)/2);
                app.bitmap.Reset();app.generation=0;return true;
            }
        }
    }
    if(LoadSvgIcon(path,size,app))return true;
    if (!allowShellIcon) return false;
    ComPtr<IShellItem> item;
    if (FAILED(SHCreateItemFromParsingName(path.c_str(), nullptr,
                                           IID_PPV_ARGS(&item)))) {
        return false;
    }
    App loaded;
    LoadAppIcon(item.Get(), size, loaded, SIIGBF_BIGGERSIZEOK);
    if (loaded.pixels.empty()) {
        return false;
    }
    app.pixels = std::move(loaded.pixels);
    app.iconSize = loaded.iconSize;
    app.bitmap.Reset();
    app.generation = 0;  // Remade on the next draw.
    return true;
}

// The app's own icon again.
void ReloadAppIcon(int size, App& app) {
    ComPtr<IShellItem> item;
    if (SUCCEEDED(SHCreateItemFromParsingName(app.target.c_str(), nullptr,
                                              IID_PPV_ARGS(&item)))) {
        App loaded;
        LoadAppIcon(item.Get(), size, loaded);
        app.pixels = std::move(loaded.pixels);
        app.iconSize = loaded.iconSize;
        app.bitmap.Reset();
        app.generation = 0;
    }
}

bool SaveCustomIcons() {
    std::vector<std::wstring> lines;
    for (const auto& [key, path] : g_customIcons) {
        lines.push_back(L"icon\t" + key + L"\t" + path);
    }
    return WriteLines(StoragePath(L"icons.txt"), lines);
}

bool IsManagedIcon(const std::wstring& path) {
    static const std::wregex pattern(LR"(custom-icons\\\{[0-9a-f-]{36}\}\.png)", std::regex::icase);
    return std::regex_match(path, pattern);
}

std::wstring CustomIconPath(const std::wstring& path) {
    return IsManagedIcon(path) ? StoragePath(path) : path;
}

bool WriteIconPng(const std::wstring& path, const App& app) {
    const UINT size = app.iconSize;
    // The encoder is told size*size pixels; make sure that is what the buffer
    // holds instead of letting WIC read past it.
    if (!size || app.pixels.size() != (size_t)size * size) return false;
    ComPtr<IWICImagingFactory> factory;
    ComPtr<IWICBitmap> pixels;
    ComPtr<IWICFormatConverter> converter;
    ComPtr<IWICStream> stream;
    ComPtr<IWICBitmapEncoder> encoder;
    ComPtr<IWICBitmapFrameEncode> frame;
    WICPixelFormatGUID format = GUID_WICPixelFormat32bppBGRA;
    return SUCCEEDED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory))) &&
           SUCCEEDED(factory->CreateBitmapFromMemory(size, size, GUID_WICPixelFormat32bppPBGRA,
               size * 4, size * size * 4, (BYTE*)app.pixels.data(), &pixels)) &&
           SUCCEEDED(factory->CreateFormatConverter(&converter)) &&
           SUCCEEDED(converter->Initialize(pixels.Get(), format, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeCustom)) &&
           SUCCEEDED(factory->CreateStream(&stream)) &&
           SUCCEEDED(stream->InitializeFromFilename(path.c_str(), GENERIC_WRITE)) &&
           SUCCEEDED(factory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder)) &&
           SUCCEEDED(encoder->Initialize(stream.Get(), WICBitmapEncoderNoCache)) &&
           SUCCEEDED(encoder->CreateNewFrame(&frame, nullptr)) &&
           SUCCEEDED(frame->Initialize(nullptr)) &&
           SUCCEEDED(frame->SetSize(size, size)) &&
           SUCCEEDED(frame->SetPixelFormat(&format)) &&
           SUCCEEDED(frame->WriteSource(converter.Get(), nullptr)) &&
           SUCCEEDED(frame->Commit()) && SUCCEEDED(encoder->Commit());
}

bool StoreCustomIcon(const std::wstring& source, std::wstring& stored) {
    if (IsManagedIcon(source)) {
        stored = source;
        App image;
        return LoadIconFile(CustomIconPath(stored), 256, image, false);
    }
    if (GetFileAttributesW(source.c_str()) == INVALID_FILE_ATTRIBUTES) return false;
    App image;
    if (!LoadIconFile(source, 256, image)) return false;
    GUID guid{};
    WCHAR id[40]{};
    if (FAILED(CoCreateGuid(&guid)) || !StringFromGUID2(guid, id, ARRAYSIZE(id))) return false;
    stored = L"custom-icons\\" + std::wstring(id) + L".png";
    auto destination = StoragePath(stored);
    if (destination.empty()) return false;
    SHCreateDirectoryExW(nullptr, std::filesystem::path(destination).parent_path().c_str(), nullptr);
    auto temporary = destination + L".tmp";
    if (WriteIconPng(temporary, image) &&
        MoveFileExW(temporary.c_str(), destination.c_str(), MOVEFILE_WRITE_THROUGH)) return true;
    DeleteFileW(temporary.c_str());
    return false;
}

bool SetCustomIcon(const std::wstring& key, const std::wstring& source) {
    if (g_dataReadOnly) return false;
    std::wstring stored;
    if (!source.empty() && !StoreCustomIcon(source, stored)) return false;
    auto previous = g_customIcons.extract(key);
    if (!stored.empty()) g_customIcons[key] = stored;
    if (SaveCustomIcons()) return true;
    g_customIcons.erase(key);
    g_customIcons.insert(std::move(previous));
    if (!stored.empty() && stored != source) DeleteFileW(CustomIconPath(stored).c_str());
    return false;
}

void ApplyCustomIcons(int size) {
    bool read = false;
    auto path = StoragePath(L"icons.txt");
    auto records = ReadLines(path, &read);
    if (!read && GetFileAttributesW(path.c_str()) != INVALID_FILE_ATTRIBUTES) return;
    g_customIcons.clear();
    for (const auto& f : records) {
        if (f.size() >= 3 && f[0] == L"icon") {
            g_customIcons[ToLower(f[1])] = f[2];
        }
    }
    if (read && !g_dataReadOnly && !g_layoutReadOnly) {
        auto previous = g_customIcons;
        bool changed = false;
        for (auto& [key, source] : g_customIcons) {
            std::wstring stored;
            if (!IsManagedIcon(source) && StoreCustomIcon(source, stored)) {
                source = std::move(stored);
                changed = true;
            }
        }
        if (changed && !SaveCustomIcons()) g_customIcons = std::move(previous);
    }
    for (const auto& app : g_apps) {
        auto it = g_customIcons.find(AppKey(*app));
        if (it != g_customIcons.end()) {
            LoadIconFile(CustomIconPath(it->second), size, *app, !IsManagedIcon(it->second));
        }
    }
}

#include "launchpad_entries.inc"
#include "launchpad_display_names.inc"
#include "launchpad_group_positions.inc"

void SavePins() {
    std::vector<std::wstring> lines;
    for (const auto& key : g_pins) {
        lines.push_back(L"pin\t" + key);
    }
    WriteLines(StoragePath(L"pins.txt"), lines);
}

void LoadTiles() {
    ClearGroupPositions();
    // Do not replace the in-memory layout or write any data if any active
    // layout/pins/icons file becomes unreadable or unsupported after startup.
    if (!ryjs_data::ValidateDirectory(DataRoot())) {
        g_layoutReadOnly = g_dataReadOnly = true;
        Wh_Log(L"Invalid/unreadable layout data. Saving blocked; restore an external backup.");
        return;
    }
    g_pins.clear();
    for (const auto& f : ReadLines(StoragePath(L"pins.txt"))) {
        if (f.size() >= 2 && f[0] == L"pin") {
            g_pins.insert(ToLower(f[1]));
        }
    }
    auto path = StoragePath(LayoutFile());
    g_layoutReadOnly = false;
    auto records = ReadLines(path);
    bool importing = g_settings.groups && GetFileAttributesW(path.c_str()) == INVALID_FILE_ATTRIBUTES;
    if (importing) {
        auto legacy = StoragePath(LegacyLayoutFile());
        records = ReadLines(legacy);
        CopyFileW(legacy.c_str(), (path + L".imported-layout.bak").c_str(), TRUE);
    }
    if (!records.empty() && records[0].size() >= 2 && records[0][0] == L"schema" && _wtoi(records[0][1].c_str()) > 1) {
        g_layoutReadOnly = true;
        records = ReadLines(path + L".bak");
        if (!records.empty() && records[0].size() >= 2 && records[0][0] == L"schema" && _wtoi(records[0][1].c_str()) > 1) records.clear();
        Wh_Log(L"Layout schema is newer than supported. Fallback is read-only.");
    }
    std::vector<std::wstring> lines;
    for (const auto& f : records) {
        std::wstring line;
        for (size_t i = 0; i < f.size(); i++) {
            line += (i ? L"\t" : L"") + f[i];
        }
        lines.push_back(std::move(line));
    }
    ArrangeTiles(lines);
    if(g_settings.groups&&LoadGroupPositions()&&records.empty()&&!g_layoutReadOnly) {
        // Fresh layouts start on the whole-cell grid. Existing arrangements
        // without a geometry file retain their legacy layout until editing.
        g_wholeCellLayout=true;
        g_freeGroupLayout=true;
    }
    if (importing) SaveTiles();
}

////////////////////////////////////////////////////////////////////////////////
// Search

std::wstring g_query;
struct ImeComposition {
    bool active = false;
    std::wstring text;
    size_t cursor = 0;
};
[[clang::no_destroy]] ImeComposition g_imeComposition;
bool g_searchActivated = false;
// 0 = the resting pill, 1 = the open field. Eased toward SearchExpanded() a
// frame at a time, so tapping the box grows it instead of snapping open.
float g_searchExpand = 0;
bool g_showingSearchResults = false;
float g_searchGridTransition = 1.0f;
bool SearchExpanded() { return g_searchActivated || !g_query.empty(); }
// The search field is a small text editor: a caret and a selection anchor as
// UTF-16 indices into g_query (the selection is the span between them), a
// tick that times the caret's blink, and the state of a drag-selection.
size_t g_caret = 0;
size_t g_anchor = 0;
UINT64 g_caretTick = 0;
bool g_searchDrag = false;
UINT64 g_lastSearchClick = 0;
int g_lastSearchX = -1000;
// The arrangement, or the apps the search finds.
[[clang::no_destroy]] Tiles g_visible;

// 0: the name starts with the text; 1: a word of the name starts with it;
// 2: it's in the middle; -1: it isn't there. Ignoring case and accents.
int MatchRank(const std::wstring& name, const std::wstring& word) {
    int pos = FindNLSStringEx(
        LOCALE_NAME_USER_DEFAULT,
        FIND_FROMSTART | LINGUISTIC_IGNORECASE | LINGUISTIC_IGNOREDIACRITIC,
        name.c_str(), (int)name.size(), word.c_str(), (int)word.size(),
        nullptr, nullptr, nullptr, 0);
    if (pos < 0) {
        return -1;
    }
    if (pos == 0) {
        return 0;
    }
    return iswalnum(name[pos - 1]) ? 2 : 1;
}

bool GroupVisible(const std::shared_ptr<Folder>& group) {
    return group && (!group->inbox || !group->apps.empty() || g_groupEditMode);
}

void UpdateFilter() {
    if (g_query.empty()) {
        g_visible.clear();
        for (const auto& tile : g_tiles) {
            if (g_settings.groups && tile.folder && !GroupVisible(tile.folder)) continue;
            if (g_settings.groups && tile.folder && !tile.folder->collapsed) g_visible.insert(g_visible.end(), tile.folder->apps.begin(), tile.folder->apps.end());
            else g_visible.push_back(tile);
        }
        return;
    }
    std::vector<std::wstring> words;
    size_t start = 0;
    while (start < g_query.size()) {
        size_t end = g_query.find(L' ', start);
        if (end == std::wstring::npos) {
            end = g_query.size();
        }
        if (end > start) {
            words.push_back(g_query.substr(start, end - start));
        }
        start = end + 1;
    }

    std::vector<std::pair<int, int>> ranked;  // (position, index)
    for (int i = 0; i < (int)g_apps.size(); i++) {
        if (g_hidden.count(AppKey(*g_apps[i]))) {
            continue;
        }
        int rank = 0;
        for (size_t w = 0; w < words.size() && rank >= 0; w++) {
            int r = MatchRank(g_apps[i]->name, words[w]);
            if (r < 0 && !g_apps[i]->originalName.empty()) r = MatchRank(g_apps[i]->originalName, words[w]);
            rank = r < 0 ? -1 : (w == 0 ? r : rank);
        }
        if (rank >= 0) {
            // With only pinned apps, the pinned ones come first.
            bool off = PinnedOnly() && !g_pins.count(AppKey(*g_apps[i]));
            ranked.emplace_back(rank + (off ? 3 : 0), i);
        }
    }
    std::stable_sort(ranked.begin(), ranked.end(),
                     [](const auto& a, const auto& b) {
                         return a.first < b.first;
                     });
    g_visible.clear();
    for (const auto& r : ranked) {
        g_visible.push_back({g_apps[r.second], nullptr});
    }
}

////////////////////////////////////////////////////////////////////////////////
// Available area
//
// The monitor's work area already excludes the visible taskbar and the side
// bars that reserve space. The Standalone Dock usually doesn't reserve it (it
// hides by itself), so its strip is removed separately.

RECT AvailableArea(HMONITOR monitor) {
    MONITORINFO mi{sizeof(mi)};
    GetMonitorInfoW(monitor, &mi);
    if (g_settings.coverScreen) {
        return mi.rcMonitor;
    }
    struct Search {
        HMONITOR monitor;
        RECT monitorRect;
        RECT area;
    } search{monitor, mi.rcMonitor, mi.rcWork};

    EnumWindows(
        [](HWND hwnd, LPARAM param) __attribute__((stdcall)) -> BOOL {
            auto* s = (Search*)param;
            WCHAR className[64];
            if (!GetClassNameW(hwnd, className, ARRAYSIZE(className)) ||
                wcscmp(className, kDockClass) ||
                MonitorFromWindow(hwnd, MONITOR_DEFAULTTONULL) != s->monitor) {
                return TRUE;
            }
            int edge = (int)(INT_PTR)GetPropW(hwnd, kDockEdgeProp);
            int reserved = (int)(INT_PTR)GetPropW(hwnd, kDockReservedProp);
            const RECT& m = s->monitorRect;
            RECT& a = s->area;
            switch (edge) {
                case 1:
                    a.bottom = std::min<LONG>(a.bottom, m.bottom - reserved);
                    break;
                case 2:
                    a.left = std::max<LONG>(a.left, m.left + reserved);
                    break;
                case 3:
                    a.right = std::min<LONG>(a.right, m.right - reserved);
                    break;
            }
            return TRUE;
        },
        (LPARAM)&search);

    // A giant Dock can't reduce the area to zero.
    RECT& a = search.area;
    if (a.right - a.left < 200 || a.bottom - a.top < 200) {
        a = mi.rcWork;
    }
    return a;
}

struct LauncherDisplay {
    HMONITOR handle;
    RECT area;
    bool primary;
    std::wstring key, friendly;
};

std::vector<LauncherDisplay> EnumerateLauncherDisplays() {
    std::vector<LauncherDisplay> displays;
    EnumDisplayMonitors(nullptr, nullptr,
        [](HMONITOR monitor, HDC, LPRECT, LPARAM data) -> BOOL {
            MONITORINFOEXW info{};
            info.cbSize = sizeof(info);
            if (!GetMonitorInfoW(monitor, &info)) return TRUE;
            LauncherDisplay display{monitor, info.rcMonitor, (info.dwFlags & MONITORINFOF_PRIMARY) != 0};
            DISPLAY_DEVICEW device{sizeof(device)};
            if (EnumDisplayDevicesW(info.szDevice, 0, &device, EDD_GET_DEVICE_INTERFACE_NAME)) {
                if (device.DeviceID[0]) display.key = L"display:" + ToLower(device.DeviceID);
                display.friendly = device.DeviceString;
            }
            reinterpret_cast<std::vector<LauncherDisplay>*>(data)->push_back(std::move(display));
            return TRUE;
        }, reinterpret_cast<LPARAM>(&displays));
    std::stable_sort(displays.begin(), displays.end(), [](const auto& a, const auto& b) {
        if (a.primary != b.primary) return a.primary;
        if (a.area.left != b.area.left) return a.area.left < b.area.left;
        return a.area.top < b.area.top;
    });
    return displays;
}

HMONITOR ResolveLauncherDisplay(const std::vector<LauncherDisplay>& displays,
                                const std::wstring& setting, HMONITOR fallback) {
    if (setting.starts_with(L"display:")) {
        auto match = std::find_if(displays.begin(), displays.end(), [&](const auto& display) {
            return display.key == setting;
        });
        return match == displays.end() ? fallback : match->handle;
    }
    // Old backups and the Windhawk page use the ordinal.
    const int ordinal = _wtoi(setting.c_str());
    return ordinal > 0 && ordinal <= (int)displays.size() ? displays[ordinal - 1].handle : fallback;
}

HMONITOR ChosenMonitor() {
    POINT point{};
    HMONITOR fallback = GetCursorPos(&point) ? MonitorFromPoint(point, MONITOR_DEFAULTTOPRIMARY)
                                             : MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY);
    if (g_settings.monitor.empty() || g_settings.monitor == L"mouse") return fallback;
    return ResolveLauncherDisplay(EnumerateLauncherDisplays(), g_settings.monitor, fallback);
}

// Puts the Dock windows above Start, which leaves the Dock's strip free.
// The background ones first: the Dock window is owned by one of them and stays above.
void RaiseDockWindows() {
    // Collects before moving: raising a window changes the order FindWindowEx
    // walks, and with two Docks it kept alternating between them endlessly.
    std::vector<HWND> windows;
    for (PCWSTR className : {kDockPlateClass, kDockClass}) {
        HWND hwnd = nullptr;
        while ((hwnd = FindWindowExW(nullptr, hwnd, className, nullptr))) {
            if (IsWindowVisible(hwnd)) {
                windows.push_back(hwnd);
            }
        }
    }
    // Without waiting for the Dock thread: the request goes to its queue.
    for (HWND hwnd : windows) {
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Drawing

[[clang::no_destroy]] ComPtr<ID2D1Factory> g_d2d;
[[clang::no_destroy]] ComPtr<IDWriteFactory> g_dwrite;
[[clang::no_destroy]] ComPtr<ID3D11Device> g_d3d;
[[clang::no_destroy]] ComPtr<ID2D1Device> g_d2dDevice;
// Private context for the label shadow renderer. Reset with the rest of the
// device objects so a device loss cannot leave it pointing at the old device.
[[clang::no_destroy]] ComPtr<ID2D1DeviceContext> g_shadowContext;
[[clang::no_destroy]] ComPtr<ID2D1DeviceContext> g_target;
[[clang::no_destroy]] ComPtr<ID2D1SolidColorBrush> g_brush;
[[clang::no_destroy]] ComPtr<ID2D1Bitmap1> g_frame;  // The GPU frame.
[[clang::no_destroy]] ComPtr<ID3D11Texture2D> g_staging;
[[clang::no_destroy]] LaunchpadPresenter g_presenter;
bool UseGpuPresenter() {
#ifdef SEAMLESS_PREVIEW
    if (Preview::smoke && !Preview::benchmarkGpu) return false;
#endif
    return g_settings.gpuPresenter && !g_presenter.failed;
}
int g_frameWidth = 0, g_frameHeight = 0;
UINT g_targetGeneration = 1;
struct TileGlassResources {
    ComPtr<ID2D1RadialGradientBrush> surface, rim;
    // A restrained crimson pair for the power dock: the area is destructive,
    // but the glow still has to read as glass rather than a red button.
    ComPtr<ID2D1RadialGradientBrush> dangerSurface, dangerRim, dangerHalo;
};
[[clang::no_destroy]] TileGlassResources g_tileGlass;

// One radial brush with the fixed warning ramp, reused by the capsule and the
// text pill. Only its centre, radii and opacity change between draws. Declared
// next to the other device resources so ResetTarget can drop it with them.
struct PowerAmbientResources {
    ComPtr<ID2D1RadialGradientBrush> brush;
    struct Clip {
        ComPtr<ID2D1RoundedRectangleGeometry> geometry;
        float left = 0, top = 0, right = 0, bottom = 0, radius = -1;
    } bar, tip;
    UINT generation = 0;
    UINT32 colour = 0xFFFFFFFF;  // rgb the cached brush was built for
};
[[clang::no_destroy]] PowerAmbientResources g_powerAmbient;

struct Surface {
    HDC dc = nullptr;
    HBITMAP bitmap = nullptr;
    HGDIOBJ old = nullptr;
    int width = 0, height = 0;
};
Surface g_surface;

void FreeSurface(Surface& s) {
    // Free each handle independently: EnsureSurface can fail between creating
    // the DC and the DIB, and a partially built surface must not leak the half
    // that did get created.
    if (s.dc && s.old) SelectObject(s.dc, s.old);
    if (s.bitmap) DeleteObject(s.bitmap);
    if (s.dc) DeleteDC(s.dc);
    s = {};
}

bool EnsureSurface(Surface& s, int width, int height) {
    if (s.dc && s.width == width && s.height == height) {
        return true;
    }
    FreeSurface(s);
    BITMAPINFO bi{};
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = width;
    bi.bmiHeader.biHeight = -height;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    void* bits = nullptr;
    s.dc = CreateCompatibleDC(nullptr);
    s.bitmap = CreateDIBSection(s.dc, &bi, DIB_RGB_COLORS, &bits, nullptr, 0);
    if (!s.bitmap) {
        DeleteDC(s.dc);
        s = {};
        return false;
    }
    s.old = SelectObject(s.dc, s.bitmap);
    s.width = width;
    s.height = height;
    return true;
}

void ResetTarget();
void ClearLauncherTextCache();
void ClearTileLabelCache();
void ReleaseMaterial();
void ReleasePanelBlur();
void ClearGroupTitleRenders();


bool EnsureTarget() {
    if (g_target) {
        return true;
    }
    // No factory, so no device can be created: return the failure instead of
    // dereferencing a null ComPtr in g_d2d.As(). StartThread checks the
    // factories before it creates a window, but a failed D2D1CreateFactory
    // must not turn the first render into an access violation.
    if (!g_d2d) {
        return false;
    }
    // A real GPU context: the grid rasterizes on the GPU and each frame is
    // copied back into the layered-window DIB. WARP is the fallback.
    for (D3D_DRIVER_TYPE type :
         {D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP}) {
        if (SUCCEEDED(D3D11CreateDevice(
                nullptr, type, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT,
                nullptr, 0, D3D11_SDK_VERSION, &g_d3d, nullptr, nullptr))) {
            break;
        }
        g_d3d.Reset();
    }
    if (!g_d3d) {
        return false;
    }
    ComPtr<IDXGIDevice> dxgi;
    ComPtr<ID2D1Factory1> factory1;
    if (FAILED(g_d3d.As(&dxgi)) || FAILED(g_d2d.As(&factory1)) ||
        FAILED(factory1->CreateDevice(dxgi.Get(), &g_d2dDevice)) ||
        FAILED(g_d2dDevice->CreateDeviceContext(
            D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &g_target))) {
        ResetTarget();
        return false;
    }
    // Layout and input coordinates already use physical pixels.
    g_target->SetDpi(96, 96);
    g_target->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
    g_target->CreateSolidColorBrush(D2D1::ColorF(0, 0), &g_brush);
    g_targetGeneration++;
    return true;
}

void ResetTarget() {
    // These cache device-dependent bitmaps. They must go before the device is
    // dropped, or a lost device stays alive through the cache and the next
    // generation can sample a texture that belongs to the old one.
    for (const auto& app : g_apps) {
        if (app) app->bitmap.Reset();
    }
    ReleaseMaterial();
    ReleasePanelBlur();
    ClearGroupTitleRenders();
    ClearLauncherTextCache();
    ClearTileLabelCache();
    g_tileGlass={};
    g_powerAmbient={};
    g_presenter.Reset();
    g_frame.Reset();
    g_staging.Reset();
    g_frameWidth = g_frameHeight = 0;
    g_brush.Reset();
    g_shadowContext.Reset();  // Private context of the label shadow renderer.
    g_target.Reset();
    g_d2dDevice.Reset();
    g_d3d.Reset();
}

// The GPU frame target and the staging texture it is copied back through.
// Recreated whenever the window size changes.
bool EnsureFrame(int width, int height) {
    if (g_frame && g_frameWidth == width && g_frameHeight == height) {
        return true;
    }
    g_frame.Reset();
    g_staging.Reset();
    D2D1_BITMAP_PROPERTIES1 props = D2D1::BitmapProperties1(
        D2D1_BITMAP_OPTIONS_TARGET,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
                          D2D1_ALPHA_MODE_PREMULTIPLIED),
        96, 96);
    if (FAILED(g_target->CreateBitmap(D2D1::SizeU(width, height), nullptr, 0,
                                      props, &g_frame))) {
        return false;
    }
    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_STAGING;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    ComPtr<ID3D11DeviceContext> ctx;
    g_d3d->GetImmediateContext(&ctx);
    if (!UseGpuPresenter() && FAILED(g_d3d->CreateTexture2D(&desc, nullptr, &g_staging))) {
        g_frame.Reset();
        return false;
    }
    g_frameWidth = width;
    g_frameHeight = height;
    return true;
}

// Copies the rendered frame from the GPU into the layered-window DIB.
void PresentFrame() {
    if (!g_frame || !g_surface.bitmap) {
        return;
    }
    if (!g_staging) {
        D3D11_TEXTURE2D_DESC desc{};
        desc.Width = g_frameWidth; desc.Height = g_frameHeight;
        desc.MipLevels = desc.ArraySize = desc.SampleDesc.Count = 1;
        desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        desc.Usage = D3D11_USAGE_STAGING;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        if (FAILED(g_d3d->CreateTexture2D(&desc, nullptr, &g_staging))) return;
    }
    DIBSECTION ds{};
    if (GetObjectW(g_surface.bitmap, sizeof(ds), &ds) != sizeof(ds) ||
        !ds.dsBm.bmBits) {
        return;
    }
    ComPtr<IDXGISurface> surface;
    ComPtr<ID3D11Texture2D> texture;
    if (FAILED(g_frame->GetSurface(&surface)) ||
        FAILED(surface.As(&texture))) {
        return;
    }
    ComPtr<ID3D11DeviceContext> ctx;
    g_d3d->GetImmediateContext(&ctx);
    ctx->CopyResource(g_staging.Get(), texture.Get());
    D3D11_MAPPED_SUBRESOURCE mapped{};
    if (FAILED(ctx->Map(g_staging.Get(), 0, D3D11_MAP_READ, 0, &mapped))) {
        return;
    }
    int width = ds.dsBm.bmWidth, height = ds.dsBm.bmHeight;
    size_t span = (size_t)width * 4;
    const BYTE* src = (const BYTE*)mapped.pData;
    BYTE* dst = (BYTE*)ds.dsBm.bmBits;
    for (int y = 0; y < height; y++) {
        memcpy(dst + (size_t)y * span, src + (size_t)y * mapped.RowPitch,
               span);
    }
    ctx->Unmap(g_staging.Get(), 0);
}

ID2D1Bitmap* GetBitmap(App& app) {
    if (app.pixels.empty()) {
        return nullptr;
    }
    if (app.generation != g_targetGeneration) {
        app.generation = g_targetGeneration;
        app.bitmap.Reset();
        g_target->CreateBitmap(
            D2D1::SizeU(app.iconSize, app.iconSize), app.pixels.data(),
            app.iconSize * 4,
            D2D1::BitmapProperties(
                D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
                                  D2D1_ALPHA_MODE_PREMULTIPLIED),
                96, 96),
            &app.bitmap);
    }
    return app.bitmap.Get();
}

D2D1_COLOR_F Alpha(D2D1_COLOR_F c, float a) {
    c.a = a;
    return c;
}

void Fill(const D2D1_RECT_F& r, D2D1_COLOR_F color, float radius = 0) {
    g_brush->SetColor(color);
    if (radius > 0) {
        g_target->FillRoundedRectangle(D2D1::RoundedRect(r, radius, radius),
                                       g_brush.Get());
    } else {
        g_target->FillRectangle(r, g_brush.Get());
    }
}

////////////////////////////////////////////////////////////////////////////////
// Background
//
// Adjustable: a screenshot taken before the window appears, uploaded once and
// then scaled down and blurred on the GPU as an effect graph. Downscaling
// first makes a strong blur cheap.


// The material engine keeps one capture per open as a small, already blurred
// texture, and caches the graded copy next to it, so moving a slider costs one
// colour-matrix pass instead of a new blur. Parameters and presets live in
// launchpad_material.h; this block is only the Direct2D plumbing.
struct MaterialCache {
    ComPtr<ID2D1Bitmap1> blurred;
    ComPtr<ID2D1Bitmap1> graded;
    int width = 0, height = 0, factor = 1;  // Blurred size.
    int sourceWidth = 0, sourceHeight = 0;  // Captured pixels.
    float sigma = 0;                        // Window pixels.
    UINT generation = 0;
    bool hasGrade = false;
    launchpad_material::Params gradedFor;
};
[[clang::no_destroy]] MaterialCache g_materialCache;

struct MaterialRuntime {
    launchpad_material::Backend backend = launchpad_material::Backend::Custom;
    launchpad_material::Params params;
    std::optional<D2D1_COLOR_F> average;
    float meanLuminance = .5f;
    RECT area{};
    float scale = 1;
    bool haveArea = false, captureFailed = false;
    int captureFailures = 0;
    ULONGLONG captureRetryAt = 0;
    double captureMs = 0, blurMs = 0, gradeMs = 0;
    ComPtr<ID2D1Bitmap> noiseTile;
    UINT noiseGeneration = 0;
    ComPtr<ID2D1BitmapBrush> noiseBrush;
};
[[clang::no_destroy]] MaterialRuntime g_material;

// Sample evenly spaced cell centers so all areas of the page contribute.
D2D1_COLOR_F AverageBackdropColor(const UINT32* pixels, int width, int height) {
    const int columns = std::min(width, 32), rows = std::min(height, 32);
    unsigned red = 0, green = 0, blue = 0;
    for (int row = 0; row < rows; ++row) {
        const auto* line = pixels + ((2 * row + 1) * height / (2 * rows)) * (size_t)width;
        for (int column = 0; column < columns; ++column) {
            const UINT32 pixel = line[(2 * column + 1) * width / (2 * columns)];
            red += (pixel >> 16) & 0xff;
            green += (pixel >> 8) & 0xff;
            blue += pixel & 0xff;
        }
    }
    const float total = 255.f * columns * rows;
    return D2D1::ColorF(red / total, green / total, blue / total);
}

// Scales source down by f and gaussian-blurs it by sigma / f into a new sw x
// sh GPU bitmap. rect is the part of source used; the rest is cropped.
ComPtr<ID2D1Bitmap1> BlurToBitmap(ID2D1Bitmap* source,
                                  const D2D1_RECT_F& rect, int f, float sigma,
                                  int sw, int sh,
                                  D2D1_POINT_2F dest = D2D1::Point2F(0, 0)) {
    D2D1_BITMAP_PROPERTIES1 props = D2D1::BitmapProperties1(
        D2D1_BITMAP_OPTIONS_TARGET,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
                          D2D1_ALPHA_MODE_PREMULTIPLIED),
        96, 96);
    ComPtr<ID2D1Bitmap1> target, small;
    ComPtr<ID2D1Effect> blur;
    if (FAILED(g_target->CreateBitmap(D2D1::SizeU(sw, sh), nullptr, 0, props,
                                      &small)) ||
        FAILED(g_target->CreateBitmap(D2D1::SizeU(sw, sh), nullptr, 0, props,
                                      &target)) ||
        FAILED(g_target->CreateEffect(CLSID_D2D1GaussianBlur, &blur))) {
        return nullptr;
    }
    // Pass 1: the part of source in rect, scaled down by f and placed at dest.
    const float w = (rect.right - rect.left) / f;
    const float h = (rect.bottom - rect.top) / f;
    g_target->BeginDraw();
    g_target->SetTarget(small.Get());
    g_target->Clear(D2D1::ColorF(0, 0));
    g_target->DrawBitmap(source,
                         D2D1::RectF(dest.x, dest.y, dest.x + w, dest.y + h),
                         1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, rect);
    if (FAILED(g_target->EndDraw())) {
        return nullptr;
    }
    // Pass 2: a gaussian blur of what pass 1 drew.
    blur->SetInput(0, small.Get());
    blur->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, sigma / f);
    blur->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
    g_target->BeginDraw();
    g_target->SetTarget(target.Get());
    g_target->Clear(D2D1::ColorF(0, 0));
    g_target->DrawImage(blur.Get(), nullptr, nullptr,
                        D2D1_INTERPOLATION_MODE_LINEAR);
    if (FAILED(g_target->EndDraw())) {
        return nullptr;
    }
    return target;
}

// One screen capture, one average/mean pass, one GPU downscale and one blur.
// Everything the effect chain needs later is stored in the cache.
void CaptureMaterial(const RECT& rc, float sigma) {
    const double started = Now();
    g_material.captureFailed = false;
    int width = rc.right - rc.left, height = rc.bottom - rc.top;
    if (width <= 0 || height <= 0 || !EnsureTarget()) {
        g_material.captureFailed = true;
        return;
    }

    BITMAPINFO bi{};
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = width;
    bi.bmiHeader.biHeight = -height;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    void* bits = nullptr;
    HDC screen = GetDC(nullptr);
    HDC dc = CreateCompatibleDC(screen);
    HBITMAP bitmap =
        CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &bits, nullptr, 0);
    if (!bitmap) {
        DeleteDC(dc);
        ReleaseDC(nullptr, screen);
        g_material.captureFailed = true;
        return;
    }
    HGDIOBJ old = SelectObject(dc, bitmap);
    // Without CAPTUREBLT, which makes the cursor flicker; the screen composed
    // by DWM already includes layered windows.
    BOOL captured = BitBlt(dc, 0, 0, width, height, screen, rc.left, rc.top, SRCCOPY);
    GdiFlush();
    auto finishCapture = [&] {
        SelectObject(dc, old);
        DeleteObject(bitmap);
        DeleteDC(dc);
        ReleaseDC(nullptr, screen);
    };
    if (!captured) {
        finishCapture();
        g_material.captureFailed = true;
        return;
    }
    // Mean luminance on a 16 px grid: the luminosity layer works against the
    // backdrop, so a dark desktop gets a light haze and a bright one a dark
    // haze. Same grid the settings preview samples.
    double luminance = 0;
    size_t samples = 0;
    for (int y = 0; y < height; y += 16) {
        const UINT32* line = (const UINT32*)bits + (size_t)y * (size_t)width;
        for (int x = 0; x < width; x += 16) {
            const UINT32 pixel = line[x];
            luminance += .2126 * ((pixel >> 16) & 255) +
                         .7152 * ((pixel >> 8) & 255) +
                         .0722 * (pixel & 255);
            ++samples;
        }
    }
    g_material.meanLuminance =
        samples ? (float)(luminance / (255.0 * (double)samples)) : .5f;
    g_material.average = AverageBackdropColor((const UINT32*)bits, width, height);
    // GDI screen copies have an undefined alpha channel; this source is opaque.
    for(size_t i=0;i<(size_t)width*height;i++)((UINT32*)bits)[i]|=0xFF000000;

    D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
                          D2D1_ALPHA_MODE_PREMULTIPLIED),
        96, 96);
    ComPtr<ID2D1Bitmap> full;
    bool uploaded =
        SUCCEEDED(g_target->CreateBitmap(D2D1::SizeU(width, height), bits,
                                         width * 4, props, &full));
    finishCapture();
    if (!uploaded) {
        g_material.captureFailed = true;
        return;
    }
    g_material.captureMs = (Now() - started) * 1000;

    const double blurStarted = Now();
    int f = sigma >= 3 ? std::clamp((int)(sigma / 3), 1, 16) : 1;
    int sw = (width + f - 1) / f, sh = (height + f - 1) / f;
    ComPtr<ID2D1Bitmap1> blurred = BlurToBitmap(
        full.Get(), D2D1::RectF(0, 0, (float)width, (float)height), f, sigma,
        sw, sh);
    g_material.blurMs = (Now() - blurStarted) * 1000;
    if (!blurred) {
        g_material.captureFailed = true;
        return;
    }
    g_materialCache = {};
    g_materialCache.blurred = blurred;
    g_materialCache.width = sw;
    g_materialCache.height = sh;
    g_materialCache.factor = f;
    g_materialCache.sourceWidth = width;
    g_materialCache.sourceHeight = height;
    g_materialCache.sigma = sigma;
    g_materialCache.generation = g_targetGeneration;
}

// Builds (or reuses) the graded copy with a colour-matrix effect. Runs before
// the frame's BeginDraw: the effect renders into its own texture.
void BuildMaterialGrade() {
    g_material.gradeMs = 0;
    if (!g_materialCache.blurred) return;
    const auto& p = g_material.params;
    if (launchpad_material::GradeIsNeutral(p)) {
        g_materialCache.graded.Reset();
        g_materialCache.hasGrade = false;
        return;
    }
    if (g_materialCache.hasGrade &&
        !launchpad_material::NeedsGrade(g_materialCache.gradedFor, p)) {
        return;
    }
    const double started = Now();
    if (!g_target) return;
    ComPtr<ID2D1Effect> effect;
    if (FAILED(g_target->CreateEffect(CLSID_D2D1ColorMatrix, &effect))) {
        return;
    }
    const launchpad_material::Matrix5x4 grade = launchpad_material::GradeMatrix(p);
    D2D1_MATRIX_5X4_F matrix{};
    memcpy(&matrix, &grade, sizeof(matrix));
    effect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, matrix);
    effect->SetInput(0, g_materialCache.blurred.Get());
    D2D1_BITMAP_PROPERTIES1 props = D2D1::BitmapProperties1(
        D2D1_BITMAP_OPTIONS_TARGET,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
                          D2D1_ALPHA_MODE_PREMULTIPLIED),
        96, 96);
    ComPtr<ID2D1Bitmap1> graded;
    if (FAILED(g_target->CreateBitmap(
            D2D1::SizeU(g_materialCache.width, g_materialCache.height), nullptr,
            0, props, &graded))) {
        return;
    }
    g_target->BeginDraw();
    g_target->SetTarget(graded.Get());
    g_target->Clear(D2D1::ColorF(0, 0));
    g_target->DrawImage(effect.Get(), nullptr, nullptr,
                        D2D1_INTERPOLATION_MODE_LINEAR,
                        D2D1_COMPOSITE_MODE_SOURCE_COPY);
    if (FAILED(g_target->EndDraw())) {
        return;
    }
    g_materialCache.graded = graded;
    g_materialCache.gradedFor = p;
    g_materialCache.hasGrade = true;
    g_material.gradeMs = (Now() - started) * 1000;
}

// Captures and blurs only when the window, size, scale or blur radius changed.
void PrepareMaterial(const RECT& area, float scale) {
    g_material.area = area;
    g_material.scale = scale;
    g_material.haveArea = true;
    if (!launchpad_material::UsesSnapshot(g_material.backend)) {
        if (g_materialCache.blurred || g_materialCache.graded) g_materialCache = {};
        return;
    }
    const float sigma = g_material.params.blurSigma * scale;
    const int width = area.right - area.left, height = area.bottom - area.top;
    const bool stale = !g_materialCache.blurred ||
                       g_materialCache.generation != g_targetGeneration ||
                       g_materialCache.sourceWidth != width ||
                       g_materialCache.sourceHeight != height ||
                       std::abs(g_materialCache.sigma - sigma) > .01f;
    // A failed grab is often transient (the desktop was changing, or a
    // full-screen allocation was refused). Retry once on the next frame, then
    // back off for five seconds while the launcher stays open; HideNow()
    // clears the counter through ReleaseMaterial().
    if (stale && (!g_material.captureFailed ||
                  g_material.captureFailures < 2 ||
                  GetTickCount64() >= g_material.captureRetryAt)) {
        CaptureMaterial(area, sigma);
        if (g_material.captureFailed) {
            ++g_material.captureFailures;
            if (g_material.captureFailures >= 2)
                g_material.captureRetryAt = GetTickCount64() + 5000;
        } else {
            g_material.captureFailures = 0;
        }
    }
    if (g_materialCache.blurred) BuildMaterialGrade();
}

ID2D1Bitmap* MaterialBitmap() {
    if (!g_materialCache.blurred) return nullptr;
    if (g_materialCache.hasGrade && g_materialCache.graded)
        return g_materialCache.graded.Get();
    return g_materialCache.blurred.Get();
}

// One 256 px grain tile, wrapped and point-sampled; rebuilt only when the
// render target generation changes.
void EnsureMaterialNoise() {
    if (g_material.noiseBrush && g_material.noiseGeneration == g_targetGeneration)
        return;
    std::vector<uint8_t> signedNoise;
    std::vector<uint32_t> pixels;
    launchpad_material::MakeNoiseTile(signedNoise);
    launchpad_material::NoiseTileToBgra(signedNoise, pixels);
    D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_STRAIGHT),
        96, 96);
    ComPtr<ID2D1Bitmap> tile;
    if (FAILED(g_target->CreateBitmap(
            D2D1::SizeU(launchpad_material::kNoiseTile,
                        launchpad_material::kNoiseTile),
            pixels.data(), launchpad_material::kNoiseTile * 4, props, &tile))) {
        return;
    }
    D2D1_BITMAP_BRUSH_PROPERTIES brush = {
        D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE_WRAP,
        D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR};
    ComPtr<ID2D1BitmapBrush> brushObject;
    if (FAILED(g_target->CreateBitmapBrush(tile.Get(), brush,
                                           &brushObject))) {
        return;
    }
    g_material.noiseTile = tile;
    g_material.noiseBrush = brushObject;
    g_material.noiseGeneration = g_targetGeneration;
}

void DrawMaterialNoise(float W, float H, float alpha) {
    const float strength = g_material.params.noise * alpha;
    if (strength <= .001f) return;
    EnsureMaterialNoise();
    if (!g_material.noiseBrush) return;
    g_material.noiseBrush->SetOpacity(std::min(1.0f, strength));
    const float grain = std::max(.01f, g_material.params.noiseScale);
    g_material.noiseBrush->SetTransform(
        D2D1::Matrix3x2F::Scale(grain, grain));
    g_target->FillRectangle(D2D1::RectF(0, 0, W, H), g_material.noiseBrush.Get());
}

void ReleaseMaterial() {
    g_materialCache = {};
    g_material.noiseTile.Reset();
    g_material.noiseBrush.Reset();
    g_material.noiseGeneration = 0;
    g_material.average.reset();
    g_material.captureFailed = false;
    g_material.captureFailures = 0;
    g_material.captureRetryAt = 0;
}

// The tint opacity, colour and dim live in Settings for the old rows and the
// panel sampling; mirror them into the engine right before every draw so the
// two can never disagree.
// Early 1.4.1 builds let each built-in preset carry and apply its own tint.
// That wrote the preset's hue into tintColor, where it stayed after switching
// presets. Any exact match is residue: put it back to neutral black.
static UINT32 LegacyPresetTint(const std::wstring& id) {
    if (id == L"Clear Glass" || id == L"Light Glass" || id == L"Soft Frost" ||
        id == L"Heavy Frost") return 0xFFFFFF;
    if (id == L"Light Acrylic") return 0xF5F7FA;
    if (id == L"Dark Acrylic") return 0x20242B;
    if (id == L"macOS Style") return 0x181E30;
    if (id == L"Stained Glass") return 0xC4D5E6;
    if (id == L"Minimal Grey") return 0xF5F5F5;
    return 0;
}

static void MigrateLegacyPresetTint() {
    static bool attempted = false;
    if (attempted || Wh_GetIntValue(L"legacyPresetTintMigrated", 0)) return;
    attempted = true;
    if (g_dataReadOnly) return; // Never turn migration into a read-only write.
    const UINT32 legacy = LegacyPresetTint(GetStringSetting(L"materialPreset"));
    const UINT32 stored = legacy ? launchpad_material::RgbFromColor(
        ParseColor(GetStringSetting(L"tintColor"), launchpad_material::ColorFromRgb(0))) : 0;
    if (legacy && stored == legacy) {
#ifndef SEAMLESS_PREVIEW
        std::wstring host = WindhawkUtils::StringSetting::make(L"tintColor").get();
        launchpad_settings_store::Save(StoragePath(L"ui-settings.ini"),
                                       {{L"tintColor", L"#000000", host}});
#endif
        // Light/dark/system themes paint their own base tint after fields load.
        if (g_settings.theme != L"light" && g_settings.theme != L"dark" &&
            g_settings.theme != L"system")
            g_settings.tint = launchpad_material::ColorFromRgb(0);
    }
    // One process- and installation-wide attempt only. A later user choice that
    // happens to equal a legacy preset colour is a real choice, not residue.
    Wh_SetIntValue(L"legacyPresetTintMigrated", 1);
}

void SyncMaterialFromSettings() {
    g_material.backend = g_settings.backend;
    g_material.params = g_settings.material;
    g_material.params.tint = g_settings.tint;
    g_material.params.tintOpacity = g_settings.opacity;
    g_material.params.dim = g_settings.backgroundDim;
}

// Blur, grade, luminosity, tint, noise, dim: the documented order.
void DrawBackdrop(float W, float H, float alpha) {
    const auto& p = g_material.params;
    using B = launchpad_material::Backend;
    switch (g_material.backend) {
        case B::Custom:
            if (ID2D1Bitmap* bitmap = MaterialBitmap()) {
                g_target->DrawBitmap(
                    bitmap, D2D1::RectF(0, 0, W, H), alpha,
                    D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
                    D2D1::RectF(0, 0, (float)g_materialCache.width,
                               (float)g_materialCache.height));
                if (p.luminosity > 0)
                    Fill(D2D1::RectF(0, 0, W, H),
                         Alpha(launchpad_material::LuminosityColor(
                                   g_material.meanLuminance),
                               p.luminosity * alpha));
                if (p.tintOpacity > 0)
                    Fill(D2D1::RectF(0, 0, W, H), Alpha(p.tint, p.tintOpacity * alpha));
                DrawMaterialNoise(W, H, alpha);
            } else if (p.tintOpacity > 0) {
                // The capture failed: keep a readable panel rather than nothing.
                Fill(D2D1::RectF(0, 0, W, H), Alpha(p.tint, p.tintOpacity * alpha));
            }
            break;
        case B::Solid:
            Fill(D2D1::RectF(0, 0, W, H), Alpha(p.tint, alpha));
            break;
        default:
            if (p.tintOpacity > 0)
                Fill(D2D1::RectF(0, 0, W, H), Alpha(p.tint, p.tintOpacity * alpha));
            break;
    }
    if (p.dim > 0)
        Fill(D2D1::RectF(0, 0, W, H), D2D1::ColorF(0, 0, 0, p.dim * alpha));
}

// The open folder's panel is frosted glass: the grid under it, blurred. The
// blur is taken from the window surface right after the grid is drawn and
// kept while nothing under the panel moves.
struct PanelBlur {
    ComPtr<ID2D1Bitmap> bitmap;
    ComPtr<ID2D1BitmapBrush> brush;
    D2D1_RECT_F rect{};  // Window pixels it was taken from.
    int x = 0, y = 0, factor = 1;
    const void* folder = nullptr;
    float scroll = 0;
    UINT generation = 0;
};
[[clang::no_destroy]] PanelBlur g_panelBlur;

// Also called from ResetTarget(): the bitmap and brush belong to the device
// generation that is being replaced.
void ReleasePanelBlur() {
    g_panelBlur = {};
}

void CapturePanelBlur(const D2D1_RECT_F& rect, const void* folder,
                      float scroll, float scale) {
    g_panelBlur = {};
    g_panelBlur.rect = rect;
    g_panelBlur.folder = folder;
    g_panelBlur.scroll = scroll;
    g_panelBlur.generation = g_targetGeneration;
    if (!g_frame) {
        return;
    }
    // The blur source is the frame just drawn, still on the GPU; only the
    // region under the panel is used.
    int x0 = std::clamp((int)std::floor(rect.left), 0, g_frameWidth);
    int y0 = std::clamp((int)std::floor(rect.top), 0, g_frameHeight);
    int x1 = std::clamp((int)std::ceil(rect.right), 0, g_frameWidth);
    int y1 = std::clamp((int)std::ceil(rect.bottom), 0, g_frameHeight);
    if (x1 <= x0 || y1 <= y0) {
        return;
    }
    float sigma = 24 * scale;
    int f = std::clamp((int)(sigma / 3), 1, 16);
    int sw = (x1 - x0 + f - 1) / f, sh = (y1 - y0 + f - 1) / f;
    ComPtr<ID2D1Bitmap1> blurred =
        BlurToBitmap(g_frame.Get(),
                     D2D1::RectF((float)x0, (float)y0, (float)x1, (float)y1),
                     f, sigma, sw, sh);
    if (!blurred) {
        return;
    }
    g_panelBlur.bitmap = blurred.Get();
    g_panelBlur.x = x0;
    g_panelBlur.y = y0;
    g_panelBlur.factor = f;
}

bool PanelBlurFresh(const D2D1_RECT_F& rect, const void* folder,
                    float scroll) {
    const PanelBlur& b = g_panelBlur;
    return b.folder == folder && b.generation == g_targetGeneration &&
           b.scroll == scroll && std::abs(b.rect.left - rect.left) < 0.5f &&
           std::abs(b.rect.top - rect.top) < 0.5f &&
           std::abs(b.rect.right - rect.right) < 0.5f &&
           std::abs(b.rect.bottom - rect.bottom) < 0.5f;
}

// Paints the kept blur into rect (window pixels, identity transform).
void DrawPanelBlur(const D2D1_RECT_F& rect, float radius) {
    PanelBlur& b = g_panelBlur;
    if (!b.bitmap) {
        return;
    }
    if (!b.brush) g_target->CreateBitmapBrush(
        b.bitmap.Get(),
        D2D1::BitmapBrushProperties(D2D1_EXTEND_MODE_CLAMP,
                                    D2D1_EXTEND_MODE_CLAMP,
                                    D2D1_BITMAP_INTERPOLATION_MODE_LINEAR),
        &b.brush);
    if (!b.brush) {
        return;
    }
    b.brush->SetTransform(D2D1::Matrix3x2F::Scale((float)b.factor,
                                                (float)b.factor) *
                        D2D1::Matrix3x2F::Translation((float)b.x, (float)b.y));
    g_target->FillRoundedRectangle(D2D1::RoundedRect(rect, radius, radius),
                                   b.brush.Get());
}

// Acrylic and Windows blur: a regular window with the DWM effect sits right
// below the layered window where everything is drawn, like in the Dock.

struct AccentPolicy {
    INT accentState;
    INT accentFlags;
    INT gradientColor;
    INT animationId;
};

struct WindowCompositionAttribData {
    DWORD attrib;
    PVOID data;
    SIZE_T size;
};

constexpr DWORD WCA_ACCENT_POLICY = 19;
constexpr INT ACCENT_DISABLED = 0;
constexpr INT ACCENT_ENABLE_BLURBEHIND = 3;
constexpr INT ACCENT_ENABLE_ACRYLICBLURBEHIND = 4;
constexpr DWORD kDwmUseImmersiveDarkMode = 20;
constexpr DWORD kDwmWindowCornerPreference = 33;
constexpr DWORD kDwmRedirectionBitmapAlpha = 39;  // Windows 11 26100+
constexpr DWORD kDwmBorderColor = 34;
// Windows 11 system backdrops (DWMWA_SYSTEMBACKDROP_TYPE).
constexpr DWORD kDwmSystemBackdropType = 38;
constexpr int kDwmBackdropNone = 1, kDwmBackdropMica = 2,
              kDwmBackdropAcrylic = 3, kDwmBackdropMicaAlt = 4;
constexpr int kSystemBackdropBuild = 22621;

int WindowsBuildNumber() {
    static const int build = [] {
        using RtlGetVersion_t = LONG(WINAPI*)(OSVERSIONINFOW*);
        auto rtlGetVersion = (RtlGetVersion_t)GetProcAddress(
            GetModuleHandleW(L"ntdll.dll"), "RtlGetVersion");
        if (!rtlGetVersion) return 0;
        OSVERSIONINFOW info{};
        info.dwOSVersionInfoSize = sizeof(info);
        return rtlGetVersion(&info) == 0 ? (int)info.dwBuildNumber : 0;
    }();
    return build;
}

bool SystemBackdropSupported() { return WindowsBuildNumber() >= kSystemBackdropBuild; }

HWND g_backdrop;

void SetAccent(HWND hwnd, INT state, DWORD abgr) {
    using SetWindowCompositionAttribute_t =
        BOOL(WINAPI*)(HWND, WindowCompositionAttribData*);
    static auto setWindowCompositionAttribute =
        (SetWindowCompositionAttribute_t)GetProcAddress(
            GetModuleHandleW(L"user32.dll"), "SetWindowCompositionAttribute");
    if (!setWindowCompositionAttribute) {
        return;
    }
    AccentPolicy policy{state, state ? 2 : 0, (INT)abgr, 0};
    WindowCompositionAttribData data{WCA_ACCENT_POLICY, &policy,
                                     sizeof(policy)};
    setWindowCompositionAttribute(hwnd, &data);
}

void ResetNativeBackdrop() {
    if (!g_backdrop) return;
    SetAccent(g_backdrop, ACCENT_DISABLED, 0);
    if (SystemBackdropSupported()) {
        int type = kDwmBackdropNone;
        DwmSetWindowAttribute(g_backdrop, kDwmSystemBackdropType, &type,
                              sizeof(type));
    }
    MARGINS margins{0, 0, 0, 0};
    DwmExtendFrameIntoClientArea(g_backdrop, &margins);
}

void ApplyBackdropEffect() {
    using B = launchpad_material::Backend;
    const B backend = g_settings.backend;

    BOOL dark = TRUE;
    DwmSetWindowAttribute(g_backdrop, kDwmUseImmersiveDarkMode, &dark,
                          sizeof(dark));
    UINT corner = 1;  // DWMWCP_DONOTROUND
    DwmSetWindowAttribute(g_backdrop, kDwmWindowCornerPreference, &corner,
                          sizeof(corner));
    COLORREF border = 0xFFFFFFFE;  // DWMWA_COLOR_NONE
    DwmSetWindowAttribute(g_backdrop, kDwmBorderColor, &border,
                          sizeof(border));

    D2D1_COLOR_F c = g_settings.tint;
    DWORD abgr = (1u << 24) | ((DWORD)(c.b * 255) << 16) |
                 ((DWORD)(c.g * 255) << 8) | (DWORD)(c.r * 255);

    if (launchpad_material::UsesSystemBackdrop(backend)) {
        // Mica and the system acrylic come from DWM behind an extended frame,
        // not from an accent policy. On builds without them, fall through to
        // the blur accent so the window never turns opaque.
        if (SystemBackdropSupported()) {
            int type = backend == B::Mica        ? kDwmBackdropMica
                       : backend == B::MicaAlt   ? kDwmBackdropMicaAlt
                                                 : kDwmBackdropAcrylic;
            DwmSetWindowAttribute(g_backdrop, kDwmSystemBackdropType, &type,
                                  sizeof(type));
            SetAccent(g_backdrop, ACCENT_DISABLED, 0);
            MARGINS margins{-1, -1, -1, -1};
            DwmExtendFrameIntoClientArea(g_backdrop, &margins);
            return;
        }
        if (backend != B::AcrylicSystem) {
            SetAccent(g_backdrop, ACCENT_ENABLE_BLURBEHIND, 0);
            return;
        }
        SetAccent(g_backdrop, ACCENT_ENABLE_ACRYLICBLURBEHIND, abgr);
        return;
    }

    // The color is drawn on the top window, with the chosen opacity; acrylic
    // only requires a nonzero alpha. Makes DWM respect the window's alpha too.
    DWM_BLURBEHIND blur{};
    blur.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
    blur.fEnable = TRUE;
    blur.hRgnBlur = CreateRectRgn(0, 0, -1, -1);
    DwmEnableBlurBehindWindow(g_backdrop, &blur);
    DeleteObject(blur.hRgnBlur);

    switch (backend) {
        case B::Acrylic:
            SetAccent(g_backdrop, ACCENT_ENABLE_ACRYLICBLURBEHIND, abgr);
            break;
        case B::Blur:
            SetAccent(g_backdrop, ACCENT_ENABLE_BLURBEHIND, 0);
            break;
        default:
            ResetNativeBackdrop();
            break;
    }
}

bool UsesDwmBackdrop() {
    return launchpad_material::UsesDwmBackdrop(g_settings.backend);
}

////////////////////////////////////////////////////////////////////////////////
// Layout

struct Layout {
    RECT area{};  // Window position on screen.
    int width = 0, height = 0;
    float scale = 1;
    bool useSavedGroupGeometry = true;
    float icon = 72, labelHeight = 20, cellWidth = 140, cellHeight = 140;
    float cellSpare = 0;  // Extra row height that fills the screen with pages.
    float cellPad = 6;
    int columns = 1;
    float gridLeft = 0;
    float viewTop = 0, viewBottom = 0;
    float groupPageBottom = 0;  // The page capacity is shared by browsing and editing.
    int visibleRows = 1;
    float contentHeight = 0;
    int perPage = 1, pages = 1;  // With the pages layout.
    float dotsY = 0;             // Center of the page dots.
    D2D1_RECT_F search{};
    D2D1_RECT_F clearButton{};  // Clear the current query.
    D2D1_RECT_F powerBar{};    // The power dock pill, aligned by dockAlignment.
    std::vector<int> powerItems;  // kPower indices shown, left to right.
    float powerButton = 36, powerGap = 6;
};

Layout g_layout;
LaunchpadMetrics g_metrics;
void FinishMetrics() {
    auto message = g_metrics.Finish(StoragePath(L"performance.csv"), Now(), g_presenter.Active() && UseGpuPresenter());
    if (!message.empty()) Wh_Log(L"%s", message.c_str());
}
// Set from GetPwrCapabilities each time the grid opens.
bool g_canSleep = true, g_canHibernate = false;
// Pixels: down the list, or, with pages, across the row of pages.
float g_scroll, g_scrollTarget;
float g_groupPagePan=0;
int g_wheelDelta;  // Wheel accumulated until it completes a row (touchpads).
[[clang::no_destroy]] std::weak_ptr<Folder> g_groupWheelTarget;
int g_groupWheelRemainder = 0;

[[clang::no_destroy]] ComPtr<IDWriteTextFormat> g_labelFormat, g_searchFormat,
    g_messageFormat, g_iconFormat, g_tipFormat, g_groupFormat;
float g_formatScale;
UINT g_formatVersion = 0;

void EnsureFormats(float scale) {
    if (g_labelFormat && g_formatScale == scale) {
        return;
    }
    g_formatScale = scale;
    ++g_formatVersion;
    ClearLauncherTextCache();
    auto make = [&](float size, DWRITE_FONT_WEIGHT weight,
                    DWRITE_TEXT_ALIGNMENT align,
                    ComPtr<IDWriteTextFormat>& format,
                    PCWSTR family = nullptr) {
        format.Reset();
        if (!family) family = g_settings.font.c_str();
        g_dwrite->CreateTextFormat(family, nullptr, weight,
                                   DWRITE_FONT_STYLE_NORMAL,
                                   DWRITE_FONT_STRETCH_NORMAL, size * scale,
                                   L"", &format);
        if (!format) {
            return;
        }
        format->SetTextAlignment(align);
        format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        format->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
        DWRITE_TRIMMING trimming{DWRITE_TRIMMING_GRANULARITY_CHARACTER, 0, 0};
        ComPtr<IDWriteInlineObject> ellipsis;
        g_dwrite->CreateEllipsisTrimmingSign(format.Get(), &ellipsis);
        format->SetTrimming(&trimming, ellipsis.Get());
    };
    // The label is the only thing that has to stay legible over glass: one
    // DIP larger and semi-bold. The weight itself is a hint — DirectWrite
    // picks the closest weight the family actually has (400/700 for Microsoft
    // YaHei), which is the proper mapping, never a fake double draw.
    make((float)g_settings.labelSize + 1, DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_TEXT_ALIGNMENT_CENTER,
         g_labelFormat);
    make(19, DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_TEXT_ALIGNMENT_CENTER, g_groupFormat);
    make(15, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_TEXT_ALIGNMENT_LEADING,
         g_searchFormat);
    make(16, DWRITE_FONT_WEIGHT_SEMI_LIGHT, DWRITE_TEXT_ALIGNMENT_CENTER,
         g_messageFormat);
    make(12.5f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_TEXT_ALIGNMENT_CENTER,
         g_tipFormat);
    // Windows 11 has Segoe Fluent Icons; Windows 10 only the older MDL2 font,
    // which has the same code points for these glyphs.
    ComPtr<IDWriteFontCollection> fonts;
    UINT32 index;
    BOOL fluent = FALSE;
    if (SUCCEEDED(g_dwrite->GetSystemFontCollection(&fonts))) {
        fonts->FindFamilyName(L"Segoe Fluent Icons", &index, &fluent);
    }
    // The dock glyphs are line icons at heart. 20 DIP leaves a little more air
    // inside the 36 DIP glass buttons while keeping the bold strokes readable.
    make(20.0f, DWRITE_FONT_WEIGHT_BOLD, DWRITE_TEXT_ALIGNMENT_CENTER,
         g_iconFormat, fluent ? L"Segoe Fluent Icons" : L"Segoe MDL2 Assets");
}

float GridMargin() {
    return std::min(g_layout.width*.12f,std::max(24*g_layout.scale,g_layout.width*.045f));
}
float SquareCellSize(int columns) {
    return std::max(1.f,g_layout.width-2*GridMargin())/std::max(1,columns);
}
int DesiredGridColumns() {
    if(g_settings.globalColumns>0)return g_settings.globalColumns;
    if(g_squareGridColumns>0)return g_squareGridColumns;
    if(g_settings.columns>0)return g_settings.columns;
    float density=(std::max(g_settings.iconSize+12.f,(g_settings.labelSize+8)*4)+g_settings.horizontalGap)*g_layout.scale;
    return std::clamp((int)std::round((g_layout.width-2*GridMargin())/density),1,30);
}

int FitSquareGridColumns(int requested, bool grouped) {
    const auto& L = g_layout;
    const float labelSpace = g_settings.showLabels ? L.labelHeight + 6 * L.scale : 0.f;
    const float labelSide = g_settings.showLabels ? L.labelHeight / .45f : 0.f;
    const float cell = std::max(L.icon + 6 * L.scale + labelSpace, labelSide);
    // Square groups reserve half a cell above the icons for the title.
    const float minSide = grouped ? std::max(cell, 48 * L.scale) : cell;
    const int fitting = std::max(1, (int)std::floor((L.width - 2 * GridMargin()) / minSide));
    return std::min(requested, fitting);
}

float GroupPitchY();
float MaxScroll() {
    if (g_settings.pages) {
        return (g_layout.pages - 1) * (float)g_layout.width;
    }
    float editRoom=g_groupEditMode&&g_settings.groups&&g_query.empty()?(g_squareGridColumns?SquareCellSize(g_squareGridColumns):GroupPitchY()):0;
    return std::max(0.0f, g_layout.contentHeight + editRoom -
                              (g_layout.viewBottom - g_layout.viewTop));
}

bool GroupedView() { return g_settings.groups && g_query.empty(); }
void ComputeGroups();
std::array<D2D1_RECT_F,3> GroupEditButtons();
float MaxPagePan(int page);
void LayoutSearch() {
    auto& L = g_layout;
    float s = L.scale, W = (float)L.width;
    // The width follows the eased amount, so the pill opens into the field
    // rather than changing size in one frame.
    float t = std::clamp(g_searchExpand, 0.f, 1.f);
    t = t * t * (3 - 2 * t);
    float width =
        std::min((44.f + (240.f - 44.f) * t) * s, std::max(1.f, W - 40 * s));
    float top = std::round(std::max(28*s, L.height * 0.045f));
    L.search = {std::round((W - width) / 2), top, std::round((W + width) / 2),
                top + std::round(36*s)};
    L.clearButton = {};
    if (!g_query.empty()) {
        float button = std::round(18*s);
        float cx = L.search.right - 18*s, cy = (L.search.top + L.search.bottom) / 2;
        L.clearButton = {cx-button/2, cy-button/2, cx+button/2, cy+button/2};
    }
    // The resting search icon no longer takes layout room: the grid starts
    // one pill higher and sinks back as the field opens. The eased amount
    // keeps that move continuous while the field grows, and computing it here
    // lets the per-frame expansion callers (OnFrame and the click path)
    // update the grid position without a full relayout.
    L.viewTop = std::round(L.search.top + 28 * s) +
                std::round(std::round(36 * s) * t);
}

void ComputeLayout() {
    Layout& L = g_layout;
    float s = L.scale;
    float W = (float)L.width, H = (float)L.height;

    LayoutSearch();

    L.icon = std::round(g_settings.iconSize * s);
    L.labelHeight = std::round((g_settings.labelSize + 8) * s);  // +1 DIP label size
    L.cellPad = std::round(6 * s);
    L.cellHeight = std::round(L.icon + L.labelHeight + (18+g_settings.verticalGap) * s);
    // viewTop comes from LayoutSearch: the grid rises when the search field
    // rests (no icon), keeping the move tied to the eased expansion.

    float side = std::max(48 * s, W * 0.07f);
    float available = std::max(L.icon * 2, W - 2 * side);
    float minCell = L.icon + g_settings.horizontalGap * s;
    L.columns = g_settings.columns > 0
                    ? std::min(g_settings.columns, std::max(1,(int)(available/(L.icon+8*s))))
                    : std::clamp((int)(available / minCell), 2, 14);
    // With few columns on a wide screen, the icons don't spread out too much.
    L.cellWidth = std::floor(
        std::min(available / L.columns, std::max(minCell, L.icon * 2.6f)));
    L.gridLeft = std::round((W - L.cellWidth * L.columns) / 2);

    // Only whole rows are visible; scrolling moves row by row. Pages keep
    // room below for the dots.
    float bottomRoom = std::round((g_settings.pages ? 44 : 16) * s);

    // The power bar: a pill of round buttons, centered at the bottom, with
    // the grid (and the page dots) above it.
    L.powerItems.clear();
    if (g_settings.powerBar) {
        for (int i : g_settings.powerOrder) {
            auto action = (PowerAction)i;
            if (g_settings.powerShown[i] &&
                (action != PowerAction::Sleep || g_canSleep) &&
                (action != PowerAction::Hibernate || g_canHibernate)) {
                L.powerItems.push_back(i);
            }
        }
    }
    L.powerBar = {};
    if (!L.powerItems.empty()) {
        L.powerButton = std::round(36 * s);
        L.powerGap = std::round(6 * s);
        float pad = std::round(5 * s);
        int n = (int)L.powerItems.size();
        float barWidth = n * L.powerButton + (n - 1) * L.powerGap + 2 * pad;
        float barHeight = L.powerButton + 2 * pad;
        float bottom = H - std::round(std::max(22 * s, H * 0.028f));
        // A DPI scaled margin in from the edge the setting picks.
        float edge = std::round(24 * s);
        float left = g_settings.dockAlign == DockAlign::Left  ? edge
                     : g_settings.dockAlign == DockAlign::Right
                         ? W - barWidth - edge
                         : std::round((W - barWidth) / 2);
        L.powerBar = D2D1::RectF(left, bottom - barHeight, left + barWidth,
                                 bottom);
        bottomRoom = H - L.powerBar.top +
                     std::round((g_settings.pages ? 30 : 10) * s);
    }

    bool grouped=GroupedView();
    if(grouped&&g_wholeCellLayout&&g_groupPositions.empty()&&!g_groupEditMode&&!g_squareGridColumns)
        g_squareGridColumns=DesiredGridColumns();
    const int requestedColumns = grouped ? g_squareGridColumns : DesiredGridColumns();
    int squareColumns = g_groupEditMode ? requestedColumns : FitSquareGridColumns(requestedColumns, grouped);
    L.useSavedGroupGeometry = !grouped || squareColumns == requestedColumns;
    if(squareColumns) {
        L.columns=squareColumns;
        L.cellWidth=L.cellHeight=SquareCellSize(squareColumns);
        L.gridLeft=GridMargin();
    }
    float viewportEnd=H-bottomRoom;
    L.groupPageBottom = std::max(L.viewTop + 1, viewportEnd);
    if(grouped&&squareColumns&&g_groupEditMode)
        viewportEnd=std::min(viewportEnd,GroupEditButtons()[0].top-6*s);
    L.visibleRows=std::max(1,(int)((viewportEnd-L.viewTop)/L.cellHeight));
    L.cellSpare = 0;
    if (g_settings.pages&&!squareColumns) {
        // Pages don't scroll, so the rows spread out to the dots instead of
        // leaving the part of a row that doesn't fit empty at the bottom.
        float rowHeight =
            std::floor((H - bottomRoom - L.viewTop) / L.visibleRows);
        L.cellSpare = std::max(0.0f, rowHeight - L.cellHeight);
        L.cellHeight += L.cellSpare;
    }
    L.viewBottom = squareColumns?std::max(L.viewTop+1,viewportEnd):L.viewTop + L.visibleRows * L.cellHeight;
    L.dotsY = std::round(
        (L.viewBottom + (L.powerItems.empty() ? H : L.powerBar.top)) / 2);

    int count = (int)g_visible.size();
    int rows = (count + L.columns - 1) / L.columns;
    L.contentHeight = rows * L.cellHeight;
    L.perPage = L.visibleRows * L.columns;
    L.pages = std::max(1, (count + L.perPage - 1) / L.perPage);
    if (GroupedView()) ComputeGroups();
    g_scrollTarget = std::clamp(g_scrollTarget, 0.0f, MaxScroll());
    g_scroll = std::clamp(g_scroll, 0.0f, MaxScroll());
    g_groupPagePan=std::clamp(g_groupPagePan,0.f,MaxPagePan(std::max(0,(int)std::lround(g_scroll/std::max(1,L.width)))));
}

struct Cell {
    D2D1_RECT_F highlight, icon, label;
    bool wrapLabel=false;
    D2D1_RECT_F unit{};
};
// A text layout pre-rendered as white text over its shadow and outline, padded
// so the blur has room around the layout box.
struct LabelShadowBitmap {
    ComPtr<ID2D1Bitmap1> bitmap;
    float padX=0, padTop=0, padBottom=0;
    int attempts = 0;
    UINT64 lastAttemptFrame = 0;
};

constexpr int kTextShadowAttempts = 8;
UINT64 g_textShadowFrame = 0;
bool g_textShadowRetryPending = false;
void StartFrames();

bool HasTextEffects(const TextEffects& effects) {
    return effects.shadowOpacity > 0 ||
           (effects.outlineWidth > 0 && effects.outlineOpacity > 0);
}

bool RenderTextWithShadow(IDWriteTextLayout* layout, float width, float height,
                          LabelShadowBitmap& out, float scale,
                          const TextEffects& effects,
                          D2D1_POINT_2F origin = {}) {
    out.bitmap.Reset();
    if (!g_d2dDevice || !layout || width <= 0 || height <= 0) return false;
    // A private device context keeps this legal at any call time: the visible
    // context sits inside its own BeginDraw session while the frame is drawn,
    // and one context cannot nest BeginDraw on itself. Everything here is
    // device-scoped, so the bitmap draws fine from the visible context.
    if (!g_shadowContext && FAILED(g_d2dDevice->CreateDeviceContext(
                    D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &g_shadowContext)))
        return false;
    auto& dc = g_shadowContext;
    const bool hasShadow = effects.shadowOpacity > 0;
    const bool hasOutline = effects.outlineWidth > 0 &&
                            effects.outlineOpacity > 0;
    if (!hasShadow && !hasOutline) return false;
    const float sigma = effects.shadowBlur * scale;
    const float drop = std::round(scale);
    const int outline = std::clamp((int)std::lround(effects.outlineWidth * scale), 1, 49);
    const float pad = std::max(hasShadow ? std::ceil(sigma * 3) : 0.f,
                               hasOutline ? static_cast<float>(outline) : 0.f);
    out.padX = pad;
    out.padTop = pad;
    out.padBottom = pad + (hasShadow ? drop : 0.f);
    D2D1_SIZE_U size{
        (UINT)std::ceil(width + out.padX * 2),
        (UINT)std::ceil(height + out.padTop + out.padBottom)};
    D2D1_BITMAP_PROPERTIES1 props{
        {DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED},
        0, 0, D2D1_BITMAP_OPTIONS_TARGET, nullptr};
    ComPtr<ID2D1Bitmap1> bitmap;
    if (FAILED(dc->CreateBitmap(size, nullptr, 0, props, &bitmap)))
        return false;
    // The white text goes into a command list: it feeds both the shadow
    // effect and the composite on top of it.
    ComPtr<ID2D1CommandList> list;
    if (FAILED(dc->CreateCommandList(&list))) return false;
    ComPtr<ID2D1SolidColorBrush> white;
    if (FAILED(dc->CreateSolidColorBrush(D2D1::ColorF(0xFFFFFF), &white))) return false;
    dc->SetTarget(list.Get());
    dc->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
    dc->BeginDraw();
    dc->DrawTextLayout(origin, layout, white.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);
    HRESULT status = dc->EndDraw();
    dc->SetTarget(nullptr);
    if (FAILED(status) || FAILED(list->Close())) return false;
    ComPtr<ID2D1Effect> shadow, dilate, stroke;
    if (hasShadow) {
        if (FAILED(dc->CreateEffect(CLSID_D2D1Shadow, &shadow))) return false;
        shadow->SetInput(0, list.Get());
        shadow->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, sigma);
        shadow->SetValue(D2D1_SHADOW_PROP_COLOR,
                         D2D1::Vector4F(0, 0, 0, effects.shadowOpacity / 100.f));
    }
    if (hasOutline) {
        if (FAILED(dc->CreateEffect(CLSID_D2D1Morphology, &dilate)) ||
            FAILED(dc->CreateEffect(CLSID_D2D1Shadow, &stroke))) return false;
        dilate->SetInput(0, list.Get());
        dilate->SetValue(D2D1_MORPHOLOGY_PROP_MODE, D2D1_MORPHOLOGY_MODE_DILATE);
        dilate->SetValue(D2D1_MORPHOLOGY_PROP_WIDTH, 2 * outline + 1);
        dilate->SetValue(D2D1_MORPHOLOGY_PROP_HEIGHT, 2 * outline + 1);
        stroke->SetInputEffect(0, dilate.Get());
        stroke->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, 0.f);
        stroke->SetValue(D2D1_SHADOW_PROP_COLOR,
                         D2D1::Vector4F(0, 0, 0, effects.outlineOpacity / 100.f));
    }
    dc->SetTarget(bitmap.Get());
    dc->BeginDraw();
    dc->Clear(D2D1::ColorF(0, 0, 0, 0));
    // Effect outputs extend left and above their input. Translate the
    // context so the text itself stays at the same origin in every layer.
    dc->SetTransform(D2D1::Matrix3x2F::Translation(out.padX, out.padTop));
    if (shadow) {
        dc->SetTransform(D2D1::Matrix3x2F::Translation(out.padX, out.padTop + drop));
        dc->DrawImage(shadow.Get());
        dc->SetTransform(D2D1::Matrix3x2F::Translation(out.padX, out.padTop));
    }
    if (stroke) dc->DrawImage(stroke.Get());
    dc->DrawImage(list.Get());
    dc->SetTransform(D2D1::Matrix3x2F::Identity());
    status = dc->EndDraw();
    dc->SetTarget(nullptr);
    if (FAILED(status)) return false;
    out.bitmap = std::move(bitmap);  // Hand the one reference to the caller.
    return true;
}

void TryBuildTextShadow(IDWriteTextLayout* layout, float width, float height,
                        LabelShadowBitmap& cached, float scale, const TextEffects& effects,
                        D2D1_POINT_2F origin = {}) {
    if (!layout || cached.bitmap || cached.attempts >= kTextShadowAttempts ||
        cached.lastAttemptFrame == g_textShadowFrame ||
        !HasTextEffects(effects) || width <= 0 || height <= 0) return;
    cached.lastAttemptFrame = g_textShadowFrame;
    ++cached.attempts;
    if (!RenderTextWithShadow(layout, width, height, cached, scale, effects, origin) &&
        cached.attempts < kTextShadowAttempts) {
        g_textShadowRetryPending = true;
        StartFrames();
    }
}

struct LauncherText {
    ComPtr<IDWriteTextLayout> layout;
    LabelShadowBitmap shadow;
    D2D1_RECT_F ink{};
    UINT generation = 0;
    bool inkReady = false;
    TextEffects effects{};

    void Draw(D2D1_POINT_2F origin, float opacity, float scale) {
        if (!layout || opacity <= 0) return;
        if (generation != g_targetGeneration) {
            generation = g_targetGeneration;
            shadow = {};
            inkReady = false;
        }
        if (HasTextEffects(effects) && !shadow.bitmap && shadow.attempts < kTextShadowAttempts) {
            if (!inkReady) {
                DWRITE_TEXT_METRICS metrics{};
                DWRITE_OVERHANG_METRICS overhang{};
                if (SUCCEEDED(layout->GetMetrics(&metrics)) && metrics.width > 0 &&
                    SUCCEEDED(layout->GetOverhangMetrics(&overhang))) {
                    // Centered labels and long edits can have a much larger
                    // layout box than the text they actually paint.
                    ink = {std::floor(-overhang.left) - 1, std::floor(-overhang.top) - 1,
                           std::ceil(layout->GetMaxWidth() + overhang.right) + 1,
                           std::ceil(layout->GetMaxHeight() + overhang.bottom) + 1};
                    inkReady = true;
                }
            }
            if (inkReady)
                TryBuildTextShadow(layout.Get(), ink.right - ink.left, ink.bottom - ink.top,
                                   shadow, scale, effects, {-ink.left, -ink.top});
        }
        if (shadow.bitmap) {
            const auto size = shadow.bitmap->GetSize();
            const float left = std::round(origin.x + ink.left) - shadow.padX;
            const float top = std::round(origin.y + ink.top) - shadow.padTop;
            g_target->DrawBitmap(shadow.bitmap.Get(), {left, top, left + size.width, top + size.height},
                                 opacity, D2D1_INTERPOLATION_MODE_LINEAR);
        } else {
            g_brush->SetColor(D2D1::ColorF(0xFFFFFF, opacity));
            g_target->DrawTextLayout(origin, layout.Get(), g_brush.Get(),
                                     D2D1_DRAW_TEXT_OPTIONS_CLIP);
        }
    }
};

enum class LauncherTextAlignment { Format, Leading, LeadingTop };
// 0 = labels, hints and messages; 1 = power dock glyphs with their own shadow.
using LauncherTextKey = std::tuple<std::wstring, IDWriteTextFormat*, float, float, LauncherTextAlignment, int>;
[[clang::no_destroy]] std::map<LauncherTextKey, LauncherText> g_launcherTextCache;

void ClearLauncherTextCache() {
    g_launcherTextCache.clear();
}

LauncherText& GetLauncherText(std::wstring_view text, IDWriteTextFormat* format,
                             float width, float height,
                             LauncherTextAlignment alignment = LauncherTextAlignment::Format,
                             int style = 0) {
    LauncherTextKey key{std::wstring(text), format, width, height, alignment, style};
    auto existing = g_launcherTextCache.find(key);
    if (existing != g_launcherTextCache.end()) return existing->second;
    if (g_launcherTextCache.size() >= 128) ClearLauncherTextCache();
    auto& cached = g_launcherTextCache.try_emplace(std::move(key)).first->second;
    cached.effects = style == 1 ? g_settings.powerEffects : g_settings.textEffects;
    if (g_dwrite && format && width > 0 && height > 0 &&
        SUCCEEDED(g_dwrite->CreateTextLayout(text.empty() ? L"" : text.data(), (UINT32)text.size(), format,
                                             width, height, &cached.layout))) {
        if (alignment != LauncherTextAlignment::Format)
            cached.layout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
        if (alignment == LauncherTextAlignment::LeadingTop)
            cached.layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    }
    return cached;
}

void DrawLauncherText(std::wstring_view text, IDWriteTextFormat* format,
                      const D2D1_RECT_F& box, float opacity = 1.f, int style = 0) {
    GetLauncherText(text, format, box.right - box.left, box.bottom - box.top,
                    LauncherTextAlignment::Format, style)
        .Draw({box.left, box.top}, opacity, g_layout.scale);
}
Cell GroupCellAt(int index);
Cell SpaceTileContents(const Tile& tile, Cell cell);

// The cell whose top left corner is x, y.
Cell MakeCell(float x, float y, float width, float height, float spare) {
    const Layout& L = g_layout;
    Cell c;
    c.unit={x,y,x+width,y+height-spare};
    c.highlight = D2D1::RectF(x + L.cellPad, y, x + width - L.cellPad,
                              y + height - spare - L.cellPad * 2);
    float iconLeft = std::round(x + (width - L.icon) / 2);
    float iconTop = std::round(y + 10 * L.scale);
    c.icon = D2D1::RectF(iconLeft, iconTop, iconLeft + L.icon, iconTop + L.icon);
    float labelTop = c.icon.bottom + std::round(8 * L.scale);
    c.label = D2D1::RectF(x + L.cellPad + 4 * L.scale, labelTop,
                          x + width - L.cellPad - 4 * L.scale,
                          labelTop + L.labelHeight);
    return c;
}

Cell MakeSquareCell(float x,float y,float side) {
    const auto& L=g_layout;
    float pad=std::min(3*L.scale,side*.05f);
    float line=std::min(L.labelHeight,side*.45f);
    float gap=g_settings.showLabels?std::min(6*L.scale,side*.08f):0;
    // Labels stay single-line everywhere: the second row made sparse groups
    // push their icons higher than dense ones, which read as misaligned.
    // Long names keep the character-level ellipsis from the label format.
    bool wrap=false;
    float label=g_settings.showLabels?line:0;
    float icon = std::clamp(side - 2 * pad - label - gap, 0.f, L.icon);
    float top=y+(side-icon-gap-label)/2;
    float left=x+(side-icon)/2;
    return {{x+pad,y+pad,x+side-pad,y+side-pad},{left,top,left+icon,top+icon},
        {x+pad,top+icon+gap,x+side-pad,top+icon+gap+label},wrap,{x,y,x+side,y+side}};
}

Cell MakeCell(float x,float y) {
    return MakeSquareCell(x,y,g_layout.cellWidth);
}

Cell CellAt(int index) {
    if (GroupedView()) return GroupCellAt(index);
    const Layout& L = g_layout;
    float x, y;
    if (g_settings.pages) {
        int page = index / L.perPage, slot = index % L.perPage;
        x = L.gridLeft + (slot % L.columns) * L.cellWidth +
            page * (float)L.width - std::round(g_scroll);
        y = L.viewTop + (slot / L.columns) * L.cellHeight-g_groupPagePan;
    } else {
        int row = index / L.columns, col = index % L.columns;
        x = L.gridLeft + col * L.cellWidth;
        y = L.viewTop + row * L.cellHeight - std::round(g_scroll);
    }
    y += std::round(L.cellSpare / 2);  // Centered in a stretched row.
    return SpaceTileContents(g_visible[index], MakeCell(x, y));
}

#include "launchpad_groups_layout.inc"

////////////////////////////////////////////////////////////////////////////////
// Hover glow
//
// A soft halo in the accent colour behind the icon of the hovered or
// keyboard-selected tile. Blurring is expensive, so one bitmap per icon size
// and spread is built once and reused; each tile only fades its opacity,
// eased over ~150 ms.

void StartFrames();

struct GlowFade {
    const void* key = nullptr;  // The tile's app or folder.
    float alpha = 0;            // 0..1, eased when drawn.
    double last = 0;
    D2D1_POINT_2F light{.5f,.5f};
    D2D1_POINT_2F rimLight{.5f,.5f};
};
constexpr int kGlowSlots = 4;
[[clang::no_destroy]] GlowFade g_glowFades[kGlowSlots];
// A single client-space pointer drives the whole view, like Meapri's
// PointerField (MIT). The edge shading in this renderer is implemented in D2D.
std::optional<D2D1_POINT_2F> g_glassPointer, g_glassScenePointer;
bool g_glassClaimed=false;

bool g_glowAnimating;  // A fade is under way; OnFrame keeps rendering.

// Advances the tile's fade and returns the eased alpha to draw with.
GlowFade* UpdateGlow(const void* key, bool on) {
    GlowFade* slot = nullptr;
    for (auto& s : g_glowFades) {
        if (s.key == key) {
            slot = &s;
            break;
        }
    }
    if (!slot && on) {
        for (auto& s : g_glowFades) {
            if (!s.key) {
                slot = &s;
                break;
            }
        }
        if (!slot) {
            for (auto& s : g_glowFades) {
                if (!slot || s.alpha < slot->alpha) {
                    slot = &s;
                }
            }
        }
        slot->key = key;
        slot->alpha = 0;
        slot->last = Now();
    }
    if (!slot) {
        return nullptr;
    }
    double now = Now();
    float dt = (float)std::clamp(now - slot->last, 0.0, 0.15);
    if (!Animate()) {
        dt = 1.0f;
    }
    slot->last = now;
    slot->alpha = std::clamp(slot->alpha + (on ? dt : -dt) / 0.15f, 0.0f, 1.0f);
    if ((on&&slot->alpha<1)||(!on&&slot->alpha>0)) {
        g_glowAnimating = true;
        StartFrames();
    } else if (!on) {
        slot->key = nullptr;  // Faded out: freed for the next tile.
    }
    return slot;
}

bool InRect(const D2D1_RECT_F& r, float x, float y) {
    return x >= r.left && x < r.right && y >= r.top && y < r.bottom;
}

// Position in the grid under the point (window coordinates), counting the
// places past the last icon, or -1 outside the rows.
int GridIndexAt(int px, int py) {
    if (GroupedView()) return GroupIndexAt(px, py);
    const Layout& L = g_layout;
    float x = (float)px, y = (float)py;
    int page = 0;
    if (g_settings.pages) {
        x += std::round(g_scroll);
        page = (int)std::floor(x / L.width);
        x -= page * (float)L.width;
    }
    if (y < L.viewTop || y >= L.viewBottom || x < L.gridLeft ||
        x >= L.gridLeft + L.cellWidth * L.columns || page < 0) {
        return -1;
    }
    int col = (int)((x - L.gridLeft) / L.cellWidth);
    // CellAt centres the whole row in the stretched page row. The hit row
    // must use the same offset or the pointer is one row / a half-spare off.
    int row = (int)((y - L.viewTop +
                     (g_settings.pages ? g_groupPagePan : std::round(g_scroll)) -
                     std::round(L.cellSpare / 2)) /
                    L.cellHeight);
    if(row<0||(g_settings.pages&&row>=L.visibleRows))return -1;
    return page*L.perPage+row*L.columns+col;
}

// An open folder: a panel over the grid with its name and its apps.
[[clang::no_destroy]] std::shared_ptr<Folder> g_openFolder;
bool g_folderRenaming = false;
[[clang::no_destroy]] std::wstring g_folderOriginalName;
void SaveFolderName() {
    if(g_openFolder&&g_openFolder->name!=g_folderOriginalName&&SaveTiles())
        g_folderOriginalName=g_openFolder->name;
}
bool g_folderTitleHover = false;
// The folder fading out after a close, and the open/close progress (0 to 1).
[[clang::no_destroy]] std::shared_ptr<Folder> g_closingFolder;
float g_folderAnim = 1;
int g_folderRow;  // First row shown, when not all of them fit.

struct FolderLayout {
    D2D1_RECT_F panel, title;
    float left, top, cellHeight;  // Of the icons.
    int columns, rows, visibleRows;
};

FolderLayout GetFolderLayout() {
    const Layout& L = g_layout;
    float s = L.scale;
    FolderLayout F{};
    int count = g_openFolder ? (int)g_openFolder->apps.size() : 0;
    F.columns = std::min(L.columns, std::max(3, std::min(count, 6)));
    F.rows = std::max(1, (count + F.columns - 1) / F.columns);
    F.cellHeight = L.cellHeight - L.cellSpare;
    float pad = std::round(20 * s), titleHeight = std::round(56 * s);
    int fit = std::max(1, (int)((L.viewBottom - L.viewTop - titleHeight - pad) /
                                F.cellHeight));
    F.visibleRows = std::min(F.rows, fit);
    float width = F.columns * L.cellWidth + 2 * pad;
    float height = titleHeight + F.visibleRows * F.cellHeight + pad;
    float left = std::round((L.width - width) / 2);
    float top = std::round(
        std::max(L.viewTop, (L.viewTop + L.viewBottom - height) / 2));
    F.panel = D2D1::RectF(left, top, left + width, top + height);
    F.title = D2D1::RectF(left + pad, top + pad / 2, left + width - pad,
                          top + titleHeight);
    F.left = left + pad;
    F.top = top + titleHeight;
    return F;
}

Cell FolderCellAt(int index) {
    FolderLayout F = GetFolderLayout();
    return SpaceTileContents(g_openFolder->apps[index],
        MakeCell(F.left + (index % F.columns) * g_layout.cellWidth,
                 F.top + (index / F.columns - g_folderRow) * F.cellHeight));
}

int FolderIndexAt(int px, int py) {
    FolderLayout F = GetFolderLayout();
    float x = px - F.left, y = py - F.top;
    if (x < 0 || x >= F.columns * g_layout.cellWidth || y < 0 ||
        y >= F.visibleRows * F.cellHeight) {
        return -1;
    }
    return ((int)(y / F.cellHeight) + g_folderRow) * F.columns +
           (int)(x / g_layout.cellWidth);
}

// What takes the pointer and the keys: the open folder, or the grid.
const Tiles& Items() {
    return g_openFolder ? g_openFolder->apps : g_visible;
}

int Columns() {
    return g_openFolder ? GetFolderLayout().columns : g_layout.columns;
}

Cell ItemCell(int index) {
    return g_openFolder ? FolderCellAt(index) : CellAt(index);
}

float g_sceneZoom = 1.f;       // Grid scene -> window transform used by the last frame.
float g_sceneOffsetY = 0.f;
extern bool g_shown;

Cell VisualItemCell(int index);

// Position in Items() of the icon under the point, or -1.
int ItemAt(int x, int y) {
    // The grid is drawn through the opening zoom / search-transition
    // transform. Hit-testing must use the same scene coordinates; otherwise
    // every visible tile is offset from the pointer for the whole transition.
    if (g_sceneZoom != 1.f || g_sceneOffsetY != 0.f) {
        const float cx = g_layout.width * .5f, cy = g_layout.height * .5f;
        const float sx = cx + (x - cx) / g_sceneZoom;
        const float sy = cy + (y - g_sceneOffsetY - cy) / g_sceneZoom;
        x = (int)std::lround(sx); y = (int)std::lround(sy);
    }
    int index = g_openFolder ? FolderIndexAt(x, y) : GridIndexAt(x, y);
    if (index < 0 || index >= (int)Items().size()) return -1;
    if (!g_shown || (!FlyInActive() && g_groupTransitions.empty() && !g_groupGesture.moved)) {
        if (!InRect(ItemCell(index).highlight, (float)x, (float)y)) return -1;
        return index;
    }
    // During fly-in / group motion, each tile is drawn at a different visual
    // position. Hit the visual cell, in reverse paint order, rather than the
    // final grid slot. This keeps click and hover on the tile under the pointer.
    for (int i = (int)Items().size() - 1; i >= 0; --i) {
        auto cell = VisualItemCell(i);
        if (InRect(cell.highlight, (float)x, (float)y)) return i;
    }
    return -1;
}

// While dragging: the place under the point (the last one when past the
// end), or -1.
int SlotAt(int x, int y) {
    int index = g_openFolder ? FolderIndexAt(x, y) : GridIndexAt(x, y);
    return index < 0 ? -1 : std::min(index, (int)Items().size() - 1);
}

// The page dot under the point, or -1. The click area is larger than the dot.
int PageDotAt(int px, int py) {
    const Layout& L = g_layout;
    if (!g_settings.pages || L.pages < 2) {
        return -1;
    }
    float step = std::min(20 * L.scale, std::max(2.0f, (L.width - 48 * L.scale) / L.pages));
    float left = L.width / 2.0f - L.pages * step / 2;
    float x = (float)px, y = (float)py;
    if (std::abs(y - L.dotsY) > 14 * L.scale || x < left ||
        x >= left + L.pages * step) {
        return -1;
    }
    return (int)((x - left) / step);
}

////////////////////////////////////////////////////////////////////////////////
// Screen state and drawing

bool g_shown;        // The windows are on screen (opening, open or closing).
bool g_closing;
float g_appear;      // 0 closed, 1 open.
double g_appearVelocity;
float g_wiggle;      // Edit mode envelope: ramps to 1 while an icon is held.
double g_lastFrame;
HANDLE g_frameWake = nullptr;
double g_refreshHz = 60, g_nextFrameDeadline = 0;

void UpdateFrameRate(HMONITOR monitor) {
    MONITORINFOEXW info{}; info.cbSize = sizeof(info);
    DEVMODEW mode{}; mode.dmSize = sizeof(mode);
    g_refreshHz = 60;
    if (GetMonitorInfoW(monitor, &info) && EnumDisplaySettingsW(info.szDevice, ENUM_CURRENT_SETTINGS, &mode) &&
        mode.dmDisplayFrequency >= 24) g_refreshHz = std::clamp((double)mode.dmDisplayFrequency, 24.0, 1000.0);
    // CCD provides fractional refresh rates (for example 60000/1001) that
    // EnumDisplaySettings rounds to an integer. This only queries the display.
    for (int attempt=0; attempt<3; attempt++) {
        UINT32 pathCount=0,modeCount=0;
        if (GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS,&pathCount,&modeCount)!=ERROR_SUCCESS) break;
        std::vector<DISPLAYCONFIG_PATH_INFO> paths(pathCount);
        std::vector<DISPLAYCONFIG_MODE_INFO> modes(modeCount);
        LONG result=QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS,&pathCount,paths.data(),&modeCount,modes.data(),nullptr);
        if (result==ERROR_INSUFFICIENT_BUFFER) continue;
        if (result!=ERROR_SUCCESS) break;
        for (UINT32 i=0;i<pathCount;i++) {
            DISPLAYCONFIG_SOURCE_DEVICE_NAME source{};
            source.header.type=DISPLAYCONFIG_DEVICE_INFO_GET_SOURCE_NAME;
            source.header.size=sizeof(source); source.header.adapterId=paths[i].sourceInfo.adapterId; source.header.id=paths[i].sourceInfo.id;
            auto rate=paths[i].targetInfo.refreshRate;
            if (DisplayConfigGetDeviceInfo(&source.header)==ERROR_SUCCESS && wcscmp(source.viewGdiDeviceName,info.szDevice)==0 && rate.Denominator) {
                double hz=(double)rate.Numerator/rate.Denominator;
                if (hz>=24 && hz<=1000) g_refreshHz=hz;
                break;
            }
        }
        break;
    }
    if (g_settings.frameRateLimit > 0) g_refreshHz = std::min(g_refreshHz, (double)g_settings.frameRateLimit);
}

void ArmNextFrame(bool reset) {
    if (!g_frameWake) return;
    double now = Now(), period = 1.0 / g_refreshHz;
    g_nextFrameDeadline = reset ? now + period : g_nextFrameDeadline + period;
    if (g_nextFrameDeadline <= now) g_nextFrameDeadline = now + period;
    LARGE_INTEGER due;
    due.QuadPart = -(LONGLONG)std::max(1.0, std::ceil((g_nextFrameDeadline - now) * 10000000.0));
    SetWaitableTimer(g_frameWake, &due, 0, nullptr, nullptr, FALSE);
}
double g_lastClosed;  // When it closed by losing focus.
// One keystroke can reach this window twice: the keyboard hook reports the tap
// and the launcher's own show request follows it. The two are one toggle, so
// whichever lands first acts and the other is dropped inside this window.
double g_keyToggleAt = -1000;
bool g_frameTimer;
int g_hover = -1, g_selected = -1, g_pressed = -1;
bool g_pressedEmpty;
bool g_trackingMouse;
bool g_searchHover;   // The pointer rests on the search bar.
bool g_clearHover;    // ... on the clear (x) button.
float g_searchGlow;   // 0..1, the bar's eased accent glow.

// Dragging an icon: it keeps a place in the grid or folder, drawn empty, and
// moves to the place the pointer rests on.
struct Drag {
    bool pending = false;  // Pressed on an icon; a drag once it moves.
    bool active = false;
    bool moved = false;
    POINT start{}, at{};
    float iconSize = 0;
    double pressTime = 0;  // When the press started, for the hold time.
    D2D1_POINT_2F grab{};  // Pointer minus the icon's corner.
    Tile tile;
    Tiles tiles;
    int target = -1;  // The icon to drop it into (makes or fills a folder).
    int slot = -1;    // The place under the pointer.
    int edge = 0;     // -1 or 1 while resting at an edge: flips or scrolls.
    bool inPanel = false;  // Has been inside the open folder.
    double targetTime = 0, slotTime = 0, edgeTime = 0;
    std::vector<std::wstring> backup;  // The arrangement before, for Esc.
};
[[clang::no_destroy]] Drag g_drag;
using AppSelection = std::vector<std::shared_ptr<App>>;
[[clang::no_destroy]] AppSelection g_appSelection;
[[clang::no_destroy]] std::unordered_set<std::wstring> g_selectedAppKeys;
struct SelectionPointer {
    AppSelection base;
    std::unordered_set<std::wstring> baseKeys;
    std::shared_ptr<App> app;
    POINT start{}, end{};
    bool pending=false, active=false;
};
[[clang::no_destroy]] SelectionPointer g_selectionPointer;
float g_batchBlend=0;
int g_batchPressed=-1,g_batchHover=-1;
size_t g_batchDisplayCount=0;
bool g_batchLaunching=false;

auto FindSelectedApp(const AppSelection& selection,const App& app) {
    return std::find_if(selection.begin(),selection.end(),[&](const auto& item){
        return item.get()==&app||AppKey(*item)==AppKey(app);
    });
}
bool AppSelected(const App& app) {
    return !g_selectedAppKeys.empty()&&g_selectedAppKeys.contains(AppKey(app));
}
void SelectionChanged() {
    g_selectedAppKeys.clear();
    for(const auto& app:g_appSelection)g_selectedAppKeys.insert(AppKey(*app));
    if(!g_appSelection.empty())g_batchDisplayCount=g_appSelection.size();
    if(!Animate())g_batchBlend=g_appSelection.empty()?0.f:1.f;
    StartFrames();
}
void ClearAppSelection() {
    g_selectionPointer={};
    g_batchPressed=g_batchHover=-1;
    if(g_appSelection.empty())return;
    g_appSelection.clear();
    SelectionChanged();
}
void ToggleAppSelection(const std::shared_ptr<App>& app) {
    auto found=FindSelectedApp(g_appSelection,*app);
    if(found==g_appSelection.end())g_appSelection.push_back(app);
    else g_appSelection.erase(found);
    SelectionChanged();
}
D2D1_RECT_F SelectionRect() {
    const auto& p=g_selectionPointer;
    return {(float)std::min(p.start.x,p.end.x),(float)std::min(p.start.y,p.end.y),
            (float)std::max(p.start.x,p.end.x),(float)std::max(p.start.y,p.end.y)};
}
D2D1_RECT_F SelectionViewport() {
    if(!g_openFolder)return {0,g_layout.viewTop,(float)g_layout.width,g_layout.viewBottom};
    auto f=GetFolderLayout();
    return {f.left,f.top,f.panel.right,f.top+f.visibleRows*f.cellHeight};
}
D2D1_RECT_F SelectionIntersection(const D2D1_RECT_F& a,const D2D1_RECT_F& b) {
    return {std::max(a.left,b.left),std::max(a.top,b.top),
            std::min(a.right,b.right),std::min(a.bottom,b.bottom)};
}
bool SelectionHasArea(const D2D1_RECT_F& r) {
    return r.right>r.left&&r.bottom>r.top;
}
bool MoveAppSelection(int x,int y) {
    auto& pointer=g_selectionPointer;
    if(!pointer.pending)return false;
    pointer.end={x,y};
    if(!pointer.active&&std::abs(x-pointer.start.x)<GetSystemMetrics(SM_CXDRAG)&&
       std::abs(y-pointer.start.y)<GetSystemMetrics(SM_CYDRAG))return true;
    pointer.active=true;
    auto rect=SelectionIntersection(SelectionRect(),SelectionViewport());
    auto selection=pointer.base;
    if(SelectionHasArea(rect)) {
        auto keys=pointer.baseKeys;
        const auto& items=Items();
        for(int i=0;i<(int)items.size();i++) {
            if(!items[i].app||!SelectionHasArea(SelectionIntersection(rect,ItemCell(i).highlight)))continue;
            if(keys.insert(AppKey(*items[i].app)).second)selection.push_back(items[i].app);
        }
    }
    if(selection!=g_appSelection) {
        g_appSelection=std::move(selection);
        SelectionChanged();
    }
    StartFrames();return true;
}

bool g_modal;  // A dialog of the grid is open: losing the focus is expected.

#ifndef SEAMLESS_PREVIEW
void ResetTaskbarStartButtons(bool ownRequest = false);
void ClearStartButtonLatch(const wchar_t* why);  // launchpad_start_xaml.h
#endif

////////////////////////////////////////////////////////////////////////////////
// Popup menus
//
// Every menu the grid opens is drawn by the mod instead of the system's #32768
// menu class: a topmost layered window with rounded corners and one surface
// color for all levels, sampled from the background of the full page.
// HMENU stays the command source
// (every caller passes TPM_RETURNCMD), so TrackLaunchpadPopupMenu is the only
// entry point that had to change.

namespace LaunchpadMenu {

constexpr WCHAR kMenuClass[] = L"RYJSLaunchpadMenu";

// A window class outlives the image that registered it. Reusing one left by
// an earlier build dispatches into code that is no longer mapped, while
// refusing to register at all removed every menu right after an in-place
// update. Each loaded image claims a class name of its own instead, so a new
// build never inherits the old window procedure.
const WCHAR* MenuClassName() {
    static WCHAR name[80]{};
    if (!name[0]) {
        swprintf_s(name, L"%ls_%p_%llu", kMenuClass, (void*)&__ImageBase,
                   (unsigned long long)GetTickCount64());
        name[ARRAYSIZE(name) - 1] = 0;
    }
    return name;
}
constexpr UINT kAnimationTimer = 1;
constexpr UINT kSubmenuTimer = 2;
constexpr UINT kSubmenuCloseTimer = 3;
constexpr int kAnimationMs = 120;
constexpr int kSubmenuDelayMs = 140;
// The only path from a row to its submenu is across the row itself, so the
// pointer must not lose the submenu while it is still there. Leaving the row
// for another one is a different story, but a hand rarely moves in a straight
// line across a 32 DIP row: the submenu gets this much grace before it goes,
// and returning to the row (or reaching the submenu) cancels the close.
constexpr int kSubmenuCloseDelayMs = 320;

// One row of a menu. Built from the HMENU the caller filled in, so the command
// ids, the disabled states and the submenus all stay exactly as they were.
struct Node {
    std::wstring text;
    std::wstring hint;  // The part after a tab, drawn right aligned.
    UINT id = 0;
    bool separator = false;
    bool enabled = true;
    bool checked = false;
    bool radio = false;
    bool bold = false;  // The default item, hit by Enter.
    WCHAR mnemonic = 0;
    std::vector<Node> sub;
    LauncherText label;
    LauncherText shortcut;
};

// One open level. Everything is in screen coordinates; the window paints with
// a translation so a panel can be hit tested against the pointer directly.
struct Panel {
    std::vector<Node>* items = nullptr;
    D2D1_RECT_F rect{};
    float width = 0;
    float contentHeight = 0;
    float scroll = 0;
    int hover = -1;
    int pressed = -1;
    int parentItem = -1;
    float checkWidth = 0, arrowWidth = 0, hintWidth = 0;
};

struct State {
    std::vector<Node> root;
    std::vector<Panel> panels;
    HWND hwnd = nullptr;
    RECT work{};
    POINT origin{};
    int width = 0, height = 0;
    float scale = 1;
    float rootTop = 0;
    float widestChild = 0, tallestChild = 0;
    bool running = false;
    UINT result = 0;
    int active = 0;
    bool ignoreFirstMove = true;
    int wheel = 0;
    int pressedPanel = -1, pressedItem = -1;
    int pendingPanel = -1, pendingItem = -1;
    double appearStart = 0;
    UINT generation = 0;
    ComPtr<IDWriteTextFormat> textFormat, boldFormat, hintFormat;
    ComPtr<ID2D1Bitmap1> frame;
    D2D1_COLOR_F background{};
    ComPtr<ID3D11Texture2D> staging;
    Surface surface;
};
// One menu at a time, and freed by Reset instead of at exit.
[[clang::no_destroy]] State g_menu;

float PadY(float scale) { return 6 * scale; }
float RowHeight(const Node& node, float scale) {
    return node.separator ? 9 * scale : 32 * scale;
}

bool PointInRect(const D2D1_RECT_F& r, float x, float y) {
    return x >= r.left && x < r.right && y >= r.top && y < r.bottom;
}

// ----------------------------------------------------------------------------
// Reading the HMENU

std::wstring MenuItemText(HMENU menu, int index) {
    WCHAR buffer[512]{};
    int length = GetMenuStringW(menu, index, buffer, ARRAYSIZE(buffer),
                                MF_BYPOSITION);
    if (length >= 0 && length < (int)ARRAYSIZE(buffer) - 1) return buffer;
    if (length < 0) return {};
    std::wstring text((size_t)length + 1, L'\0');
    GetMenuStringW(menu, index, text.data(), (int)text.size(), MF_BYPOSITION);
    text.resize(wcslen(text.c_str()));
    return text;
}

// "&&" is a literal ampersand and a single "&" marks the keyboard mnemonic;
// neither is drawn. A tab separates the shortcut column.
std::wstring CleanMenuItem(const std::wstring& raw, WCHAR& mnemonic,
                           std::wstring& hint) {
    std::wstring text = raw;
    size_t tab = text.find(L'\t');
    if (tab != std::wstring::npos) {
        hint = text.substr(tab + 1);
        text.erase(tab);
    }
    std::wstring out;
    out.reserve(text.size());
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] != L'&') {
            out += text[i];
            continue;
        }
        if (i + 1 < text.size() && text[i + 1] == L'&') {
            out += L'&';
            i++;
            continue;
        }
        if (i + 1 < text.size()) {
            if (!mnemonic) {
                WCHAR c = text[i + 1];
                mnemonic =
                    c >= L'a' && c <= L'z' ? (WCHAR)(c - L'a' + L'A') : c;
            }
            i++;
        }
    }
    return out;
}

bool ReadMenu(HMENU menu, std::vector<Node>& nodes) {
    int count = GetMenuItemCount(menu);
    if (count <= 0) return false;
    nodes.clear();
    nodes.reserve((size_t)count);
    for (int i = 0; i < count; i++) {
        MENUITEMINFOW info{};
        info.cbSize = sizeof(info);
        info.fMask = MIIM_FTYPE | MIIM_STATE | MIIM_ID | MIIM_SUBMENU;
        if (!GetMenuItemInfoW(menu, (UINT)i, TRUE, &info)) continue;
        Node node;
        node.id = info.wID;
        node.separator = (info.fType & MFT_SEPARATOR) != 0;
        node.enabled = !(info.fState & (MFS_DISABLED | MFS_GRAYED));
        node.checked = (info.fState & MFS_CHECKED) != 0;
        node.radio = (info.fType & MFT_RADIOCHECK) != 0;
        node.bold = (info.fState & MFS_DEFAULT) != 0;
        if (!node.separator) {
            node.text =
                CleanMenuItem(MenuItemText(menu, i), node.mnemonic, node.hint);
        }
        if (info.hSubMenu) ReadMenu(info.hSubMenu, node.sub);
        nodes.push_back(std::move(node));
    }
    return !nodes.empty();
}

// ----------------------------------------------------------------------------
// Measuring

void EnsureFormats(State& m, float scale) {
    if (m.textFormat && m.scale == scale) return;
    m.scale = scale;
    auto make = [&](float size, DWRITE_FONT_WEIGHT weight,
                    ComPtr<IDWriteTextFormat>& format) {
        format.Reset();
        g_dwrite->CreateTextFormat(g_settings.font.c_str(), nullptr, weight,
                                   DWRITE_FONT_STYLE_NORMAL,
                                   DWRITE_FONT_STRETCH_NORMAL, size * scale,
                                   L"", &format);
        if (!format) return;
        format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        format->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
        DWRITE_TRIMMING trimming{DWRITE_TRIMMING_GRANULARITY_CHARACTER, 0, 0};
        ComPtr<IDWriteInlineObject> ellipsis;
        g_dwrite->CreateEllipsisTrimmingSign(format.Get(), &ellipsis);
        format->SetTrimming(&trimming, ellipsis.Get());
    };
    make(14.0f, DWRITE_FONT_WEIGHT_NORMAL, m.textFormat);
    make(14.0f, DWRITE_FONT_WEIGHT_SEMI_BOLD, m.boldFormat);
    make(12.5f, DWRITE_FONT_WEIGHT_NORMAL, m.hintFormat);
    if (m.hintFormat) m.hintFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
}

ComPtr<IDWriteTextLayout> MakeLayout(IDWriteTextFormat* format,
                                     const std::wstring& text, float maxWidth,
                                     float maxHeight) {
    ComPtr<IDWriteTextLayout> layout;
    if (!g_dwrite || !format || text.empty()) return layout;
    g_dwrite->CreateTextLayout(text.c_str(), (UINT32)text.size(), format,
                               maxWidth, maxHeight, &layout);
    return layout;
}

void MeasurePanel(State& m, Panel& panel) {
    const float s = m.scale;
    const float rowHeight = 32 * s;
    const float maxText = 380 * s;
    float widest = 0, widestHint = 0;
    bool submenu = false, checks = false;
    const auto& effects = g_settings.textEffects;
    for (auto& node : *panel.items) {
        if (node.separator) continue;
        if (!node.sub.empty()) submenu = true;
        if (node.checked) checks = true;
        if (node.text.empty()) continue;
        if (!node.label.layout || !SameTextEffects(node.label.effects, effects)) {
            node.label = {};
            node.label.effects = effects;
            node.label.layout = MakeLayout(node.bold && m.boldFormat ? m.boldFormat.Get()
                                                                    : m.textFormat.Get(),
                                           node.text, maxText, rowHeight);
        }
        if (node.label.layout) {
            DWRITE_TEXT_METRICS metrics{};
            if (SUCCEEDED(node.label.layout->GetMetrics(&metrics)))
                widest = std::max(widest, metrics.width);
        }
        if (!node.hint.empty()) {
            if (!node.shortcut.layout || !SameTextEffects(node.shortcut.effects, effects)) {
                node.shortcut = {};
                node.shortcut.effects = effects;
                node.shortcut.layout = MakeLayout(m.hintFormat.Get(), node.hint,
                                                  180 * s, rowHeight);
            }
            if (node.shortcut.layout) {
                DWRITE_TEXT_METRICS metrics{};
                if (SUCCEEDED(node.shortcut.layout->GetMetrics(&metrics)))
                    widestHint = std::max(widestHint, metrics.width);
            }
        }
    }
    panel.checkWidth = checks ? 18 * s : 0;
    panel.arrowWidth = submenu ? 16 * s : 0;
    panel.hintWidth = widestHint > 0 ? widestHint + 18 * s : 0;
    float frame = 14 * s + panel.checkWidth + panel.hintWidth +
                  panel.arrowWidth + 14 * s;
    panel.width = std::max(172 * s, frame + widest);
    float height = 2 * PadY(s);
    for (const auto& node : *panel.items) height += RowHeight(node, s);
    panel.contentHeight = height;
}

// Every level is measured up front: the window is one fixed size for the life
// of the menu, so a submenu never has to resize it (which would need a fresh
// screenshot).
void MeasureChildren(State& m, std::vector<Node>& nodes) {
    for (auto& node : nodes) {
        if (node.sub.empty()) continue;
        Panel panel;
        panel.items = &node.sub;
        MeasurePanel(m, panel);
        m.widestChild = std::max(m.widestChild, panel.width);
        m.tallestChild = std::max(m.tallestChild, panel.contentHeight);
        MeasureChildren(m, node.sub);
    }
}

// ----------------------------------------------------------------------------
// Placement

void PlaceRoot(State& m, POINT point, UINT flags) {
    Panel& panel = m.panels[0];
    const float s = m.scale;
    const float margin = 4 * s;
    const float workWidth = (float)(m.work.right - m.work.left);
    const float workHeight = (float)(m.work.bottom - m.work.top);
    float w = std::min(panel.width, workWidth - margin * 2);
    float h = std::min(panel.contentHeight, workHeight - margin * 2);
    float x = (float)point.x, y = (float)point.y;
    if (flags & TPM_RIGHTALIGN) x -= w;
    else if (flags & TPM_CENTERALIGN) x -= w * 0.5f;
    if (flags & TPM_BOTTOMALIGN) y -= h;
    else if (flags & TPM_VCENTERALIGN) y -= h * 0.5f;
    float left = (float)m.work.left, top = (float)m.work.top;
    float maxX = (float)m.work.right - margin - w;
    float maxY = (float)m.work.bottom - margin - h;
    if (maxX < left) maxX = left;
    if (maxY < top) maxY = top;
    x = std::clamp(x, left + margin, maxX);
    y = std::clamp(y, top + margin, maxY);
    panel.rect = D2D1::RectF(x, y, x + w, y + h);
    m.rootTop = y;
}

void OpenSubmenu(int level, int item);

// ----------------------------------------------------------------------------
// Drawing

void DrawItem(State& m, Panel& panel, int index, Node& node, float top,
              float height, float left, float right) {
    const float s = m.scale;
    if (node.separator) {
        float y = std::round(top + height * 0.5f) + 0.5f;
        g_brush->SetColor(D2D1::ColorF(0xFFFFFF, 0.16f));
        g_target->DrawLine(D2D1::Point2F(left + 8 * s, y),
                           D2D1::Point2F(right - 8 * s, y), g_brush.Get(), 1);
        return;
    }
    const float radius = 8 * s;
    D2D1_RECT_F pill =
        D2D1::RectF(left, top + 1 * s, right, top + height - 1 * s);
    if (panel.hover == index) {
        // Same white wash the dock and the search box use for hover, floored so
        // the row always reads as selected even with the glow turned down.
        float glow = std::clamp(g_settings.glowStrength, 0.f, 1.f);
        Fill(pill, D2D1::ColorF(0xFFFFFF, 0.08f + 0.08f * glow), radius);
    }
    float textLeft = left + 10 * s + panel.checkWidth;
    if (panel.hintWidth > 0 && node.shortcut.layout) {
        float hintRight = right - 8 * s - panel.arrowWidth;
        node.shortcut.Draw({hintRight - node.shortcut.layout->GetMaxWidth(), top},
                           node.enabled ? .9f : .5f, s);
    }
    node.label.Draw({textLeft, top}, node.enabled ? 1.f : .5f, s);
    if (panel.checkWidth > 0 && node.checked) {
        float cx = left + 10 * s + panel.checkWidth * 0.5f;
        float cy = top + height * 0.5f;
        g_brush->SetColor(D2D1::ColorF(0xFFFFFF, node.enabled ? 0.92f : 0.32f));
        if (node.radio) {
            g_target->FillEllipse(
                D2D1::Ellipse(D2D1::Point2F(cx, cy), 3.2f * s, 3.2f * s),
                g_brush.Get());
        } else {
            g_target->DrawLine(D2D1::Point2F(cx - 4 * s, cy),
                               D2D1::Point2F(cx - 1 * s, cy + 3.2f * s),
                               g_brush.Get(), 1.6f * s);
            g_target->DrawLine(D2D1::Point2F(cx - 1 * s, cy + 3.2f * s),
                               D2D1::Point2F(cx + 4.2f * s, cy - 3.4f * s),
                               g_brush.Get(), 1.6f * s);
        }
    }
    if (!node.sub.empty()) {
        float cx = right - 8 * s - panel.arrowWidth * 0.5f;
        float cy = top + height * 0.5f;
        float arm = 3.6f * s;
        g_brush->SetColor(D2D1::ColorF(0xFFFFFF, node.enabled ? 0.70f : 0.30f));
        g_target->DrawLine(D2D1::Point2F(cx - arm * 0.5f, cy - arm),
                           D2D1::Point2F(cx + arm * 0.7f, cy), g_brush.Get(),
                           1.4f * s);
        g_target->DrawLine(D2D1::Point2F(cx + arm * 0.7f, cy),
                           D2D1::Point2F(cx - arm * 0.5f, cy + arm),
                           g_brush.Get(), 1.4f * s);
    }
}

void DrawPanelSurface(const D2D1_RECT_F& r, float radius, float s, D2D1_COLOR_F background) {
    const float shift = 2 * s;
    Fill(D2D1::RectF(r.left, r.top + shift, r.right, r.bottom + shift),
         D2D1::ColorF(0, 0.14f), radius);
    Fill(r, background, radius);
    g_brush->SetColor(D2D1::ColorF(0xFFFFFF, 0.10f + 0.08f * g_settings.chromeOpacity));
    g_target->DrawRoundedRectangle(
        D2D1::RoundedRect(
            D2D1::RectF(r.left + .5f, r.top + .5f, r.right - .5f, r.bottom - .5f),
            radius - .5f, radius - .5f),
        g_brush.Get(), 1);
}

void DrawPanel(State& m, Panel& panel) {
    const float s = m.scale;
    const D2D1_RECT_F r = panel.rect;
    DrawPanelSurface(r, 10 * s, s, m.background);
    const float padY = PadY(s);
    g_target->PushAxisAlignedClip(
        D2D1::RectF(r.left, r.top + padY, r.right, r.bottom - padY),
        D2D1_ANTIALIAS_MODE_ALIASED);
    float top = r.top + padY - panel.scroll;
    float pillLeft = r.left + 6 * s, pillRight = r.right - 6 * s;
    for (int i = 0; i < (int)panel.items->size(); i++) {
        Node& node = (*panel.items)[i];
        float height = RowHeight(node, s);
        if (top + height > r.top + padY && top < r.bottom - padY) {
            DrawItem(m, panel, i, node, top, height, pillLeft, pillRight);
        }
        top += height;
    }
    g_target->PopAxisAlignedClip();
}

std::optional<D2D1_COLOR_F> SampleMonitorColor(const RECT& work) {
    constexpr int side = 32;
    BITMAPINFO info{};
    info.bmiHeader.biSize = sizeof(info.bmiHeader);
    info.bmiHeader.biWidth = side;
    info.bmiHeader.biHeight = -side;
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biBitCount = 32;
    info.bmiHeader.biCompression = BI_RGB;
    void* bits = nullptr;
    HDC screen = GetDC(nullptr);
    if (!screen) return std::nullopt;
    HDC memory = CreateCompatibleDC(screen);
    HBITMAP bitmap = memory ? CreateDIBSection(memory, &info, DIB_RGB_COLORS, &bits, nullptr, 0) : nullptr;
    std::optional<D2D1_COLOR_F> color;
    if (bitmap) {
        HGDIOBJ old = SelectObject(memory, bitmap);
        SetStretchBltMode(memory, HALFTONE);
        SetBrushOrgEx(memory, 0, 0, nullptr);
        if (StretchBlt(memory, 0, 0, side, side, screen, work.left, work.top,
                       work.right - work.left, work.bottom - work.top, SRCCOPY)) {
            GdiFlush();
            color = AverageBackdropColor(static_cast<const UINT32*>(bits), side, side);
        }
        SelectObject(memory, old);
        DeleteObject(bitmap);
    }
    if (memory) DeleteDC(memory);
    ReleaseDC(nullptr, screen);
    return color;
}

D2D1_COLOR_F PanelBackground(std::optional<D2D1_COLOR_F> sample) {
    auto color = sample.value_or(g_settings.tint);
    const auto tint = g_settings.tint;
    const float amount = g_settings.opacity * tint.a;
    color.r = std::lerp(color.r, tint.r, amount);
    color.g = std::lerp(color.g, tint.g, amount);
    color.b = std::lerp(color.b, tint.b, amount);
    // Keep the sampled hue and reserve sufficient contrast for white text,
    // including the brighter hover surface.
    const float brightest = std::max({color.r, color.g, color.b, .01f});
    const float shade = std::min(.86f * (1 - g_settings.backgroundDim), .29f / brightest);
    return D2D1::ColorF(.055f + color.r * shade, .055f + color.g * shade, .055f + color.b * shade);
}

D2D1_COLOR_F MenuBackground(const State& m) {
    // Sample from before the launcher so its contents cannot feed back into the tint.
    return PanelBackground(g_shown ? g_material.average : SampleMonitorColor(m.work));
}

bool EnsureMenuFrame(State& m) {
    if (m.frame) return true;
    D2D1_BITMAP_PROPERTIES1 props = D2D1::BitmapProperties1(
        D2D1_BITMAP_OPTIONS_TARGET,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
                          D2D1_ALPHA_MODE_PREMULTIPLIED),
        96, 96);
    if (FAILED(g_target->CreateBitmap(D2D1::SizeU(m.width, m.height), nullptr,
                                      0, props, &m.frame))) {
        return false;
    }
    if (!EnsureSurface(m.surface, m.width, m.height)) {
        m.frame.Reset();
        return false;
    }
    m.staging.Reset();
    return true;
}

bool CopyToSurface(State& m) {
    if (!m.frame || !m.surface.bitmap || !g_d3d) return false;
    if (!m.staging) {
        D3D11_TEXTURE2D_DESC desc{};
        desc.Width = m.width;
        desc.Height = m.height;
        desc.MipLevels = desc.ArraySize = desc.SampleDesc.Count = 1;
        desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        desc.Usage = D3D11_USAGE_STAGING;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        if (FAILED(g_d3d->CreateTexture2D(&desc, nullptr, &m.staging)))
            return false;
    }
    DIBSECTION ds{};
    if (GetObjectW(m.surface.bitmap, sizeof(ds), &ds) != sizeof(ds) ||
        !ds.dsBm.bmBits) {
        return false;
    }
    ComPtr<IDXGISurface> surface;
    ComPtr<ID3D11Texture2D> texture;
    if (FAILED(m.frame->GetSurface(&surface)) ||
        FAILED(surface.As(&texture))) {
        return false;
    }
    ComPtr<ID3D11DeviceContext> ctx;
    g_d3d->GetImmediateContext(&ctx);
    ctx->CopyResource(m.staging.Get(), texture.Get());
    D3D11_MAPPED_SUBRESOURCE mapped{};
    if (FAILED(ctx->Map(m.staging.Get(), 0, D3D11_MAP_READ, 0, &mapped)))
        return false;
    size_t span = (size_t)m.width * 4;
    const BYTE* src = (const BYTE*)mapped.pData;
    BYTE* dst = (BYTE*)ds.dsBm.bmBits;
    for (int y = 0; y < m.height; y++)
        memcpy(dst + (size_t)y * span, src + (size_t)y * mapped.RowPitch, span);
    ctx->Unmap(m.staging.Get(), 0);
    return true;
}

float AppearAmount(State& m) {
    float t = (float)((Now() - m.appearStart) * 1000.0 / kAnimationMs);
    t = std::clamp(t, 0.0f, 1.0f);
    return t * t * (3 - 2 * t);
}

bool DrawFrame(State& m) {
    if (m.width <= 0 || m.height <= 0) return false;
    if (!EnsureTarget()) return false;
    if (m.generation != g_targetGeneration) {
        // The cached frame belongs to the old device.
        m.frame.Reset();
        m.staging.Reset();
        m.generation = g_targetGeneration;
    }
    if (!EnsureMenuFrame(m)) return false;
    g_target->SetTarget(m.frame.Get());
    g_target->BeginDraw();
    g_target->SetTransform(D2D1::Matrix3x2F::Translation(
        -(float)m.origin.x, -(float)m.origin.y));
    g_target->Clear(D2D1::ColorF(0, 0, 0, 0));
    for (auto& panel : m.panels) DrawPanel(m, panel);
    g_target->SetTransform(D2D1::Matrix3x2F::Identity());
    if (g_target->EndDraw() == D2DERR_RECREATE_TARGET) {
        ResetTarget();
        return false;
    }
    if (!CopyToSurface(m)) return false;
    return true;
}

// Puts the finished frame on screen, fading in over the first ~120 ms.
void Present(State& m) {
    if (!m.hwnd || !m.surface.dc) return;
    POINT dst{m.origin.x, m.origin.y}, src{0, 0};
    SIZE size{m.width, m.height};
    BLENDFUNCTION blend{AC_SRC_OVER, 0,
                        (BYTE)std::lround(255 * AppearAmount(m)),
                        AC_SRC_ALPHA};
    UpdateLayeredWindow(m.hwnd, nullptr, &dst, &size, m.surface.dc, &src, 0,
                        &blend, ULW_ALPHA);
}

bool Paint(State& m) {
    if (!m.hwnd) return false;
    if (!DrawFrame(m)) return false;
    Present(m);
    return true;
}

// ----------------------------------------------------------------------------
// Interaction

void Finish(UINT command) {
    g_menu.result = command;
    g_menu.running = false;
}

int PanelAt(float x, float y) {
    State& m = g_menu;
    for (int i = (int)m.panels.size() - 1; i >= 0; i--) {
        if (PointInRect(m.panels[i].rect, x, y)) return i;
    }
    return -1;
}

int HitTest(const Panel& panel, float x, float y) {
    if (!PointInRect(panel.rect, x, y)) return -1;
    const float s = g_menu.scale;
    float top = panel.rect.top + PadY(s) - panel.scroll;
    for (int i = 0; i < (int)panel.items->size(); i++) {
        const Node& node = (*panel.items)[i];
        float height = RowHeight(node, s);
        if (y >= top && y < top + height) {
            return node.separator || !node.enabled ? -1 : i;
        }
        top += height;
    }
    return -1;
}

void SetHover(int level, int item) {
    State& m = g_menu;
    bool changed = false;
    for (int i = 0; i < (int)m.panels.size(); i++) {
        int value = i == level ? item : -1;
        if (m.panels[i].hover != value) {
            m.panels[i].hover = value;
            changed = true;
        }
    }
    if (changed) Paint(m);
}

// What a row in an ancestor panel means for the levels opened below it.
enum class DeeperAction {
    Keep,       // The row owns the open submenu: the pointer is on its way.
    CloseNow,   // Another row with a submenu: its own level replaces this one.
    CloseSoon,  // Any other row, separator or padding edge: wait for the hand.
};

DeeperAction DeeperLevelsAction(int item, int owner, bool itemHasSubmenu) {
    if (item == owner) return DeeperAction::Keep;
    if (item >= 0 && itemHasSubmenu) return DeeperAction::CloseNow;
    return DeeperAction::CloseSoon;
}

void CloseDeeper(int level) {
    State& m = g_menu;
    m.pendingPanel = m.pendingItem = -1;
    if (m.hwnd) {
        KillTimer(m.hwnd, kSubmenuTimer);
        KillTimer(m.hwnd, kSubmenuCloseTimer);
    }
    if ((int)m.panels.size() > level + 1) {
        m.panels.resize(level + 1);
        m.active = level;
        Paint(m);
    }
}

// The row's top edge inside a panel, in screen pixels.
float RowTop(const Panel& panel, int index) {
    const float s = g_menu.scale;
    float top = panel.rect.top + PadY(s) - panel.scroll;
    for (int i = 0; i < index; i++) top += RowHeight((*panel.items)[i], s);
    return top;
}

void ScrollPanel(int level, float delta) {
    State& m = g_menu;
    if (level < 0 || level >= (int)m.panels.size()) return;
    Panel& panel = m.panels[level];
    float visible = panel.rect.bottom - panel.rect.top;
    float limit = std::max(0.0f, panel.contentHeight - visible);
    float value = std::clamp(panel.scroll + delta, 0.0f, limit);
    if (value == panel.scroll) return;
    panel.scroll = value;
    panel.hover = -1;
    m.pendingPanel = m.pendingItem = -1;
    if (m.hwnd) {
        KillTimer(m.hwnd, kSubmenuTimer);
        KillTimer(m.hwnd, kSubmenuCloseTimer);
    }
    Paint(m);
}

void EnsureVisible(int level, int index) {
    State& m = g_menu;
    Panel& panel = m.panels[level];
    float s = m.scale;
    float visible = panel.rect.bottom - panel.rect.top - PadY(s) * 2;
    float top = RowTop(panel, index) - panel.rect.top - PadY(s) + panel.scroll;
    float height = RowHeight((*panel.items)[index], s);
    float limit = std::max(0.0f, panel.contentHeight - (panel.rect.bottom - panel.rect.top));
    if (top < panel.scroll) panel.scroll = top;
    else if (top + height > panel.scroll + visible)
        panel.scroll = top + height - visible;
    panel.scroll = std::clamp(panel.scroll, 0.0f, limit);
}

void MoveHover(int step) {
    State& m = g_menu;
    if (m.panels.empty()) return;
    int level = std::clamp(m.active, 0, (int)m.panels.size() - 1);
    Panel& panel = m.panels[level];
    int count = (int)panel.items->size();
    if (!count) return;
    int index = panel.hover;
    for (int probe = 0; probe < count; probe++) {
        index = index < 0 ? (step > 0 ? 0 : count - 1)
                          : (index + step + count) % count;
        const Node& node = (*panel.items)[index];
        if (!node.separator && node.enabled) break;
    }
    panel.hover = index;
    EnsureVisible(level, index);
    Paint(m);
}

void OpenSubmenu(int level, int item) {
    State& m = g_menu;
    if (level < 0 || level >= (int)m.panels.size()) return;
    if (item < 0 || item >= (int)m.panels[level].items->size()) return;
    std::vector<Node>& nodes = *m.panels[level].items;
    if (nodes[item].sub.empty()) return;
    if ((int)m.panels.size() > level + 1 &&
        m.panels[level + 1].parentItem == item) {
        return;
    }
    const D2D1_RECT_F parentRect = m.panels[level].rect;
    const float itemTop = RowTop(m.panels[level], item);
    m.panels.resize(level + 1);
    Panel child;
    child.items = &nodes[item].sub;
    child.parentItem = item;
    MeasurePanel(m, child);
    const float s = m.scale;
    float maxHeight = (float)m.work.bottom - m.rootTop - 8 * s;
    float height = std::min(child.contentHeight, std::max(maxHeight, 4 * s));
    float x = parentRect.right - 2 * s;
    if (x + child.width > (float)m.work.right - 4 * s) {
        x = parentRect.left - child.width + 2 * s;
    }
    float minX = (float)m.work.left + 4 * s;
    float maxX = std::max(minX, (float)m.work.right - 4 * s - child.width);
    x = std::clamp(x, minX, maxX);
    float maxY = std::max(m.rootTop, (float)m.work.bottom - 4 * s - height);
    float y = std::clamp(itemTop - 4 * s, m.rootTop, maxY);
    child.rect = D2D1::RectF(x, y, x + child.width, y + height);
    m.panels.push_back(child);
    m.active = (int)m.panels.size() - 1;
    m.panels[level].hover = item;
    Paint(m);
}

void Activate(int level, int item) {
    State& m = g_menu;
    if (level < 0 || level >= (int)m.panels.size()) return;
    Panel& panel = m.panels[level];
    if (item < 0 || item >= (int)panel.items->size()) return;
    Node& node = (*panel.items)[item];
    if (node.separator || !node.enabled) return;
    if (!node.sub.empty()) {
        OpenSubmenu(level, item);
        return;
    }
    Finish(node.id);
}

void OnKey(WPARAM key) {
    State& m = g_menu;
    if (m.panels.empty()) return;
    int level = std::clamp(m.active, 0, (int)m.panels.size() - 1);
    Panel& panel = m.panels[level];
    switch (key) {
        case VK_UP:
            MoveHover(-1);
            break;
        case VK_DOWN:
            MoveHover(1);
            break;
        case VK_HOME:
            panel.hover = -1;
            MoveHover(1);
            break;
        case VK_END:
            panel.hover = -1;
            MoveHover(-1);
            break;
        case VK_PRIOR:
            ScrollPanel(level, -10 * RowHeight(Node{}, m.scale));
            break;
        case VK_NEXT:
            ScrollPanel(level, 10 * RowHeight(Node{}, m.scale));
            break;
        case VK_RIGHT:
            if (panel.hover >= 0) {
                int item = panel.hover;
                if (!(*panel.items)[item].sub.empty()) {
                    OpenSubmenu(level, item);
                    m.active = (int)m.panels.size() - 1;
                    m.panels[m.active].hover = -1;
                    MoveHover(1);
                }
            }
            break;
        case VK_LEFT:
            if (level > 0) {
                int parentItem = m.panels[level].parentItem;
                CloseDeeper(level - 1);
                m.active = level - 1;
                m.panels[level - 1].hover = parentItem;
                Paint(m);
            }
            break;
        case VK_RETURN:
            if (panel.hover >= 0) Activate(level, panel.hover);
            break;
        case VK_ESCAPE:
            if (level > 0) {
                CloseDeeper(level - 1);
                m.active = level - 1;
                Paint(m);
            } else {
                Finish(0);
            }
            break;
        default:
            break;
    }
}

void OnMnemonic(WPARAM key) {
    State& m = g_menu;
    int level = std::clamp(m.active, 0, (int)m.panels.size() - 1);
    Panel& panel = m.panels[level];
    WCHAR letter = key >= L'a' && key <= L'z' ? (WCHAR)(key - L'a' + L'A')
                                             : (WCHAR)key;
    for (int i = 0; i < (int)panel.items->size(); i++) {
        if ((*panel.items)[i].mnemonic != letter) continue;
        panel.hover = i;
        Paint(m);
        Activate(level, i);
        return;
    }
}

// ----------------------------------------------------------------------------
// The window

LRESULT CALLBACK MenuWndProc(HWND hwnd, UINT message, WPARAM wParam,
                             LPARAM lParam) {
    State& m = g_menu;
    switch (message) {
        case WM_MOUSEACTIVATE:
            return MA_ACTIVATE;
        case WM_ACTIVATE:
            if (LOWORD(wParam) == WA_INACTIVE && m.running) Finish(0);
            return 0;
        case WM_SETCURSOR:
            SetCursor(LoadCursorW(nullptr, IDC_ARROW));
            return TRUE;
        case WM_MOUSEMOVE: {
            if (m.ignoreFirstMove) {
                m.ignoreFirstMove = false;
                return 0;
            }
            float x = (float)GET_X_LPARAM(lParam) + m.origin.x;
            float y = (float)GET_Y_LPARAM(lParam) + m.origin.y;
            int level = PanelAt(x, y);
            if (level < 0) {
                SetHover(-1, -1);  // Over the shadow or outside: keep the levels.
                return 0;
            }
            int item = HitTest(m.panels[level], x, y);
            if (level < (int)m.panels.size() - 1) {
                // A level below this panel belongs to one of its rows. Keeping
                // that row's submenu alive is what makes the row reachable:
                // closing it as soon as another row is touched left no way to
                // walk from "Move to group" to the groups it lists.
                const int owner = m.panels[level + 1].parentItem;
                const bool itemHasSubmenu =
                    item >= 0 && !(*m.panels[level].items)[item].sub.empty();
                switch (DeeperLevelsAction(item, owner, itemHasSubmenu)) {
                    case DeeperAction::Keep:
                        KillTimer(hwnd, kSubmenuCloseTimer);
                        break;
                    case DeeperAction::CloseNow:
                        KillTimer(hwnd, kSubmenuCloseTimer);
                        CloseDeeper(level);
                        break;
                    case DeeperAction::CloseSoon:
                        SetTimer(hwnd, kSubmenuCloseTimer, kSubmenuCloseDelayMs,
                                 nullptr);
                        break;
                }
            } else {
                KillTimer(hwnd, kSubmenuCloseTimer);
            }
            SetHover(level, item);
            if (item >= 0 && (int)m.panels.size() == level + 1 &&
                !(*m.panels[level].items)[item].sub.empty()) {
                if (m.pendingPanel != level || m.pendingItem != item) {
                    m.pendingPanel = level;
                    m.pendingItem = item;
                    SetTimer(hwnd, kSubmenuTimer, kSubmenuDelayMs, nullptr);
                }
            } else {
                m.pendingPanel = m.pendingItem = -1;
                KillTimer(hwnd, kSubmenuTimer);
            }
            return 0;
        }
        case WM_LBUTTONDOWN: {
            float x = (float)GET_X_LPARAM(lParam) + m.origin.x;
            float y = (float)GET_Y_LPARAM(lParam) + m.origin.y;
            int level = PanelAt(x, y);
            if (level < 0) {
                Finish(0);  // The click that dismisses is swallowed.
                return 0;
            }
            int item = HitTest(m.panels[level], x, y);
            m.pressedPanel = level;
            m.pressedItem = item;
            for (auto& panel : m.panels) panel.pressed = -1;
            m.panels[level].pressed = item;
            Paint(m);
            return 0;
        }
        case WM_LBUTTONUP: {
            float x = (float)GET_X_LPARAM(lParam) + m.origin.x;
            float y = (float)GET_Y_LPARAM(lParam) + m.origin.y;
            int level = PanelAt(x, y);
            int item = level >= 0 ? HitTest(m.panels[level], x, y) : -1;
            int pressedLevel = m.pressedPanel, pressedItem = m.pressedItem;
            m.pressedPanel = m.pressedItem = -1;
            for (auto& panel : m.panels) panel.pressed = -1;
            if (level < 0) {
                Finish(0);
                return 0;
            }
            if (level == pressedLevel && item >= 0 && item == pressedItem) {
                Activate(level, item);
            } else {
                SetHover(level, item);
            }
            return 0;
        }
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            Finish(0);
            return 0;
        case WM_MOUSEWHEEL: {
            POINT point{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            int level = PanelAt((float)point.x, (float)point.y);
            if (level < 0) level = m.active;
            m.wheel += GET_WHEEL_DELTA_WPARAM(wParam);
            float step = 3 * 32 * m.scale;
            while (m.wheel >= WHEEL_DELTA) {
                ScrollPanel(level, -step);
                m.wheel -= WHEEL_DELTA;
            }
            while (m.wheel <= -WHEEL_DELTA) {
                ScrollPanel(level, step);
                m.wheel += WHEEL_DELTA;
            }
            return 0;
        }
        case WM_TIMER:
            if (wParam == kSubmenuTimer) {
                KillTimer(hwnd, kSubmenuTimer);
                int level = m.pendingPanel, item = m.pendingItem;
                m.pendingPanel = m.pendingItem = -1;
                if (level >= 0 && level < (int)m.panels.size() && item >= 0 &&
                    item < (int)m.panels[level].items->size() &&
                    (int)m.panels.size() == level + 1 &&
                    m.panels[level].hover == item) {
                    if (!(*m.panels[level].items)[item].sub.empty())
                        OpenSubmenu(level, item);
                }
            } else if (wParam == kSubmenuCloseTimer) {
                KillTimer(hwnd, kSubmenuCloseTimer);
                // Re-read the pointer instead of closing blind: it may have
                // reached the submenu after the move that armed this timer.
                POINT point{};
                if (GetCursorPos(&point)) {
                    int level = PanelAt((float)point.x, (float)point.y);
                    if (level >= 0 && level < (int)m.panels.size() - 1) {
                        int item = HitTest(m.panels[level], (float)point.x,
                                           (float)point.y);
                        if (item != m.panels[level + 1].parentItem) {
                            CloseDeeper(level);
                        }
                    }
                }
            } else if (wParam == kAnimationTimer) {
                float t = AppearAmount(m);
                if (t >= 1.0f) KillTimer(hwnd, kAnimationTimer);
                Paint(m);
            }
            return 0;
        case WM_KEYDOWN:
            OnKey(wParam);
            return 0;
        case WM_SYSKEYDOWN:
            if (wParam == VK_F4 && (GetKeyState(VK_MENU) & 0x8000)) {
                Finish(0);
            } else if (wParam != VK_MENU) {
                OnMnemonic(wParam);
            }
            return 0;
        case WM_CAPTURECHANGED:
            if (m.running && (HWND)lParam != hwnd) Finish(0);
            return 0;
        case WM_ERASEBKGND:
            return 1;
        case WM_DESTROY:
            return 0;
    }
    return DefWindowProcW(hwnd, message, wParam, lParam);
}

bool g_menuClassRegistered = false;

bool EnsureMenuClass() {
    if (g_menuClassRegistered) return true;
    WNDCLASSEXW wc{sizeof(wc)};
    wc.lpfnWndProc = MenuWndProc;
    wc.hInstance = (HINSTANCE)&__ImageBase;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.lpszClassName = MenuClassName();
    if (RegisterClassExW(&wc)) {
        g_menuClassRegistered = true;
    } else {
        // The name is unique per image, so this is a real registration
        // failure rather than a class left behind by an earlier build.
        Wh_Log(L"Menu class %ls unavailable: %u", MenuClassName(),
               GetLastError());
        g_menuClassRegistered = false;
    }
    return g_menuClassRegistered;
}

void UnregisterMenuClass() {
    if (!g_menuClassRegistered) return;
    UnregisterClassW(MenuClassName(), (HINSTANCE)&__ImageBase);
    g_menuClassRegistered = false;
}

void Reset() {
    State& m = g_menu;
    if (m.hwnd) {
        KillTimer(m.hwnd, kAnimationTimer);
        KillTimer(m.hwnd, kSubmenuTimer);
        KillTimer(m.hwnd, kSubmenuCloseTimer);
    }
    FreeSurface(m.surface);
    m.surface = {};
    m.frame.Reset();
    m.staging.Reset();
    m.root.clear();
    m.panels.clear();
    m.hwnd = nullptr;
    m.running = false;
    m.result = 0;
    m.active = 0;
    m.ignoreFirstMove = true;
    m.wheel = 0;
    m.pressedPanel = m.pressedItem = -1;
    m.pendingPanel = m.pendingItem = -1;
    m.width = m.height = 0;
    m.origin = {};
    m.rootTop = 0;
    m.widestChild = m.tallestChild = 0;
    m.work = {};
}

}  // namespace LaunchpadMenu

// Reads the HMENU, measures every level and fixes the window rectangle. One
// window size covers every level that can open, so a submenu never has to
// resize it and the blurred backdrop is taken exactly once.
bool Prepare(LaunchpadMenu::State& m, HMENU menu, int x, int y, UINT flags) {
    using namespace LaunchpadMenu;
    if (!ReadMenu(menu, m.root)) return false;
    if (!EnsureTarget()) return false;
    POINT point{x, y};
    HMONITOR monitor = MonitorFromPoint(point, MONITOR_DEFAULTTONEAREST);
    MONITORINFO info{sizeof(info)};
    if (!GetMonitorInfoW(monitor, &info)) {
        info.rcWork = RECT{0, 0, GetSystemMetrics(SM_CXSCREEN),
                           GetSystemMetrics(SM_CYSCREEN)};
    }
    m.work = info.rcWork;
    UINT dpiX = USER_DEFAULT_SCREEN_DPI, dpiY = USER_DEFAULT_SCREEN_DPI;
    if (FAILED(GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY)))
        dpiX = USER_DEFAULT_SCREEN_DPI;
    EnsureFormats(m, dpiX / (float)USER_DEFAULT_SCREEN_DPI);
    m.generation = g_targetGeneration;

    Panel root;
    root.items = &m.root;
    MeasurePanel(m, root);
    MeasureChildren(m, m.root);
    m.panels.push_back(root);
    PlaceRoot(m, point, flags);

    const float s = m.scale;
    const D2D1_RECT_F rootRect = m.panels[0].rect;
    float padding = 24 * s;
    float spread = m.widestChild > 0 ? m.widestChild + 12 * s : 0;
    float drop = m.tallestChild > 0 ? m.tallestChild + 12 * s : 0;
    RECT bounds{};
    bounds.left = (LONG)std::floor(
        std::max((float)m.work.left, rootRect.left - spread - padding));
    bounds.top = (LONG)std::floor(
        std::max((float)m.work.top, rootRect.top - padding));
    bounds.right = (LONG)std::ceil(
        std::min((float)m.work.right, rootRect.right + spread + padding));
    bounds.bottom = (LONG)std::ceil(
        std::min((float)m.work.bottom, rootRect.bottom + drop + padding));
    m.origin = POINT{bounds.left, bounds.top};
    m.width = std::max(1, (int)(bounds.right - bounds.left));
    m.height = std::max(1, (int)(bounds.bottom - bounds.top));
    return true;
}

UINT TrackLaunchpadPopupMenu(HMENU menu, UINT flags, int x, int y, HWND owner,
                             LPTPMPARAMS params) {
    using namespace LaunchpadMenu;
    State& m = g_menu;
    if (m.running) return 0;
    Reset();
    if (!Prepare(m, menu, x, y, flags)) return 0;
    if (!EnsureMenuClass()) return 0;
    Wh_Log(L"menu: %zu root items at %d,%d size %dx%d", m.root.size(),
           m.origin.x, m.origin.y, m.width, m.height);
    bool previousModal = g_modal;
    g_modal = true;
    g_pressed = -1;
    g_pressedEmpty = false;
    g_drag.pending = false;
    m.result = 0;
    m.running = true;
    m.hwnd = CreateWindowExW(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED,
                             MenuClassName(), L"", WS_POPUP, m.origin.x,
                             m.origin.y, m.width, m.height, owner, nullptr,
                             (HINSTANCE)&__ImageBase, nullptr);
    if (!m.hwnd) {
        Wh_Log(L"menu: CreateWindowExW failed: %u", GetLastError());
        m.running = false;
        g_modal = previousModal;
        Reset();
        return 0;
    }
    m.background = MenuBackground(m);
    m.appearStart = Now();
    Paint(m);
    ShowWindow(m.hwnd, SW_SHOWNA);
    if (!SetForegroundWindow(m.hwnd)) SetActiveWindow(m.hwnd);
    SetFocus(m.hwnd);
    SetCapture(m.hwnd);
    SetTimer(m.hwnd, kAnimationTimer, 8, nullptr);

    MSG message;
    int quitCode = 0;
    bool quit = false;
    while (m.running) {
        DWORD ready = MsgWaitForMultipleObjectsEx(0, nullptr, INFINITE,
                                                  QS_ALLINPUT,
                                                  MWMO_INPUTAVAILABLE);
        if (ready == WAIT_FAILED) break;
        while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT) {
                quit = true;
                quitCode = (int)message.wParam;
                m.running = false;
                break;
            }
            TranslateMessage(&message);
            DispatchMessageW(&message);
            if (!m.running) break;
        }
    }

    if (GetCapture() == m.hwnd) ReleaseCapture();
    if (m.hwnd) DestroyWindow(m.hwnd);
    m.hwnd = nullptr;
    UINT result = m.result;
    Reset();
    g_modal = previousModal;
    Wh_Log(L"menu: closed with command %u", result);
    if (!quit && g_shown && owner && IsWindowVisible(owner)) {
        SetForegroundWindow(owner);
    }
    if (quit) PostQuitMessage(quitCode);
    return result;
}

#ifdef SEAMLESS_PREVIEW
// Offscreen menu render for the smoke harness: lays a menu out exactly as the
// window would and draws it over the launcher's last frame, so the material can
// be checked without a window, input or a screen capture. hover and submenu
// select an item and a level to open. The result is left in
// LaunchpadMenu::g_menu.surface.
bool PreviewRenderMenu(HMENU menu, int x, int y, UINT flags, int hover = -1,
                       int submenu = -1) {
    using namespace LaunchpadMenu;
    State& m = g_menu;
    if (m.running) return false;
    Reset();
    if (!Prepare(m, menu, x, y, flags)) return false;
    m.background = MenuBackground(m);
    if (hover >= 0 && !m.panels.empty() &&
        hover < (int)m.panels[0].items->size()) {
        m.panels[0].hover = hover;
    }
    if (submenu >= 0 && !m.panels.empty() &&
        submenu < (int)m.panels[0].items->size()) {
        OpenSubmenu(0, submenu);
        if (m.panels.size() > 1) m.panels[1].hover = 1;
    }
    return DrawFrame(m);
}
#endif

void EndDrag(bool drop);
void StartDrag();
void DragTick();

constexpr float kTwoPi = 6.2831853f;

float Ease(float t) {
    t = std::clamp(t, 0.0f, 1.0f);
    if (g_settings.easing == L"settle") return t >= 1 ? 1 : (1 - std::exp(-6.0f * t)) / (1 - std::exp(-6.0f));
    if (g_settings.easing == L"quint") return 1 - std::pow(1 - t, 5.0f);
    return 1 - (1 - t) * (1 - t) * (1 - t);
}

// A button's hit area runs a little past what is drawn.
bool InButton(const D2D1_RECT_F& r, float x, float y) {
    float pad = 6 * g_layout.scale;
    return InRect(D2D1::RectF(r.left - pad, r.top - pad, r.right + pad,
                              r.bottom + pad),
                  x, y);
}

void EnsureGlassBrushes(ID2D1RenderTarget* target, TileGlassResources& glass) {
    const D2D1_GRADIENT_STOP surface[] = {
        {0, D2D1::ColorF(0xFFFFFF, .14f)}, {.35f, D2D1::ColorF(0xFFFFFF, .08f)},
        {.72f, D2D1::ColorF(0xFFFFFF, .024f)}, {1, D2D1::ColorF(0xFFFFFF, 0)}};
    const D2D1_GRADIENT_STOP rim[] = {
        {0, D2D1::ColorF(0xFFFFFF, .82f)}, {.24f, D2D1::ColorF(0xFFFFFF, .62f)},
        {.58f, D2D1::ColorF(0xFFFFFF, .21f)}, {1, D2D1::ColorF(0xFFFFFF, 0)}};
    const D2D1_GRADIENT_STOP dangerSurface[] = {
        {0, D2D1::ColorF(0xFF4650, .44f)}, {.35f, D2D1::ColorF(0xFF4650, .30f)},
        {.72f, D2D1::ColorF(0xFF4650, .12f)}, {1, D2D1::ColorF(0xFF4650, 0)}};
    const D2D1_GRADIENT_STOP dangerRim[] = {
        {0, D2D1::ColorF(0xFFFFE9EA, .95f)}, {.24f, D2D1::ColorF(0xFFFF8791, .88f)},
        {.58f, D2D1::ColorF(0xFFFF4B56, .48f)}, {1, D2D1::ColorF(0xFFFF4B56, 0)}};
    const D2D1_GRADIENT_STOP dangerHalo[] = {
        {0, D2D1::ColorF(0xFFFF3D4A, .52f)}, {.40f, D2D1::ColorF(0xFFFF3D4A, .28f)},
        {.72f, D2D1::ColorF(0xFFFF3D4A, .10f)}, {1, D2D1::ColorF(0xFFFF3D4A, 0)}};
    auto make = [&](auto& stops, auto& brush) {
        if (brush) return;
        ComPtr<ID2D1GradientStopCollection> colors;
        if (SUCCEEDED(target->CreateGradientStopCollection(stops, ARRAYSIZE(stops), &colors)))
            target->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties({0, 0}, {0, 0}, 1, 1),
                                               colors.Get(), &brush);
    };
    make(surface, glass.surface);
    make(rim, glass.rim);
    make(dangerSurface, glass.dangerSurface);
    make(dangerRim, glass.dangerRim);
    make(dangerHalo, glass.dangerHalo);
}

void DrawControlGlass(ID2D1RenderTarget* target, TileGlassResources& glass,
                      const D2D1_RECT_F& box, float radius, float scale,
                      D2D1_POINT_2F pointer, float opacity) {
    const float width = box.right - box.left, height = box.bottom - box.top;
    if (opacity <= .001f || width < 4 * scale || height < 4 * scale) return;
    EnsureGlassBrushes(target, glass);
    const float stroke = 1.5f * scale, inset = stroke / 2;
    const float round = radius - inset;
    const auto outline = D2D1::RoundedRect(
        {box.left + inset, box.top + inset, box.right - inset, box.bottom - inset}, round, round);
    const D2D1_POINT_2F center{std::clamp(pointer.x, box.left, box.right),
                              std::clamp(pointer.y, box.top, box.bottom)};
    auto place = [&](ID2D1RadialGradientBrush* brush, float spread) {
        brush->SetCenter(center);
        brush->SetRadiusX(width * spread);
        brush->SetRadiusY(height * spread);
        brush->SetOpacity(opacity);
    };
    if (glass.surface) {
        place(glass.surface.Get(), .72f);
        target->FillRoundedRectangle(outline, glass.surface.Get());
    }
    if (glass.rim) {
        place(glass.rim.Get(), .85f);
        target->DrawRoundedRectangle(outline, glass.rim.Get(), stroke);
    }
}

void DrawClearButton(float opacity) {
    if (g_openFolder || g_query.empty()) {
        return;
    }
    const D2D1_RECT_F& r = g_layout.clearButton;
    float s = g_layout.scale;
    float radius = (r.bottom - r.top) / 2;
    if (g_clearHover) {
        Fill(r, D2D1::ColorF(0xFFFFFF, 0.16f * opacity), radius);
    }
    float cx = (r.left + r.right) / 2, cy = (r.top + r.bottom) / 2;
    float arm = 4 * s;
    g_brush->SetColor(D2D1::ColorF(0xFFFFFF, (g_clearHover ? 0.95f : 0.8f) * opacity));
    g_target->DrawLine(D2D1::Point2F(cx - arm, cy - arm),
                       D2D1::Point2F(cx + arm, cy + arm), g_brush.Get(),
                       1.4f * s);
    g_target->DrawLine(D2D1::Point2F(cx - arm, cy + arm),
                       D2D1::Point2F(cx + arm, cy - arm), g_brush.Get(),
                       1.4f * s);
}

////////////////////////////////////////////////////////////////////////////////
// Power dock: a glass pill of round buttons at the bottom, aligned by
// the dock position setting. Without "Skip confirmation", the first click
// arms a button (outlined, "Click again to ...") and a second click on it
// runs the action.

int g_powerHover = -1;    // Slot in g_layout.powerItems.
int g_powerPressed = -1;  // Action ID, independent of display order.
int g_powerArmed = -1;    // Action ID, independent of display order.

void HideNow(bool resetStartButton = true);
void StartFrames();

void RefreshPowerCaps() {
    SYSTEM_POWER_CAPABILITIES caps{};
    if (GetPwrCapabilities(&caps)) {
        // MinGW's header calls AoAc (modern standby) spare2[2]; the Windows
        // SDK names the three bytes Hiberboot, WakeAlarmPresent and AoAc.
        g_canSleep = caps.SystemS1 || caps.SystemS2 || caps.SystemS3 ||
                     caps.spare2[2];
        g_canHibernate = caps.SystemS4 && caps.HiberFilePresent;
    }
}

enum class PowerUpdateStatus { Unknown, NoRestartPending, RestartRequired };
PowerUpdateStatus g_powerUpdateStatus = PowerUpdateStatus::Unknown;

bool IsUpdatePowerAction(PowerAction action) {
    return action == PowerAction::Restart || action == PowerAction::ShutDown;
}

#ifndef SEAMLESS_PREVIEW
HANDLE g_powerUpdateProbe = nullptr;
ULONGLONG g_lastPowerUpdateProbe = 0;

bool HasUpdateRebootMarker(PCWSTR path) {
    HKEY key = nullptr;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, path, 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY,
                      &key) != ERROR_SUCCESS) return false;
    RegCloseKey(key);
    return true;
}

PowerUpdateStatus ReadPowerUpdateStatus() {
    // Check Windows Update and servicing separately. The WUA automation
    // object can report false while the orchestrator has a pending reboot.
    if (HasUpdateRebootMarker(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\WindowsUpdate\\Orchestrator\\RebootRequired") ||
        HasUpdateRebootMarker(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\WindowsUpdate\\Auto Update\\RebootRequired") ||
        HasUpdateRebootMarker(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Component Based Servicing\\RebootPending")) {
        return PowerUpdateStatus::RestartRequired;
    }

    const HRESULT initialized = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(initialized)) return PowerUpdateStatus::Unknown;
    struct Apartment {
        ~Apartment() { CoUninitialize(); }
    } apartment;
    CLSID clsid{};
    ComPtr<ISystemInformation> info;
    VARIANT_BOOL required = VARIANT_FALSE;
    if (FAILED(CLSIDFromProgID(L"Microsoft.Update.SystemInfo", &clsid)) ||
        FAILED(CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER,
                                IID_PPV_ARGS(&info))) ||
        FAILED(info->get_RebootRequired(&required))) {
        return PowerUpdateStatus::Unknown;
    }
    return required != VARIANT_FALSE ? PowerUpdateStatus::RestartRequired
                                     : PowerUpdateStatus::NoRestartPending;
}

DWORD WINAPI PowerUpdateProbeThread(void*) {
    const auto status = ReadPowerUpdateStatus();
    if (HWND wnd = g_wnd)
        PostMessageW(wnd, kPowerUpdateStatusReady, static_cast<WPARAM>(status), 0);
    return 0;
}

void StopPowerUpdateProbe() {
    if (!g_powerUpdateProbe) return;
    WaitForSingleObject(g_powerUpdateProbe, INFINITE);
    CloseHandle(g_powerUpdateProbe);
    g_powerUpdateProbe = nullptr;
}

void RefreshPowerUpdateStatus() {
    constexpr ULONGLONG interval = 15000;
    const auto now = GetTickCount64();
    if (g_lastPowerUpdateProbe && now - g_lastPowerUpdateProbe < interval) return;
    if (g_powerUpdateProbe) {
        if (WaitForSingleObject(g_powerUpdateProbe, 0) != WAIT_OBJECT_0) return;
        StopPowerUpdateProbe();
    }
    if (std::none_of(g_layout.powerItems.begin(), g_layout.powerItems.end(), [](int action) {
        return IsUpdatePowerAction(static_cast<PowerAction>(action));
    })) return;
    g_powerUpdateProbe = CreateThread(nullptr, 0, PowerUpdateProbeThread, nullptr, 0, nullptr);
    if (g_powerUpdateProbe) g_lastPowerUpdateProbe = now;
}
#endif

D2D1_RECT_F PowerButtonRect(int slot) {
    const Layout& L = g_layout;
    float pad = (L.powerBar.bottom - L.powerBar.top - L.powerButton) / 2;
    float left = L.powerBar.left + pad + slot * (L.powerButton + L.powerGap);
    float top = L.powerBar.top + pad;
    return D2D1::RectF(left, top, left + L.powerButton, top + L.powerButton);
}

// Only the visible button is interactive. The dock padding is inert.
int PowerAt(int x, int y, float pad = 0) {
    const Layout& L = g_layout;
    if (g_batchBlend>0 || !g_appSelection.empty() || L.powerItems.empty()) return -1;
    const D2D1_RECT_F bar{L.powerBar.left - pad, L.powerBar.top - pad,
                          L.powerBar.right + pad, L.powerBar.bottom + pad};
    if (!InRect(bar, (float)x, (float)y)) return -1;
    int best = -1;
    float bestDistance = 0;
    for (int slot = 0; slot < (int)L.powerItems.size(); ++slot) {
        const auto button = PowerButtonRect(slot);
        const float radius = L.powerButton / 2 + pad;
        const float dx = x - (button.left + L.powerButton / 2);
        const float dy = y - (button.top + L.powerButton / 2);
        const float distance = dx * dx + dy * dy;
        if (distance <= radius * radius && (best < 0 || distance < bestDistance)) {
            best = slot;
            bestDistance = distance;
        }
    }
    return best;
}

void DisarmPower() {
    if (g_powerArmed >= 0) {
        g_powerArmed = -1;
        KillTimer(g_wnd, kTimerPowerArm);
        StartFrames();
    }
}

void EnablePrivilege(PCWSTR name) {
    HANDLE token;
    if (!OpenProcessToken(GetCurrentProcess(),
                          TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token)) {
        return;
    }
    TOKEN_PRIVILEGES tp{1};
    if (LookupPrivilegeValueW(nullptr, name, &tp.Privileges[0].Luid)) {
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(token, FALSE, &tp, 0, nullptr, nullptr);
    }
    CloseHandle(token);
}

// Shut down like Start does: a fast startup shutdown when that's on.
bool FastStartupOn() {
    DWORD value = 0, size = sizeof(value);
    return RegGetValueW(HKEY_LOCAL_MACHINE,
                        L"SYSTEM\\CurrentControlSet\\Control\\Session "
                        L"Manager\\Power",
                        L"HiberbootEnabled", RRF_RT_REG_DWORD, nullptr, &value,
                        &size) == ERROR_SUCCESS &&
           value == 1;
}

// Off the UI thread: sleep and hibernate return only after the PC wakes.
// The handle is kept and joined before the mod unloads: Windhawk calls
// FreeLibrary right after Wh_ModUninit, so no mod thread may outlive it.
HANDLE g_powerThread = nullptr;

DWORD WINAPI PowerThread(void* param) {
    auto action = (PowerAction)(INT_PTR)param;
    // Never EWX_FORCE: apps with unsaved work get to ask, as with Start.
    DWORD reason = SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER |
                   SHTDN_REASON_FLAG_PLANNED;
    BOOL ok = TRUE;
    switch (action) {
        case PowerAction::Lock:
            ok = LockWorkStation();
            break;
        case PowerAction::SignOut:
            ok = ExitWindowsEx(EWX_LOGOFF, reason);
            break;
        case PowerAction::Sleep:
        case PowerAction::Hibernate:
            EnablePrivilege(SE_SHUTDOWN_NAME);
            ok = SetSuspendState(action == PowerAction::Hibernate, FALSE,
                                 FALSE);
            if (!ok && action == PowerAction::Sleep) {
                // Modern standby PCs can't be suspended by a program;
                // turning the screen off lets them go to sleep.
                if (HWND wnd = g_wnd) {
                    PostMessageW(wnd, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
                    ok = TRUE;
                }
            }
            break;
        case PowerAction::Restart:
            EnablePrivilege(SE_SHUTDOWN_NAME);
            ok = ExitWindowsEx(EWX_REBOOT, reason);
            break;
        case PowerAction::ShutDown:
            EnablePrivilege(SE_SHUTDOWN_NAME);
            ok = ExitWindowsEx(
                EWX_POWEROFF | (FastStartupOn() ? EWX_HYBRID_SHUTDOWN : 0),
                reason);
            break;
    }
    if (!ok) {
        Wh_Log(L"Power action %s failed: %u", kPower[(int)action].key,
               GetLastError());
    }
    return 0;
}

void RunPowerAction(PowerAction action) {
#ifdef SEAMLESS_PREVIEW
    MessageBoxW(g_wnd, Tr(L"Preview: power actions are disabled."), Tr(L"Lumiflow"), MB_OK | MB_ICONINFORMATION);
    return;
#endif
    Wh_Log(L"Power action: %s", kPower[(int)action].key);
    DisarmPower();
    HideNow();  // At once: no closing animation over a lock or sleep.
    if (g_powerThread) {
        if (WaitForSingleObject(g_powerThread, 0) != WAIT_OBJECT_0) {
            Wh_Log(L"Power action already in progress");
            return;
        }
        CloseHandle(g_powerThread);
        g_powerThread = nullptr;
    }
    g_powerThread = CreateThread(nullptr, 0, PowerThread,
                                 (void*)(INT_PTR)action, 0, nullptr);
    if (!g_powerThread) {
        Wh_Log(L"Power thread failed: %u", GetLastError());
    }
}

void PowerClick(int slot) {
    if (slot < 0 || slot >= (int)g_layout.powerItems.size()) {
        return;
    }
    const int action = g_layout.powerItems[slot];
    if (!g_settings.powerNoConfirm && g_powerArmed != action) {
        g_powerArmed = action;
        SetTimer(g_wnd, kTimerPowerArm, 4000, nullptr);
        StartFrames();
        return;
    }
    RunPowerAction(static_cast<PowerAction>(action));
}

D2D1_RECT_F BatchBar() {
    float s=g_layout.scale;
    float width=std::min(312*s,std::max(0.f,g_layout.width-16*s));
    float bottom=g_layout.height-std::round(std::max(22*s,g_layout.height*.028f));
    float cx=g_layout.width*.5f;
    return {cx-width*.5f,bottom-46*s,cx+width*.5f,bottom};
}
D2D1_RECT_F ToolBarBounds() {
    auto batch=BatchBar();
    auto normal=g_layout.powerBar;
    if(g_layout.powerItems.empty())normal={(batch.left+batch.right)*.5f,batch.top,(batch.left+batch.right)*.5f,batch.bottom};
    float t=Ease(g_batchBlend);
    return {std::lerp(normal.left,batch.left,t),std::lerp(normal.top,batch.top,t),
            std::lerp(normal.right,batch.right,t),std::lerp(normal.bottom,batch.bottom,t)};
}
int BatchActionAt(int x,int y) {
    if(g_appSelection.empty()||g_batchBlend<1.f||g_batchLaunching)return -1;
    auto bar=BatchBar();
    if(!InRect(bar,(float)x,(float)y))return -1;
    float fraction=(x-bar.left)/(bar.right-bar.left);
    // 0 Cancel | count | 2 Delete | 1 Open
    if(fraction<.24f)return 0;
    if(fraction>=.75f)return 1;
    return fraction>=.50f?2:-1;
}
bool ToolBarConsumes(int x,int y) {
    return (g_batchBlend>0||!g_appSelection.empty())&&
        (InRect(ToolBarBounds(),(float)x,(float)y)||
         (g_batchBlend<1.f&&!g_layout.powerItems.empty()&&InRect(g_layout.powerBar,(float)x,(float)y))||
         (!g_appSelection.empty()&&InRect(BatchBar(),(float)x,(float)y)));
}
void DrawGlassSurface(const D2D1_RECT_F& bar,float radius,float opacity,float hover) {
    if(!SelectionHasArea(bar)||opacity<=0)return;
    float shift=2*g_layout.scale;
    Fill({bar.left,bar.top+shift,bar.right,bar.bottom+shift},D2D1::ColorF(0,opacity*.14f),radius);
    Fill(bar,D2D1::ColorF(0xE8E8E8,opacity*(.07f+.17f*g_settings.chromeOpacity)),radius);
    g_brush->SetColor(D2D1::ColorF(0xFFFFFF,opacity*(.22f+.20f*hover)));
    g_target->DrawRoundedRectangle(
        D2D1::RoundedRect(D2D1::RectF(bar.left+.5f,bar.top+.5f,bar.right-.5f,bar.bottom-.5f),
                          radius-.5f,radius-.5f),g_brush.Get(),1);
}
void DrawGlassPill(const D2D1_RECT_F& bar,float opacity,float hover) {
    DrawGlassSurface(bar,(bar.bottom-bar.top)/2,opacity,hover);
}

void DrawPowerUpdateDot(const D2D1_RECT_F& button, float opacity) {
    float s = g_layout.scale;
    const auto center = D2D1::Point2F(button.right - 10 * s, button.top + 10 * s);
    auto dot = D2D1::Ellipse(center, 3.2f * s, 3.2f * s);
    g_brush->SetColor(D2D1::ColorF(0xFBBF42, opacity));
    g_target->FillEllipse(dot, g_brush.Get());
    g_brush->SetColor(D2D1::ColorF(0x392606, .65f * opacity));
    g_target->DrawEllipse(dot, g_brush.Get(), .7f * s);
    g_brush->SetColor(D2D1::ColorF(0xFFF3CB, .68f * opacity));
    g_target->FillEllipse(D2D1::Ellipse({center.x - .75f * s, center.y - 1.1f * s},
                                       1.35f * s, .75f * s), g_brush.Get());
}

void DrawTileGlass(const D2D1_RECT_F& unit, D2D1_POINT_2F light,
                   D2D1_POINT_2F rimLight, float opacity, float radius = 0, float danger = 0.f);

////////////////////////////////////////////////////////////////////////////////
// Power dock: a local warning concentration inside the one shared glass pill
//
// The dock keeps its single liquid-glass capsule. Hovering a power button adds
// no surface: a #A91223 energy lobe is painted into the capsule's own glass,
// centred on that button's click region, so the material reads as one glass
// system with a local warning concentration rather than a lit button. The lobe
// is wider than it is tall, dies out well before the next button, and a thin
// edge response at the capsule's bottom grounds the glass without moving the
// visual centroid downwards.
//
// The text pill above the dock receives a much weaker, position-dependent
// reflection of the same lobe. It is clipped to that pill's own glass and never
// forms a halo, outline or second background.
//
// Painting is pure Direct2D: one radial-gradient brush with fixed stops, reused
// every frame, clipped by the capsule's rounded geometry. No capture mosaic, no
// per-pixel bake, no extra render pass.

namespace power_ambient {

// Every number the warning tint uses. Retune the dock here, not in the draw
// code below.
struct Spec {
    UINT32 base = 0xA91223;       // warning red, sRGB
    float centreAlpha = .78f;     // concentration at the centre

    // Radial stops: shares of the centre concentration.
    float stop35 = .88f;          // at 35% of the radius
    float stop65 = .48f;          // at 65% of the radius
    // 100% is fully transparent.

    // A circle, not an ellipse: the glow has the same reach in every
    // direction. 23 DIP is one half of the 46 DIP capsule height, so the
    // visible lobe fits inside the glass without being cropped into a band.
    float radiusX = 23.f;         // DIP at 100% scaling
    float radiusY = 23.f;         // DIP at 100% scaling, equal to X

    // A thin response at the capsule's bottom edge, under the same button.
    float bottomStrength = .65f;  // share of the centre concentration
    float bottomRadiusX = 30.f;
    float bottomRadiusY = 1.f;

    // The same lobe reaching the text pill above, heavily attenuated and kept
    // close to the pill's lower edge so it cannot lift the main lobe's centre.
    float bleedStrength = .15f;   // peak share of the centre concentration
    float bleedRadiusX = 30.f;
    float bleedRadiusY = 10.f;

    // Visual compensation only; 0 keeps the lobe exactly on the capsule centre.
    float yOffset = 0.f;          // DIP, allowed range -1..+1

    // One continuous source: position and intensity have separate constants.
    float positionMs = 35.f;      // pointer tracking within a button
    float transferMs = 61.f;     // handoff after an input jump
    float pointerTransferMs = 12.f; // follow pointer travel between adjacent buttons
    int riseMs = 120;             // warning intensity fading in
    int fallMs = 180;             // warning intensity fading out
    float pressScale = .95f;
    int pressMs = 120, releaseMs = 180;
};

inline const Spec& Get() {
    static const Spec spec;
    return spec;
}

}  // namespace power_ambient

struct PowerGlassState {
    int slot = -1;              // Hovered/armed slot, -1 when the pointer is in a gap.
    float alpha = 0;            // Light intensity 0..1.
    float targetAlpha = 0;      // Where the intensity is heading.
    float x = 0;                // Light centre in the launcher's DIP space.
    float y = 0;                // Fixed to the capsule's vertical geometry centre.
    float targetX = 0;
    int litSlot = -1;           // Destination, retained across a gap or fade.
    std::array<float, std::size(kPower)> weights{};  // Light held by each button.
    float press = 0;            // 0..1, 1 -> 0.95 scale.
    double tick = 0;            // Last advance.
};
[[clang::no_destroy]] PowerGlassState g_powerGlass;

// The raw pointer drives the connection even outside the circular hit areas.
// The lobes are drawn at their button anchors.
float g_powerPointerX = 0;
bool g_powerPointerInside = false;

inline bool PowerWarningEnabled(int action) {
    return g_settings.powerWarning && action >= 0 &&
           action < (int)std::size(kPower) && g_settings.powerWarningActions[action];
}

inline float PowerBarCentreY(int slot = -1) {
    const float offset = power_ambient::Get().yOffset * g_layout.scale;
    if (slot >= 0 && slot < (int)g_layout.powerItems.size()) {
        const D2D1_RECT_F r = PowerButtonRect(slot);
        return (r.top + r.bottom) * .5f + offset;
    }
    return (g_layout.powerBar.top + g_layout.powerBar.bottom) * .5f + offset;
}

float PowerButtonCentreX(int slot) {
    const auto rect = PowerButtonRect(slot);
    return (rect.left + rect.right) * .5f;
}

// Find the adjacent warned buttons whose centres bracket this pointer.
int WarnedPowerPairAt(float x) {
    const auto& items = g_layout.powerItems;
    for (int right = 1; right < (int)items.size(); ++right) {
        const int left = right - 1;
        if (x < PowerButtonCentreX(left) || x >= PowerButtonCentreX(right)) continue;
        if (PowerWarningEnabled(items[left]) && PowerWarningEnabled(items[right])) return left;
        break;
    }
    return -1;
}

int PowerGapSlot(float x) {
    const int left = WarnedPowerPairAt(x);
    if (left < 0) return -1;
    const int previous = g_powerGlass.litSlot;
    if (previous == left || previous == left + 1) return previous;
    const float mid = (PowerButtonCentreX(left) + PowerButtonCentreX(left + 1)) * .5f;
    return x < mid ? left : left + 1;
}

// Which button the layer belongs to: the hovered one, or the one waiting for
// its second click. Closing the launcher or opening a folder hides it.
int PowerGlassSlot() {
    if (g_openFolder || g_batchBlend > 0 || g_layout.powerItems.empty()) return -1;
    if (g_powerArmed >= 0) {
        auto armed = std::find(g_layout.powerItems.begin(), g_layout.powerItems.end(),
                               g_powerArmed);
        if (armed != g_layout.powerItems.end())
            return (int)(armed - g_layout.powerItems.begin());
    }
    return g_powerHover;
}

int PowerWarningSlot(int slot) {
    if (g_openFolder || g_batchBlend > 0) return -1;
    if (slot >= 0 && slot < (int)g_layout.powerItems.size())
        return PowerWarningEnabled(g_layout.powerItems[slot]) ? slot : -1;
    return g_powerPointerInside ? PowerGapSlot(g_powerPointerX) : -1;
}

float PowerGlassTargetX(int active, int hovered) {
    if (active < 0) return g_powerGlass.x;
    const float center = PowerButtonCentreX(active);
    if (g_settings.powerWarningFollowButton || g_powerArmed >= 0) return center;
    if (hovered < 0 && active == g_powerGlass.litSlot) return g_powerGlass.targetX;
    if (hovered < 0 || !g_powerPointerInside) return center;
    // Keep the reflection on its button even when the mouse is at its edge.
    return std::lerp(center, g_powerPointerX, .26f);
}

struct PowerPointerTransfer {
    int left = -1;
    float rightShare = 0.f;
};

PowerPointerTransfer PowerTransferAtPointer() {
    if (!g_powerPointerInside || g_powerArmed >= 0) return {};
    const int left = WarnedPowerPairAt(g_powerPointerX);
    if (left < 0) return {};
    const float leftX = PowerButtonCentreX(left), rightX = PowerButtonCentreX(left + 1);
    const float pitch = rightX - leftX;
    if (pitch <= 0.f) return {};
    // Span both inner shoulders as well as the actual hit-test gap, so the
    // connection starts before the mouse leaves the first round button.
    const float t = std::clamp(((g_powerPointerX - leftX) / pitch - .27f) / .46f, 0.f, 1.f);
    if (t <= 0.f || t >= 1.f) return {};
    return {left, t * t * (3.f - 2.f * t)};
}

void SetPowerGlassAnchor(int slot) {
    g_powerGlass.litSlot = slot;
    g_powerGlass.weights.fill(0.f);
    if (slot >= 0) g_powerGlass.weights[slot] = 1.f;
}

void ResetPowerGlass() {
    g_powerGlass = {};
    g_powerGlass.tick = Now();
    g_powerPointerInside = false;
}

// Also used by the isolated preview to set up a settled frame.
void SettlePowerGlass() {
    const int slot = PowerGlassSlot();
    const int active = PowerWarningSlot(slot);
    const float targetX = PowerGlassTargetX(active, slot);
    SetPowerGlassAnchor(active);
    g_powerGlass.slot = slot;
    g_powerGlass.alpha = g_powerGlass.targetAlpha = active >= 0 ? 1.f : 0.f;
    g_powerGlass.x = g_powerGlass.targetX = targetX;
    g_powerGlass.y = PowerBarCentreY(active);
    g_powerGlass.press = slot >= 0 && g_layout.powerItems[slot] == g_powerPressed ? 1.f : 0.f;
    g_powerGlass.tick = Now();
}

void UpdatePowerGlass() {
    const auto& S = power_ambient::Get();
    const double now = Now();
    const float dtMs = (float)std::clamp((now - g_powerGlass.tick) * 1000., 0., 250.);
    g_powerGlass.tick = now;
    if (!Animate()) {
        SettlePowerGlass();
        return;
    }

    const int slot = PowerGlassSlot();
    const int active = PowerWarningSlot(slot);
    const float targetX = PowerGlassTargetX(active, slot);
    g_powerGlass.slot = slot;
    g_powerGlass.y = PowerBarCentreY(active);
    g_powerGlass.targetX = targetX;
    g_powerGlass.targetAlpha = active >= 0 ? 1.f : 0.f;
    const bool pressed = slot >= 0 && g_layout.powerItems[slot] == g_powerPressed;
    const float pressTarget = pressed ? 1.f : 0.f;
    const auto follow = [dtMs](float current, float target, float timeMs, float threshold) {
        const float next = std::lerp(target, current, std::exp(-dtMs / timeMs));
        return std::abs(target - next) < threshold ? target : next;
    };

    bool moving = false;
    if (active >= 0) {
        if (g_powerGlass.litSlot < 0 || g_powerGlass.alpha <= .002f) {
            SetPowerGlassAnchor(active);
            g_powerGlass.x = targetX;
        }
        const auto transfer = PowerTransferAtPointer();
        std::array<float, std::size(kPower)> targets{};
        targets[active] = 1.f;
        float time = S.transferMs;
        if (transfer.left >= 0) {
            targets[active] = 0.f;
            targets[transfer.left] = 1.f - transfer.rightShare;
            targets[transfer.left + 1] = transfer.rightShare;
            time = S.pointerTransferMs;
        }
        g_powerGlass.litSlot = active;
        for (size_t i = 0; i < targets.size(); ++i) {
            auto& weight = g_powerGlass.weights[i];
            weight = follow(weight, targets[i], time, .002f);
            moving |= weight != targets[i];
        }
        g_powerGlass.x = follow(g_powerGlass.x, targetX, S.positionMs, .02f);
    }
    const float alphaTime = g_powerGlass.targetAlpha > g_powerGlass.alpha ? S.riseMs : S.fallMs;
    g_powerGlass.alpha = follow(g_powerGlass.alpha, g_powerGlass.targetAlpha, alphaTime, .002f);
    g_powerGlass.press = follow(g_powerGlass.press, pressTarget,
                               pressed ? S.pressMs : S.releaseMs, .002f);
    if (moving || g_powerGlass.alpha != g_powerGlass.targetAlpha ||
        g_powerGlass.x != targetX || g_powerGlass.press != pressTarget) {
        g_glowAnimating = true;
        StartFrames();
    }
}

bool EnsurePowerAmbientBrush() {
    if (!g_target) return false;
    const D2D1_COLOR_F c = g_settings.powerWarningColor;
    const UINT32 rgb = (UINT32)(std::lround(c.r * 255.f) << 16 |
                                std::lround(c.g * 255.f) << 8 |
                                std::lround(c.b * 255.f));
    if (g_powerAmbient.brush && g_powerAmbient.generation == g_targetGeneration &&
        g_powerAmbient.colour == rgb)
        return true;
    const auto& S = power_ambient::Get();
    g_powerAmbient.brush.Reset();
    const D2D1_GRADIENT_STOP stops[] = {
        {0.f, D2D1::ColorF(c.r, c.g, c.b, 1.f)},
        {.35f, D2D1::ColorF(c.r, c.g, c.b, S.stop35)},
        {.65f, D2D1::ColorF(c.r, c.g, c.b, S.stop65)},
        {1.f, D2D1::ColorF(c.r, c.g, c.b, 0.f)},
    };
    ComPtr<ID2D1GradientStopCollection> collection;
    if (FAILED(g_target->CreateGradientStopCollection(stops, ARRAYSIZE(stops),
                                                      &collection)) ||
        FAILED(g_target->CreateRadialGradientBrush(
            D2D1::RadialGradientBrushProperties(D2D1::Point2F(0, 0),
                                                D2D1::Point2F(0, 0), 1.f, 1.f),
            collection.Get(), &g_powerAmbient.brush)))
        return false;
    g_powerAmbient.generation = g_targetGeneration;
    g_powerAmbient.colour = rgb;
    return true;
}

// Cached rounded geometry, one for the capsule and one for the text pill.
ID2D1RoundedRectangleGeometry* PowerAmbientClip(bool tip, const D2D1_RECT_F& rect,
                                                float radius) {
    auto& entry = tip ? g_powerAmbient.tip : g_powerAmbient.bar;
    if (entry.geometry && entry.left == rect.left && entry.top == rect.top &&
        entry.right == rect.right && entry.bottom == rect.bottom &&
        entry.radius == radius) {
        return entry.geometry.Get();
    }
    entry.geometry.Reset();
    if (!g_d2d) return nullptr;
    if (FAILED(g_d2d->CreateRoundedRectangleGeometry(
            D2D1::RoundedRect(rect, radius, radius), &entry.geometry)))
        return nullptr;
    entry.left = rect.left;
    entry.top = rect.top;
    entry.right = rect.right;
    entry.bottom = rect.bottom;
    entry.radius = radius;
    return entry.geometry.Get();
}

// Draw the warning inside the dock's glass, centred vertically on the button.
void DrawPowerAmbient(float opacity) {
    const auto& S = power_ambient::Get();
    if (g_powerGlass.alpha <= .001f || opacity <= 0) return;
    if (!EnsurePowerAmbientBrush()) return;

    const float s = g_layout.scale;
    const D2D1_RECT_F bar = g_layout.powerBar;
    // Keep the capsule's own hairline rim: the tint belongs to the glass body,
    // not to the outline of the control.
    const float inset = std::max(1.f, 1.5f * s);
    const D2D1_RECT_F inner{bar.left + inset, bar.top + inset, bar.right - inset,
                            bar.bottom - inset};
    const float radius = std::max(0.f, (bar.bottom - bar.top) * .5f - inset);
    auto* clip = PowerAmbientClip(false, inner, radius);
    if (!clip) return;

    const float pressScale = std::lerp(1.f, S.pressScale, g_powerGlass.press);
    const float spread = g_settings.powerWarningSpread / 100.f;
    const float centre = g_settings.powerWarningStrength / 100.f;
    const float fade = g_powerGlass.alpha * opacity;
    const int destination = g_powerGlass.litSlot;
    if (destination < 0 || destination >= (int)g_layout.powerItems.size()) return;
    const float cy = std::clamp(g_powerGlass.y, inner.top, inner.bottom);
    const float radiusX = S.radiusX * s * spread * pressScale;
    const float radiusY = S.radiusY * s * spread * pressScale;

    g_target->PushLayer(D2D1::LayerParameters1(D2D1::InfiniteRect(), clip,
                                               D2D1_ANTIALIAS_MODE_PER_PRIMITIVE),
                        nullptr);

    auto* brush = g_powerAmbient.brush.Get();
    const D2D1_RECT_F edge{inner.left, inner.bottom - 5.f * s, inner.right, inner.bottom};
    const float edgeShare = (g_settings.powerWarningEdge / 100.f) * .45f;
    const auto drawLobe = [&](float x, float rx, float ry, float weight) {
        brush->SetCenter({x, cy});
        brush->SetRadiusX(rx);
        brush->SetRadiusY(ry);
        brush->SetOpacity(centre * fade * weight);
        g_target->FillRectangle(inner, brush);
        brush->SetOpacity(centre * fade * weight * edgeShare);
        g_target->FillRectangle(edge, brush);
    };

    // Connect adjacent lobes in spatial order. Hover can change while the
    // lobes are mixed, so using the hovered button as the destination here
    // would flip the shape at the hit-test boundary.
    const auto& weights = g_powerGlass.weights;
    for (int right = 1; right < (int)g_layout.powerItems.size(); ++right) {
        const float leftWeight = weights[right - 1], rightWeight = weights[right];
        if (leftWeight <= .002f || rightWeight <= .002f) continue;
        const float leftX = PowerButtonCentreX(right - 1), rightX = PowerButtonCentreX(right);
        const float neck = 2.f * std::min(leftWeight, rightWeight);
        const float flow = rightWeight / (leftWeight + rightWeight);
        drawLobe(std::lerp(leftX, rightX, .18f + .64f * flow),
                 (rightX - leftX) * (.40f + .16f * neck) + .20f * radiusX,
                 radiusY * (.17f + .24f * neck), .54f * neck);
    }
    for (int slot = 0; slot < (int)g_layout.powerItems.size(); ++slot) {
        const float weight = weights[slot];
        if (weight <= .002f) continue;
        const float rootWeight = std::sqrt(weight);
        const float size = .30f + .70f * rootWeight;
        const float stretch = .32f * (1.f - weight);
        const float anchor = PowerButtonCentreX(slot);
        // In button mode the centres never move. Mouse mode can nudge the
        // nearly settled lobe, but the crossing itself still uses anchors.
        const float local = !g_settings.powerWarningFollowButton && slot == destination
            ? Ease(std::clamp((weight - .82f) / .18f, 0.f, 1.f)) : 0.f;
        const float x = std::lerp(anchor, g_powerGlass.x, local);
        const float brightness = std::lerp(weight, rootWeight, .55f);
        drawLobe(std::clamp(x, inner.left, inner.right), radiusX * size * (1.f + stretch),
                 radiusY * size * (1.f - stretch * .65f), brightness);
    }

    g_target->PopLayer();
}

// The reflection enters the label along the vertical axis of its button.
void DrawPowerAmbientBleed(const D2D1_RECT_F& tip, float buttonCenterX, float opacity) {
    const auto& S = power_ambient::Get();
    if (g_powerGlass.alpha <= .001f || opacity <= 0) return;
    if (!EnsurePowerAmbientBrush()) return;

    const float s = g_layout.scale;
    const float inset = std::max(1.f, 1.5f * s);
    const D2D1_RECT_F inner{tip.left + inset, tip.top + inset, tip.right - inset,
                            tip.bottom - inset};
    const float radius = std::max(0.f, (tip.bottom - tip.top) * .5f - inset);
    auto* clip = PowerAmbientClip(true, inner, radius);
    if (!clip) return;

    const float cx = std::clamp(buttonCenterX, inner.left, inner.right);
    // Inside the pill's lower edge: the reflection enters from the capsule
    // below and dies out before the text line.
    const float cy = tip.bottom - 2.f * s;
    const float fade = g_powerGlass.alpha * opacity;
    const float centre = g_settings.powerWarningStrength / 100.f;
    const float spread = g_settings.powerWarningSpread / 100.f;

    g_target->PushLayer(D2D1::LayerParameters1(D2D1::InfiniteRect(), clip,
                                               D2D1_ANTIALIAS_MODE_PER_PRIMITIVE),
                        nullptr);
    auto* brush = g_powerAmbient.brush.Get();
    brush->SetCenter({cx, cy});
    brush->SetRadiusX(S.bleedRadiusX * s * spread);
    brush->SetRadiusY(S.bleedRadiusY * s * spread);
    brush->SetOpacity(centre * (g_settings.powerWarningBleed / 100.f) * fade);
    g_target->FillRectangle(inner, brush);
    g_target->PopLayer();
}

void DrawPowerGlyph(const D2D1_RECT_F& box, WCHAR glyph, float opacity) {
    if (!g_iconFormat || opacity <= 0) return;
    auto& text = GetLauncherText(std::wstring_view(&glyph, 1), g_iconFormat.Get(),
                                 box.right - box.left, box.bottom - box.top,
                                 LauncherTextAlignment::Format, 1);
    if (!text.layout) return;
    // The glyph stays white over the warning glass; the material carries the
    // warning, not the icon.
    text.Draw({box.left, box.top}, opacity, g_layout.scale);
}

void DrawPowerBarContent(float opacity) {
    const Layout& L=g_layout;
    if(L.powerItems.empty())return;
    float s=L.scale;
    UpdatePowerGlass();
    DrawPowerAmbient(opacity);
    for (int slot = 0; slot < (int)L.powerItems.size(); slot++) {
        const int action = L.powerItems[slot];
        const PowerButton& b = kPower[action];
        const D2D1_RECT_F r = PowerButtonRect(slot);
        const bool pressed = slot == g_powerHover && !g_openFolder && action == g_powerPressed;
        const D2D1_RECT_F glyphBox{r.left, r.top + (pressed ? s : 0), r.right, r.bottom + (pressed ? s : 0)};
        DrawPowerGlyph(glyphBox, b.glyph, opacity);
        if (g_powerUpdateStatus == PowerUpdateStatus::RestartRequired &&
            IsUpdatePowerAction(static_cast<PowerAction>(action))) {
            DrawPowerUpdateDot(r, opacity);
        }
    }

    // The label of the button under the mouse, or the confirmation prompt,
    // in a small pill above the bar.
    const auto armed = std::find(L.powerItems.begin(), L.powerItems.end(), g_powerArmed);
    const bool confirming = armed != L.powerItems.end();
    const int tipSlot = confirming ? (int)(armed - L.powerItems.begin()) : g_openFolder ? -1 : g_powerHover;
    if (tipSlot < 0 || tipSlot >= (int)L.powerItems.size() || !g_tipFormat) {
        return;
    }
    const PowerButton& b = kPower[L.powerItems[tipSlot]];
    std::wstring text = confirming
                            ? launchpad_i18n::Text(std::wstring(L"Click again to ") + b.verb)
                            : std::wstring(Tr(b.label));
    if (g_powerUpdateStatus == PowerUpdateStatus::RestartRequired &&
        IsUpdatePowerAction(static_cast<PowerAction>(L.powerItems[tipSlot]))) {
        text += L" · ";
        text += Tr(L"Windows Update needs a restart");
    }
    ComPtr<IDWriteTextLayout> layout;
    g_dwrite->CreateTextLayout(text.c_str(), (UINT32)text.size(),
                               g_tipFormat.Get(), 1000 * s, 100 * s, &layout);
    if (!layout) {
        return;
    }
    DWRITE_TEXT_METRICS m{};
    layout->GetMetrics(&m);
    D2D1_RECT_F button = PowerButtonRect(tipSlot);
    float w = std::min(std::ceil(m.width) + 24 * s, std::max(1.f, L.width - 16 * s));
    float h = std::round(28 * s);
    float cx = (button.left + button.right) / 2;
    float left = std::round(std::clamp(cx - w / 2, 8 * s, std::max(8 * s, L.width - w - 8 * s)));
    float bottom = L.powerBar.top - std::round(8 * s);
    D2D1_RECT_F tip = D2D1::RectF(left, bottom - h, left + w, bottom);
    DrawGlassPill(tip,opacity,0);
    if (PowerWarningEnabled(L.powerItems[tipSlot]))
        DrawPowerAmbientBleed(tip, cx, opacity);
    DrawLauncherText(text, g_tipFormat.Get(), tip, opacity);
}
std::wstring CountLabel(PCWSTR key,size_t count) {
    std::wstring text=Tr(key);
    auto offset=text.find(L"{0}");
    if(offset!=std::wstring::npos)text.replace(offset,3,std::to_wstring(count));
    return text;
}
void DrawBatchBar(float sceneOpacity) {
    float opacity=Ease(g_batchBlend)*sceneOpacity;
    auto bar=BatchBar();
    float s=g_layout.scale,width=bar.right-bar.left;
    float left=bar.left+5*s,right=bar.right-5*s;
    float middle1=bar.left+width*.24f,middle2=bar.left+width*.50f,middle3=bar.left+width*.75f;
    D2D1_RECT_F slots[]{{left,bar.top+5*s,middle1,bar.bottom-5*s},
                        {middle1,bar.top,middle2,bar.bottom},
                        {middle2,bar.top+5*s,middle3,bar.bottom-5*s},
                        {middle3,bar.top+5*s,right,bar.bottom-5*s}};
    std::wstring labels[]{Tr(L"Cancel"),CountLabel(L"Selected {0}",g_batchDisplayCount),
                          Tr(L"Delete"),CountLabel(L"Open {0}",g_batchDisplayCount)};
    for(int i=0;i<4;i++) {
        int action=i==0?0:i==3?1:i==2?2:-1;
        if(action>=0) {
            bool hover=g_batchHover==action;
            auto accent=D2D1::ColorF(0xFFFFFF);
            accent.a=opacity*(hover?(g_batchPressed==action?.36f:.25f):i==3?.14f:0.f);
            Fill(slots[i],accent,18*s);
        }
        DrawLauncherText(labels[i], g_tipFormat.Get(), slots[i], opacity);
    }
}
void DrawToolBar(bool afterFolder, float opacity) {
    if (opacity <= 0.f) return;
    float blend=Ease(g_batchBlend);
    if(afterFolder&&(!g_openFolder||blend<=0))return;
    if(afterFolder||!g_openFolder||blend<=0)
        DrawGlassPill(ToolBarBounds(), opacity * (g_layout.powerItems.empty() ? blend : 1.f), 0);
    if(!afterFolder&&!g_layout.powerItems.empty()&&blend<1) {
        if(blend>0)g_target->PushLayer(D2D1::LayerParameters1(D2D1::InfiniteRect(),nullptr,
            D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,D2D1::IdentityMatrix(),1-blend),nullptr);
        DrawPowerBarContent(opacity);
        if(blend>0)g_target->PopLayer();
    }
    if(blend>0&&(afterFolder||!g_openFolder))DrawBatchBar(opacity);
}
void DrawAppSelectionRect() {
    if(!g_selectionPointer.active)return;
    auto r=SelectionIntersection(SelectionRect(),SelectionViewport());
    if(!SelectionHasArea(r))return;
    Fill(r,D2D1::ColorF(0xFFFFFF,.13f),3*g_layout.scale);
    g_brush->SetColor(D2D1::ColorF(0xFFFFFF,.80f));
    g_target->DrawRectangle(r,g_brush.Get(),g_layout.scale);
}


////////////////////////////////////////////////////////////////////////////////
// The search field as a text editor: caret, selection and the usual keys.
// Positions are UTF-16 indices into g_query; a step crosses a whole surrogate
// pair so characters outside the basic plane stay whole.

void QueryChanged();
void Render();

// The field the editor works on: the search box, or a folder's name while
// its folder is open.
std::wstring& EditText() {
    return g_openFolder ? g_openFolder->name : g_query;
}

size_t MaxEditChars() {
    return g_openFolder ? 60 : 200;
}

// A rename only redraws; the search also refilters the apps.
void EditChanged() {
    if (g_openFolder) {
        Render();
    } else {
        ClearAppSelection();
        QueryChanged();
    }
}

bool SearchSelection(size_t* from, size_t* to) {
    size_t size = EditText().size();
    *from = std::min(std::min(g_anchor, g_caret), size);
    *to = std::min(std::max(g_anchor, g_caret), size);
    return *from != *to;
}

size_t CaretStep(size_t pos, int dir) {
    const std::wstring& text = EditText();
    if (dir < 0) {
        pos = std::min(pos, text.size());
        if (pos >= 2 && IS_LOW_SURROGATE(text[pos - 1]) &&
            IS_HIGH_SURROGATE(text[pos - 2])) {
            return pos - 2;
        }
        return pos ? pos - 1 : 0;
    }
    if (pos + 1 < text.size() && IS_HIGH_SURROGATE(text[pos]) &&
        IS_LOW_SURROGATE(text[pos + 1])) {
        return pos + 2;
    }
    return std::min(pos + 1, text.size());
}

bool IsWordChar(WCHAR ch) {
    return iswalnum(ch) || ch == L'_';
}

size_t WordStart(size_t pos) {
    const std::wstring& text = EditText();
    size_t i = std::min(pos, text.size());
    while (i > 0 && !IsWordChar(text[i - 1])) {
        i--;
    }
    while (i > 0 && IsWordChar(text[i - 1])) {
        i--;
    }
    return i;
}

size_t WordEnd(size_t pos) {
    const std::wstring& text = EditText();
    size_t n = text.size(), i = std::min(pos, n);
    while (i < n && !IsWordChar(text[i])) {
        i++;
    }
    while (i < n && IsWordChar(text[i])) {
        i++;
    }
    return i;
}

void TouchCaret() {  // Restarts the blink, like after every keystroke.
    g_caretTick = GetTickCount64();
}

// Undo and redo for the field being edited. A snapshot is the whole field,
// which is small. Consecutive keystrokes of one kind merge into one step;
// pastes and cuts stay their own step. The folder's history resets each time
// a folder opens, so one folder's steps never land on another's name.
struct EditSnapshot {
    std::wstring text;
    size_t caret;
    size_t anchor;
};
struct EditHistory {
    std::vector<EditSnapshot> undo;
    std::vector<EditSnapshot> redo;
    int lastOp = 0;  // 0 paste or cut, 1 typing, 2 delete
    size_t lastEnd = 0;
    ULONGLONG lastTick = 0;
};
EditHistory g_hist[2];

void EditPush(int op) {  // Snapshot the field before a mutation.
    EditHistory& h = g_hist[g_openFolder != nullptr];
    ULONGLONG now = GetTickCount64();
    bool merge = (op == 1 || op == 2) && op == h.lastOp &&
                 now - h.lastTick < 1000 && g_caret == h.lastEnd &&
                 !h.undo.empty();
    if (!merge) {
        h.undo.push_back({EditText(), g_caret, g_anchor});
        if (h.undo.size() > 100) {
            h.undo.erase(h.undo.begin());
        }
        h.redo.clear();
    }
    h.lastOp = op;
    h.lastTick = now;
}

void EditDone() {  // Remember where the mutation left the caret, for merging.
    g_hist[g_openFolder != nullptr].lastEnd = g_caret;
}

void UndoEdit() {
    EditHistory& h = g_hist[g_openFolder != nullptr];
    if (h.undo.empty()) {
        return;
    }
    h.redo.push_back({EditText(), g_caret, g_anchor});
    if (h.redo.size() > 100) {
        h.redo.erase(h.redo.begin());
    }
    EditSnapshot s = h.undo.back();
    h.undo.pop_back();
    EditText() = s.text;
    g_caret = std::min(s.caret, EditText().size());
    g_anchor = std::min(s.anchor, EditText().size());
    h.lastOp = 0;
    TouchCaret();
    EditChanged();
}

void RedoEdit() {
    EditHistory& h = g_hist[g_openFolder != nullptr];
    if (h.redo.empty()) {
        return;
    }
    h.undo.push_back({EditText(), g_caret, g_anchor});
    if (h.undo.size() > 100) {
        h.undo.erase(h.undo.begin());
    }
    EditSnapshot s = h.redo.back();
    h.redo.pop_back();
    EditText() = s.text;
    g_caret = std::min(s.caret, EditText().size());
    g_anchor = std::min(s.anchor, EditText().size());
    h.lastOp = 0;
    TouchCaret();
    EditChanged();
}

bool CaretVisible() {
    UINT blink = GetCaretBlinkTime();
    if (!blink || blink > 5000) {
        blink = 530;  // The Windows default.
    }
    return (GetTickCount64() - g_caretTick) / blink % 2 == 0;
}

void SetCaret(size_t pos, bool extend) {
    g_caret = std::min(pos, EditText().size());
    if (!extend) {
        g_anchor = g_caret;
    }
    g_hist[g_openFolder != nullptr].lastOp = 0;  // A caret move starts a new
    TouchCaret();                                // undo step, like anywhere.
}

bool DeleteSelection() {
    size_t from, to;
    if (!SearchSelection(&from, &to)) {
        return false;
    }
    EditText().erase(from, to - from);
    g_caret = g_anchor = from;
    TouchCaret();
    return true;
}

void InsertAtCaret(std::wstring text) {
    DeleteSelection();
    size_t max = MaxEditChars();
    size_t room = max - std::min(EditText().size(), max);
    if (text.size() > room) {
        text.resize(room);
    }
    if (text.empty()) {
        return;
    }
    g_caret = std::min(g_caret, EditText().size());
    EditText().insert(g_caret, text);
    g_caret = g_anchor = g_caret + text.size();
    TouchCaret();
}

void CopySelection(bool cut) {
    size_t from, to;
    if (!SearchSelection(&from, &to) || !OpenClipboard(g_wnd)) {
        return;
    }
    if (EmptyClipboard()) {
        if (HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE,
                                      (to - from + 1) * sizeof(WCHAR))) {
            if (WCHAR* p = (WCHAR*)GlobalLock(mem)) {
                memcpy(p, EditText().c_str() + from,
                       (to - from) * sizeof(WCHAR));
                p[to - from] = 0;
                GlobalUnlock(mem);
                if (!SetClipboardData(CF_UNICODETEXT, mem)) {
                    GlobalFree(mem);
                }
            } else {
                GlobalFree(mem);
            }
        }
    }
    CloseClipboard();
    if (cut) {
        EditPush(0);
        DeleteSelection();
        EditDone();
        EditChanged();
    }
}

// The search field's text box, or the open folder's name.
D2D1_RECT_F FolderTitlePill() {
    auto title = GetFolderLayout().title;
    float scale = g_layout.scale;
    float width = 140 * scale;
    if (g_openFolder && g_dwrite && g_messageFormat) {
        ComPtr<IDWriteTextLayout> layout;
        const auto& name = g_openFolder->name;
        if (SUCCEEDED(g_dwrite->CreateTextLayout(name.c_str(), (UINT32)name.size(),
                g_messageFormat.Get(), 10000, 100 * scale, &layout))) {
            DWRITE_TEXT_METRICS metrics{};
            layout->GetMetrics(&metrics);
            width = std::max(width, metrics.widthIncludingTrailingWhitespace + 36 * scale);
        }
    }
    width = std::min(width, title.right-title.left);
    float cx = (title.left+title.right)/2, cy = (title.top+title.bottom)/2;
    return D2D1::RectF(cx-width/2, cy-17*scale, cx+width/2, cy+17*scale);
}

D2D1_RECT_F EditBox(bool folder) {
    if (folder) {
        auto r = FolderTitlePill();
        r.left += 14 * g_layout.scale;
        r.right -= 14 * g_layout.scale;
        return r;
    }
    const D2D1_RECT_F& r = g_layout.search;
    float s = g_layout.scale;
    if (!SearchExpanded()) return r;
    float right = g_query.empty() ? r.right - 14*s : g_layout.clearButton.left - 4*s;
    return D2D1::RectF(r.left+36*s, r.top, std::max(r.left+36*s, right), r.bottom);
}

// A text field as drawn: its layout, the box it is clipped to, and where the
// layout's left edge sits. Text too long for the box slides so the caret
// stays in view; a folder name that fits stays centered. A null layout when
// there is no format.
struct EditView {
    LauncherText* text = nullptr;
    IDWriteTextLayout* layout = nullptr;
    D2D1_RECT_F box;
    float originX,originY;
    size_t caret = 0, compositionStart = 0, compositionLength = 0;
};
float SearchTextTop(const D2D1_RECT_F& box) {
    static UINT version=0;
    static float inkCenter=0;
    if(version!=g_formatVersion&&g_searchFormat) {
        version=g_formatVersion;
        ComPtr<IDWriteTextLayout> sample;
        if(SUCCEEDED(g_dwrite->CreateTextLayout(L"Ag中",3,g_searchFormat.Get(),1000,100,&sample))) {
            sample->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
            DWRITE_OVERHANG_METRICS ink{};
            if(SUCCEEDED(sample->GetOverhangMetrics(&ink)))inkCenter=(100+ink.bottom-ink.top)/2;
        }
    }
    return (box.top+box.bottom)/2-inkCenter;
}

EditView GetEditView(bool folder) {
    EditView v{};
    v.box = EditBox(folder);
    v.originX = v.box.left;
    v.originY = folder?v.box.top:SearchTextTop(v.box);
    IDWriteTextFormat* format =
        folder ? g_messageFormat.Get() : g_searchFormat.Get();
    if (!format || (folder && !g_openFolder)) {
        return v;
    }
    const auto& committed = folder ? g_openFolder->name : g_query;
    std::wstring composed;
    std::wstring_view text = committed;
    v.caret = std::min(g_caret, text.size());
    if (g_imeComposition.active && (folder ? g_folderRenaming : !g_openFolder)) {
        size_t from, to;
        SearchSelection(&from, &to);
        v.compositionStart = from;
        v.compositionLength = g_imeComposition.text.size();
        if (v.compositionLength) {
            composed = committed;
            composed.replace(from, to - from, g_imeComposition.text);
            text = composed;
            v.caret = from + std::min(g_imeComposition.cursor, v.compositionLength);
        }
    }
    // An empty field still needs a layout for caret positioning.
    v.text = &GetLauncherText(text, format, 10000, v.box.bottom - v.box.top,
                              folder ? LauncherTextAlignment::Leading : LauncherTextAlignment::LeadingTop);
    v.layout = v.text->layout.Get();
    if (!v.layout) {
        return v;
    }
    DWRITE_TEXT_METRICS metrics{};
    v.layout->GetMetrics(&metrics);
    float width = metrics.widthIncludingTrailingWhitespace;
    float room = std::max(0.f, v.box.right - v.box.left - 3 * g_layout.scale);
    if (folder && width <= room) {
        v.originX =
            std::round(v.box.left + (v.box.right - v.box.left - width) / 2);
        return v;
    }
    float endShift = std::max(0.0f, width - room);
    float shift = endShift;
    DWRITE_HIT_TEST_METRICS m{};
    float x = 0, y = 0;
    bool editing = folder ? g_folderRenaming : !g_openFolder;
    if (editing &&
        SUCCEEDED(v.layout->HitTestTextPosition(
            (UINT32)v.caret, FALSE, &x, &y, &m))) {
        if (x - shift < 0) {
            shift = std::min(std::max(x, 0.0f), endShift);
        }
        if (x - shift > room) {
            shift = std::min(std::max(x - room, 0.0f), endShift);
        }
    }
    v.originX = v.box.left - shift;
    return v;
}

bool CanEditWithIme() {
    return g_shown && !g_closing && !g_groupEditMode && !g_drag.active &&
           !g_drag.pending && (!g_openFolder || g_folderRenaming);
}

POINT ImePoint(const EditView& view, size_t offset) {
    float x = 0, y = 0;
    DWRITE_HIT_TEST_METRICS metrics{};
    if (view.layout) view.layout->HitTestTextPosition((UINT32)offset, FALSE, &x, &y, &metrics);
    return {(LONG)std::lround(std::clamp(view.originX + x, view.box.left, view.box.right)),
            (LONG)std::lround(std::clamp(view.originY + y, view.box.top, view.box.bottom))};
}

RECT ImeEditRect(const EditView& view) {
    return {(LONG)view.box.left, (LONG)view.box.top, (LONG)view.box.right, (LONG)view.box.bottom};
}

// The launcher draws composition text inline. The input method owns its
// candidate UI and receives its location in the expanded field.
void PositionIme(HWND hwnd, DWORD candidates = 1) {
    HIMC ime = ImmGetContext(hwnd);
    if (!ime) return;
    const auto view = GetEditView(g_openFolder != nullptr);
    const auto caret = ImePoint(view, view.caret);
    COMPOSITIONFORM composition{};
    composition.dwStyle = CFS_POINT;
    composition.ptCurrentPos = caret;
    ImmSetCompositionWindow(ime, &composition);
    for (DWORD index = 0; index < 4; ++index) {
        if (!(candidates & (1u << index))) continue;
        CANDIDATEFORM candidate{};
        candidate.dwIndex = index;
        candidate.dwStyle = CFS_EXCLUDE;
        candidate.rcArea = ImeEditRect(view);
        candidate.ptCurrentPos = {caret.x, candidate.rcArea.bottom};
        ImmSetCandidateWindow(ime, &candidate);
    }
    ImmReleaseContext(hwnd, ime);
}

bool QueryImePosition(HWND hwnd, IMECHARPOSITION* position) {
    if (!position || position->dwSize < sizeof(*position) || !CanEditWithIme() ||
        !g_imeComposition.active) return false;
    const auto view = GetEditView(g_openFolder != nullptr);
    if (position->dwCharPos > view.compositionLength) return false;
    position->pt = ImePoint(view, view.compositionStart + position->dwCharPos);
    position->rcDocument = ImeEditRect(view);
    position->cLineHeight = std::max(1L, position->rcDocument.bottom - position->rcDocument.top);
    MapWindowPoints(hwnd, HWND_DESKTOP, &position->pt, 1);
    MapWindowPoints(hwnd, HWND_DESKTOP, reinterpret_cast<POINT*>(&position->rcDocument), 2);
    return true;
}

std::wstring ReadImeString(HIMC context, DWORD part) {
    if (!context) return {};
    const LONG bytes = ImmGetCompositionStringW(context, part, nullptr, 0);
    if (bytes <= 0 || bytes > 65536 || bytes % sizeof(WCHAR)) return {};
    std::wstring text(bytes / sizeof(WCHAR), L'\0');
    if (ImmGetCompositionStringW(context, part, text.data(), bytes) != bytes) return {};
    return text;
}

void RevealImeSearch() {
    if (g_openFolder) return;
    if (g_searchActivated && g_searchExpand == 1.f) return;
    ClearAppSelection();
    g_searchActivated = true;
    // Give the candidate window its final position before the IME shows it.
    g_searchExpand = 1.f;
    ComputeLayout();
    StartFrames();
}

void CancelImeComposition(HWND hwnd) {
    if (!g_imeComposition.active) return;
    g_imeComposition = {};
    if (HIMC ime = ImmGetContext(hwnd)) {
        ImmNotifyIME(ime, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
        ImmReleaseContext(hwnd, ime);
    }
}

// The caret index under a mouse x, in the field being edited.
size_t CaretAtPoint(float x) {
    EditView v = GetEditView(g_openFolder != nullptr);
    size_t size = EditText().size();
    if (!v.layout) {
        return size;
    }
    BOOL trailing = FALSE, inside = FALSE;
    DWRITE_HIT_TEST_METRICS m{};
    float px = std::max(
        std::min(std::max(x, v.box.left), v.box.right) - v.originX, 0.0f);
    if (FAILED(v.layout->HitTestPoint(px, (v.box.bottom + v.box.top) / 2-v.originY,
                                      &trailing, &inside, &m))) {
        return size;
    }
    return std::min((size_t)(m.textPosition + (trailing ? m.length : 0)), size);
}

// The text, with the selection behind it and a caret that blinks like
// everywhere else in Windows, when this is the field being edited.
void DrawEditView(const EditView& v, bool editing, float caretHeight, float opacity = 1.f) {
    if (!v.layout) {
        return;
    }
    const D2D1_RECT_F& box = v.box;
    float caretW = std::max(1.0f, g_layout.scale);
    g_target->PushAxisAlignedClip(
        D2D1::RectF(box.left - caretW - 1, box.top, box.right, box.bottom),
        D2D1_ANTIALIAS_MODE_ALIASED);
    size_t selFrom, selTo;
    if (editing && !v.compositionLength && SearchSelection(&selFrom, &selTo)) {
        DWRITE_HIT_TEST_METRICS rects[8];
        UINT32 count = 0;
        if (SUCCEEDED(v.layout->HitTestTextRange((UINT32)selFrom,
                                                 (UINT32)(selTo - selFrom), 0,
                                                 0, rects, 8, &count))) {
            g_brush->SetColor(D2D1::ColorF(0xFFFFFF, 0.35f * opacity));
            for (UINT32 i = 0; i < count && i < 8; i++) {
                g_target->FillRectangle(
                    D2D1::RectF(v.originX + rects[i].left,
                                v.originY + rects[i].top,
                                v.originX + rects[i].left + rects[i].width,
                                v.originY + rects[i].top + rects[i].height),
                    g_brush.Get());
            }
        }
    }
    v.text->Draw({v.originX, v.originY}, opacity, g_layout.scale);
    if (v.compositionLength) {
        DWRITE_HIT_TEST_METRICS ranges[8];
        UINT32 count = 0;
        if (SUCCEEDED(v.layout->HitTestTextRange((UINT32)v.compositionStart,
                      (UINT32)v.compositionLength, v.originX, v.originY, ranges, ARRAYSIZE(ranges), &count))) {
            const float y = (box.top + box.bottom + caretHeight) / 2 + g_layout.scale;
            g_brush->SetColor(D2D1::ColorF(0xFFFFFF, .8f * opacity));
            for (UINT32 i = 0; i < count; ++i)
                g_target->DrawLine({ranges[i].left, y}, {ranges[i].left + ranges[i].width, y},
                                   g_brush.Get(), g_layout.scale);
        }
    }
    if (editing && (v.compositionLength || CaretVisible())) {
        DWRITE_HIT_TEST_METRICS m{};
        float x = 0, y = 0;
        v.layout->HitTestTextPosition(
            (UINT32)v.caret, FALSE, &x, &y, &m);
        // Just left of the insertion point, so it never touches the first
        // glyph.
        float caretX = std::round(v.originX + (float)x) - caretW;
        float mid = (box.top + box.bottom) / 2;
        Fill(D2D1::RectF(caretX, mid - caretHeight / 2, caretX + caretW,
                         mid + caretHeight / 2),
             D2D1::ColorF(0xFFFFFF, 0.9f * opacity));
    }
    g_target->PopAxisAlignedClip();
}

void DrawSearchBox(float opacity) {
    const Layout& L = g_layout;
    float s = L.scale;
    const D2D1_RECT_F& r = L.search;
    // No resting icon any more: the whole field fades in only while opening,
    // so at rest the top of the window is just the grid, one row higher.
    float t = std::clamp(g_searchExpand, 0.f, 1.f);
    if (t < 0.02f) return;
    float fade = std::min(1.f, t * 6.f);
    t = t * t * (3 - 2 * t);
    DrawGlassPill(r,opacity*fade,g_searchGlow*fade);

    float lens=5.5f*s, handle=8.5f*s;
    // The magnifier rides along with the opening: centred in the pill, 18 px in
    // once the field is out.
    float midX = (r.left + r.right) * 0.5f +
                 (r.left + 18 * s - (r.left + r.right) * 0.5f) * t;
    float cx=midX+(lens-handle)/2,cy=(r.top+r.bottom+lens-handle)/2;
    g_brush->SetColor(D2D1::ColorF(0xFFFFFF,.85f*opacity*fade));
    g_target->DrawEllipse(D2D1::Ellipse({cx,cy},lens,lens),g_brush.Get(),1.6f*s);
    g_target->DrawLine({cx+4*s,cy+4*s},{cx+handle,cy+handle},g_brush.Get(),1.6f*s);

    // The text follows the box out and is clipped to it, so it slides in
    // instead of popping up at full width.
    if (g_searchExpand < 0.02f) return;
    if (g_query.empty() && g_imeComposition.text.empty() && g_searchFormat) {
        auto hint = EditBox(false);
        hint.left += 4*s;
        PCWSTR prompt = Tr(L"Search");
        DrawLauncherText(prompt, g_searchFormat.Get(), hint, .85f * opacity * fade);
    }
    DrawEditView(GetEditView(false), !g_openFolder, 18 * s, opacity * fade);
}

void DrawMessage(PCWSTR text) {
    const Layout& L = g_layout;
    if (!g_messageFormat) {
        return;
    }
    DrawLauncherText(text, g_messageFormat.Get(),
                     D2D1::RectF(0, L.viewTop, (float)L.width, L.viewTop + 120 * L.scale),
                     FlyInChromeOpacity());
}

bool IsDragged(const Tile& tile) {
    return g_drag.active && std::find(g_drag.tiles.begin(), g_drag.tiles.end(), tile) != g_drag.tiles.end();
}

const std::wstring& TileName(const Tile& tile) {
    return tile.folder ? tile.folder->name : tile.app->name;
}

void DrawTileIcon(const Tile& tile, const D2D1_RECT_F& r, float opacity) {
    float side = r.right - r.left;
    if (tile.folder) {
        // A rounded square with the first nine apps.
        Fill(r, D2D1::ColorF(0x424242, 0.55f * opacity), side * 0.22f);
        // A resting edge (Mocha overlay0) so the folder stands off the backdrop.
        float w = 1.25f * g_layout.scale, in = w / 2;
        g_brush->SetColor(D2D1::ColorF(0xD0D0D0, 0.6f * opacity));
        g_target->DrawRoundedRectangle(
            D2D1::RoundedRect(D2D1::RectF(r.left + in, r.top + in, r.right - in, r.bottom - in),
                              side * 0.22f - in, side * 0.22f - in),
            g_brush.Get(), w);
        float pad = side * 0.12f, gap = side * 0.04f;
        float mini = (side - 2 * pad - 2 * gap) / 3;
        const Tiles& apps = tile.folder->apps;
        for (int i = 0; i < 9 && i < (int)apps.size(); i++) {
            float x = r.left + pad + (i % 3) * (mini + gap);
            float y = r.top + pad + (i / 3) * (mini + gap);
            if (ID2D1Bitmap* bitmap = GetBitmap(*apps[i].app)) {
                g_target->DrawBitmap(bitmap,
                                     D2D1::RectF(x, y, x + mini, y + mini),
                                     opacity,
                                     D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
            }
        }
        return;
    }
    // No backplate: an icon without a bitmap draws as nothing, like
    // Launchpad's blank tiles.
    if (ID2D1Bitmap* bitmap = GetBitmap(*tile.app)) {
        g_target->DrawBitmap(bitmap, r, opacity,
                             D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
    } else {
        Fill(r,D2D1::ColorF(0xFFFFFF,0.7f*opacity),side*0.20f);
        auto center=D2D1::Point2F((r.left+r.right)/2,(r.top+r.bottom)/2);
        g_brush->SetColor(D2D1::ColorF(0xFFFFFF,opacity));
        if(!tile.app->websiteUrl.empty()) {
            g_target->DrawEllipse(D2D1::Ellipse(center,side*0.28f,side*0.28f),g_brush.Get(),side*0.04f);
            g_target->DrawEllipse(D2D1::Ellipse(center,side*0.12f,side*0.28f),g_brush.Get(),side*0.03f);
            g_target->DrawLine({r.left+side*0.22f,center.y},{r.right-side*0.22f,center.y},g_brush.Get(),side*0.03f);
        } else {
            g_target->DrawRectangle({r.left+side*0.2f,r.top+side*0.22f,r.right-side*0.2f,r.bottom-side*0.28f},g_brush.Get(),side*0.04f);
            g_target->DrawLine({center.x,r.bottom-side*0.28f},{center.x,r.bottom-side*0.15f},g_brush.Get(),side*0.04f);
        }
    }
}

// live: the icon takes the pointer and keys (not the grid behind a folder).
struct TileMotion {
    const void* key;
    D2D1_POINT_2F from;
};
std::vector<TileMotion> g_tileMotion;
double g_tileMotionStart = 0;
bool g_tileMotionAnimating = false;

const void* MotionKey(const Tile& tile) {
    return tile.folder ? (const void*)tile.folder.get() : (const void*)tile.app.get();
}

Cell AnimatedCell(const Tile& tile, Cell cell) {
    if (!Animate()) return cell;
    float entry=(1-GroupTileProgress(tile))*30*g_layout.scale;
    for (auto rect : {&cell.icon,&cell.label,&cell.highlight,&cell.unit}) { rect->left+=entry; rect->right+=entry; }
    float t = std::clamp((float)((Now() - g_tileMotionStart) / 0.20), 0.0f, 1.0f);
    if (t >= 1) return cell;
    for (const auto& motion : g_tileMotion) {
        if (motion.key != MotionKey(tile)) continue;
        float remaining = (1-t)*(1-t)*(1-t);
        float dx = (motion.from.x-cell.icon.left)*remaining;
        float dy = (motion.from.y-cell.icon.top)*remaining;
        for (auto rect : {&cell.icon, &cell.label, &cell.highlight, &cell.unit}) {
            rect->left += dx; rect->right += dx;
            rect->top += dy; rect->bottom += dy;
        }
        g_tileMotionAnimating |= std::abs(dx) > 0.1f || std::abs(dy) > 0.1f;
        break;
    }
    return cell;
}

Cell VisualItemCell(int index) {
    if (index < 0 || index >= (int)Items().size()) return {};
    Cell cell = ItemCell(index);
    if (g_openFolder) return cell;
    // Mirror DrawTile's group offset first, then its fly-in projection.
    if (GroupedView()) {
        for (const auto& geometry : g_groupGeometry) {
            if (index >= geometry.start && index < geometry.start + geometry.count) {
                cell = GroupVisualCell(geometry, cell);
                break;
            }
        }
    }
    cell = AnimatedCell(g_visible[index], cell);
    if (!FlyInActive()) return cell;
    const auto center = D2D1::Point2F((cell.icon.left + cell.icon.right) * .5f,
                                      (cell.icon.top + cell.icon.bottom) * .5f);
    auto pose = FlyInPose(center, 1.8f * std::max(cell.unit.right - cell.unit.left,
                                                  cell.unit.bottom - cell.unit.top));
    if (pose.opacity <= 0) return {};
    auto project = [&](D2D1_RECT_F r) {
        return D2D1::RectF(center.x + pose.dx + (r.left - center.x) * pose.scale,
                           center.y + pose.dy + (r.top - center.y) * pose.scale,
                           center.x + pose.dx + (r.right - center.x) * pose.scale,
                           center.y + pose.dy + (r.bottom - center.y) * pose.scale);
    };
    cell.icon = project(cell.icon);
    cell.label = project(cell.label);
    cell.highlight = project(cell.highlight);
    cell.unit = project(cell.unit);
    return cell;
}

void CaptureTileMotion() {
    if (!Animate()) { g_tileMotion.clear(); return; }
    std::vector<TileMotion> next;
    const Tiles& tiles = g_openFolder ? g_openFolder->apps : g_visible;
    for (int i = 0; i < (int)tiles.size(); ++i) {
        Cell c = AnimatedCell(tiles[i], g_openFolder ? FolderCellAt(i) : CellAt(i));
        next.push_back({MotionKey(tiles[i]), {c.icon.left, c.icon.top}});
    }
    g_tileMotion = std::move(next);
    g_tileMotionStart = Now();
    StartFrames();
}

struct CachedTileLabel {
    std::wstring name;
    float width = 0, height = 0;
    UINT version = 0;
    bool wrap=false;
    float inkTop = 0, inkBottom = 0;
    ComPtr<IDWriteTextLayout> layout;
    LabelShadowBitmap shadow;
};
[[clang::no_destroy]] std::map<const void*, CachedTileLabel> g_labelCache;

void ClearTileLabelCache() {
    g_labelCache.clear();
}

// Layout can run before Direct2D has a device. Only cache text metrics here.
CachedTileLabel& TileLabel(const Tile& tile, const D2D1_RECT_F& box,bool wrap) {
    auto& cached = g_labelCache[MotionKey(tile)];
    const auto& name = TileName(tile);
    float width = box.right - box.left, height = box.bottom - box.top;
    if (!cached.layout || cached.version != g_formatVersion || cached.name != name || cached.width != width || cached.height != height || cached.wrap!=wrap) {
        cached.name = name; cached.width = width; cached.height = height; cached.version = g_formatVersion; cached.wrap=wrap;
        cached.layout.Reset();
        cached.shadow = {};
        cached.inkTop = 0;
        cached.inkBottom = height;
        g_dwrite->CreateTextLayout(name.c_str(), (UINT32)name.size(), g_labelFormat.Get(), width, height, &cached.layout);
        if(cached.layout) {
            if(wrap) {
                static UINT metricsVersion=0;
                static DWRITE_LINE_METRICS line{};
                if(metricsVersion!=g_formatVersion) {
                    metricsVersion=g_formatVersion;
                    ComPtr<IDWriteTextLayout> sample;
                    UINT count=0;
                    if(SUCCEEDED(g_dwrite->CreateTextLayout(L"Ag中",3,g_labelFormat.Get(),1000,100,&sample)))
                        sample->GetLineMetrics(&line,1,&count);
                }
                float advance=height/2;
                float baseline=std::clamp(line.baseline+(advance-line.height)/2,0.f,advance);
                cached.layout->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_UNIFORM,advance,baseline);
                cached.layout->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
            }
            const float fittedFont = std::max(1.f, height / 1.3f);
            if (fittedFont < g_labelFormat->GetFontSize())
                cached.layout->SetFontSize(fittedFont, {0, (UINT32)name.size()});
            DWRITE_OVERHANG_METRICS ink{};
            if (SUCCEEDED(cached.layout->GetOverhangMetrics(&ink))) {
                cached.inkTop = std::clamp(-ink.top, 0.f, height);
                cached.inkBottom = std::clamp(height + ink.bottom, cached.inkTop, height);
            }
        }
    }
    return cached;
}

Cell SpaceTileContents(const Tile& tile, Cell cell) {
    float height = cell.unit.bottom - cell.unit.top;
    float icon = cell.icon.bottom - cell.icon.top;
    float inkTop = 0, inkHeight = 0;
    if (g_settings.showLabels && g_labelFormat &&
        cell.label.right > cell.label.left && cell.label.bottom > cell.label.top) {
        const auto& label = TileLabel(tile, cell.label, cell.wrapLabel);
        if (label.layout) {
            inkTop = label.inkTop;
            inkHeight = label.inkBottom - inkTop;
        }
    }
    float gap = std::max(0.f, height - icon - inkHeight) / (inkHeight > 0 ? 3 : 2);
    cell.icon.top = cell.unit.top + gap;
    cell.icon.bottom = cell.icon.top + icon;
    float boxHeight = cell.label.bottom - cell.label.top;
    cell.label.top = cell.icon.bottom + gap - inkTop;
    cell.label.bottom = cell.label.top + boxHeight;
    return cell;
}

// The glass keeps a crisp bevel over a softer reflection inside the facing
// edge. Both respond to the same pointer direction, including at the corners.
// danger > 0 swaps the white glass for the dock's muted crimson pair. The
// power buttons pass a strength so destructive actions read hotter than Lock.
void DrawTileGlass(const D2D1_RECT_F& unit, D2D1_POINT_2F light,
                   D2D1_POINT_2F rimLight, float opacity, float radius, float danger) {
    float width=unit.right-unit.left,height=unit.bottom-unit.top;
    float side=std::min(width,height),s=g_layout.scale;
    if(side<4||opacity<=.001f)return;
    float bevel=std::min(1.4f*s,side*.045f);
    D2D1_RECT_F face{unit.left+bevel,unit.top+bevel,unit.right-bevel,unit.bottom-bevel};
    auto& glass=g_tileGlass;
    if(!glass.rim||!glass.surface || (danger > 0 && (!glass.dangerSurface || !glass.dangerRim))) {
        EnsureGlassBrushes(g_target.Get(), glass);
    }
    const bool red = danger > 0;
    auto* surface = red && glass.dangerSurface ? glass.dangerSurface.Get() : glass.surface.Get();
    auto* rim = red && glass.dangerRim ? glass.dangerRim.Get() : glass.rim.Get();
    const float tint = red ? danger : 1.f;
    D2D1_POINT_2F center{unit.left+light.x*width,unit.top+light.y*height};
    auto place=[&](ID2D1RadialGradientBrush* brush,float radius,float strength) {
        brush->SetCenter(center);
        brush->SetRadiusX(radius*width);brush->SetRadiusY(radius*height);
        brush->SetOpacity(opacity*strength);
    };

    const float faceRadius = std::max(0.f, radius - bevel);
    // The tile's contact shadow sits along its lower edge.
    if (radius == 0) {
        Fill({face.left,face.top+bevel,face.right,unit.bottom},D2D1::ColorF(0,opacity*.10f));
        Fill({face.left,face.bottom,face.right,face.bottom+bevel*.5f},D2D1::ColorF(0,opacity*.09f));
    }
    Fill(face,red ? D2D1::ColorF(0xFF4650,.14f*opacity*tint)
                  : D2D1::ColorF(0xFFFFFF,.035f*opacity),faceRadius);
    if(surface) {
        place(surface,.34f,tint);
        if (radius > 0)
            g_target->FillRoundedRectangle(D2D1::RoundedRect(face, faceRadius, faceRadius), surface);
        else
            g_target->FillRectangle(face,surface);
    }
    if(rim) {
        center = {unit.left + rimLight.x * width, unit.top + rimLight.y * height};
        float inner=2*bevel;
        D2D1_RECT_F catchRect{face.left+inner,face.top+inner,face.right-inner,face.bottom-inner};
        place(rim,.51f,tint);
        if (radius > 0) {
            const float innerRadius = std::max(0.f, faceRadius - inner);
            rim->SetOpacity(opacity * tint * .23f);
            g_target->DrawRoundedRectangle(D2D1::RoundedRect(catchRect, innerRadius, innerRadius),
                                           rim, bevel);
            place(rim, .48f, tint);
            g_target->DrawRoundedRectangle(D2D1::RoundedRect(face, faceRadius, faceRadius),
                                           rim, bevel);
            return;
        }
        // Stretch the radial falloff along the edge and fade it to zero
        // before it reaches the middle of the tile. The bevel is drawn on top.
        const float depth = std::min(16.f * s, side * .19f);
        const float inset = depth * .18f;
        const float across = depth - inset;
        const float alongX = std::min(width * .50f, 72.f * s);
        const float alongY = std::min(height * .50f, 72.f * s);
        const auto fillEdgeGlow = [&](const D2D1_RECT_F& band, D2D1_POINT_2F peak,
                                      float rx, float ry, float distance) {
            const float facing = std::max(0.f, 1.f - distance / .38f);
            if (facing <= .01f) return;
            rim->SetCenter(peak);
            rim->SetRadiusX(rx);
            rim->SetRadiusY(ry);
            rim->SetOpacity(opacity * tint * .66f * facing * facing);
            g_target->FillRectangle(band, rim);
        };
        fillEdgeGlow({face.left, face.top, face.left + depth, face.bottom},
                     {face.left + inset, center.y}, across, alongY, rimLight.x);
        fillEdgeGlow({face.right - depth, face.top, face.right, face.bottom},
                     {face.right - inset, center.y}, across, alongY, 1.f - rimLight.x);
        fillEdgeGlow({face.left, face.top, face.right, face.top + depth},
                     {center.x, face.top + inset}, alongX, across, rimLight.y);
        fillEdgeGlow({face.left, face.bottom - depth, face.right, face.bottom},
                     {center.x, face.bottom - inset}, alongX, across, 1.f - rimLight.y);

        place(rim, .51f, tint);
        auto catchEdge=[&](D2D1_POINT_2F from,D2D1_POINT_2F to,float distance) {
            float facing=std::clamp(1-distance*1.6f,0.f,1.f);
            rim->SetOpacity(opacity*tint*(.07f+.23f*facing*facing));
            g_target->DrawLine(from,to,rim,bevel);
        };
        D2D1_POINT_2F tl{catchRect.left,catchRect.top},tr{catchRect.right,catchRect.top};
        D2D1_POINT_2F bl{catchRect.left,catchRect.bottom},br{catchRect.right,catchRect.bottom};
        catchEdge(tl,bl,rimLight.x);catchEdge(tr,br,1-rimLight.x);
        catchEdge(tl,tr,rimLight.y);catchEdge(bl,br,1-rimLight.y);
        place(rim,.48f,tint * .78f);
        g_target->DrawRectangle(face,rim,bevel);
    }
}

void SetGlassScenePointer() {
    g_glassScenePointer.reset();
    if(!g_glassPointer||g_groupEditMode||g_closing||g_drag.active||
       g_selectionPointer.active||g_searchDrag)return;
    D2D1::Matrix3x2F transform;
    g_target->GetTransform(&transform);
    if(transform.Invert()) {
        auto point=transform.TransformPoint(*g_glassPointer);
        if(!ToolBarConsumes((int)point.x,(int)point.y))g_glassScenePointer=point;
    }
}

// Distance from a point to a box, 0 inside it. The glow reach is measured
// from the icon itself, not from its hit box: the hit boxes tile the whole grid,
// so while the pointer sat inside a cell the hovered tile was always 0 away and
// every reach looked the same. The icon is the thing the eye reads as "far".
float PointBoxDistance(const D2D1_RECT_F& box, float x, float y) {
    const float dx = std::max(0.f, std::max(box.left - x, x - box.right));
    const float dy = std::max(0.f, std::max(box.top - y, y - box.bottom));
    return std::sqrt(dx * dx + dy * dy);
}

// The tile whose icon is closest to the pointer, resolved once per frame. The
// glow follows that tile wherever the pointer is, so setting a distance actually
// widens the area: with 0 only the icon under the pointer lights, with 80 the
// nearest icon within 80 DIP does. The result is kept by the app pointer, not by
// index: the grid and the folder panel index different lists.
int g_glowNearest = -2;  // -2 unresolved this frame, -1 resolved to none
const void* g_glowNearestKey = nullptr;
bool GlowNearestIs(const void* key, float reach) {
    if (g_glowNearest == -2) {
        g_glowNearest = -1;
        g_glowNearestKey = nullptr;
        if (g_glassScenePointer) {
            const Layout& L = g_layout;
            const auto p = *g_glassScenePointer;
            if (p.y >= L.viewTop && p.y < L.viewBottom) {
                float best = reach;
                for (int i = 0; i < (int)Items().size(); ++i) {
                    const Tile& tile = Items()[i];
                    if (!tile.app) continue;
                    const Cell cell = VisualItemCell(i);
                    if (cell.icon.right <= cell.icon.left) continue;  // mid fly-in
                    const float d = PointBoxDistance(cell.icon, p.x, p.y);
                    if (d <= reach && (g_glowNearest < 0 || d < best)) {
                        best = d;
                        g_glowNearest = i;
                        g_glowNearestKey = tile.app.get();
                    }
                }
            }
        }
    }
    return g_glowNearest >= 0 && g_glowNearestKey == key;
}

// Project the pointer's direction from the icon centre onto the cell rim.
// The face stays under the pointer, while a wider reach brings the rim light
// onto the facing edge within the first few DIP. Blend from zero displacement
// so passing through the centre does not flip the highlight abruptly.
D2D1_POINT_2F TileRimLight(const Cell& cell, D2D1_POINT_2F light, int reach, float scale) {
    const auto& bounds = cell.unit;
    const float width = std::max(1.f, bounds.right - bounds.left);
    const float height = std::max(1.f, bounds.bottom - bounds.top);
    const D2D1_POINT_2F origin{
        std::clamp(((cell.icon.left + cell.icon.right) * .5f - bounds.left) / width, 0.f, 1.f),
        std::clamp(((cell.icon.top + cell.icon.bottom) * .5f - bounds.top) / height, 0.f, 1.f),
    };
    const float dx = light.x - origin.x;
    const float dy = light.y - origin.y;
    const float travel = std::max(std::abs(dx) / std::max(.001f, dx < 0 ? origin.x : 1.f - origin.x),
                                  std::abs(dy) / std::max(.001f, dy < 0 ? origin.y : 1.f - origin.y));
    if (travel <= 0.f || reach <= 0) return light;

    const float movement = std::hypot(dx * width, dy * height);
    const float response = (reach / 80.f) * std::min(1.f, movement / (4.f * scale));
    return {
        std::lerp(light.x, origin.x + dx / travel, response),
        std::lerp(light.y, origin.y + dy / travel, response),
    };
}

void DrawTile(const Tile& tile, const Cell& c, int i, bool live,
              float opacity = 1.f, D2D1_POINT_2F labelOffset = {}) {
    if (IsDragged(tile)) {
        return;  // Its place stays empty; the icon follows the pointer.
    }
    float s = g_layout.scale;
    if(live&&!g_groupEditMode&&i==g_selected) {
        auto box=c.highlight;
        box.bottom=std::max(box.bottom,c.label.bottom+labelOffset.y);
        Fill(box,D2D1::ColorF(0xFFFFFF,.16f*opacity),10*s);
    }
    if(live&&!g_groupEditMode&&tile.app) {
        const auto& bounds=c.unit;
        const float reach=g_settings.glowDistance*s;
        // Whichever icon is nearest to the pointer lights, and the reach is measured
        // from that icon's own box. Nothing here consults the hover: the hit boxes
        // tile the grid, so "outside the hovered cell" almost never happened and a
        // reach of 0 and 80 looked the same. With the distance from the icon this
        // setting now widens the lit area as it says it does, and the lit icon is
        // always the closest one, never a neighbour.
        bool on=!g_glassClaimed&&g_glassScenePointer&&GlowNearestIs(tile.app.get(),reach);
        if(on)g_glassClaimed=true;
        if(auto fade=UpdateGlow(tile.app.get(),on)) {
            if(on) {
                const float width=std::max(1.f,bounds.right-bounds.left);
                const float height=std::max(1.f,bounds.bottom-bounds.top);
                fade->light={std::clamp((g_glassScenePointer->x-bounds.left)/width,0.f,1.f),
                             std::clamp((g_glassScenePointer->y-bounds.top)/height,0.f,1.f)};
                fade->rimLight = TileRimLight(c, fade->light, g_settings.glowDistance, s);
            }
            DrawTileGlass(bounds, fade->light, fade->rimLight, Ease(fade->alpha) * opacity);
        }
    }
    D2D1_MATRIX_3X2_F base;
    bool wiggle = g_wiggle > 0.001f;
    if (wiggle) {
        // Edit mode: every icon rocks around its center, neighbours out of
        // phase, with the hold ramping the angle in and out.
        g_target->GetTransform(&base);
        float cx = (c.icon.left + c.icon.right) / 2;
        float cy = (c.icon.top + c.icon.bottom) / 2;
        float angle = 2.4f * g_wiggle *
                      std::sin((float)Now() * kTwoPi * 3.1f +
                               (c.icon.left - c.icon.top) * 0.02f);
        g_target->SetTransform(D2D1::Matrix3x2F::Rotation(
                                    angle, D2D1::Point2F(cx, cy)) *
                                base);
    }
    if(live&&!g_groupEditMode&&tile.folder&&i==g_hover)
        Fill(c.highlight,D2D1::ColorF(0xFFFFFF,.12f*opacity),8*s);
    if(live&&tile.app&&AppSelected(*tile.app)) {
        auto bounds=c.highlight;
        bounds.bottom=std::max(bounds.bottom,c.label.bottom+labelOffset.y+4*s);
        Fill(bounds,D2D1::ColorF(0xFFFFFF,.15f*opacity),7*s);
        g_brush->SetColor(D2D1::ColorF(0xFFFFFF,.72f*opacity));
        g_target->DrawRoundedRectangle(D2D1::RoundedRect(bounds,7*s,7*s),g_brush.Get(),s);
    }
    if (live && i == g_drag.target) {
        float grow = 8 * s;
        Fill(D2D1::RectF(c.icon.left - grow, c.icon.top - grow,
                         c.icon.right + grow, c.icon.bottom + grow),
             D2D1::ColorF(0xFFFFFF, 0.3f*opacity), 18 * s);
    }
    float entryAlpha=GroupTileProgress(tile)*opacity;
    DrawTileIcon(tile, c.icon, (live && i == g_pressed ? 0.6f : 1.0f)*entryAlpha);
    if(tile.app && tile.app->unavailable) {
        auto badge=c.icon;badge.left=badge.right-18*s;badge.bottom=badge.top+18*s;
        Fill(badge,D2D1::ColorF(0xB43B3B,entryAlpha),9*s);
        DrawLauncherText(L"!", g_tipFormat.Get(), badge, entryAlpha);
    }
    if (g_settings.showLabels && g_labelFormat && c.label.right > c.label.left && c.label.bottom > c.label.top) {
        auto& cached = TileLabel(tile, c.label,c.wrapLabel);
        auto& shadow = cached.shadow;
        if (entryAlpha > 0)
            TryBuildTextShadow(cached.layout.Get(), cached.width, cached.height,
                               shadow, s, g_settings.textEffects);
        if (shadow.bitmap) {
            // The blur has padding around the original text box.
            g_target->DrawBitmap(shadow.bitmap.Get(),
                D2D1::RectF(c.label.left + labelOffset.x - shadow.padX,
                            c.label.top + labelOffset.y - shadow.padTop,
                            c.label.right + labelOffset.x + shadow.padX,
                            c.label.bottom + labelOffset.y + shadow.padBottom),
                entryAlpha, D2D1_INTERPOLATION_MODE_LINEAR);
        } else {
            auto* text = cached.layout.Get();
            g_brush->SetColor(D2D1::ColorF(0xFFFFFF,entryAlpha));
            if (text) g_target->DrawTextLayout({c.label.left+labelOffset.x, c.label.top+labelOffset.y},
                                               text, g_brush.Get(), D2D1_DRAW_TEXT_OPTIONS_CLIP);
        }
    }
    if (wiggle) {
        g_target->SetTransform(base);
    }
}

void DrawFolder() {
    if (!g_openFolder) {
        return;
    }
    const Layout& L = g_layout;
    float s = L.scale;
    FolderLayout F = GetFolderLayout();
    auto folderColor=D2D1::ColorF(0x292929,.62f);
    Fill(F.panel, folderColor, 24 * s);
    g_brush->SetColor(D2D1::ColorF(0xFFFFFF, 0.35f));
    g_target->DrawRoundedRectangle(
        D2D1::RoundedRect(D2D1::RectF(F.panel.left + 0.5f, F.panel.top + 0.5f,
                                      F.panel.right - 0.5f,
                                      F.panel.bottom - 0.5f),
                          24 * s, 24 * s),
        g_brush.Get(), 1);

    DrawEditView(GetEditView(true), g_folderRenaming, 20 * s);

    const Tiles& apps = g_openFolder->apps;
    g_target->PushAxisAlignedClip(
        D2D1::RectF(F.panel.left, F.top, F.panel.right,
                    F.top + F.visibleRows * F.cellHeight),
        D2D1_ANTIALIAS_MODE_ALIASED);
    int first = g_folderRow * F.columns;
    int last = std::min((int)apps.size(),
                        (g_folderRow + F.visibleRows) * F.columns);
    SetGlassScenePointer();
    if(g_closingFolder==g_openFolder||!g_glassScenePointer||
       !InRect({F.left,F.top,F.left+F.columns*L.cellWidth,F.top+F.visibleRows*F.cellHeight},
               g_glassScenePointer->x,g_glassScenePointer->y))g_glassScenePointer.reset();
    for (int i = first; i < last; i++) {
        DrawTile(apps[i], AnimatedCell(apps[i], FolderCellAt(i)), i, true);
    }
    g_target->PopAxisAlignedClip();
}

void DrawDragged() {
    if (!g_drag.active) {
        return;
    }
    float icon = g_drag.iconSize, size = std::round(icon * 1.06f);
    float x = g_drag.at.x - g_drag.grab.x - (size - icon) / 2;
    float y = g_drag.at.y - g_drag.grab.y - (size - icon) / 2;
    int depth = std::clamp((int)g_drag.tiles.size() - 1, 0, 2);
    for (auto it = g_drag.tiles.rbegin(); it != g_drag.tiles.rend() && depth; ++it) {
        if (it->app == g_drag.tile.app) continue;
        float shift = 5 * g_layout.scale * depth;
        DrawTileIcon(*it, D2D1::RectF(x + shift, y - shift, x + shift + size, y - shift + size),
                     depth == 1 ? .75f : .55f);
        --depth;
    }
    DrawTileIcon(g_drag.tile, D2D1::RectF(x, y, x + size, y + size), 0.92f);
}

bool g_scrollHover=false;
D2D1_RECT_F ScrollThumb() {
    float maxScroll=MaxScroll();
    if(g_settings.pages||maxScroll<=0||g_openFolder)return {};
    const auto& L=g_layout;
    float track=L.viewBottom-L.viewTop;
    if(track<=0)return {};
    float thumb=std::clamp(track*track/L.contentHeight,std::min(40*L.scale,track),track);
    float top=L.viewTop+(track-thumb)*(g_scroll/maxScroll);
    float right=L.width-10*L.scale;
    return {right-4*L.scale,top,right,top+thumb};
}
void DrawGrid() {
    const Layout& L = g_layout;
    float s = L.scale;
    if (!g_appsLoaded && g_apps.empty()) {
        DrawMessage(Tr(L"Loading apps\u2026"));
        return;
    }
    if (g_visible.empty() && !GroupedView()) {
        DrawMessage(!g_query.empty() ? Tr(L"No apps found")
                    : PinnedOnly()
                        ? Tr(L"Nothing pinned yet. Type to find an app, then right-click it to pin it")
                        : Tr(L"No apps"));
        return;
    }

    const bool flying = FlyInActive();
    // Start at the screen edges and restore the normal scroll viewport
    // as the travelling icons settle into their cells.
    float viewport = flying ? launchpad_fly_in::SmoothStep(g_appear / .72f) : 1.f;
    float clipTop = L.viewTop * viewport;
    float clipBottom = std::lerp((float)L.height, L.viewBottom, viewport);
    g_target->PushAxisAlignedClip(D2D1::RectF(0, clipTop, (float)L.width, clipBottom),
                                  D2D1_ANTIALIAS_MODE_ALIASED);
    if (flying) g_glassScenePointer.reset();
    else SetGlassScenePointer();
    if(g_glassScenePointer&&(g_glassScenePointer->y<L.viewTop||g_glassScenePointer->y>=L.viewBottom))
        g_glassScenePointer.reset();
    auto drawCell=[&](int i,Cell cell) {
        if(!g_groupEditMode)cell=AnimatedCell(g_visible[i],cell);
        if(cell.unit.bottom<=L.viewTop||cell.unit.top>=L.viewBottom||cell.unit.right<=0||cell.unit.left>=L.width)return;
        if (!flying) {
            DrawTile(g_visible[i],cell,i,!g_openFolder);
            return;
        }
        auto center = D2D1::Point2F((cell.icon.left + cell.icon.right) * .5f,
                                    (cell.icon.top + cell.icon.bottom) * .5f);
        auto pose = FlyInPose(center, 1.8f * std::max(cell.unit.right - cell.unit.left,
                                                    cell.unit.bottom - cell.unit.top));
        if (pose.opacity <= 0) return;
        auto project = [&](D2D1_RECT_F r) {
            return D2D1::RectF(
                center.x + pose.dx + (r.left - center.x) * pose.scale,
                center.y + pose.dy + (r.top - center.y) * pose.scale,
                center.x + pose.dx + (r.right - center.x) * pose.scale,
                center.y + pose.dy + (r.bottom - center.y) * pose.scale);
        };
        cell.unit = project(cell.unit);
        if (cell.unit.right <= 0 || cell.unit.left >= L.width ||
            cell.unit.bottom <= clipTop || cell.unit.top >= clipBottom) return;
        cell.icon = project(cell.icon);
        cell.highlight = project(cell.highlight);
        // Carry the label beneath its icon at the original font size. Keep
        // its layout width so fractional perspective does not reflow text.
        float labelDy = pose.dy + (cell.label.top - center.y) * (pose.scale - 1.f);
        DrawTile(g_visible[i],cell,i,!g_openFolder,pose.opacity,{pose.dx,labelDy});
    };
    if(GroupedView()) {
        DrawGroupGrid();
        auto resizePreview=GroupResizePreviewGeometry();
        const auto* preview=resizePreview?&*resizePreview:nullptr;
        auto drawGroups=[&](bool activeOnly) {
            DrawGroupHeaders(activeOnly,preview);
            for(const auto& current:g_groupGeometry) {
                bool selected=g_groupGesture.moved&&GroupMovesWithGesture(g_groupGesture,current.group);
                bool active=ActiveGroupPreview(current);
                if(active!=activeOnly||(selected&&!active))continue;
                const auto& geometry=active&&preview?*preview:current;
                auto area=VisualGroupRect(geometry);
                if(area.right<=0||area.left>=L.width||area.bottom<=L.viewTop||area.top>=L.viewBottom)continue;
                bool clip=active||(!flying&&g_groupTransitions.count(geometry.group));
                if(clip)g_target->PushAxisAlignedClip(area,D2D1_ANTIALIAS_MODE_ALIASED);
                for(int slot=0;slot<geometry.count;++slot) {
                    int i=geometry.start+slot;
                    if(i>=(int)g_visible.size())break;
                    auto cell=active&&preview?ProjectGroupCell(MakeExpandedGroupCell(geometry,slot)):CellAt(i);
                    cell=GroupVisualCell(geometry,cell);
                    if(active&&g_groupGesture.resize&&!GroupIconFits(cell,area))continue;
                    drawCell(i,cell);
                }
                DrawGroupPageIndicator(geometry, area);
                if(clip)g_target->PopAxisAlignedClip();
            }
        };
        drawGroups(false);
        if(g_groupGesture.moved)drawGroups(true);
        DrawGroupEditPreview();
    } else {
        int first,last;
        if (g_settings.pages) {  // The one or two pages on screen.
            int page=std::max(0,(int)(g_scroll/L.width));
            first=page*L.perPage;last=std::min((int)g_visible.size(),(page+2)*L.perPage);
        } else {
            int firstRow=std::max(0,(int)(g_scroll/L.cellHeight));
            int lastRow=(int)((g_scroll+L.viewBottom-L.viewTop)/L.cellHeight)+1;
            first=firstRow*L.columns;last=std::min((int)g_visible.size(),(lastRow+1)*L.columns);
        }
        for(int i=first;i<last;++i)drawCell(i,CellAt(i));
    }
    g_target->PopAxisAlignedClip();

    float chrome = FlyInChromeOpacity();
    if (g_settings.pages) {
        // One dot per page; the current one is brighter.
        if (L.pages >= 1) {
            float step = std::min(20 * s, std::max(2.0f, (L.width - 48 * s) / L.pages));
            float r = std::min(3.5f * s, step * 0.28f);
            float left = L.width / 2.0f - L.pages * step / 2 + step / 2;
            float current = g_scroll / L.width;
            for (int i = 0; i < L.pages; i++) {
                float closeness = std::max(0.0f, 1 - std::abs(current - i));
                g_brush->SetColor(D2D1::ColorF(0xFFFFFF, chrome * (0.25f + 0.65f * closeness)));
                g_target->FillEllipse(
                    D2D1::Ellipse(D2D1::Point2F(left + i * step, L.dotsY), r, r),
                    g_brush.Get());
            }
        }
        return;
    }

    auto bar=ScrollThumb();
    if(SelectionHasArea(bar))Fill(bar,D2D1::ColorF(0xFFFFFF,chrome*(g_scrollHover?.62f:.32f)),2*s);
}

void Render() {
    ++g_textShadowFrame;
    g_textShadowRetryPending = false;
    double renderStarted = Now();
    if (!g_shown || !EnsureTarget()) {
        return;
    }
    const Layout& L = g_layout;
    if (!EnsureFrame(L.width, L.height)) {
        return;
    }
    g_target->SetTarget(g_frame.Get());
    float W = (float)L.width, H = (float)L.height;
    const bool flyIn = g_settings.iconFlyIn && !g_groupEditMode;
    float background = flyIn ? launchpad_fly_in::SmoothStep(g_appear / .58f) : 1.f;
    float chrome = FlyInChromeOpacity();
    float windowOpacity = flyIn ? 1.f : Ease(g_appear);
    g_target->BeginDraw();
    g_target->SetTransform(D2D1::Matrix3x2F::Identity());
    // Minimum alpha instead of zero: fully transparent pixels of a layered
    // window let the click pass through to what's behind.
    g_target->Clear(D2D1::ColorF(0, 0, 0, 1 / 255.0f));
    SyncMaterialFromSettings();
    DrawBackdrop(W, H, background);

    float zoom = flyIn || g_groupEditMode ? 1.f : 1.06f - 0.06f * Ease(g_appear);
    g_sceneZoom = zoom;
    g_sceneOffsetY = 0.f;
    g_target->SetTransform(
        D2D1::Matrix3x2F::Scale(zoom, zoom, D2D1::Point2F(W / 2, H / 2)));
    if (chrome > 0.f) {
        DrawSearchBox(chrome);
        DrawClearButton(chrome);
    }
    if (chrome > 0.f && (g_layoutReadOnly || g_fullBackupBusy)) {
        PCWSTR warning=Tr(L"Layout data is unreadable or unsupported: changes are read-only. Restore a verified backup.");
        if(g_fullBackupBusy)warning=Tr(L"Complete backup/import in progress…");
        DrawLauncherText(warning, g_tipFormat.Get(),
            {20*L.scale,L.search.bottom,(float)L.width-20*L.scale,L.viewTop}, chrome);
    }
    g_glassClaimed=false;
    g_glowNearest=-2;    if (g_searchGridTransition < 1.0f) {
        const float progress = launchpad_fly_in::SmoothStep(g_searchGridTransition);
        const float offset = (g_showingSearchResults ? 12.0f : -12.0f) * L.scale * (1.0f - progress);
        g_sceneOffsetY = offset;
        const auto transform = D2D1::Matrix3x2F::Scale(zoom, zoom, D2D1::Point2F(W / 2, H / 2));
        g_target->SetTransform(D2D1::Matrix3x2F::Translation(0, offset) * transform);
        g_target->PushLayer(D2D1::LayerParameters1(D2D1::RectF(0, L.viewTop, W, L.viewBottom),
            nullptr, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE, D2D1::IdentityMatrix(),
            std::lerp(0.2f, 1.0f, progress)), nullptr);
        DrawGrid();
        g_target->PopLayer();
        g_target->SetTransform(transform);
    } else {
        DrawGrid();
    }
    DrawToolBar(false, chrome);
    // A closing folder keeps drawing until its fade ends.
    std::shared_ptr<Folder> realOpen = g_openFolder;
    if (!g_openFolder && g_closingFolder) {
        g_openFolder = g_closingFolder;
    }
    bool folderLayer = false;
    if (g_openFolder) {
        float e = Ease(std::clamp(g_folderAnim, 0.0f, 1.0f));
        // Dim the grid, then lay the folder's glass over it: a blur of the
        // pixels just drawn, read back from the surface when they changed.
        Fill(D2D1::RectF(0, 0, W, H), D2D1::ColorF(0x171717, 0.35f * e * chrome));
        D2D1_MATRIX_3X2_F m;
        g_target->GetTransform(&m);
        D2D1_RECT_F panel = GetFolderLayout().panel;
        // The panel grows from 92% around its centre as it fades in.
        float k = 0.92f + 0.08f * e;
        m = D2D1::Matrix3x2F::Scale(
                k, k,
                D2D1::Point2F((panel.left + panel.right) / 2,
                              (panel.top + panel.bottom) / 2)) *
            D2D1::Matrix3x2F(m._11, m._12, m._21, m._22, m._31, m._32);
        D2D1_RECT_F dev = D2D1::RectF(
            panel.left * m._11 + m._31, panel.top * m._22 + m._32,
            panel.right * m._11 + m._31, panel.bottom * m._22 + m._32);
        const void* folder = g_openFolder.get();
        g_target->SetTransform(D2D1::Matrix3x2F::Identity());
        // Folder growth clips one full-size cached blur instead of generating
        // a new blur for each intermediate panel rectangle.
        D2D1_RECT_F captureRect = g_appear >= 1 ? panel : dev;
        if (g_drag.active || !PanelBlurFresh(captureRect, folder, g_scroll)) {
            if (g_target->EndDraw() == D2DERR_RECREATE_TARGET) {
                g_openFolder = realOpen;  // Do not leave a closing folder open.
                ResetTarget();
                g_groupTitleRenders.clear();
                return;
            }
            CapturePanelBlur(captureRect, folder, g_scroll, L.scale);
            g_target->SetTarget(g_frame.Get());
            g_target->BeginDraw();
        }
        if (e * chrome < 0.999f) {
            g_target->PushLayer(
                D2D1::LayerParameters1(D2D1::InfiniteRect(), nullptr,
                                       D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
                                       D2D1::IdentityMatrix(), e * chrome),
                nullptr);
            folderLayer = true;
        }
        DrawPanelBlur(dev, 24 * L.scale * m._11);
        g_target->SetTransform(m);
    }
    DrawFolder();
    if (folderLayer) {
        g_target->PopLayer();
    }
    g_openFolder = realOpen;
    DrawDragged();
    g_target->SetTransform(D2D1::Matrix3x2F::Scale(zoom,zoom,D2D1::Point2F(W/2,H/2)));
    DrawAppSelectionRect();
    DrawToolBar(true, chrome);
    g_target->SetTransform(D2D1::Matrix3x2F::Identity());
    DrawGroupEditBar();

    if (g_target->EndDraw() == D2DERR_RECREATE_TARGET) {
        ResetTarget();
        g_groupTitleRenders.clear();
        return;
    }
    if (UseGpuPresenter()) {
        if (g_presenter.Ensure(g_wnd, g_d3d.Get(), L.width, L.height)) {
            g_presenter.Position(g_wnd, L.area);
            if (g_presenter.Present(g_d3d.Get(), g_frame.Get(), windowOpacity)) {
                DXGI_FRAME_STATISTICS statistics{};
                if (g_presenter.Statistics(statistics)) g_metrics.Presented(statistics);
                g_metrics.Rendered((Now() - renderStarted) * 1000);
                return;
            }
        }
        g_presenter.Reset(); g_presenter.failed = true;
        Wh_Log(L"GPU composition unavailable; falling back to layered-window presentation");
    }
    if (g_presenter.Active()) g_presenter.Reset();
    LaunchpadPresenter::LegacyStyle(g_wnd);
    if (!EnsureSurface(g_surface, L.width, L.height)) return;
    PresentFrame();
    POINT dst{L.area.left, L.area.top}, src{0, 0};
    SIZE size{L.width, L.height};
    BLENDFUNCTION blend{AC_SRC_OVER, 0, (BYTE)std::lround(255 * windowOpacity),
                        AC_SRC_ALPHA};
    UpdateLayeredWindow(g_wnd, nullptr, &dst, &size, g_surface.dc, &src, 0,
                        &blend, ULW_ALPHA);
    g_metrics.Rendered((Now() - renderStarted) * 1000);
}

void StartFrames() {
    if (!g_frameTimer) {
        g_frameTimer = true;
        g_lastFrame = Now();
        if (g_frameWake) ArmNextFrame(true);
        else SetTimer(g_wnd, kTimerFrame, (UINT)std::max(1.0, std::round(1000.0 / g_refreshHz)), nullptr);
    }
}

void HideNow([[maybe_unused]] bool resetStartButton) {
    CancelImeComposition(g_wnd);
#ifndef SEAMLESS_PREVIEW
    if (resetStartButton) {
        if (g_shown) ResetTaskbarStartButtons();
        ClearStartButtonLatch(L"latch-hide");
    }
    KillTimer(g_wnd, kTimerPowerUpdate);
#endif
    CancelGroupPointer();
    CancelGroupEdit();
    ClearAppSelection();g_batchBlend=0;g_batchDisplayCount=0;g_groupTransitions.clear();
    if(!g_freeGroupLayout)g_groupPositions.clear();
    g_groupDropFolder.reset(); g_groupDropSlot = -1;
    FinishMetrics();
    if (g_frameWake) CancelWaitableTimer(g_frameWake);
    KillTimer(g_wnd, kTimerFrame);
    g_frameTimer = false;
    EndDrag(false);
    g_searchDrag = false;
    KillTimer(g_wnd, kTimerCaret);
    DisarmPower();
    g_powerHover = g_powerPressed = -1;
    ResetPowerGlass();  // No glass, no bake, nothing left to animate.
    if (g_openFolder) {
        SaveFolderName();
        g_openFolder = nullptr;
    }
    g_closingFolder = nullptr;
    g_folderAnim = 0;
    HWND wnd = g_wnd;
    ShowWindow(wnd, SW_HIDE);
    ShowWindow(g_backdrop, SW_HIDE);
    g_shown = false;
    g_closing = false;
    g_appear = 0;
    g_appearVelocity = 0;
    ReleaseMaterial();
    g_panelBlur = {};
    FreeSurface(g_surface);
    g_trackingMouse = false;
    g_scrollHover = false;
    g_clearHover = g_searchHover = false;
    g_searchGlow = 0;
}

float AdvanceTransition(float value, float target, double elapsed, int durationMs) {
    float step = (float)(std::max(0.0, elapsed) * 1000.0 / std::max(1, durationMs));
    return value < target ? std::min(target, value + step) : std::max(target, value - step);
}

void OnFrame() {
    double now = Now();
    g_metrics.Frame(now);
    float dt = (float)std::clamp(now - g_lastFrame, 0.0, 5.0);
    g_lastFrame = now;
    if (!Animate()) {
        dt = 1000.0f;  // Every configured transition lands this frame.
    }
    bool animating = false;

    // Use elapsed time, independent of the number of submitted frames.
    if (g_openFolder || g_closingFolder) {
        float folderTarget = g_openFolder ? 1.0f : 0.0f;
        if (g_folderAnim != folderTarget) {
            g_folderAnim = AdvanceTransition(g_folderAnim, folderTarget, dt, g_settings.folderMs);
            animating = true;
        }
        if (!g_openFolder && g_folderAnim <= 0) {
            g_closingFolder = nullptr;
            g_panelBlur = {};
        }
    }

    float target = g_closing ? 0.0f : 1.0f;
    if (g_appear != target) {
        int duration = g_closing ? g_settings.closeMs : g_settings.openMs;
        g_appear = g_settings.iconFlyIn
            ? launchpad_fly_in::Advance(g_appear, g_appearVelocity, target, dt, duration)
            : AdvanceTransition(g_appear, target, dt, duration);
        animating |= g_appear != target;
    }
    if (g_scroll != g_scrollTarget) {
        g_scroll += (g_scrollTarget - g_scroll) * (Animate() ? 1 - std::exp(-dt * 16) : 1);
        if (std::abs(g_scrollTarget - g_scroll) < 0.5f) {
            g_scroll = g_scrollTarget;
        }
        animating |= g_scroll != g_scrollTarget;
    }

    // Edit mode only once the drag is real: the press moved past the system
    // drag threshold or has been held for the hold time. A plain click never
    // wiggles.
    float wiggleTarget =
        g_drag.active && g_settings.wiggle && Animate() ? 1.0f : 0.0f;
    if (g_wiggle != wiggleTarget) {
        g_wiggle += (wiggleTarget - g_wiggle) * (Animate() ? 1 - std::exp(-dt * 12) : 1);
        if (std::abs(wiggleTarget - g_wiggle) < 0.01f) {
            g_wiggle = wiggleTarget;
        }
        animating = true;
    }
    if (g_wiggle > 0.001f) {
        animating = true;
    }

    // A press held still for the hold time lifts the icon into edit mode.
    if (g_drag.pending && !g_drag.active &&
        now - g_drag.pressTime >= g_settings.holdMs / 1000.0) {
        StartDrag();
        animating = true;
    }
    if (g_drag.pending && !g_drag.active) animating=true;

    // The search bar's accent glow eases in and out, about 140 ms.
    float glowTarget =
        g_searchHover || g_clearHover || SearchExpanded() ? 1.0f : 0.0f;
    if (g_searchGlow != glowTarget) {
        g_searchGlow +=
            (glowTarget - g_searchGlow) * (Animate() ? 1 - std::exp(-dt * 18) : 1);
        if (std::abs(glowTarget - g_searchGlow) < 0.01f) {
            g_searchGlow = glowTarget;
        }
        animating = true;
    }
    // The search field itself: 44 px pill at rest, 240 px field open, with the
    // box, its magnifier and its text sliding together about 200 ms.
    float expandTarget = SearchExpanded() ? 1.0f : 0.0f;
    if (g_searchExpand != expandTarget) {
        g_searchExpand +=
            (expandTarget - g_searchExpand) * (Animate() ? 1 - std::exp(-dt * 16) : 1);
        if (std::abs(expandTarget - g_searchExpand) < 0.01f) {
            g_searchExpand = expandTarget;
        }
        LayoutSearch();
        animating = true;
    }
    if (g_searchGridTransition < 1.0f) {
        g_searchGridTransition = AdvanceTransition(g_searchGridTransition, 1.0f, dt, 180);
        animating = true;
    }
    if (g_drag.active) {
        DragTick();
        animating = true;
    }
    if (g_groupHeaderDragging) { GroupHeaderTick(); animating=true; }
    if(g_groupGesture.moved)animating|=TickGroupGesture();
    for(auto it=g_groupTransitions.begin();it!=g_groupTransitions.end();) {
        bool done=GroupEase(it->second.started)>=1;
        animating|=!done;
        if(done&&!g_groupGesture.moved)it=g_groupTransitions.erase(it);
        else ++it;
    }
    if(g_groupResizeHover&&g_groupResizeGlow<1) {
        g_groupResizeGlow=std::min(1.f,g_groupResizeGlow+(float)(dt/.14));
        animating=true;
    }
    float batchTarget=g_appSelection.empty()?0.f:1.f;
    if(g_batchBlend!=batchTarget) {
        g_batchBlend=AdvanceTransition(g_batchBlend,batchTarget,dt,Animate()?140:0);
        animating=true;
    }

    if (g_closing && g_appear <= 0) {
        HideNow();
        animating = false;
    } else {
        g_glowAnimating = false;
        g_tileMotionAnimating = false;
        Render();
        animating |= g_glowAnimating || g_tileMotionAnimating || g_textShadowRetryPending;
    }
    if (!animating) {
        FinishMetrics();
        KillTimer(g_wnd, kTimerFrame);
        if (g_frameWake) CancelWaitableTimer(g_frameWake);
        g_frameTimer = false;
    } else if (g_frameWake) {
        ArmNextFrame(false);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Opening and closing

// SetForegroundWindow succeeds when the calling process received the last
// input event; an empty SendInput provides it.
void ForceForeground(HWND hwnd) {
    INPUT input{};
#ifndef SEAMLESS_PREVIEW
    SendInput(1, &input, sizeof(INPUT));
#endif
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);
}

void PlaceWindows() {
    const RECT& a = g_layout.area;
    // Always on top: Windows sometimes leaves "always on top" windows (the
    // Dock included) below regular ones, and positioning below the Dock
    // put Start behind the windows.
    SetWindowPos(g_wnd, HWND_TOPMOST, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    if (!g_settings.coverScreen) {
        RaiseDockWindows();
    }
    if (UsesDwmBackdrop()) {
        SetWindowPos(g_backdrop, g_wnd, a.left, a.top, a.right - a.left,
                     a.bottom - a.top, SWP_NOACTIVATE | SWP_SHOWWINDOW);
    }
}

// Once per process, the first time the grid is actually shown: a silent
// read-only folder looks like a bug ("my changes don't stick"), and a silent
// empty migration looks like data loss ("where are my apps").
bool g_storageNoticeShown = false;
void ShowStorageNotice() {
    if (g_storageNoticeShown || !g_wnd) return;
    g_storageNoticeShown = true;
    std::wstring message;
    if (g_storageMigrationSkipped) {
        message = Tr(L"Lumiflow could not read the previous launcher's data, so it started with an empty layout. That folder was left unchanged. Open Settings to import a complete backup or to restore the old layout.");
    } else if (g_dataReadOnly) {
        message = Tr(L"Lumiflow opened its data read-only because it is damaged or was written by a newer version. Nothing is being overwritten. Open Settings to import a complete backup.");
    } else {
        return;
    }
    bool oldModal = g_modal;
    g_modal = true;
    MessageBoxW(g_wnd, message.c_str(), Tr(L"Lumiflow"), MB_OK | MB_ICONINFORMATION);
    g_modal = oldModal;
    // The dialog took the focus away from the grid; closing it is not a click
    // outside, so make sure the launcher stays open.
    if (g_shown) ForceForeground(g_wnd);
}

void Open() {
    if (g_fullImportBusy) return;
    double requested = Now();
    if (g_shown) {
        if (g_closing) {  // Reopened in the middle of closing.
            g_closing = false;
            ForceForeground(g_wnd);
            PlaceWindows();
            StartFrames();
        }
        return;
    }

    HMONITOR monitor = ChosenMonitor();
    UpdateFrameRate(monitor);
    if (g_settings.performanceLogging) g_metrics.Begin("open", requested, 0, 0, 1, g_refreshHz);
    UINT dpiX = 96, dpiY = 96;
    GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);

    Layout& L = g_layout;
    L.area = AvailableArea(monitor);
    L.width = L.area.right - L.area.left;
    L.height = L.area.bottom - L.area.top;
    L.scale = dpiX / 96.0f;
    g_metrics.width = L.width; g_metrics.height = L.height; g_metrics.scale = L.scale;
    EnsureFormats(L.scale);

    g_query.clear();
    g_searchActivated = false;
    g_searchExpand = 0;
    g_showingSearchResults = false;
    g_searchGridTransition = 1.0f;
    g_caret = g_anchor = 0;
    g_hist[0] = {};  // A fresh search starts a fresh history.
    g_searchDrag = false;
    TouchCaret();
    g_glassPointer.reset();
    for(auto& fade:g_glowFades)fade={};
    g_hover = g_selected = g_pressed = -1;
    g_scroll = g_scrollTarget = g_groupPagePan = 0;
    g_wheelDelta = 0;
    g_groupWheelTarget.reset();
    g_groupWheelRemainder = 0;
    g_openFolder = nullptr;
    g_tileMotion.clear();
    g_folderRow = 0;
    g_powerHover = g_powerPressed = g_powerArmed = -1;
    ResetPowerGlass();
    RefreshPowerCaps();
    UpdateFilter();
    ComputeLayout();
#ifndef SEAMLESS_PREVIEW
    RefreshPowerUpdateStatus();
    if (std::any_of(L.powerItems.begin(), L.powerItems.end(), [](int action) {
            return IsUpdatePowerAction(static_cast<PowerAction>(action));
        })) {
        SetTimer(g_wnd, kTimerPowerUpdate, 15000, nullptr);
    }
#endif

    // Custom materials capture the screen before any of the mod's windows
    // appear, so the grid's own pixels never feed back into the backdrop.
    // Native backends let DWM draw the material behind the grid instead.
    SyncMaterialFromSettings();
    PrepareMaterial(L.area, L.scale);    if (UsesDwmBackdrop()) {
        ApplyBackdropEffect();
    } else {
        ResetNativeBackdrop();
    }

    g_shown = true;
    g_closing = false;
    g_appear = Animate() ? 0 : 1;
    g_appearVelocity = 0;
    Render();
    ShowWindow(g_wnd, SW_SHOW);
    if (g_metrics.active) g_metrics.visibleMs = (Now() - requested) * 1000;
    UINT blink = GetCaretBlinkTime();
    SetTimer(g_wnd, kTimerCaret, blink ? blink : 530, nullptr);
    ForceForeground(g_wnd);
    PlaceWindows();
    if (Animate()) StartFrames(); else FinishMetrics();

    if (!g_appsLoaded || Now() - g_appsTime > 60 ||
        g_appsKey != AppsKey()) {
        StartLoadingApps();
    }
    ShowStorageNotice();
}

void Close() {
    if (!g_shown || g_closing || !ConfirmGroupEditClose()) {
        return;
    }
    FinishMetrics();
    if (g_settings.performanceLogging) g_metrics.Begin("close", Now(), g_layout.width, g_layout.height, g_layout.scale, g_refreshHz);
    g_closing = true;
    g_pressed = g_powerPressed = -1;
    DisarmPower();
    EndDrag(false);
    if (g_openFolder) {
        SaveFolderName();
    }
    if (GetCapture() == g_wnd) {
        ReleaseCapture();
    }
    if (Animate()) StartFrames(); else HideNow();
}

void Toggle() {
    if (g_shown && !g_closing) {
        Close();
    } else {
        Open();
    }
}

// A lone Windows key tap can reach the window twice: the keyboard hook reports
// the tap, and Windows' own request for Start follows it into the ShowStartView
// hook. Both describe one keystroke, so the first one to arrive acts as the
// toggle and the other is dropped.
bool FirstOfKeystroke() {
    double t = Now();
    if (t - g_keyToggleAt < 0.15) return false;
    g_keyToggleAt = t;
    return true;
}

// A packaged app with no Start-menu shortcut: runas on its raw AppsFolder
// target is a silent no-op, so a temporary .lnk aimed at the target carries
// the elevation instead — the shell brokers that into an elevated launch
// (verified: the spawned Terminal answers the elevation token query).
// The link stays in the mod's own storage, is unique per launch, and is
// removed again; nothing is left in the user's profile.
void LaunchTarget(const std::wstring& target, PCWSTR verb, const App* entry,
                  bool quiet);
void ElevateViaTempLnk(const std::wstring& target) {
    auto directory = ModTempPath();
    if (directory.empty() ||
        (FAILED(SHCreateDirectoryExW(nullptr, directory.c_str(), nullptr)) &&
         GetFileAttributesW(directory.c_str()) == INVALID_FILE_ATTRIBUTES)) {
        Wh_Log(L"Cannot create the elevation folder: %s", directory.c_str());
        return;
    }
    static LONG launches = 0;
    WCHAR name[80];
    swprintf_s(name, L"runas-%lu-%llu-%ld.lnk", GetCurrentProcessId(),
               (unsigned long long)GetTickCount64(),
               InterlockedIncrement(&launches));
    std::wstring lnk = directory + L"\\" + name;

    IShellLinkW* link = nullptr;
    bool saved = false;
    if (SUCCEEDED(CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER,
                                   IID_PPV_ARGS(&link)))) {
        IPersistFile* file = nullptr;
        if (SUCCEEDED(link->SetPath(target.c_str())) &&
            SUCCEEDED(link->QueryInterface(IID_PPV_ARGS(&file)))) {
            saved = SUCCEEDED(file->Save(lnk.c_str(), TRUE));
            file->Release();
        }
        link->Release();
    }
    if (!saved) {
        Wh_Log(L"Could not write the elevation shortcut: %s", lnk.c_str());
        DeleteFileW(lnk.c_str());
        return;
    }
    // Quiet waits for the shell to consume the link, so deleting it below
    // cannot race the elevated launch.
    LaunchTarget(lnk, L"runas", nullptr, true);
    DeleteFileW(lnk.c_str());
}

bool RequestLaunch(const std::wstring& target,PCWSTR verb,const App* entry,bool quiet=false) {
    SHELLEXECUTEINFOW sei{sizeof(sei)};
    sei.fMask = quiet ? SEE_MASK_NOASYNC | SEE_MASK_FLAG_NO_UI : SEE_MASK_ASYNCOK;
    sei.lpVerb = verb;
    sei.lpFile = target.c_str();
    // Normal activations retain arguments and working directory from entry edits.
    if (entry) {sei.lpParameters=entry->arguments.empty()?nullptr:entry->arguments.c_str();sei.lpDirectory=entry->workingDirectory.empty()?nullptr:entry->workingDirectory.c_str();}
    sei.nShow = SW_SHOWNORMAL;
    AllowSetForegroundWindow(ASFW_ANY);
    return ShellExecuteExW(&sei);
}
void LaunchTarget(const std::wstring& target, PCWSTR verb = nullptr,
                  const App* entry = nullptr, bool quiet = false) {
    if (RequestLaunch(target,verb,entry,quiet)) Close();
    else if (GetLastError() != ERROR_CANCELLED) {
        // A cancelled UAC prompt is the user saying no, not a broken entry.
        bool previous=g_modal;
        g_modal=true;
        MessageBoxW(g_wnd,Tr(L"Could not open this entry. Use Edit entry to repair its target."),Tr(L"Launch failed"),MB_OK|MB_ICONWARNING);
        g_modal=previous;
        if(g_shown)ForceForeground(g_wnd);
    } else if (g_shown) {
        Close();
    }
}

void ShowPowerUpdateMenu(HWND owner, POINT point) {
    HMENU menu = CreatePopupMenu();
    if (!menu) return;
    DisarmPower();
    if (g_powerUpdateStatus == PowerUpdateStatus::RestartRequired) {
        AppendMenuW(menu, MF_STRING | MF_GRAYED, 2, Tr(L"Windows Update needs a restart"));
        AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    }
    AppendMenuW(menu, MF_STRING, 1, Tr(L"Open Windows Update"));
    SetMenuDefaultItem(menu, 1, FALSE);
    const auto command = TrackLaunchpadPopupMenu(menu, TPM_RETURNCMD | TPM_NONOTIFY | TPM_RIGHTBUTTON,
                                                point.x, point.y, owner, nullptr);
    DestroyMenu(menu);
    if (command == 1) LaunchTarget(L"ms-settings:windowsupdate");
}
template<typename Request>
std::vector<std::wstring> RequestSelectedApps(const AppSelection& selection,Request request) {
    std::vector<std::wstring> failures;
    for(const auto& app:selection)if(!request(*app))failures.push_back(app->name);
    return failures;
}
void LaunchSelectedApps() {
    if(g_batchLaunching||g_appSelection.empty())return;
    g_batchLaunching=true;
    bool wasModal=std::exchange(g_modal,true);
    auto selection=g_appSelection;
    g_batchHover=-1;
    auto failures=RequestSelectedApps(selection,[](const App& app){
        return RequestLaunch(app.target,nullptr,&app,true);
    });
    ClearAppSelection();
    if(!failures.empty()) {
        std::wstring message=CountLabel(L"Could not open {0} selected apps.",failures.size());
        for(size_t i=0;i<std::min<size_t>(failures.size(),8);i++)message+=L"\n"+failures[i];
        if(failures.size()>8)message+=L"\n…";
        MessageBoxW(g_wnd,message.c_str(),Tr(L"Launch failed"),MB_OK|MB_ICONWARNING);
    }
    g_modal=wasModal;g_batchLaunching=false;
    if(failures.size()<selection.size())Close();
    else Render();
}


// The settings window, from the "..." menu. Its overrides are saved in this
// mod's writable storage. A later Windhawk edit to the same key takes priority.

constexpr WCHAR kSettingsClass[] = L"WindhawkRYJSLaunchpadSettings";
constexpr UINT WM_APP_SETTINGS_RELOAD = WM_APP + 1, WM_APP_SETTINGS_ACTIVATE = WM_APP + 2;
constexpr UINT WM_APP_SETTINGS_TICK = WM_APP + 3;
HANDLE g_settingsThread = nullptr;
DWORD g_settingsThreadId = 0;
std::atomic<HWND> g_settingsWnd;
std::atomic<bool> g_settingsStop;
// The settings page is not the launcher's GPU frame loop, but it can still
// use a high-resolution waitable timer instead of SetTimer's ~15.6 ms tick.
HANDLE g_setFrameWake = nullptr;
HANDLE g_setFrameThread = nullptr;
std::atomic<bool> g_setFrameStop{false};
volatile LONG g_setFramePending = 0;
void Settings_Close();
static int GetState(PCWSTR key, int fallback) { return Wh_GetIntValue(key, fallback); }
static void SetState(PCWSTR key, int value) { Wh_SetIntValue(key, value); }
static std::wstring StateKey(PCWSTR window, PCWSTR field) { return std::wstring(window)+L"_"+field; }
// The settings window's own palette. The launcher panel keeps its separate
// themeMode tint; this only decides how the settings window is painted, so
// "light" no longer leaves a dark settings window behind.
namespace settings_palette {
struct Tones {
    bool light;
    UINT32 base, mantle, crust, surface0, surface1, surface2, overlay0, overlay1,
        overlay2, subtext0, subtext1, text, red, peach, green, onAccent, button1,
        button2;
};
const Tones& Dark() {
    static const Tones tones{false, 0x242424, 0x1D1D1D, 0x171717, 0x343434, 0x494949,
                             0x606060, 0x777777, 0x909090, 0xA0A0A0, 0xB4B4B4, 0xCCCCCC,
                             0xFFFFFF, 0xF38BA8, 0xFAB387, 0xA6E3A1, 0x171717, 0xE0E0E0,
                             0xFFFFFF};
    return tones;
}
const Tones& Light() {
    // Same roles, inverted: white cards on a pale chrome, ink text, and the
    // status colours darkened so they still read on white.
    static const Tones tones{true, 0xFFFFFF, 0xF7F7F7, 0xECECEC, 0xE9E9E9, 0xDCDCDC,
                             0xCACACA, 0x9A9A9A, 0x7E7E7E, 0x666666, 0x555555, 0x333333,
                             0x111111, 0xB3261E, 0x9A5B00, 0x2E6B32, 0x141414, 0xD9D9D9,
                             0xF2F2F2};
    return tones;
}
bool SystemLight() {
    DWORD value = 0, size = sizeof(value);
    RegGetValueW(HKEY_CURRENT_USER,
                 L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                 L"AppsUseLightTheme", RRF_RT_REG_DWORD, nullptr, &value, &size);
    return value != 0;
}
long g_epoch = 1;  // Bumped when the theme may have changed.
void Refresh() { ++g_epoch; }
const Tones& Active() {
    static long epoch = -1;
    static bool light = false;
    if (epoch != g_epoch) {
        epoch = g_epoch;
        const std::wstring theme = GetStringSetting(L"themeMode");
        light = theme == L"light" ? true
                : theme == L"dark"  ? false
                                    : SystemLight();  // "system" and "custom"
    }
    return light ? Light() : Dark();
}
}  // namespace settings_palette
inline const settings_palette::Tones& pal() { return settings_palette::Active(); }
inline float SetGlassTint() { return pal().light ? .88f : .68f; }
inline D2D1_COLOR_F D2DColor(UINT32 rgb, float a = 1.0f) {
    return D2D1::ColorF(((rgb >> 16) & 0xFF) / 255.0f, ((rgb >> 8) & 0xFF) / 255.0f,
                        (rgb & 0xFF) / 255.0f, a);
}
inline COLORREF RefFromRgb(UINT32 rgb) { return RGB((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF); }

// ---------------------------------------------------------------------------
// 50-settings.cpp : the mod's own settings window. A sidebar of pages, grouped
// cards, toggles, dropdowns, sliders, key boxes and colour chips, all drawn
// with Direct2D in the style of Seamless Dictate's settings. It shows the same
// setting keys as Windhawk's page, using a private atomic settings store and
// a reload message. It requires neither an elevated CLI nor admin rights.
//
// One window, one Direct2D target that fills the whole client rect (the window
// edge is rounded by DWM, never by us). Text typed into a number, text or colour
// box goes through one small native EDIT that sits on top of the box while it is
// edited. Key boxes read WM_KEYDOWN themselves.
// ---------------------------------------------------------------------------

template <class T>
class SetCom {
   public:
    T* p = nullptr;
    SetCom() = default;
    SetCom(const SetCom& o) : p(o.p) {
        if (p) p->AddRef();
    }
    SetCom& operator=(const SetCom& o) {
        if (o.p) o.p->AddRef();
        reset();
        p = o.p;
        return *this;
    }
    ~SetCom() { reset(); }
    void reset() {
        if (p) p->Release();
        p = nullptr;
    }
    T** put() {
        reset();
        return &p;
    }
    T* get() const { return p; }
    T* operator->() const { return p; }
    explicit operator bool() const { return p != nullptr; }
};

struct SetField {
    enum Kind { Choice, Number, Text, Hotkey, Check, Color };
    Kind kind;
    PCWSTR key;
    PCWSTR label;
    PCWSTR desc = nullptr;
    PCWSTR def = L"";                                // The schema default ("Reset to suggested").
    std::vector<std::pair<std::wstring, std::wstring>> options;  // Value, label. Owned: the monitor list is built per window.
    int min = 0, max = 0;
    int page = 0;        // Which sidebar page shows it.
    PCWSTR group = L"";  // Heading above it on that page.
    std::wstring value;  // As loaded, so a save writes only what changed.
    std::wstring cur;    // What the window shows now.
    float fitW = 0;      // Box width that fits the text (Set_FitBoxes); 0 until measured.
    float fitText = -1;  // Measured text width behind fitW.
    std::wstring fitFor; // The text that was measured.
};

static bool Set_HasSlider(const SetField& f) {
    return f.max - f.min <= 100 || wcscmp(f.key,L"openDuration")==0 ||
        wcscmp(f.key,L"closeDuration")==0 || wcscmp(f.key,L"folderDuration")==0 ||
        wcscmp(f.key,L"materialBlur")==0 || wcscmp(f.key,L"materialSaturation")==0 ||
        wcscmp(f.key,L"materialBrightness")==0 || wcscmp(f.key,L"materialNoiseScale")==0 ||
        wcscmp(f.key,L"powerWarningSpread")==0;
}

// Names, descriptions and option labels match the ==WindhawkModSettings== block word for word.
// Generated by test-settings\gen_table.py from src\00-header.cpp.
static const std::vector<std::wstring>& Set_InstalledFontFamilies() {
    static const std::vector<std::wstring> families = [] {
        std::vector<std::wstring> result;
        ComPtr<IDWriteFactory> factory;
        ComPtr<IDWriteFontCollection> collection;
        if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)factory.GetAddressOf())) ||
            FAILED(factory->GetSystemFontCollection(&collection))) return result;
        for (UINT32 i = 0; i < collection->GetFontFamilyCount(); i++) {
            ComPtr<IDWriteFontFamily> family;
            ComPtr<IDWriteLocalizedStrings> names;
            if (FAILED(collection->GetFontFamily(i, &family)) || FAILED(family->GetFamilyNames(&names))) continue;
            UINT32 index = 0, length = 0; BOOL found = FALSE;
            names->FindLocaleName(L"en-us", &index, &found);
            if (!found) index = 0;
            if (FAILED(names->GetStringLength(index, &length))) continue;
            std::wstring text(length + 1, L'\0');
            if (SUCCEEDED(names->GetString(index, text.data(), length + 1))) {
                text.resize(length);
                result.push_back(std::move(text));
            }
        }
        std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) { return ToLower(a) < ToLower(b); });
        result.erase(std::unique(result.begin(), result.end()), result.end());
        return result;
    }();
    return families;
}

static void Set_MonitorLabels(std::vector<std::pair<std::wstring, std::wstring>>& options) {
    const auto displays = EnumerateLauncherDisplays();
    for (size_t i = 0; i < displays.size(); ++i) {
        const auto& display = displays[i];
        auto number = std::to_wstring(i + 1);
        std::wstring label = std::wstring(Tr(L"Monitor")) + L" " + number;
        if (!display.friendly.empty()) label += L" · " + display.friendly;
        label += L" (" + std::to_wstring(display.area.right - display.area.left) + L"×" +
                 std::to_wstring(display.area.bottom - display.area.top) + L")";
        options.emplace_back(display.key.empty() ? std::move(number) : display.key, std::move(label));
    }
}

static std::vector<SetField> Set_MakeFields() {
    using K = SetField;
    std::vector<SetField> fields = {
// Custom settings begin
{K::Check, L"gpuPresenter", L"GPU composition", L"Use GPU composition for smoother animation. Turn off for the compatibility renderer.", L"1", {}, 0, 100, 1, L"Rendering"},
{K::Check, L"performanceLogging", L"Record performance", L"Record opening latency, frame intervals and rendering cost in the mod storage folder.", L"0", {}, 0, 100, 1, L"Rendering"},
{K::Number, L"frameRateLimit", L"Frame rate limit", L"0 follows the selected monitor. Other values cap animation updates without changing the monitor.", L"0", {}, 0, 1000, 1, L"Rendering"},
{K::Choice, L"launchpadAnimation", L"Startup animation style", L"Choose how the launcher opens and closes.", L"iconFlyIn", {{L"iconFlyIn", L"iOS icon fly-in"}, {L"classic", L"Classic zoom and fade"}}, 0, 100, 1, L"Motion"},
{K::Number, L"openDuration", L"Open duration (ms)", L"Duration of the opening transition.", L"280", {}, 60, 1500, 1, L"Motion"},
{K::Number, L"closeDuration", L"Close duration (ms)", L"Duration of the closing transition.", L"180", {}, 60, 1500, 1, L"Motion"},
{K::Number, L"folderDuration", L"Folder duration (ms)", L"Duration of folder expansion and collapse.", L"180", {}, 60, 1500, 1, L"Motion"},
{K::Choice, L"animationEasing", L"Animation easing", L"Choose how movement slows down. These curves do not overshoot.", L"settle", {{L"settle", L"Gentle settle"}, {L"cubic", L"Cubic ease-out"}, {L"quint", L"Quintic ease-out"}}, 0, 100, 1, L"Motion"},
{K::Check, L"reducedMotion", L"Reduce motion", L"Use immediate transitions. Windows animation accessibility settings are also respected.", L"0", {}, 0, 100, 1, L"Motion"},
{K::Number, L"labelFontSize", L"Application name size", L"Text size in DIP. Display scaling is applied automatically.", L"13", {}, 9, 32, 1, L"Typography"},
{K::Number, L"textShadowOpacity", L"Text shadow opacity", L"0 turns off the shadow. 100 makes it fully opaque.", L"72", {}, 0, 100, 1, L"Typography"},
{K::Number, L"textShadowBlur", L"Text shadow blur", L"Blur radius at 100% display scaling. 0 gives the shadow a hard edge.", L"2", {}, 0, 6, 1, L"Typography"},
{K::Number, L"textOutlineOpacity", L"Text outline opacity", L"0 turns off the outline. 100 makes it fully opaque.", L"75", {}, 0, 100, 1, L"Typography"},
{K::Number, L"textOutlineWidth", L"Text outline width", L"Outline width at 100% display scaling. 0 turns off the outline.", L"1", {}, 0, 3, 1, L"Typography"},
{K::Number, L"horizontalSpacing", L"Horizontal icon spacing", L"Preferred horizontal space between icons, in DIP.", L"56", {}, 8, 240, 1, L"Grid"},
{K::Number, L"verticalSpacing", L"Vertical icon spacing", L"Additional space between icon rows, in DIP.", L"16", {}, 0, 180, 1, L"Grid"},
{K::Number, L"groupWidth", L"Group width", L"Preferred group width in DIP. 0 adapts to the screen and number of groups.", L"0", {}, 0, 1600, 1, L"Expanded groups"},
{K::Number, L"groupSpacing", L"Group spacing", L"Minimum horizontal and vertical space between groups, in DIP.", L"32", {}, 8, 160, 1, L"Expanded groups"},
{K::Choice, L"groupFlow", L"Group arrangement", L"Arrange groups in columns or use one full-width group per row.", L"columns", {{L"columns", L"Responsive columns"}, {L"rows", L"Full-width rows"}}, 0, 100, 1, L"Expanded groups"},
{K::Choice, L"groupNavigation", L"Group navigation", L"Large groups continue on the next page when paging is selected.", L"scroll", {{L"scroll", L"Vertical scrolling"}, {L"pages", L"Horizontal pages"}}, 0, 100, 1, L"Expanded groups"},
{K::Choice, L"themeMode", L"Theme mode", L"Choose a preset, follow Windows, or keep your own colors.", L"custom", {{L"custom", L"Custom"}, {L"light", L"Light"}, {L"dark", L"Dark"}, {L"system", L"Follow Windows"}}, 0, 100, 2, L"Theme"},
{K::Text, L"powerOrder", L"Power button order", L"Comma-separated action keys in left-to-right order: lock, signOut, sleep, hibernate, restart, shutDown.", L"lock,signOut,sleep,hibernate,restart,shutDown", {}, 0, 100, -1, L"Power buttons"},
{K::Choice, L"blurMode", L"Background material", L"How the backdrop is drawn. The adjustable material is the one the controls below shape; the Windows materials are drawn by DWM and ignore blur, grade and grain.", L"custom", {{L"custom", L"Adjustable material"}, {L"acrylicSystem", L"Windows 11 system acrylic"}, {L"mica", L"Windows 11 Mica"}, {L"micaAlt", L"Windows 11 Mica Alt"}, {L"acrylic", L"Windows acrylic (accent)"}, {L"blur", L"Windows blur (accent)"}, {L"transparent", L"Transparent"}, {L"solid", L"Solid colour"}}, 0, 100, 2, L"Material"},
{K::Choice, L"materialPreset", L"Material preset", L"Changes blur, transparency and texture only; your tint colour stays as it is. Custom keeps the controls as they are.", L"Default", {{L"Default", L"Default"}, {L"Clear Glass", L"Clear Glass"}, {L"Light Glass", L"Light Glass"}, {L"Soft Frost", L"Soft Frost"}, {L"Light Acrylic", L"Light Acrylic"}, {L"Dark Acrylic", L"Dark Acrylic"}, {L"macOS Style", L"macOS Style"}, {L"Heavy Frost", L"Heavy Frost"}, {L"Vivid Glass", L"Vivid Glass"}, {L"Minimal Grey", L"Minimal Grey"}, {L"custom", L"Custom"}}, 0, 100, 2, L"Material"},
{K::Text, L"materialPresetName", L"Preset name", L"Name the Save preset button uses. Leave empty for a numbered name.", L"", {}, 0, 100, 2, L"Material"},
{K::Number, L"materialBlur", L"Blur radius (DIP)", L"Gaussian radius of the backdrop, 0 to 120 DIP. 0 keeps the screen sharp.", L"36", {}, 0, 120, 2, L"Material"},
{K::Number, L"materialSaturation", L"Saturation (%)", L"0 is grey, 100 keeps the backdrop's colour, 200 doubles it.", L"100", {}, 0, 200, 2, L"Material"},
{K::Number, L"materialBrightness", L"Brightness (%)", L"40 to 160. 100 keeps the backdrop as captured.", L"100", {}, 40, 160, 2, L"Material"},
{K::Number, L"materialContrast", L"Contrast (%)", L"50 to 150, pivoting on mid grey. 100 keeps the backdrop as captured.", L"100", {}, 50, 150, 2, L"Material"},
{K::Number, L"backgroundOpacity", L"Tint opacity (%)", L"0 shows only the material, 100 covers it with the tint colour.", L"30", {}, 0, 100, 2, L"Material"},
{K::Color, L"tintColor", L"Tint colour", L"Hexadecimal, like #1E1E2E. Mixed over the material at the tint opacity.", L"#000000", {}, 0, 100, 2, L"Material"},
{K::Number, L"materialLuminosity", L"Adaptive haze (%)", L"Adds a light haze over a dark backdrop and a dark one over a bright backdrop, like acrylic's luminosity layer.", L"0", {}, 0, 100, 2, L"Material"},
{K::Number, L"materialNoise", L"Grain (%)", L"0 to 15. Adds fine noise so large flat areas do not band.", L"0", {}, 0, 15, 2, L"Material"},
{K::Number, L"materialNoiseScale", L"Grain size (%)", L"50 to 200. Larger grain is coarser.", L"100", {}, 50, 200, 2, L"Material"},
{K::Number, L"backgroundDim", L"Dim (%)", L"0 to 80. Darkens the finished material for readability.", L"0", {}, 0, 80, 2, L"Material"},
{K::Number, L"powerShadowOpacity", L"Power icon shadow opacity", L"Shadow strength behind the power dock glyphs. It is separate from the text shadow.", L"72", {}, 0, 100, 3, L"Power icon shadow"},
{K::Number, L"powerShadowBlur", L"Power icon shadow blur", L"Blur radius for the power dock glyphs, at 100% display scaling.", L"2", {}, 0, 6, 3, L"Power icon shadow"},
{K::Number, L"glowDistance", L"Glow trigger distance", L"Sets how far the nearest app icon reacts outside its bounds and how quickly its rim lights as the pointer moves away from the icon centre. At 80, a small movement lights the facing edge.", L"24", {}, 0, 80, 2, L"Appearance"},
{K::Check, L"webIconBackgroundFetch", L"Background website icon download", L"Fetch missing icons for websites and desktop URL shortcuts after the app list loads. Turn off to allow downloads only when you save or manually refresh an entry; cached icons still work.", L"1", {}, 0, 100, 3, L"Network"},
// Custom settings end
{K::Choice, L"interfaceLanguage", L"Interface language", L"Follow Windows display language. Save to apply; app names stay unchanged.", L"auto", {{L"auto", L"Automatic (Windows)"}, {L"zh-CN", L"Simplified Chinese"}, {L"en", L"English"}}, 0, 100, 2, L"Language"},
{K::Choice, L"uiFont", L"Interface font", L"Choose a locally installed font. Applies to text only; symbol icons use their own font.", L"auto", {{L"auto", L"Automatic (language default)"}}, 0, 100, 2, L"Typography"},
{K::Choice, L"winKeyAction", L"Windows key", L"Also Ctrl+Esc.", L"grid", {{L"grid", L"Open this grid"}, {L"start", L"Open the original Start"}, {L"program", L"Run the program below"}}, 0, 100, 0, L"Open Lumiflow"},
{K::Choice, L"shiftWinKeyAction", L"Shift + Windows key", L"", L"start", {{L"grid", L"Open this grid"}, {L"start", L"Open the original Start"}, {L"program", L"Run the program below"}}, 0, 100, 0, L"Open Lumiflow"},
{K::Choice, L"startButtonAction", L"Start button", L"The taskbar Start button, the Standalone Dock's, and every other way Start opens that is not the keyboard.", L"grid", {{L"grid", L"Open this grid"}, {L"start", L"Open the original Start"}, {L"program", L"Run the program below"}}, 0, 100, 0, L"Open Lumiflow"},
{K::Text, L"program", L"Program", L"What \"Run the program below\" starts. A command line such as \"C:\\Program Files\\App\\app.exe\" --flag, or a shortcut, file, folder or web address", L"", {}, 0, 100, 0, L"Open Lumiflow"},
{K::Choice, L"monitor", L"Monitor", L"A selected display is remembered by device. While it is disconnected, the grid follows the mouse, with the primary display as a fallback.", L"mouse", {{L"mouse", L"The monitor with the mouse"}}, 0, 100, 1, L"Grid"},
{K::Choice, L"appSource", L"Apps to show", L"With pinned apps, search still finds every app. Right-click an app to pin or unpin it.", L"allApps", {{L"allApps", L"Start menu apps (the All apps list)"}, {L"desktop", L"Desktop icons"}, {L"both", L"Start menu apps and desktop icons"}, {L"pinned", L"Only the apps you pin"}}, 0, 100, 1, L"Grid"},
{K::Choice, L"gridLayout", L"Layout", L"How the grid is arranged.", L"groups", {{L"scroll", L"One list that scrolls up and down"}, {L"pages", L"Pages that flip left and right"}, {L"groups", L"Expanded groups"}}, 0, 100, 1, L"Grid"},
{K::Number, L"globalColumns", L"Columns per row", L"Global grid columns, 1–30. 0 keeps the saved density or estimates it for a new layout. Preview changes in the layout editor.", L"0", {}, 0, 30, 1, L"Grid"},
{K::Check, L"coverScreen", L"Cover the whole screen", L"On, the grid also covers the taskbar, side bars and the Standalone Dock. Off, they stay visible.", L"0", {}, 0, 100, 1, L"Grid"},
{K::Number, L"gridIconSize", L"Icon size", L"24 to 192 (Suggested: 56). Pixels at 100% Windows scaling. The grid uses fewer columns when needed.", L"56", {}, 24, 192, 1, L"Grid"},
{K::Check, L"showLabels", L"Icon labels", L"On, the app name shows under each icon. Off, icons only.", L"1", {}, 0, 100, 1, L"Grid"},
{K::Check, L"normalizeIcons", L"Fill small icons", L"Scale small or padded icons to full size.", L"1", {}, 0, 100, 1, L"Grid"},
{K::Number, L"glowStrength", L"Glow strength", L"0 to 100 (Suggested: 60). How strongly hovered buttons glow.", L"60", {}, 0, 100, 2, L"Appearance"},
{K::Number, L"chromeOpacity", L"Search bar and dock opacity (%)", L"0 to 100 (Suggested: 90). How solid the search bar and the power dock are.", L"90", {}, 0, 100, 2, L"Appearance"},
{K::Check, L"powerBar", L"Power dock", L"The pill of power buttons at the bottom of the grid.", L"1", {}, 0, 100, 3, L"Power buttons"},
{K::Choice, L"dockAlignment", L"Dock position", L"Where the power dock sits along the bottom edge.", L"center", {{L"left", L"Left"}, {L"center", L"Center"}, {L"right", L"Right"}}, 0, 100, 3, L"Power buttons"},
{K::Check, L"powerButtons.lock", L"Lock", L"", L"1", {}, 0, 100, 3, L"Power buttons"},
{K::Check, L"powerButtons.signOut", L"Sign out", L"", L"1", {}, 0, 100, 3, L"Power buttons"},
{K::Check, L"powerButtons.sleep", L"Sleep", L"", L"1", {}, 0, 100, 3, L"Power buttons"},
{K::Check, L"powerButtons.hibernate", L"Hibernate", L"Shows only when hibernation is turned on in Windows.", L"1", {}, 0, 100, 3, L"Power buttons"},
{K::Check, L"powerButtons.restart", L"Restart", L"", L"1", {}, 0, 100, 3, L"Power buttons"},
{K::Check, L"powerButtons.shutDown", L"Shut down", L"", L"1", {}, 0, 100, 3, L"Power buttons"},
{K::Check, L"powerNoConfirm", L"Skip confirmation", L"On, a power button acts on the first click. Off, the first click arms it and a second click on the same button confirms.", L"0", {}, 0, 100, 3, L"Power buttons"},
{K::Check, L"powerWarning", L"Warning light", L"One shared red environment light inside the power capsule. It marks only the actions ticked below.", L"1", {}, 0, 100, 3, L"Power warning"},
{K::Check, L"powerWarningActions.lock", L"Lock", L"", L"0", {}, 0, 100, 3, L"Power warning"},
{K::Check, L"powerWarningActions.signOut", L"Sign out", L"", L"1", {}, 0, 100, 3, L"Power warning"},
{K::Check, L"powerWarningActions.sleep", L"Sleep", L"", L"0", {}, 0, 100, 3, L"Power warning"},
{K::Check, L"powerWarningActions.hibernate", L"Hibernate", L"", L"0", {}, 0, 100, 3, L"Power warning"},
{K::Check, L"powerWarningActions.restart", L"Restart", L"", L"1", {}, 0, 100, 3, L"Power warning"},
{K::Check, L"powerWarningActions.shutDown", L"Shut down", L"", L"1", {}, 0, 100, 3, L"Power warning"},
{K::Choice, L"powerWarningFollow", L"Light follows", L"Mouse: the light responds slightly to the pointer within each button. Button: it stays centred on the icon.", L"mouse", {{L"mouse", L"Follow mouse"}, {L"button", L"Follow button"}}, 0, 100, 3, L"Power warning"},
{K::Color, L"powerWarningColor", L"Warning colour", L"Hexadecimal, like #A91223. Only the environment light and its gentle reflection on the label above use it.", L"#A91223", {}, 0, 100, 3, L"Power warning"},
{K::Number, L"powerWarningStrength", L"Warning strength (%)", L"Centre concentration of the warning light. 78 is the reference value.", L"78", {}, 0, 100, 3, L"Power warning"},
{K::Number, L"powerWarningSpread", L"Diffusion range (%)", L"Percent of the 48 DIP reference radius, scaled with the capsule. 100 keeps the reference spread.", L"100", {}, 40, 200, 3, L"Power warning"},
{K::Number, L"powerWarningEdge", L"Edge tint (%)", L"Relative strength of the red along the bottom glass edge.", L"65", {}, 0, 100, 3, L"Power warning"},
{K::Number, L"powerWarningBleed", L"Label bleed (%)", L"Very weak reflection on the text capsule above, relative to the warning light.", L"15", {}, 0, 40, 3, L"Power warning"},
{K::Check, L"wiggle", L"Edit mode wiggle", L"While you drag an icon, all icons wiggle like on iOS.", L"1", {}, 0, 100, 1, L"Grid"},
{K::Number, L"holdTime", L"Drag hold time", L"150 to 1500 (Suggested: 420). Milliseconds to hold an icon before dragging starts.", L"420", {}, 150, 1500, 1, L"Grid"},
{K::Check, L"animations", L"Animations", L"Off, everything snaps instantly. Opening, folders, hover fades, tile moves and wiggle all turn off.", L"1", {}, 0, 100, 1, L"Grid"},
{K::Check, L"settingsGlass", L"Blurred settings window", L"On, the window draws rounded glass over a blurred desktop snapshot (transparent corners on Windows 11 24H2+). Off, it is solid. Applies when you save.", L"1", {}, 0, 100, 2, L"Settings window"},
{K::Check, L"nativeSettingsTitleBar", L"Native Windows title bar", L"Use the Windows title bar and window buttons for this settings window. Applies when you save.", L"0", {}, 0, 100, 2, L"Settings window"},
{K::Check, L"settingsTrayIcon", L"Tray icon", L"A Lumiflow icon in the system tray. Click it to open these settings, right-click it for a menu.", L"1", {}, 0, 100, 2, L"Settings window"}
    };
    for (auto& f : fields) if (wcscmp(f.key, L"uiFont") == 0)
        for (const auto& name : Set_InstalledFontFamilies()) f.options.emplace_back(name, name);
    return fields;
}

struct SetPageInfo {
    PCWSTR name, intro;
    WCHAR glyph;
    // Sidebar caption, when the page title is too long to read well in a 36 px row.
    // Null keeps `name`, so only the pages that need it carry a second string.
    PCWSTR rail;
};
static const SetPageInfo kSetPages[] = {
{L"Keys", L"Choose how you open Lumiflow and other programs.", 0xE765, L"Startup"},
{L"Grid", L"Apps, icon sizes, pages and motion.", 0xE80A, L"Grid layout"},
{L"Look", L"Blur, colors, glow, search bar and this window.", 0xE790},
{L"Power", L"Choose which actions appear in the dock. Use the arrows or Ctrl+Up/Down to change their order.", 0xE7E8},
{L"About", L"Version, credits, support and links.", 0xE946},
};
constexpr int kSetPageCount = (int)(sizeof(kSetPages) / sizeof(kSetPages[0]));
// About sits apart at the end of the sidebar, as in Seamless Dictate.
constexpr int kSetAboutPage = kSetPageCount - 1;
constexpr float kSetAboutGap = 17;

// All sizes are DIPs; the render target does the scaling.
constexpr float kSetPadX = 40, kSetMaxW = 720, kSetRowMin = 56, kSetRowGap = 8, kSetBoxH = 32, kSetRowPad = 16,
                kSetDropItem = 32, kSetRailTop = 76, kSetRailItem = 36, kSetRailPitch = 40;
// The material preview panel in the Look page.
constexpr float kSetPreviewH = 236;
constexpr int kSetDropMax = 9;  // A dropdown with more options than this scrolls.
// The sidebar is a floating dock: dragged between min and max, or collapsed to an icon rail.
constexpr float kSetDockGap = 10, kSetRailDef = 232, kSetRailMin = 200, kSetRailMax = 380, kSetRailMini = 60,
                kSetRailSnap = 130, kSetEdgeGrab = 5;
// The footer is a floating pill dock at the bottom right of the page.
constexpr float kSetFootBtnH = 36, kSetFootPad = 8, kSetFootH = kSetFootBtnH + 2 * kSetFootPad, kSetFootGap = 14;
constexpr float kSetHeadingTop = 26, kSetHeadingSize = 48, kSetIntroTop = 86;
constexpr int kSetChipCount = 10;
constexpr UINT32 kSetChips[kSetChipCount] = {
    0x000000,0x171717,0x292929,0x424242,0x626262,0x808080,0xA0A0A0,0xC0C0C0,0xE0E0E0,0xFFFFFF
};
constexpr UINT_PTR kSetTimerId = 1;
constexpr PCWSTR kSetKofiUrl = L"https://ko-fi.com/DavidHiFi";
constexpr PCWSTR kSetAuthorUrl = L"https://github.com/DavidHiFi";
constexpr PCWSTR kSetDittoUrl = L"https://windhawk.net/mods/fullscreen-start";
constexpr PCWSTR kSetRepoUrl = L"https://github.com/DavidHiFi/davids-windhawk-mods";
// Title is resolved through Tr so the caption matches the current interface language.
// The self-drawn title bar. Everything below it is laid out as before, in "content" DIPs.
static float kSetCapH = 40;
constexpr float kSetCornerDip = 20;  // The custom-composed outer radius.
constexpr float kSetCapBtnW = 46;

struct SetItem {
    enum Type { Group, Row, Banner, About, Link, Support, Preview, Actions } type = Row;
    int field = -1;  // Row: index into fields.
    int previewKind = 0;  // 0 material, 1 power warning.
    float y = 0, h = 0;
    std::wstring text, sub, url;
    int powerPosition = -1;  // Position among all six power actions.
};

// What the pointer is over. id picks the hover glow.
struct SetHit {
    enum Kind { None, Rail, Row, Control, PowerMove, Slider, Clear, Chip, Footer, Drop, Link, Scroll, Popup, Caption, RailToggle, RailEdge, Action } kind = None;
    int a = -1, b = -1;
    int id = -1;
};

struct SetWindow {
    HWND hwnd = nullptr, edit = nullptr, owner = nullptr;
    HFONT editFont = nullptr;
    HBRUSH editBrush = nullptr;
    std::vector<SetField> fields = Set_MakeFields();
    std::vector<SetItem> items;
    UINT dpi = 96;
    float cw = 980, ch = 700;  // Content size in DIPs: the client area below the title bar.
    HICON iconSmall = nullptr, iconBig = nullptr;
    int capDown = 0;  // Title bar button held down: 1 minimize, 2 maximize, 3 close.
    bool capTracking = false, active = true;
    bool nativeTitle = false;  // Windows draws the title bar and its buttons.
    bool glass = false;        // Acrylic behind the window; the page is drawn see-through.
    ULONGLONG lastTick = 0;
    int page = 0, recording = -1, editing = -1, focus = -1, openSel = -1;
    bool kbd = false;  // Show the focus ring (after a Tab).
    D2D1_RECT_F dropRect{};
    int dropHover = -1, dropTop = 0;  // dropTop: first option shown when the dropdown scrolls.
    bool frameApplied = false;        // Set_ApplyFrame has run once.
    float scrollY = 0, scrollTarget = 0, contentH = 0, contentX = 0, contentW = 720, pageT = 1;
    SetHit hover;
    int dragSlider = -1;
    bool dragScroll = false, tracking = false, timer = false;
    float dragOff = 0;
    // Sidebar dock: railW is the chosen open width, railCur the drawn width (eased toward
    // kSetRailMini when collapsed).
    float railW = kSetRailDef, railCur = kSetRailDef;
    bool railCollapsed = false, dragRail = false;
    float mouseX = -1, mouseY = -1;
    std::map<int, float> glow;  // Eased hover amount by hit id.
    std::map<int, float> knob;  // Eased toggle position by field.
    std::wstring status;
    bool statusError = false;
    // The material preview: a baked miniature, rebuilt only when its inputs change.
    SetCom<ID2D1Bitmap> preview;
    std::wstring previewKey;
    std::vector<uint32_t> previewWallpaper;  // Cached scene: only the bake repeats.
    std::vector<launchpad_material::UserPreset> userPresets;  // Loaded from materialUserPresets.
    int previewWallpaperW = 0, previewWallpaperH = 0, previewWallpaperTheme = -1;
    bool saving = false, destroyed = false, render = false;
    // The live window renders into a 32-bit DIB so DWM can consume premultiplied
    // alpha (DWMWA_REDIRECTIONBITMAP_ALPHA). Tests and PNGs still pass their own
    // WIC target straight to Set_Draw.
    SetCom<ID2D1RenderTarget> rt;
    SetCom<ID2D1DCRenderTarget> dcRt;
    HDC memDC = nullptr;
    HBITMAP memBitmap = nullptr;
    HBITMAP memOldBitmap = nullptr;
    void* memBits = nullptr;
    int memW = 0, memH = 0;
    bool alphaComposition = false;  // Custom title bar, no acrylic, per-pixel alpha.
    // A downscaled, blurred snapshot of the whole virtual desktop, captured
    // while this window is still hidden. It is the glass backdrop; the page's
    // own tint and grain are drawn over it.
    std::vector<uint8_t> deskBits;
    int deskW = 0, deskH = 0;
    RECT deskVirtual{};
    SetCom<ID2D1Bitmap> deskBitmap;
    SetCom<ID2D1Bitmap> mark;
    SetCom<ID2D1RoundedRectangleGeometry> roundGeo;
    SetCom<ID2D1Layer> roundLayer;
    ID2D1RenderTarget* roundTarget = nullptr;
    D2D1_SIZE_F roundSize{};
    std::wstring family, iconFamily;
    SetCom<IDWriteTextFormat> fLabel, fHint, fTitle, fIntro, fSection, fValue, fValueC, fButton, fIcon, fIconS,
        fIconL, fBig, fSmall, fStatusLine, fCap, fCapGlyph;
    RECT normalRect{};  // Last restored-state rect, remembered on close.
    COLORREF customColors[16] = {};
};

// These four outlive every path that can release them safely, so none of them
// may be released by a static destructor. On a normal unload the settings
// thread releases them in WM_NCDESTROY and SettingsThreadTeardown(); when
// Explorer exits with the window open, Wh_ModUninit never runs, the thread is
// already gone, and a destructor here would release a render target and three
// factories on the exiting thread under the loader lock. no_destroy leaves
// that to process teardown, which is what the grid's own objects do too.
[[clang::no_destroy]] static std::shared_ptr<SetWindow> g_setWin;
static UINT g_setForceDpi;  // Harness renders at a chosen scale.
[[clang::no_destroy]] static SetCom<ID2D1Factory> g_setD2d;
[[clang::no_destroy]] static SetCom<IDWriteFactory> g_setDw;
[[clang::no_destroy]] static SetCom<IWICImagingFactory> g_setWic;
#ifdef SCM_HARNESS
static std::wstring g_setFakeKeyStatus;  // Lets the test render the hotkey warning line.
#endif

static std::wstring Set_WindowText(HWND hwnd) {
    std::wstring text(GetWindowTextLengthW(hwnd) + 1, L'\0');
    text.resize(GetWindowTextW(hwnd, text.data(), (int)text.size()));
    return text;
}

static std::wstring Set_KeyStatusText() {
#ifdef SCM_HARNESS
    if (!g_setFakeKeyStatus.empty()) return g_setFakeKeyStatus;
#endif
#ifdef SEAMLESS_PREVIEW
    return launchpad_i18n::Text(L"Independent preview. Keyboard hooks and system integration are inactive.");
#else
    return std::wstring();
#endif
}

// ---- Factories and fonts ---------------------------------------------------

static bool Set_EnsureFactories() {
    if (!g_setD2d) {
        D2D1_FACTORY_OPTIONS o{};
        if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &o,
                                     (void**)g_setD2d.put())))
            return false;
    }
    if (!g_setDw && FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
                                               (IUnknown**)g_setDw.put())))
        return false;
    if (!g_setWic && FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                                             __uuidof(IWICImagingFactory), (void**)g_setWic.put())))
        return false;
    return true;
}

static void Set_ReleaseFactories() {
    g_setWic.reset();
    g_setDw.reset();
    g_setD2d.reset();
}

static bool Set_FamilyExists(const std::wstring& name) {
    SetCom<IDWriteFontCollection> sys;
    if (name.empty() || FAILED(g_setDw->GetSystemFontCollection(sys.put(), FALSE))) return false;
    UINT32 index = 0;
    BOOL exists = FALSE;
    return SUCCEEDED(sys->FindFamilyName(name.c_str(), &index, &exists)) && exists;
}

// The font setting, or its Nerd Font build ("Fira Code" is installed as
// "FiraCode Nerd Font"), or Segoe UI.
static std::wstring Set_ResolveFamily() {
    auto chosen = GetStringSetting(L"uiFont");
    if (!chosen.empty() && chosen != L"auto" && Set_FamilyExists(chosen)) return chosen;
    if (launchpad_i18n::chinese.load()) return L"Microsoft YaHei UI";
    if (Set_FamilyExists(std::wstring(L"FiraCode Nerd Font"))) return std::wstring(L"FiraCode Nerd Font");
    std::wstring joined;
    for (WCHAR c : std::wstring(L"FiraCode Nerd Font"))
        if (c != L' ') joined += c;
    for (const std::wstring& face : {joined + L" Nerd Font", std::wstring(L"FiraCode Nerd Font") + L" Nerd Font", joined})
        if (Set_FamilyExists(face)) return face;
    return L"Segoe UI";
}

static float Set_TextW(IDWriteTextFormat* fmt, const std::wstring& text);

static SetCom<IDWriteTextFormat> Set_Fmt(const std::wstring& family, float size, int weight, bool wrap,
                                         DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_LEADING,
                                         bool vcenter = false) {
    SetCom<IDWriteTextFormat> f;
    if (FAILED(g_setDw->CreateTextFormat(family.c_str(), nullptr, (DWRITE_FONT_WEIGHT)weight, DWRITE_FONT_STYLE_NORMAL,
                                         DWRITE_FONT_STRETCH_NORMAL, size, launchpad_i18n::Locale(), f.put())))
        return f;
    f->SetWordWrapping(wrap ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP);
    f->SetTextAlignment(align);
    f->SetParagraphAlignment(vcenter ? DWRITE_PARAGRAPH_ALIGNMENT_CENTER : DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    if (!wrap) {
        SetCom<IDWriteInlineObject> sign;
        g_setDw->CreateEllipsisTrimmingSign(f.get(), sign.put());
        DWRITE_TRIMMING t{DWRITE_TRIMMING_GRANULARITY_CHARACTER, 0, 0};
        f->SetTrimming(&t, sign.get());
    }
    return f;
}

// std::wstring(L"FiraCode Nerd Font") at 500; emphasis is one step heavier, never past bold.
static void Set_MakeFormats(SetWindow& w) {
    w.family = Set_ResolveFamily();
    w.iconFamily = Set_FamilyExists(L"Segoe Fluent Icons") ? L"Segoe Fluent Icons" : L"Segoe MDL2 Assets";
    constexpr int base = DWRITE_FONT_WEIGHT_NORMAL, bold = DWRITE_FONT_WEIGHT_SEMI_BOLD;
    const auto L_ = DWRITE_TEXT_ALIGNMENT_LEADING, C_ = DWRITE_TEXT_ALIGNMENT_CENTER;
    w.fLabel = Set_Fmt(w.family, 14, base, true);
    w.fHint = Set_Fmt(w.family, 12.5f, base, true);
    w.fTitle = Set_Fmt(w.family, 26, bold, false);
    w.fIntro = Set_Fmt(w.family, 13.5f, base, true);
    w.fSection = Set_Fmt(w.family, 13, bold, false);
    w.fValue = Set_Fmt(w.family, 13, base, false, L_, true);
    w.fValueC = Set_Fmt(w.family, 13, base, false, C_, true);
    w.fButton = Set_Fmt(w.family, 13.5f, bold, false, C_, true);
    w.fBig = Set_Fmt(w.family, 20, bold, false);
    w.fSmall = Set_Fmt(w.family, 12, base, false, L_, true);
    w.fIcon = Set_Fmt(w.iconFamily, 16, 400, false, C_, true);
    w.fIconS = Set_Fmt(w.iconFamily, 12, 400, false, C_, true);
    w.fIconL = Set_Fmt(w.iconFamily, 34, 400, false, C_, true);
    w.fCap = Set_Fmt(w.family, 13, bold, false, L_, true);
    w.fCapGlyph = Set_Fmt(w.iconFamily, 10, 400, false, C_, true);
    w.fStatusLine = Set_Fmt(w.family, 12.5f, base, false, L_, true);
}

// ---- Values ----------------------------------------------------------------

static bool Set_IsPowerOrder(const SetField& f) {
    return std::wstring_view(f.key) == L"powerOrder";
}

static int Set_PowerAction(const SetField& f) {
    constexpr std::wstring_view prefix = L"powerButtons.";
    std::wstring_view key = f.key;
    return key.starts_with(prefix) ? PowerActionIndex(key.substr(prefix.size())) : -1;
}

static std::wstring Set_FieldValue(const SetField& f) {
    if (Set_IsPowerOrder(f)) return FormatPowerOrder(ParsePowerOrder(f.cur));
    switch (f.kind) {
        case SetField::Number: {
            if (f.cur.empty()) return f.value;
            return std::to_wstring(std::clamp(_wtoi(f.cur.c_str()), f.min, f.max));
        }
        case SetField::Check:
            return f.cur == L"0" || f.cur.empty() ? L"0" : L"1";
        default:
            return f.cur;
    }
}

static void Set_PutValue(SetField& f, const std::wstring& value) {
    switch (f.kind) {
        case SetField::Choice: {
            int index = -1;
            for (size_t i = 0; i < f.options.size(); i++) {
                if (value == f.options[i].first) index = (int)i;
            }
            for (size_t i = 0; index < 0 && i < f.options.size(); i++) {
                if (wcscmp(f.def, f.options[i].first.c_str()) == 0) index = (int)i;
            }
            f.cur = f.options[std::max(index, 0)].first;
            break;
        }
        case SetField::Check:
            f.cur = value != L"0" && !value.empty() ? L"1" : L"0";
            break;
        default:
            f.cur = value;
    }
}

static SetField* Set_FieldByKey(SetWindow& w, PCWSTR key);
static void Set_ReloadUserPresets(SetWindow& w);

static void Set_LoadFields(SetWindow& w) {
    settings_palette::Refresh();  // The theme may have changed in this save.
    for (auto& f : w.fields) {
        bool isInt = f.kind == SetField::Number || f.kind == SetField::Check;
        auto value = isInt ? std::to_wstring(IntSetting(f.key, _wtoi(f.def))) : GetStringSetting(f.key);
        if (value.empty()) value = f.def;
        if (wcscmp(f.key, L"monitor") == 0) {
            f.options.resize(1);
            Set_MonitorLabels(f.options);
            if (std::none_of(f.options.begin(), f.options.end(), [&](const auto& option) {
                return option.first == value;
            })) {
                std::wstring label = value.starts_with(L"display:")
                    ? Tr(L"Saved display is disconnected")
                    : std::wstring(Tr(L"Legacy display number")) + L" " + value;
                f.options.emplace_back(value, std::move(label));
            }
        }
        Set_PutValue(f, value);
        f.value = Set_FieldValue(f);  // What the window shows, so an untouched field never saves.
    }
    Set_ReloadUserPresets(w);
}

static int Set_ChangedCount(const SetWindow& w) {
    int n = 0;
    for (auto& f : w.fields)
        if (Set_FieldValue(f) != f.value) n++;
    return n;
}

static bool Set_ParseHex(const std::wstring& text, UINT32& rgb) {
    if (text.size() != 7 || text[0] != L'#') return false;
    UINT32 v = 0;
    for (size_t i = 1; i < 7; i++) {
        WCHAR c = text[i];
        int d = c >= L'0' && c <= L'9' ? c - L'0' : c >= L'a' && c <= L'f' ? c - L'a' + 10 : c >= L'A' && c <= L'F' ? c - L'A' + 10 : -1;
        if (d < 0) return false;
        v = v * 16 + (UINT32)d;
    }
    rgb = v;
    return true;
}

static std::wstring Set_HexFromRgb(UINT32 rgb) {
    WCHAR text[16];
    swprintf_s(text, L"#%06X", rgb & 0xFFFFFF);
    return text;
}

// ---- Material presets ---------------------------------------------------------

static SetField* Set_FieldByKey(SetWindow& w, PCWSTR key) {
    for (auto& f : w.fields)
        if (wcscmp(f.key, key) == 0) return &f;
    return nullptr;
}

// Built-ins, then the user's own, then Custom. A preset that vanished from
// storage falls back to Custom instead of showing a dangling name.
static void Set_RefreshPresetOptions(SetWindow& w) {
    SetField* field = Set_FieldByKey(w, L"materialPreset");
    if (!field) return;
    field->options.clear();
    for (const auto& preset : launchpad_material::Presets())
        field->options.emplace_back(preset.id, preset.id);
    for (const auto& preset : w.userPresets)
        field->options.emplace_back(preset.name, preset.name);
    field->options.emplace_back(L"custom", L"Custom");
    bool known = false;
    for (const auto& option : field->options) known = known || option.first == field->cur;
    if (!known) field->cur = L"custom";
}

static void Set_ReloadUserPresets(SetWindow& w) {
    launchpad_material::ParseUserPresets(GetStringSetting(L"materialUserPresets"),
                                        w.userPresets);
    Set_RefreshPresetOptions(w);
}

static launchpad_material::Preset Set_PresetFromParams(const launchpad_material::Params& p) {
    launchpad_material::Preset preset{};
    preset.blur = (int)std::lround(p.blurSigma);
    preset.saturation = (int)std::lround(p.saturation * 100);
    preset.brightness = (int)std::lround(p.brightness * 100);
    preset.contrast = (int)std::lround(p.contrast * 100);
    preset.tintOpacity = (int)std::lround(p.tintOpacity * 100);
    preset.noise = (int)std::lround(p.noise * 100);
    preset.dim = (int)std::lround(p.dim * 100);
    preset.luminosity = (int)std::lround(p.luminosity * 100);
    preset.tint = launchpad_material::RgbFromColor(p.tint);
    return preset;
}

static void Set_ApplyMaterialParams(SetWindow& w, const launchpad_material::Params& p) {
    auto put = [&](PCWSTR key, std::wstring value) {
        if (auto* f = Set_FieldByKey(w, key)) Set_PutValue(*f, value);
    };
    put(L"materialBlur", std::to_wstring((int)std::lround(p.blurSigma)));
    put(L"materialSaturation", std::to_wstring((int)std::lround(p.saturation * 100)));
    put(L"materialBrightness", std::to_wstring((int)std::lround(p.brightness * 100)));
    put(L"materialContrast", std::to_wstring((int)std::lround(p.contrast * 100)));
    put(L"backgroundOpacity", std::to_wstring((int)std::lround(p.tintOpacity * 100)));
    put(L"materialLuminosity", std::to_wstring((int)std::lround(p.luminosity * 100)));
    put(L"materialNoise", std::to_wstring((int)std::lround(p.noise * 100)));
    put(L"materialNoiseScale", std::to_wstring((int)std::lround(p.noiseScale * 100)));
    put(L"backgroundDim", std::to_wstring((int)std::lround(p.dim * 100)));
    put(L"tintColor", Set_HexFromRgb(launchpad_material::RgbFromColor(p.tint)));
    w.previewKey.clear();
}

static const wchar_t* Set_ChoiceLabel(const SetField& f) {
    for (auto& o : f.options)
        if (f.cur == o.first) return Tr(o.second.c_str());
    return f.options.empty() ? L"" : Tr(f.options[0].second.c_str());
}

static bool Set_Anim(const SetWindow& w) {
    return !w.render && Animate();
}

static void Set_Invalidate(SetWindow& w) {
    if (w.hwnd && !w.render) InvalidateRect(w.hwnd, nullptr, FALSE);
}

static void Set_SetStatus(SetWindow& w, PCWSTR text, bool error = false) {
    w.status = text;
    w.statusError = error;
    Set_Invalidate(w);
}

// ---- Text measuring ---------------------------------------------------------

static float Set_TextH(IDWriteTextFormat* fmt, const std::wstring& text, float width) {
    SetCom<IDWriteTextLayout> l;
    if (!fmt || FAILED(g_setDw->CreateTextLayout(text.c_str(), (UINT32)text.size(), fmt, width, 10000, l.put())))
        return 18;
    DWRITE_TEXT_METRICS m{};
    l->GetMetrics(&m);
    return m.height;
}

static float Set_TextW(IDWriteTextFormat* fmt, const std::wstring& text) {
    SetCom<IDWriteTextLayout> l;
    if (!fmt || FAILED(g_setDw->CreateTextLayout(text.c_str(), (UINT32)text.size(), fmt, 10000, 100, l.put())))
        return 0;
    DWRITE_TEXT_METRICS m{};
    l->GetMetrics(&m);
    return m.widthIncludingTrailingWhitespace;
}

// Boxes grow to fit their text, so no option, hotkey or value is ever cut off: a choice box fits
// its longest option, a hotkey or text box its value. A box never takes more than half the row;
// past that the dropdown list still shows every option in full. Text is measured only when it
// changes. Returns true when a width changed, so the page is laid out again.
static bool Set_FitBoxes(SetWindow& w) {
    if (!w.fValue) return false;
    bool changed = false;
    const float cap = std::floor(w.contentW * 0.5f);
    for (auto& f : w.fields) {
        if (wcscmp(f.key, L"uiFont") == 0) {
            float width = std::min(cap, 280.0f);
            if (f.fitW != width) { f.fitW = width; changed = true; }
            continue;
        }
        float base, pad;
        std::wstring text;
        if (f.kind == SetField::Choice) {
            base = 200, pad = 46;  // 12 before the text, 30 for the arrow, 4 spare.
            for (const auto& o : f.options) text += std::wstring(Tr(o.second.c_str())) + L'\n';
        } else if (f.kind == SetField::Hotkey) {
            base = 220, pad = 60;  // The key cap's own padding and the clear button.
            text = f.cur;
        } else if (f.kind == SetField::Text) {
            base = 280, pad = 26;
            text = f.cur;
        } else {
            continue;
        }
        if (f.fitText < 0 || text != f.fitFor) {
            float m = 0;
            if (f.kind == SetField::Choice) {
                for (const auto& o : f.options) m = std::max(m, Set_TextW(w.fValue.get(), Tr(o.second.c_str())));
            } else {
                m = Set_TextW(w.fValue.get(), f.cur);
            }
            f.fitText = std::ceil(m);
            f.fitFor = text;
        }
        float want = std::min(std::max(base, f.fitText + pad), std::max(base, cap));
        if (fabsf(want - f.fitW) > 0.5f) {
            f.fitW = want;
            changed = true;
        }
    }
    return changed;
}

// ---- Layout -----------------------------------------------------------------

// The right-hand control's width, so the label column knows how much room it has.
static float Set_FitOr(const SetField& f, float base) { return f.fitW > 0 ? f.fitW : base; }

static float Set_ControlW(const SetField& f) {
    switch (f.kind) {
        case SetField::Check: return Set_PowerAction(f) >= 0 ? 120 : 40;
        case SetField::Choice: return Set_FitOr(f, 200);
        case SetField::Number: return Set_HasSlider(f) ? 150 + 12 + 64 : 96;
        case SetField::Text: return Set_FitOr(f, 280);
        case SetField::Hotkey: return Set_FitOr(f, 220);
        case SetField::Color: return 362;
    }
    return 100;
}

static float Set_ViewH(const SetWindow& w) {
    return std::max(0.f, w.ch - kSetFootH - kSetFootGap - 10);
}

static D2D1_RECT_F Set_RailDock(const SetWindow& w) {
    return {kSetDockGap, kSetDockGap, kSetDockGap + w.railCur, std::max(kSetDockGap + 120, w.ch - kSetDockGap)};
}
// Right edge of the sidebar dock: the page starts after it.
static float Set_RailSpace(const SetWindow& w) { return kSetDockGap + w.railCur; }
// 0 collapsed to the icon rail, 1 fully open.
static float Set_RailOpen(const SetWindow& w) {
    return std::clamp((w.railCur - kSetRailMini) / std::max(1.0f, w.railW - kSetRailMini), 0.0f, 1.0f);
}
static D2D1_RECT_F Set_RailToggleRect(const SetWindow& w) {
    D2D1_RECT_F d = Set_RailDock(w);
    return {d.left + 12, d.top + 14, d.left + 48, d.top + 50};
}

// Everything a field's control is made of, in window DIPs for a given row.
struct SetParts {
    D2D1_RECT_F box{}, track{}, clear{}, chip[kSetChipCount]{}, powerMove[2]{};
    bool slider = false;
};

static SetParts Set_Parts(const SetField& f, const D2D1_RECT_F& row) {
    SetParts p;
    float r = row.right - kSetRowPad, mid = (row.top + row.bottom) / 2, h = kSetBoxH / 2;
    switch (f.kind) {
        case SetField::Check:
            p.box = {r - 40, mid - 11, r, mid + 11};
            if (Set_PowerAction(f) >= 0) {
                p.powerMove[0] = {r - 120, mid - 14, r - 88, mid + 14};
                p.powerMove[1] = {r - 83, mid - 14, r - 51, mid + 14};
            }
            break;
        case SetField::Choice:
            p.box = {r - Set_FitOr(f, 200), mid - h, r, mid + h};
            break;
        case SetField::Number:
            if (Set_HasSlider(f)) {
                p.slider = true;
                p.box = {r - 64, mid - h, r, mid + h};
                p.track = {p.box.left - 12 - 150, mid - 2, p.box.left - 12, mid + 2};
            } else {
                p.box = {r - 96, mid - h, r, mid + h};
            }
            break;
        case SetField::Text:
            p.box = {r - Set_FitOr(f, 280), mid - h, r, mid + h};
            break;
        case SetField::Hotkey:
            p.box = {r - Set_FitOr(f, 220), mid - h, r, mid + h};
            p.clear = {p.box.right - 30, p.box.top, p.box.right, p.box.bottom};
            break;
        case SetField::Color: {
            p.box = {r - 96, mid - h, r, mid + h};
            float x = p.box.left - 12 - (kSetChipCount * 26 - 6);
            for (int i = 0; i < kSetChipCount; i++) p.chip[i] = {x + i * 26, mid - 10, x + i * 26 + 20, mid + 10};
            break;
        }
    }
    return p;
}

static bool Set_In(const D2D1_RECT_F& r, float x, float y) { return x >= r.left && x < r.right && y >= r.top && y < r.bottom; }

static D2D1_RECT_F Set_RowRect(const SetWindow& w, const SetItem& it) {
    return {w.contentX, it.y, w.contentX + w.contentW, it.y + it.h};  // Content y (not scrolled).
}

static bool Set_CanMovePower(int position, int step) {
    const int target = position + step;
    return position >= 0 && target >= 0 && target < (int)std::size(kPower);
}

static auto Set_PowerOrderField(SetWindow& w) {
    return std::find_if(w.fields.begin(), w.fields.end(), Set_IsPowerOrder);
}

// Lays out the current page's items for the current width.
// ---- Material preview ---------------------------------------------------------
//
// The panel above the material controls bakes the same fixed scene the presets
// were tuned on, at half resolution, with the current (unsaved) field values.

static const std::vector<std::pair<std::wstring, std::wstring>>& Set_MaterialPreviewKeys() {
    static const std::vector<std::pair<std::wstring, std::wstring>> keys = {
        {L"materialBlur", L"materialBlur"}, {L"materialSaturation", L"materialSaturation"},
        {L"materialBrightness", L"materialBrightness"}, {L"materialContrast", L"materialContrast"},
        {L"backgroundOpacity", L"backgroundOpacity"}, {L"materialLuminosity", L"materialLuminosity"},
        {L"materialNoise", L"materialNoise"}, {L"materialNoiseScale", L"materialNoiseScale"},
        {L"backgroundDim", L"backgroundDim"}, {L"tintColor", L"tintColor"}};
    return keys;
}

static int Set_MaterialNumber(SetWindow& w, PCWSTR key, int fallback) {
    for (const auto& f : w.fields)
        if (wcscmp(f.key, key) == 0) return _wtoi(f.cur.c_str());
    return fallback;
}

static launchpad_material::Params Set_MaterialParams(SetWindow& w) {
    launchpad_material::Params p;
    p.blurSigma = (float)Set_MaterialNumber(w, L"materialBlur", 36);
    p.saturation = Set_MaterialNumber(w, L"materialSaturation", 100) / 100.0f;
    p.brightness = Set_MaterialNumber(w, L"materialBrightness", 100) / 100.0f;
    p.contrast = Set_MaterialNumber(w, L"materialContrast", 100) / 100.0f;
    p.tintOpacity = Set_MaterialNumber(w, L"backgroundOpacity", 30) / 100.0f;
    p.luminosity = Set_MaterialNumber(w, L"materialLuminosity", 0) / 100.0f;
    p.noise = Set_MaterialNumber(w, L"materialNoise", 0) / 100.0f;
    p.noiseScale = Set_MaterialNumber(w, L"materialNoiseScale", 100) / 100.0f;
    p.dim = Set_MaterialNumber(w, L"backgroundDim", 0) / 100.0f;
    for (const auto& f : w.fields)
        if (wcscmp(f.key, L"tintColor") == 0) {
            UINT32 rgb = 0;
            Set_ParseHex(f.cur, rgb);
            p.tint = D2D1::ColorF(rgb);
            break;
        }
    launchpad_material::Clamp(p);
    return p;
}

static std::wstring Set_MaterialPreviewKey(SetWindow& w) {
    std::wstring key;
    for (const auto& entry : Set_MaterialPreviewKeys()) {
        for (const auto& f : w.fields)
            if (wcscmp(f.key, entry.first.c_str()) == 0) {
                key += f.cur;
                key += L'|';
            }
    }
    return key;
}

static const std::vector<uint8_t>& Set_PreviewNoise() {
    static const std::vector<uint8_t> tile = [] {
        std::vector<uint8_t> noise;
        launchpad_material::MakeNoiseTile(noise);
        return noise;
    }();
    return tile;
}

static void Set_PreparePreview(SetWindow& w, ID2D1RenderTarget* rt, float width) {
    if (!rt || width < 40) return;
    const std::wstring key = std::to_wstring((int)width) + L":" + Set_MaterialPreviewKey(w);
    // The sample wallpaper belongs to the settings window, so it follows the
    // window's palette rather than the launcher's separate tint setting.
    const int theme = pal().light ? 1 : 0;
    if (w.preview && key == w.previewKey && w.previewWallpaperTheme == theme) return;
    const auto params = Set_MaterialParams(w);
    const int bw = std::max(48, (int)std::lround(width / 2));
    const int bh = (int)std::lround(kSetPreviewH / 2);
    // The scene is the same for every parameter, so only its first build pays
    // for the gradient maths; slider moves pay for the bake alone.
    if (w.previewWallpaperW != bw || w.previewWallpaperH != bh ||
        w.previewWallpaperTheme != theme) {
        launchpad_material::MakeWallpaper(theme, bw, bh, w.previewWallpaper);
        w.previewWallpaperW = bw;
        w.previewWallpaperH = bh;
        w.previewWallpaperTheme = theme;
    }
    std::vector<uint32_t> baked;
    launchpad_material::Bake(w.previewWallpaper, bw, bh, .5f, params,
                             Set_PreviewNoise(), baked);
    D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
                          D2D1_ALPHA_MODE_PREMULTIPLIED),
        96, 96);
    ID2D1Bitmap* bitmap = nullptr;
    if (FAILED(rt->CreateBitmap(D2D1::SizeU((UINT32)bw, (UINT32)bh), baked.data(),
                                  (UINT32)bw * 4, props, &bitmap)))
        return;
    w.preview.reset();
    w.preview.p = bitmap;
    w.previewKey = key;
}


static void Set_BuildPage(SetWindow& w) {
    w.items.clear();
    const float space = Set_RailSpace(w), avail = w.cw - space;
    w.contentW = std::clamp(avail - 2 * kSetPadX, 420.0f, kSetMaxW);
    w.contentX = space + std::max(kSetPadX, (avail - w.contentW) / 2);
    float introH = Set_TextH(w.fIntro.get(), Tr(kSetPages[w.page].intro), w.contentW);
    float y = kSetIntroTop + introH + 24;
    if (w.page == 0) {
        std::wstring status = Set_KeyStatusText();
        if (!status.empty()) {
            SetItem it;
            it.type = SetItem::Banner;
            it.text = status;
            it.y = y;
            it.h = Set_TextH(w.fHint.get(), status, w.contentW - 32) + 28;
            y += it.h + kSetRowGap + 8;
            w.items.push_back(it);
        }
    }
    std::vector<int> pageFields;
    std::vector<size_t> powerRows;
    std::array<int, std::size(kPower)> fieldByAction{};
    for (int k = 0; k < (int)w.fields.size(); ++k) {
        const auto& field = w.fields[k];
        if (field.page != w.page) continue;
        if (const int action = Set_PowerAction(field); action >= 0) {
            powerRows.push_back(pageFields.size());
            fieldByAction[action] = k;
        }
        pageFields.push_back(k);
    }
    if (powerRows.size() == std::size(kPower)) {
        const auto orderField = Set_PowerOrderField(w);
        const auto order = orderField != w.fields.end() ? ParsePowerOrder(orderField->cur) : DefaultPowerOrder();
        for (size_t i = 0; i < order.size(); ++i) pageFields[powerRows[i]] = fieldByAction[order[i]];
    }

    PCWSTR lastGroup = nullptr;
    int powerPosition = 0;
    auto addMaterialActions = [&] {
        SetItem actions;
        actions.type = SetItem::Actions;
        actions.y = y;
        actions.h = 44;
        y += actions.h + kSetRowGap;
        w.items.push_back(actions);
    };
    for (int k : pageFields) {
        const SetField& f = w.fields[k];
        if (!lastGroup || wcscmp(lastGroup, f.group) != 0) {
            if (lastGroup && wcscmp(lastGroup, L"Material") == 0 &&
                wcscmp(f.group, L"Material") != 0)
                addMaterialActions();
            SetItem g;
            g.type = SetItem::Group;
            g.text = Tr(f.group);
            y += lastGroup ? 14 : 0;
            g.y = y;
            g.h = 22;
            y += g.h + 6;
            w.items.push_back(g);
            lastGroup = f.group;
            if (wcscmp(f.group, L"Material") == 0) {
                SetItem preview;
                preview.type = SetItem::Preview;
                preview.y = y;
                preview.h = kSetPreviewH;
                y += preview.h + kSetRowGap;
                w.items.push_back(preview);
            }
            if (wcscmp(f.group, L"Power warning") == 0) {
                SetItem preview;
                preview.type = SetItem::Preview;
                preview.previewKind = 1;
                preview.y = y;
                preview.h = 142;
                y += preview.h + kSetRowGap;
                w.items.push_back(preview);
            }
        }
        SetItem it;
        it.type = SetItem::Row;
        it.field = k;
        if (Set_PowerAction(f) >= 0) it.powerPosition = powerPosition++;
        float textW = w.contentW - 2 * kSetRowPad - Set_ControlW(f) - 24;
        float lh = Set_TextH(w.fLabel.get(), Tr(f.label), textW);
        float dh = f.desc && *f.desc ? Set_TextH(w.fHint.get(), Tr(f.desc), textW) + 3 : 0;
        it.y = y;
        it.h = std::max(kSetRowMin, 28 + lh + dh);
        y += it.h + kSetRowGap;
        w.items.push_back(it);
    }
    if (lastGroup && wcscmp(lastGroup, L"Material") == 0) addMaterialActions();
    if (w.page == kSetAboutPage) {
        auto add = [&](SetItem::Type t, float h, PCWSTR text, PCWSTR sub, PCWSTR url) {
            SetItem it;
            it.type = t;
            it.y = y;
            it.h = h;
            it.text = text;
            it.sub = sub;
            it.url = url;
            y += h + kSetRowGap;
            w.items.push_back(it);
        };
        auto group = [&](PCWSTR text) {
            SetItem g;
            g.type = SetItem::Group;
            g.text = text;
            g.y = y + 14;
            g.h = 22;
            y = g.y + g.h + 6;
            w.items.push_back(g);
        };
        add(SetItem::About, 146, L"", L"", L"");
        add(SetItem::Support, 84, Tr(L"Like it? Help the mod keep going."),
            Tr(L"Free and open source. A tip on Ko-fi keeps it moving."), kSetKofiUrl);
        group(Tr(L"Author"));
        add(SetItem::Link, 56, Tr(L"DavidHiFi on GitHub"), L"github.com/DavidHiFi", kSetAuthorUrl);
        add(SetItem::Link, 56, L"Ko-fi", L"ko-fi.com/DavidHiFi", kSetKofiUrl);
        group(Tr(L"Credits"));
        add(SetItem::Link, 56, Tr(L"Based on Fullscreen Start by Rafael Mendes"), Tr(L"The original Windhawk mod"), kSetDittoUrl);
        group(Tr(L"Project"));
        add(SetItem::Link, 56, Tr(L"Upstream source on GitHub"), L"github.com/DavidHiFi/davids-windhawk-mods", kSetRepoUrl);
    }
    w.contentH = y + 16;
    w.scrollTarget = std::clamp(w.scrollTarget, 0.0f, std::max(0.0f, w.contentH - Set_ViewH(w)));
    w.scrollY = std::clamp(w.scrollY, 0.0f, std::max(0.0f, w.contentH - Set_ViewH(w)));
}

static const SetItem* Set_ItemOfField(const SetWindow& w, int field) {
    for (auto& it : w.items)
        if (it.type == SetItem::Row && it.field == field) return &it;
    return nullptr;
}

// The footer's status line: an explicit message, else the count of unsaved changes.
static std::wstring Set_FooterStatus(const SetWindow& w, UINT32* color) {
    std::wstring status = w.status;
    if (color) *color = w.statusError ? pal().red : (status == Tr(L"Saved") ? pal().green : pal().subtext0);
    int changed = Set_ChangedCount(w);
    if (status.empty() && changed)
        status = changed == 1 ? Tr(L"1 change not saved") : std::to_wstring(changed) + Tr(L" changes not saved");
    return status;
}

constexpr float kSetFootW[3] = {172, 96, 100};  // Reset to suggested, Cancel, Save.

static float Set_FooterButtonsW() { return kSetFootW[0] + kSetFootW[1] + kSetFootW[2] + 2 * 8; }

// Width the status text gets inside the footer dock (0 when there is none).
static float Set_FooterStatusW(const SetWindow& w) {
    std::wstring s = Set_FooterStatus(w, nullptr);
    if (s.empty()) return 0;
    float room = w.contentW - Set_FooterButtonsW() - 2 * kSetFootPad - 30;
    return std::max(0.0f, std::min(Set_TextW(w.fStatusLine.get(), s) + 2, room));
}

// The pill that holds the status and the three buttons, sized to fit them, right-aligned to the page.
static D2D1_RECT_F Set_FooterDock(const SetWindow& w) {
    float sw = Set_FooterStatusW(w);
    float width = Set_FooterButtonsW() + 2 * kSetFootPad + (sw > 0 ? sw + 30 : 0);
    float right = w.contentX + w.contentW, bottom = w.ch - kSetFootGap;
    return {right - width, bottom - kSetFootH, right, bottom};
}

static D2D1_RECT_F Set_FooterButton(const SetWindow& w, int which) {  // 0 reset, 1 cancel, 2 save
    D2D1_RECT_F d = Set_FooterDock(w);
    float x = d.right - kSetFootPad, top = d.top + kSetFootPad;
    D2D1_RECT_F out{};
    for (int i = 2; i >= 0; i--) {
        out = {x - kSetFootW[i], top, x, top + kSetFootBtnH};
        if (i == which) return out;
        x -= kSetFootW[i] + 8;
    }
    return out;
}

static D2D1_RECT_F Set_RailRect(const SetWindow& w, int page) {
    D2D1_RECT_F d = Set_RailDock(w);
    float y = d.top + kSetRailTop + kSetRailPitch * page + (page == kSetAboutPage ? kSetAboutGap : 0);
    return {d.left + 8, y, d.right - 8, y + kSetRailItem};
}

static D2D1_RECT_F Set_SupportButton(const SetWindow& w, const SetItem& it) {
    D2D1_RECT_F r = Set_RowRect(w, it);
    return {r.right - kSetRowPad - 148, (r.top + r.bottom) / 2 - 18, r.right - kSetRowPad, (r.top + r.bottom) / 2 + 18};
}

static D2D1_RECT_F Set_ScrollThumb(const SetWindow& w) {
    float view = Set_ViewH(w), range = w.contentH - view;
    if (range <= 0) return {};
    float th = std::max(36.0f, view * view / w.contentH);
    float top = (w.scrollY / range) * (view - th - 8) + 4;
    return {w.cw - 18, top + 4, w.cw - 12, top + th - 4};
}

// ---- Painting ---------------------------------------------------------------

static D2D1_COLOR_F Set_A1(float a) { return D2DColor(pal().button1, a); }
static D2D1_COLOR_F Set_A2(float a) { return D2DColor(pal().button2, a); }

static float Set_Ease(float t) {
    t = std::clamp(t, 0.0f, 1.0f);
    return 1 - (1 - t) * (1 - t) * (1 - t);
}

struct SetPaint {
    SetWindow& w;
    ID2D1RenderTarget* rt;
    SetCom<ID2D1SolidColorBrush> sb;
    TileGlassResources glass;
    D2D1_POINT_2F pointer{w.mouseX, w.mouseY};
    float fade = 1;
    SetPaint(SetWindow& win, ID2D1RenderTarget* target) : w(win), rt(target) {
        rt->CreateSolidColorBrush(D2DColor(0), sb.put());
    }
    ID2D1SolidColorBrush* B(UINT32 rgb, float a = 1) {
        sb->SetColor(D2DColor(rgb, a * fade));
        return sb.get();
    }
    // Soft white highlight across the control.
    SetCom<ID2D1LinearGradientBrush> G(const D2D1_RECT_F& r, float a = 1) {
        SetCom<ID2D1LinearGradientBrush> out;
        D2D1_GRADIENT_STOP stops[2] = {{0, Set_A1(a * fade)}, {1, Set_A2(a * fade)}};
        SetCom<ID2D1GradientStopCollection> coll;
        if (SUCCEEDED(rt->CreateGradientStopCollection(stops, 2, coll.put())))
            rt->CreateLinearGradientBrush(
                D2D1::LinearGradientBrushProperties(D2D1::Point2F(r.left, (r.top + r.bottom) / 2),
                                                    D2D1::Point2F(std::max(r.right, r.left + 1), (r.top + r.bottom) / 2)),
                coll.get(), out.put());
        return out;
    }
    void Fill(const D2D1_RECT_F& r, UINT32 c, float a = 1) { rt->FillRectangle(r, B(c, a)); }
    void Round(const D2D1_RECT_F& r, float rad, UINT32 c, float a = 1) {
        rt->FillRoundedRectangle(D2D1::RoundedRect(r, rad, rad), B(c, a));
    }
    void Edge(const D2D1_RECT_F& r, float rad, UINT32 c, float a, float width = 1) {
        if (a <= 0.003f) return;
        float h = width / 2;
        rt->DrawRoundedRectangle(D2D1::RoundedRect({r.left + h, r.top + h, r.right - h, r.bottom - h}, rad - h, rad - h), B(c, a), width);
    }
    void GradFill(const D2D1_RECT_F& r, float rad, float a) {
        if (a <= 0.003f) return;
        if (auto g = G(r, a)) rt->FillRoundedRectangle(D2D1::RoundedRect(r, rad, rad), g.get());
    }
    void GradRing(const D2D1_RECT_F& r, float rad, float a, float width = 1.5f) {
        if (a <= 0.003f) return;
        D2D1_RECT_F in{r.left + width / 2, r.top + width / 2, r.right - width / 2, r.bottom - width / 2};
        if (auto g = G(r, a)) rt->DrawRoundedRectangle(D2D1::RoundedRect(in, rad, rad), g.get(), width);
    }
    void Glass(const D2D1_RECT_F& r, float rad, float strength) {
        DrawControlGlass(rt, glass, r, rad, 1, pointer, strength * fade);
    }
    void Panel(const D2D1_RECT_F& r, float rad, float alpha = 1) {
        Round({r.left, r.top + 2, r.right, r.bottom + 2}, rad, 0, .15f);
        Round(r, rad, pal().base, alpha);
        Edge(r, rad, pal().text, .13f);
    }
    void Text(IDWriteTextFormat* f, const std::wstring& s, const D2D1_RECT_F& r, UINT32 c, float a = 1) {
        if (!f || s.empty()) return;
        // Settings text is drawn once. The launcher's pre-rendered shadow
        // bitmaps are appropriate for the desktop grid, but replaying the
        // layout nine times for every label here made scrolling CPU-bound.
        rt->DrawText(s.c_str(), (UINT32)s.size(), f, r, B(c, a), D2D1_DRAW_TEXT_OPTIONS_CLIP);
    }
    void Glyph(IDWriteTextFormat* f, WCHAR g, const D2D1_RECT_F& r, UINT32 c, float a = 1) {
        if (f) rt->DrawText(&g, 1, f, r, B(c, a), D2D1_DRAW_TEXT_OPTIONS_CLIP);
    }
};

static float Set_GlowOf(const SetWindow& w, int id) {
    auto it = w.glow.find(id);
    return it == w.glow.end() ? 0 : it->second;
}

// Ids that count as "pointer is here": the row (or rail item, button, link) and the control inside it.
static void Set_HoverIds(const SetHit& h, int ids[2]) {
    ids[0] = h.id;
    ids[1] = -1;
    if (h.kind == SetHit::Control || h.kind == SetHit::Slider || h.kind == SetHit::Clear || h.kind == SetHit::Chip)
        ids[1] = 6000 + h.a;
    if (h.kind == SetHit::PowerMove) ids[1] = 8000 + 2 * h.a + h.b;
}

static UINT32 SetBoxFill() { return pal().crust; }
static void Set_DrawBox(SetPaint& p, SetWindow& w, int k, const D2D1_RECT_F& box, bool active) {
    p.Round(box, 8, SetBoxFill());
    p.Edge(box, 8, pal().text, active ? .58f : .22f);
    p.Glass(box, 8, Set_GlowOf(w, 6000 + k));
}

static void Set_DrawCard(SetPaint& p, const D2D1_RECT_F& row, float hov) {
    p.Panel(row, 12, p.w.glass ? .83f : 1.f);
    p.Round(row, 12, pal().text, .065f * hov);
    p.Glass(row, 12, hov);
}

static void Set_DrawRow(SetPaint& p, SetWindow& w, const SetItem& it) {
    const SetField& f = w.fields[it.field];
    int k = it.field;
    D2D1_RECT_F row = Set_RowRect(w, it);
    Set_DrawCard(p, row, Set_GlowOf(w, 1000 + k));
    if (w.kbd && w.focus == k) p.GradRing(row, 10, 1.0f, 2.0f);

    float textW = w.contentW - 2 * kSetRowPad - Set_ControlW(f) - 24;
    float lh = Set_TextH(w.fLabel.get(), Tr(f.label), textW);
    float dh = f.desc && *f.desc ? Set_TextH(w.fHint.get(), Tr(f.desc), textW) + 3 : 0;
    float top = row.top + (it.h - lh - dh) / 2;
    p.Text(w.fLabel.get(), Tr(f.label), {row.left + kSetRowPad, top, row.left + kSetRowPad + textW, top + lh + 2}, pal().text);
    if (dh > 0)
        p.Text(w.fHint.get(), Tr(f.desc), {row.left + kSetRowPad, top + lh + 3, row.left + kSetRowPad + textW, top + lh + dh + 3},
               pal().subtext0);

    SetParts pt = Set_Parts(f, row);
    const D2D1_RECT_F& b = pt.box;
    switch (f.kind) {
        case SetField::Check: {
            if (it.powerPosition >= 0) {
                const WCHAR arrows[]{0xE70E, 0xE70D};
                for (int i = 0; i < 2; ++i) {
                    const auto& arrow = pt.powerMove[i];
                    const bool enabled = Set_CanMovePower(it.powerPosition, i == 0 ? -1 : 1);
                    const float hover = enabled ? Set_GlowOf(w, 8000 + 2 * k + i) : 0.f;
                    p.Round(arrow, 8, pal().text, (enabled ? .055f : .02f) + .12f * hover);
                    p.Edge(arrow, 8, pal().text, enabled ? .20f : .07f);
                    p.Glass(arrow, 8, hover);
                    p.Glyph(w.fIconS.get(), arrows[i], arrow, pal().text, enabled ? 1.f : .33f);
                }
            }
            bool on = f.cur == L"1";
            float kn = w.knob.count(k) ? w.knob.at(k) : (on ? 1.0f : 0.0f);
            D2D1_RECT_F tr = pt.box;
            p.Round(tr, 11, pal().crust);
            p.Round(tr, 11, pal().text, .28f * kn);
            p.Edge(tr, 11, pal().text, .20f + .26f * kn);
            p.Glass(tr, 11, Set_GlowOf(w, 6000 + k));
            float cx = tr.left + 11 + kn * 18, cy = (tr.top + tr.bottom) / 2;
            p.rt->FillEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), 8, 8), p.B(on ? pal().text : pal().subtext1));
            break;
        }
        case SetField::Choice: {
            Set_DrawBox(p, w, k, b, w.openSel == k);
            p.Text(w.fValue.get(), Set_ChoiceLabel(f), {b.left + 12, b.top, b.right - 30, b.bottom}, pal().text);
            p.Glyph(w.fIconS.get(), 0xE70D, {b.right - 30, b.top, b.right - 6, b.bottom}, pal().subtext0);
            break;
        }
        case SetField::Number: {
            bool ed = w.editing == k;
            Set_DrawBox(p, w, k, b, ed);
            if (!ed) p.Text(w.fValueC.get(), Set_FieldValue(f), b, pal().text);
            if (pt.slider) {
                float v = (float)(_wtoi(Set_FieldValue(f).c_str()) - f.min) / (float)std::max(1, f.max - f.min);
                D2D1_RECT_F tr = pt.track;
                p.Round(tr, 2, pal().surface1);
                D2D1_RECT_F fillR{tr.left, tr.top, tr.left + (tr.right - tr.left) * v, tr.bottom};
                p.GradFill(fillR, 2, 1);
                float cx = tr.left + (tr.right - tr.left) * v, cy = (tr.top + tr.bottom) / 2;
                float kg = std::max(Set_GlowOf(w, 6000 + k), w.dragSlider == k ? 1.0f : 0.0f);
                p.rt->FillEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), 8 + kg, 8 + kg), p.B(pal().text));
                p.rt->FillEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), 5, 5), p.B(pal().crust));
            }
            break;
        }
        case SetField::Text: {
            bool ed = w.editing == k;
            Set_DrawBox(p, w, k, b, ed);
            if (!ed) p.Text(w.fValue.get(), f.cur, {b.left + 12, b.top, b.right - 10, b.bottom}, pal().text);
            break;
        }
        case SetField::Hotkey: {
            bool rec = w.recording == k;
            Set_DrawBox(p, w, k, b, rec);
            if (rec) {
                p.Text(w.fValue.get(), Tr(L"Press keys..."), {b.left + 12, b.top, b.right - 10, b.bottom}, pal().text);
            } else if (f.cur.empty()) {
                p.Text(w.fValue.get(), Tr(L"Not set"), {b.left + 12, b.top, b.right - 10, b.bottom}, pal().overlay0);
            } else {
                float tw = std::min(Set_TextW(w.fValue.get(), f.cur) + 16, b.right - b.left - 40);
                D2D1_RECT_F cap{b.left + 6, (b.top + b.bottom) / 2 - 12, b.left + 6 + tw, (b.top + b.bottom) / 2 + 12};
                p.Round(cap, 6, pal().surface0);
                p.Text(w.fValueC.get(), f.cur, cap, pal().text);
                float cg = Set_GlowOf(w, 7000 + k);
                p.Glyph(w.fIconS.get(), 0xE711, pt.clear, cg > 0.5f ? pal().red : pal().overlay1);
            }
            break;
        }
        case SetField::Color: {
            bool ed = w.editing == k;
            Set_DrawBox(p, w, k, b, ed);
            UINT32 rgb = 0;
            bool okColor = Set_ParseHex(f.cur, rgb);
            if (okColor) {
                p.rt->FillEllipse(D2D1::Ellipse(D2D1::Point2F(b.left + 17, (b.top + b.bottom) / 2), 6, 6), p.B(rgb));
            }
            if (!ed) p.Text(w.fValue.get(), f.cur, {b.left + 30, b.top, b.right - 6, b.bottom}, okColor ? pal().text : pal().red);
            for (int i = 0; i < kSetChipCount; i++) {
                const D2D1_RECT_F& c = pt.chip[i];
                bool sel = okColor && (rgb & 0xFFFFFF) == kSetChips[i];
                p.Round(c, 6, kSetChips[i]);
                if (sel) {
                    D2D1_RECT_F ring{c.left - 3, c.top - 3, c.right + 3, c.bottom + 3};
                    p.GradRing(ring, 8, 1, 2);
                }
            }
            break;
        }
    }
}

static void Set_DrawGroup(SetPaint& p, SetWindow& w, const SetItem& it) {
    p.Text(w.fSection.get(), it.text, {w.contentX + 3, it.y, w.contentX + w.contentW, it.y + it.h},
           pal().subtext1);
}

static void Set_DrawLinkRow(SetPaint& p, SetWindow& w, const SetItem& it, int idx) {
    D2D1_RECT_F row = Set_RowRect(w, it);
    float hov = Set_GlowOf(w, 3000 + idx);
    Set_DrawCard(p, row, hov);
    p.Text(w.fLabel.get(), it.text, {row.left + kSetRowPad, row.top + 9, row.right - 50, row.top + 30}, pal().text);
    p.Text(w.fHint.get(), it.sub, {row.left + kSetRowPad, row.top + 30, row.right - 50, row.bottom - 4}, pal().subtext0);
    p.Glyph(w.fIcon.get(), 0xE8A7, {row.right - 46, row.top, row.right - 12, row.bottom}, pal().subtext1);
}

static void Set_DrawSupport(SetPaint& p, SetWindow& w, const SetItem& it, int idx) {
    D2D1_RECT_F row = Set_RowRect(w, it);
    float card = std::max(Set_GlowOf(w, 3700 + idx), Set_GlowOf(w, 3500 + idx));
    Set_DrawCard(p, row, card);
    p.GradFill(row, 10, 0.13f * card);
    p.GradRing(row, 10, card, 1.5f);
    p.Text(w.fLabel.get(), it.text, {row.left + kSetRowPad, row.top + 14, row.right - 180, row.top + 36}, pal().text);
    p.Text(w.fHint.get(), it.sub, {row.left + kSetRowPad, row.top + 38, row.right - 180, row.bottom - 6}, pal().subtext0);
    D2D1_RECT_F b = Set_SupportButton(w, it);
    float hov = Set_GlowOf(w, 3500 + idx);
    p.GradFill(b, 8, 0.85f + 0.15f * hov);
    p.GradRing(b, 8, hov, 2);
    p.Text(w.fButton.get(), Tr(L"Open Ko-fi"), b, pal().onAccent);
}

static void Set_DrawAbout(SetPaint& p, SetWindow& w, const SetItem& it, int idx) {
    D2D1_RECT_F row = Set_RowRect(w, it);
    float hov = Set_GlowOf(w, 3800 + idx);
    Set_DrawCard(p, row, hov);
    D2D1_RECT_F mark{row.left + 20, row.top + 20, row.left + 92, row.top + 92};
    p.GradFill(mark, 18, 1);
    p.Glyph(w.fIconL.get(), 0xE80A, mark, pal().onAccent);
    float x = mark.right + 20;
    p.Text(w.fBig.get(), Tr(L"Lumiflow"), {x, row.top + 18, row.right - 16, row.top + 46}, pal().text);
#ifdef WH_MOD_VERSION
    std::wstring ver = std::wstring(Tr(L"Version ")) + WH_MOD_VERSION + Tr(L"  \u00B7  MIT license  \u00B7  A Windhawk mod");
#else
    std::wstring ver = L"MIT license  \u00B7  A Windhawk mod";
#endif
    p.Text(w.fHint.get(), ver, {x, row.top + 48, row.right - 16, row.top + 68}, pal().subtext0);
    p.Text(w.fHint.get(), Tr(L"Apps, folders and search in one fullscreen grid, in place of Start."),
           {x, row.top + 72, row.right - 16, row.top + 110}, pal().subtext1);
    p.Text(w.fHint.get(), Tr(L"Maintained by the upstream authors. Based on MIT-licensed open-source projects."),
           {x, row.top + 112, row.right - 16, row.bottom - 8}, pal().overlay2);
}

static void Set_DrawBanner(SetPaint& p, SetWindow& w, const SetItem& it) {
    D2D1_RECT_F r = Set_RowRect(w, it);
    p.Round(r, 10, pal().peach, 0.14f);
    p.Text(w.fHint.get(), it.text, {r.left + 16, r.top + 14, r.right - 16, r.bottom - 8}, pal().peach);
}

// Panel-left mark, as in T3's sidebar button: a window with its left pane shaded while open.
static void Set_DrawPanelIcon(SetPaint& p, const D2D1_RECT_F& r, float open, UINT32 c) {
    float cx = (r.left + r.right) / 2, cy = (r.top + r.bottom) / 2;
    D2D1_RECT_F box{cx - 9, cy - 7.5f, cx + 9, cy + 7.5f};
    p.Round({box.left + 1, box.top + 1, box.left + 6.5f, box.bottom - 1}, 1.5f, c, 0.25f + 0.35f * open);
    p.rt->DrawRoundedRectangle(D2D1::RoundedRect(box, 3, 3), p.B(c), 1.5f);
    p.rt->DrawLine({box.left + 6.5f, box.top + 0.75f}, {box.left + 6.5f, box.bottom - 0.75f}, p.B(c), 1.5f);
}

static void Set_DrawMark(SetPaint& p, IDWriteTextFormat* glyph, const D2D1_RECT_F& r, float a,
                         ID2D1Bitmap* appIcon = nullptr);
static void Set_DrawRail(SetPaint& p, SetWindow& w) {
    D2D1_RECT_F d = Set_RailDock(w);
    const float open = Set_RailOpen(w), textA = std::clamp((open - 0.45f) / 0.55f, 0.0f, 1.0f);
    p.Panel(d, 16, w.glass ? .70f : 1.f);
    const float edge = std::max(Set_GlowOf(w, 41), w.dragRail ? 1.0f : 0.0f);
    p.Glass(d, 16, edge);

    D2D1_RECT_F tb = Set_RailToggleRect(w);
    float th = Set_GlowOf(w, 42);
    p.Round(tb, 9, pal().text, .06f + .10f * th);
    p.Edge(tb, 9, pal().text, .17f);
    p.Glass(tb, 9, th);
    if (w.kbd && w.focus == -20) p.GradRing(tb, 9, 1, 2);
    Set_DrawPanelIcon(p, tb, open, th > 0.5f ? pal().text : pal().subtext1);

    p.rt->PushAxisAlignedClip(D2D1::RectF(d.left, d.top, d.right - 6, d.bottom), D2D1_ANTIALIAS_MODE_ALIASED);
    for (int i = 0; i < kSetPageCount; i++) {
        D2D1_RECT_F r = Set_RailRect(w, i);
        if (i == kSetAboutPage) {
            float ly = std::round(r.top - (kSetAboutGap + kSetRailPitch - kSetRailItem) / 2);
            p.Fill({r.left + 6, ly, r.right - 6, ly + 1}, pal().surface1, 0.7f);
        }
        float hov = Set_GlowOf(w, 100 + i);
        bool act = i == w.page;
        p.Round(r, 9, pal().text, (act ? .16f : .035f) + .10f * hov);
        p.Edge(r, 9, pal().text, act ? .43f : .10f);
        p.Glass(r, 9, hov);
        if (w.kbd && w.focus == -10 - i) p.GradRing(r, 8, 1, 2);
        UINT32 c = act ? pal().text : pal().subtext1;
        // The glyph sits in a fixed 44-wide slot, so it stays centered in the collapsed rail.
        p.Glyph(w.fIcon.get(), kSetPages[i].glyph, {r.left, r.top, r.left + 44, r.bottom}, act ? pal().text : pal().subtext0);
        p.Text(w.fLabel.get(), Tr(kSetPages[i].rail ? kSetPages[i].rail : kSetPages[i].name), {r.left + 50, r.top + 8, r.left + w.railW - 24, r.bottom}, c, textA);
    }
#ifdef WH_MOD_VERSION
    p.Text(w.fSmall.get(), std::wstring(Tr(L"Version ")) + WH_MOD_VERSION,
           {d.left + 18, d.bottom - 34, d.left + w.railW - 12, d.bottom - 12}, pal().overlay1, textA);
#endif
    p.rt->PopAxisAlignedClip();
}

static void Set_DrawFooter(SetPaint& p, SetWindow& w) {
    D2D1_RECT_F d = Set_FooterDock(w);
    const float rad = kSetFootH / 2, brad = kSetFootBtnH / 2;
    p.Panel(d, rad, w.render ? 1.f : .86f);
    UINT32 sc;
    std::wstring status = Set_FooterStatus(w, &sc);
    float sw = Set_FooterStatusW(w);
    if (sw > 0) p.Text(w.fStatusLine.get(), status, {d.left + 18, d.top, d.left + 18 + sw, d.bottom}, sc);
    const PCWSTR names[3] = {Tr(L"Reset to suggested"), Tr(L"Cancel"), Tr(L"Save")};
    for (int i = 0; i < 3; i++) {
        D2D1_RECT_F b = Set_FooterButton(w, i);
        float hov = Set_GlowOf(w, 10 + i);
        p.Round(b, brad, pal().text, (i == 2 ? .15f : .045f) + .11f * hov);
        p.Edge(b, brad, pal().text, i == 2 ? .38f : .17f);
        p.Glass(b, brad, hov);
        p.Text(w.fButton.get(), names[i], b, pal().text, w.saving ? 0.5f : 1.0f);
        if (w.kbd && w.focus == -1 - i) p.GradRing(b, brad, 1, 2);
    }
}


// Neutral scrollbar thumb, brighter while
// hovered or dragged. Never a flat grey thumb.
static void Set_ThumbFill(SetPaint& p, const D2D1_RECT_F& r, float a) {
    if (r.bottom <= r.top) return;
    float rad = (r.right - r.left) / 2;
    SetCom<ID2D1LinearGradientBrush> g;
    D2D1_GRADIENT_STOP stops[2] = {{0, D2DColor(pal().text, a * p.fade)}, {1, D2DColor(pal().text, a * p.fade)}};
    SetCom<ID2D1GradientStopCollection> coll;
    if (SUCCEEDED(p.rt->CreateGradientStopCollection(stops, 2, coll.put())) &&
        SUCCEEDED(p.rt->CreateLinearGradientBrush(
            D2D1::LinearGradientBrushProperties(D2D1::Point2F(r.left, r.top), D2D1::Point2F(r.left, r.bottom)),
            coll.get(), g.put())))
        p.rt->FillRoundedRectangle(D2D1::RoundedRect(r, rad, rad), g.get());
}

static int Set_DropVisible(const SetWindow& w) {
    return w.openSel < 0 ? 0 : std::min((int)w.fields[w.openSel].options.size(), kSetDropMax);
}

// Keeps option i inside the visible part of a scrolling dropdown.
static void Set_DropShow(SetWindow& w, int i) {
    int vis = Set_DropVisible(w), n = w.openSel < 0 ? 0 : (int)w.fields[w.openSel].options.size();
    if (i < 0 || vis <= 0) return;
    if (i < w.dropTop) w.dropTop = i;
    if (i >= w.dropTop + vis) w.dropTop = i - vis + 1;
    w.dropTop = std::clamp(w.dropTop, 0, std::max(0, n - vis));
}

static void Set_DrawPopup(SetPaint& p, SetWindow& w) {
    if (w.openSel < 0) return;
    const SetField& f = w.fields[w.openSel];
    const D2D1_RECT_F& r = w.dropRect;
    const int n = (int)f.options.size(), vis = Set_DropVisible(w);
    const bool scrolls = n > vis;
    p.Panel(r, 10);
    const float right = r.right - (scrolls ? 12 : 4);
    for (int v = 0; v < vis; v++) {
        int i = w.dropTop + v;
        if (i >= n) break;
        D2D1_RECT_F it{r.left + 4, r.top + 4 + v * kSetDropItem, right, r.top + 4 + (v + 1) * kSetDropItem};
        bool sel = f.cur == f.options[i].first;
        if (i == w.dropHover) {
            p.Round(it, 7, pal().text, .13f);
            p.Glass(it, 7, 1.f);
        }
        p.Text(w.fValue.get(), Tr(f.options[i].second.c_str()),
               {it.left + 10, it.top, it.right - 28, it.bottom}, pal().text);
        if (sel) p.Glyph(w.fIconS.get(), 0xE73E, {it.right - 28, it.top, it.right - 6, it.bottom}, pal().text);
    }
    if (scrolls) {
        float track = r.bottom - r.top - 12, th = std::max(24.0f, track * vis / n);
        float top = r.top + 6 + (track - th) * w.dropTop / std::max(1, n - vis);
        Set_ThumbFill(p, {r.right - 9, top, r.right - 5, top + th}, 0.85f);
    }
}

// Title bar buttons, in title bar DIPs: 0 minimize, 1 maximize or restore, 2 close.
static D2D1_RECT_F Set_CapButton(const SetWindow& w, int i) {
    float right = w.cw - (2 - i) * kSetCapBtnW;
    return {right - kSetCapBtnW, 0, right, kSetCapH};
}

// y is in content DIPs, so the title bar is y < 0.
static int Set_CapButtonAt(const SetWindow& w, float x, float y) {
    if (w.nativeTitle) return -1;
    for (int i = 0; i < 3; i++)
        if (Set_In(Set_CapButton(w, i), x, y + kSetCapH)) return i;
    return -1;
}

// Icon and title are centered on the whole width, laid out from the current size on every paint,
// so a move or snap can never leave them off center.
static D2D1_RECT_F Set_CapTitleRect(const SetWindow& w) {
    float tw = std::min(Set_TextW(w.fCap.get(), Tr(L"Lumiflow")) + 2, std::max(0.0f, w.cw - 4 * kSetCapBtnW - 40));
    float x = std::round((w.cw - (16 + 8 + tw)) / 2);
    return {x, 0, x + 16 + 8 + tw, kSetCapH};
}

static void Set_DrawMark(SetPaint& p, IDWriteTextFormat* glyph, const D2D1_RECT_F& r, float a,
                         ID2D1Bitmap* appIcon) {
    const float s = r.right - r.left;
    const float rad = s * 0.24f;
    const float alpha = a * p.fade;
    (void)glyph;
    if (alpha <= 0.001f || s <= 0) return;
    if (!appIcon) {
        // Per-window ownership, released together with that render target.
        if (!p.w.mark) {
            auto icon = CreateAppIconBitmap(p.rt);
            if (icon) *p.w.mark.put() = icon.Detach();
        }
        appIcon = p.w.mark.get();
    }
    if (appIcon) {
        p.rt->DrawBitmap(appIcon, r, alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
        return;
    }
    // Fallback only when WIC cannot decode the embedded mark.
    D2D1_GRADIENT_STOP bodyStops[3] = {
        {0, D2DColor(0xE7FAFF, alpha)},
        {0.52f, D2DColor(0xA6D8FF, alpha)},
        {1, D2DColor(0x6DB4ED, alpha)}};
    SetCom<ID2D1GradientStopCollection> bodyColors;
    SetCom<ID2D1LinearGradientBrush> body;
    if (SUCCEEDED(p.rt->CreateGradientStopCollection(bodyStops, 3, bodyColors.put())) &&
        SUCCEEDED(p.rt->CreateLinearGradientBrush(
            D2D1::LinearGradientBrushProperties(D2D1::Point2F(r.left, r.top),
                                                D2D1::Point2F(r.right, r.bottom)),
            bodyColors.get(), body.put())))
        p.rt->FillRoundedRectangle(D2D1::RoundedRect(r, rad, rad), body.get());

    D2D1_GRADIENT_STOP sheenStops[4] = {
        {0, D2DColor(0xFFFFFF, alpha * .42f)},
        {0.30f, D2DColor(0xFFFFFF, alpha * .16f)},
        {0.64f, D2DColor(0xFFFFFF, alpha * .025f)},
        {1, D2DColor(0xFFFFFF, 0)}};
    SetCom<ID2D1GradientStopCollection> sheenColors;
    SetCom<ID2D1LinearGradientBrush> sheen;
    if (SUCCEEDED(p.rt->CreateGradientStopCollection(sheenStops, 4, sheenColors.put())) &&
        SUCCEEDED(p.rt->CreateLinearGradientBrush(
            D2D1::LinearGradientBrushProperties(D2D1::Point2F(r.left, r.top),
                                                D2D1::Point2F(r.right, r.bottom)),
            sheenColors.get(), sheen.put())))
        p.rt->FillRoundedRectangle(D2D1::RoundedRect(r, rad, rad), sheen.get());

    D2D1_GRADIENT_STOP specStops[3] = {
        {0, D2DColor(0xFFFFFF, alpha * .58f)},
        {0.55f, D2DColor(0xFFFFFF, alpha * .16f)},
        {1, D2DColor(0xFFFFFF, 0)}};
    SetCom<ID2D1GradientStopCollection> specColors;
    SetCom<ID2D1RadialGradientBrush> spec;
    const D2D1_POINT_2F specCenter{r.left + s * .38f, r.top + s * .30f};
    if (SUCCEEDED(p.rt->CreateGradientStopCollection(specStops, 3, specColors.put())) &&
        SUCCEEDED(p.rt->CreateRadialGradientBrush(
            D2D1::RadialGradientBrushProperties(specCenter, specCenter, s * .30f, s * .30f),
            specColors.get(), spec.put())))
        p.rt->FillEllipse(D2D1::Ellipse(specCenter, s * .30f, s * .30f), spec.get());

    p.Edge(r, rad, 0xE8FBFF, .86f * a, 1);
}

static void Set_DrawCaption(SetPaint& p, SetWindow& w) {
    if (!w.glass) p.Fill({0, 0, w.cw, kSetCapH}, pal().crust);  // Glass: the window tint already covers it.
    D2D1_RECT_F t = Set_CapTitleRect(w);
    float mid = kSetCapH / 2;
    Set_DrawMark(p, w.fCapGlyph.get(), {t.left, mid - 8, t.left + 16, mid + 8}, w.active ? 1.0f : 0.6f);
    p.Text(w.fCap.get(), Tr(L"Lumiflow"), {t.left + 24, 0, t.right, kSetCapH}, w.active ? pal().text : pal().overlay1);
    for (int i = 0; i < 3; i++) {
        D2D1_RECT_F b = Set_CapButton(w, i);
        float hov = Set_GlowOf(w, 20 + i);
        if (w.capDown == i + 1) hov = 1;
        if (i == 2) p.Fill(b, pal().red, 0.92f * hov);
        else p.Fill(b, pal().surface0, 0.85f * hov);
        UINT32 c = i == 2 && hov > 0.5f ? pal().crust : (w.active ? pal().text : pal().overlay1);
        WCHAR glyph = i == 0 ? 0xE921 : i == 1 ? (w.hwnd && IsZoomed(w.hwnd) ? 0xE923 : 0xE922) : 0xE8BB;
        p.Glyph(w.fCapGlyph.get(), glyph, b, c);
    }
}

// Draws the whole window into rt (DIPs). The target's DPI does the scaling.
// The title bar is drawn first; everything else is in content DIPs, shifted down by kSetCapH.
static void Set_PlaceEdit(SetWindow& w);

static bool Set_FieldOn(const SetWindow& w, PCWSTR key, bool def);
static bool Set_FieldCurOn(const SetWindow& w, PCWSTR key, bool def);

// The settings preview is drawn into a WIC target. Its premultiplied surface
// cannot carry a radial gradient whose stop alphas vary, so the same optical
// profile is composed from concentric ellipses with constant-alpha fills. The
// live power dock keeps its continuous gradient; this is preview-only.
static float Set_PowerPreviewAlpha(float t, float strength) {
    t = std::clamp(t, 0.f, 1.f);
    float profile = t <= .35f ? std::lerp(1.f, .88f, t / .35f)
                    : t <= .65f ? std::lerp(.88f, .48f, (t - .35f) / .30f)
                                : std::lerp(.48f, 0.f, (t - .65f) / .35f);
    return strength * profile;
}

static void Set_FillPowerPreviewLight(SetPaint& p, const D2D1_RECT_F& clipRect,
                                      float clipRadius, float cx, float cy,
                                      float rx, float ry, UINT32 rgb, float strength) {
    SetCom<ID2D1RoundedRectangleGeometry> clip;
    if (FAILED(g_setD2d->CreateRoundedRectangleGeometry(
            D2D1::RoundedRect(clipRect, clipRadius, clipRadius), clip.put())))
        return;
    D2D1_LAYER_PARAMETERS params{};
    params.contentBounds = D2D1::InfiniteRect();
    params.geometricMask = clip.get();
    params.maskAntialiasMode = D2D1_ANTIALIAS_MODE_PER_PRIMITIVE;
    params.maskTransform = D2D1::Matrix3x2F::Identity();
    params.opacity = 1.f;
    params.layerOptions = D2D1_LAYER_OPTIONS_NONE;
    p.rt->PushLayer(params, nullptr);
    const int rings = 256;
    for (int i = 0; i < rings; ++i) {
        const float t = 1.f - (float)i / rings;
        const float outer = 1.f - (float)(i ? i - 1 : 0) / rings;
        const float a = Set_PowerPreviewAlpha(t, strength);
        const float prev = Set_PowerPreviewAlpha(std::min(1.f, outer), strength);
        const float layer = prev < .999f ? (a - prev) / (1.f - prev) : 0.f;
        if (layer <= .001f) continue;
        p.rt->FillEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), rx * t, ry * t),
                          p.B(rgb, layer));
    }
    p.rt->PopLayer();
}

// The bottom edge is the same field seen through the lowest few DIPs of the
// glass; it is clipped by the capsule geometry and softened in horizontal
// strips so the preview cannot show a hard red bar.
static void Set_FillPowerPreviewEdge(SetPaint& p, const D2D1_RECT_F& clipRect,
                                     float clipRadius, UINT32 rgb, float strength) {
    SetCom<ID2D1RoundedRectangleGeometry> clip;
    if (FAILED(g_setD2d->CreateRoundedRectangleGeometry(
            D2D1::RoundedRect(clipRect, clipRadius, clipRadius), clip.put())))
        return;
    D2D1_LAYER_PARAMETERS params{};
    params.contentBounds = D2D1::InfiniteRect();
    params.geometricMask = clip.get();
    params.maskAntialiasMode = D2D1_ANTIALIAS_MODE_PER_PRIMITIVE;
    params.maskTransform = D2D1::Matrix3x2F::Identity();
    params.opacity = 1.f;
    params.layerOptions = D2D1_LAYER_OPTIONS_NONE;
    p.rt->PushLayer(params, nullptr);
    const int strips = 24;
    for (int i = 0; i < strips; ++i) {
        const float top = clipRect.bottom - (float)(i + 1) / strips * 6.f;
        const float bottom = clipRect.bottom - (float)i / strips * 6.f;
        p.rt->FillRectangle({clipRect.left, top, clipRect.right, bottom},
                            p.B(rgb, strength * (float)(i + 1) / strips));
    }
    p.rt->PopLayer();
}


static void Set_DrawPowerPreview(SetPaint& p, SetWindow& w, const SetItem& it) {
    const D2D1_RECT_F r = Set_RowRect(w, it);
    p.Fill(r, pal().surface0, .55f);
    p.Edge(r, 12, pal().text, .10f);

    const bool on = Set_FieldCurOn(w, L"powerWarning", true);
    UINT32 rgb = 0xA91223;
    if (auto* f = Set_FieldByKey(w, L"powerWarningColor")) Set_ParseHex(Set_FieldValue(*f), rgb);
    auto number = [&](PCWSTR key, int def) {
        auto* field = Set_FieldByKey(w, key);
        return field ? _wtoi(Set_FieldValue(*field).c_str()) : def;
    };
    const float strength = (float)std::clamp(number(L"powerWarningStrength", 78), 0, 100) / 100.f;
    const float spread = (float)std::clamp(number(L"powerWarningSpread", 100), 40, 200) / 100.f;
    const float edge = (float)std::clamp(number(L"powerWarningEdge", 65), 0, 100) / 100.f;
    const float bleed = (float)std::clamp(number(L"powerWarningBleed", 15), 0, 40) / 100.f;

    const float btn = 42, gap = 8, pad = 6;
    const float capW = 3 * btn + 2 * gap + 2 * pad, capH = btn + 2 * pad;
    const float capLeft = (r.left + r.right) / 2 - capW / 2;
    const float capTop = r.top + 34;
    const D2D1_RECT_F cap{capLeft, capTop, capLeft + capW, capTop + capH};
    p.Round(cap, capH / 2, pal().text, .045f);
    p.Glass(cap, capH / 2, .55f);
    p.Edge(cap, capH / 2, pal().text, .16f);

    const D2D1_RECT_F second{capLeft + pad + btn + gap, capTop + pad,
                             capLeft + pad + 2 * btn + gap, capTop + pad + btn};
    const float cx = (second.left + second.right) / 2;
    const float cy = (cap.top + cap.bottom) / 2;
    const float rx = 23 * spread, ry = 23 * spread;  // A circle, not a wide ellipse.
    const float inset = 1.5f;
    const D2D1_RECT_F inner{cap.left + inset, cap.top + inset, cap.right - inset,
                            cap.bottom - inset};
    const float innerRad = capH / 2 - inset;
    if (on) {
        Set_FillPowerPreviewLight(p, inner, innerRad, cx, cy, rx, ry, rgb, strength);
        Set_FillPowerPreviewEdge(p, inner, innerRad, rgb, strength * edge * .45f);
    }

    for (int i = 0; i < 3; ++i) {
        const int action = i == 0 ? 1 : i == 1 ? 4 : 5;
        const D2D1_RECT_F b{capLeft + pad + i * (btn + gap), capTop + pad,
                            capLeft + pad + i * (btn + gap) + btn, capTop + pad + btn};
        p.Glyph(w.fIcon.get(), kPower[action].glyph, b, pal().text);
    }

    // The text pill above the middle button, with the same weak, local bleed.
    const std::wstring label = Tr(L"Restart");
    const D2D1_RECT_F pill{cx - 38, cap.top - 30, cx + 38, cap.top - 6};
    p.Round(pill, 12, pal().text, .045f);
    p.Glass(pill, 12, .35f);
    p.Edge(pill, 12, pal().text, .10f);
    if (on && bleed > 0) {
        const D2D1_RECT_F pin{pill.left + 1.5f, pill.top + 1.5f,
                              pill.right - 1.5f, pill.bottom - 1.5f};
        Set_FillPowerPreviewLight(p, pin, 10.5f, cx, pill.bottom - 2,
                                  30 * spread, 10 * spread, rgb, strength * bleed);
    }
    p.Text(w.fHint.get(), label, pill, pal().text);

    const std::wstring caption = Tr(L"The preview follows these controls; nothing is applied until Save.");
    D2D1_RECT_F text{r.left + 14, r.bottom - 26, r.right - 14, r.bottom - 6};
    p.Text(w.fHint.get(), caption, text, pal().subtext0);
}

static void Set_DrawPreview(SetPaint& p, SetWindow& w, const SetItem& it) {
    if (it.previewKind == 1) {
        Set_DrawPowerPreview(p, w, it);
        return;
    }
    D2D1_RECT_F r = Set_RowRect(w, it);
    Set_PreparePreview(w, p.rt, r.right - r.left);
    p.Fill(r, pal().surface0, .55f);
    if (w.preview) {
        // A bitmap brush scaled into the row keeps the 12 DIP rounding without
        // a layer: layers here are subject to the page transform.
        ID2D1BitmapBrush* brush = nullptr;
        D2D1_BITMAP_BRUSH_PROPERTIES props = {
            D2D1_EXTEND_MODE_CLAMP, D2D1_EXTEND_MODE_CLAMP,
            D2D1_BITMAP_INTERPOLATION_MODE_LINEAR};
        if (SUCCEEDED(p.rt->CreateBitmapBrush(w.preview.p, props, &brush))) {
            const D2D1_SIZE_F size = w.preview.p->GetSize();
            brush->SetTransform(
                D2D1::Matrix3x2F::Scale((r.right - r.left) / size.width,
                                        (r.bottom - r.top) / size.height) *
                D2D1::Matrix3x2F::Translation(r.left, r.top));
            p.rt->FillRoundedRectangle(D2D1::RoundedRect(r, 12, 12), brush);
            brush->Release();
        }
    }
    p.Edge(r, 12, pal().text, .10f);
    // The panel always bakes the adjustable material; native materials are DWM's.
    const std::wstring caption =
        Tr(L"Preview shows the adjustable material. Windows materials are drawn by the system.");
    D2D1_RECT_F text{r.left + 14, r.bottom - 30, r.right - 14, r.bottom - 10};
    const float captionW = std::min(text.right - text.left, Set_TextW(w.fHint.get(), caption));
    p.Round({text.left - 6, text.top - 4, text.left + captionW + 6, text.bottom + 4}, 9,
            0x000000, .45f);
    p.Text(w.fHint.get(), caption, text, pal().text, 1.0f);
}

static D2D1_RECT_F Set_ActionRect(const SetWindow& w, const SetItem& it, int index) {
    const float gap = 8, buttonW = (w.contentW - 3 * gap) / 4;
    float left = w.contentX + index * (buttonW + gap);
    return {left, it.y + 4, left + buttonW, it.y + 40};
}

static void Set_DrawActions(SetPaint& p, SetWindow& w, const SetItem& it) {
    const PCWSTR labels[4] = {Tr(L"Save preset"), Tr(L"Delete preset"),
                              Tr(L"Export…"), Tr(L"Import…")};
    for (int i = 0; i < 4; i++) {
        D2D1_RECT_F b = Set_ActionRect(w, it, i);
        const float hov = Set_GlowOf(w, 4100 + i);
        p.Round(b, 8, pal().text, .045f + .11f * hov);
        p.Edge(b, 8, pal().text, .17f);
        p.Glass(b, 8, hov);
        p.Text(w.fButton.get(), labels[i], b, pal().text);
    }
}

static void Set_ReleaseRt(SetWindow& w);
static bool Set_DrawDesktop(SetWindow& w, ID2D1RenderTarget* rt, float total, bool live);
static bool Set_EnsureRoundClip(SetWindow& w, ID2D1RenderTarget* rt, float total);

static void Set_Draw(SetWindow& w, ID2D1RenderTarget* rt) {
    if (Set_FitBoxes(w)) {
        Set_BuildPage(w);
        Set_PlaceEdit(w);
    }
    SetPaint p(w, rt);
    const auto base = D2D1::Matrix3x2F::Translation(0, kSetCapH);
    const bool live = !w.render && rt == w.rt.get();
    const float total = w.ch + kSetCapH;
    // The custom title bar is composed by us: no acrylic, no DWM frame, a
    // premultiplied DIB and a 20 DIP rounded layer mask. Native title mode stays
    // the system's opaque window because DWM owns its caption and corners.
    bool rounded = !w.nativeTitle && (!live || w.alphaComposition);
    rt->BeginDraw();
    rt->SetTransform(D2D1::Matrix3x2F::Identity());
    if (!w.nativeTitle && rounded) {
        rt->Clear(D2D1::ColorF(0, 0));
        if (!Set_EnsureRoundClip(w, rt, total)) {
            rounded = false;
        } else {
            D2D1_LAYER_PARAMETERS params{};
            params.contentBounds = D2D1::InfiniteRect();
            params.geometricMask = w.roundGeo.get();
            params.maskAntialiasMode = D2D1_ANTIALIAS_MODE_PER_PRIMITIVE;
            params.maskTransform = D2D1::Matrix3x2F::Identity();
            params.opacity = 1;
            params.layerOptions = D2D1_LAYER_OPTIONS_NONE;
            rt->PushLayer(params, w.roundLayer.get());
            if (w.glass) {
                const bool hasDesktop = Set_DrawDesktop(w, rt, total, live);
                p.Fill({0, 0, w.cw, total}, pal().crust, hasDesktop ? SetGlassTint() : 1.f);
            } else {
                p.Fill({0, 0, w.cw, total}, pal().crust, 1.f);
            }
        }
    }
    if (!rounded) {
        // Native title bar, or a system without the alpha attribute: an opaque
        // page. The custom 20 DIP mask is not drawn when the window itself is
        // opaque, otherwise its cut corners would show the solid window beneath.
        rt->Clear(D2DColor(pal().crust));
    }
    if (!w.nativeTitle) Set_DrawCaption(p, w);
    rt->SetTransform(base);
    float viewH = Set_ViewH(w);

    rt->PushAxisAlignedClip(D2D1::RectF(Set_RailSpace(w) + 4, 0, w.cw, viewH), D2D1_ANTIALIAS_MODE_ALIASED);
    auto place = [&](int index) {
        float t = w.pageT >= 1 ? 1.0f : Set_Ease(w.pageT * 1.7f - std::min(index, 9) * 0.07f);
        p.fade = t;
        const float rise = (1 - t) * 10;
        p.pointer.y = w.mouseY + w.scrollY - rise;
        rt->SetTransform(D2D1::Matrix3x2F::Translation(0, -w.scrollY + rise) * base);
    };
    place(0);
    const D2D1_RECT_F badge{w.contentX, kSetHeadingTop, w.contentX + kSetHeadingSize, kSetHeadingTop + kSetHeadingSize};
    p.Panel(badge, 11);
    p.Round({badge.left + 3, badge.top + 3, badge.right - 3, badge.bottom - 3}, 8, pal().text, .07f);
    p.Glyph(w.fIcon.get(), kSetPages[w.page].glyph, badge, pal().text);
    p.Text(w.fTitle.get(), Tr(kSetPages[w.page].name),
            {badge.right + 14, badge.top + 7, w.contentX + w.contentW, badge.bottom}, pal().text);
    place(1);
    const PCWSTR intro = Tr(kSetPages[w.page].intro);
    p.Text(w.fIntro.get(), intro,
            {w.contentX, kSetIntroTop, w.contentX + w.contentW,
             kSetIntroTop + Set_TextH(w.fIntro.get(), intro, w.contentW) + 2}, pal().subtext0);
    for (size_t i = 0; i < w.items.size(); i++) {
        const SetItem& it = w.items[i];
        if (it.y + it.h < w.scrollY - 20 || it.y > w.scrollY + viewH + 20) continue;
        place((int)i + 2);
        switch (it.type) {
            case SetItem::Group: Set_DrawGroup(p, w, it); break;
            case SetItem::Row: Set_DrawRow(p, w, it); break;
            case SetItem::Banner: Set_DrawBanner(p, w, it); break;
            case SetItem::About: Set_DrawAbout(p, w, it, (int)i); break;
            case SetItem::Link: Set_DrawLinkRow(p, w, it, (int)i); break;
            case SetItem::Support: Set_DrawSupport(p, w, it, (int)i); break;
            case SetItem::Preview: Set_DrawPreview(p, w, it); break;
            case SetItem::Actions: Set_DrawActions(p, w, it); break;
        }
    }
    p.fade = 1;
    p.pointer = {w.mouseX, w.mouseY};
    rt->SetTransform(base);
    rt->PopAxisAlignedClip();

    D2D1_RECT_F th = Set_ScrollThumb(w);
    Set_ThumbFill(p, th, w.dragScroll || w.hover.kind == SetHit::Scroll ? 1.0f : 0.7f);

    Set_DrawFooter(p, w);
    Set_DrawRail(p, w);
    Set_DrawPopup(p, w);
    rt->SetTransform(D2D1::Matrix3x2F::Identity());
    if (rounded) rt->PopLayer();
    HRESULT hr = rt->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET) Set_ReleaseRt(w);
}

// ---- Hit testing --------------------------------------------------------------

static SetHit Set_Hit(SetWindow& w, float x, float y) {
    SetHit h;
    if (y < 0) {
        int b = Set_CapButtonAt(w, x, y);
        if (b >= 0) {
            h.kind = SetHit::Caption;
            h.a = b;
            h.id = 20 + b;
        }
        return h;
    }
    if (w.openSel >= 0) {
        if (Set_In(w.dropRect, x, y)) {
            int v = (int)((y - w.dropRect.top - 4) / kSetDropItem), i = w.dropTop + v;
            if (v >= 0 && v < Set_DropVisible(w) && i < (int)w.fields[w.openSel].options.size() &&
                y >= w.dropRect.top + 4 && x < w.dropRect.right - (Set_DropVisible(w) < (int)w.fields[w.openSel].options.size() ? 12 : 4)) {
                h.kind = SetHit::Drop;
                h.a = w.openSel;
                h.b = i;
            } else {
                h.kind = SetHit::Popup;
            }
        }
        return h;
    }
    D2D1_RECT_F dock = Set_RailDock(w);
    if (y >= dock.top + 14 && y < dock.bottom - 14 && fabsf(x - dock.right) <= kSetEdgeGrab) {
        h.kind = SetHit::RailEdge;
        h.id = 41;
        return h;
    }
    if (x < dock.right + kSetDockGap) {
        if (Set_In(Set_RailToggleRect(w), x, y)) {
            h.kind = SetHit::RailToggle;
            h.id = 42;
            return h;
        }
        for (int i = 0; i < kSetPageCount; i++) {
            if (Set_In(Set_RailRect(w, i), x, y)) {
                h.kind = SetHit::Rail;
                h.a = i;
                h.id = 100 + i;
                return h;
            }
        }
        return h;
    }
    D2D1_RECT_F foot = Set_FooterDock(w);
    if (Set_In(foot, x, y)) {
        for (int i = 0; i < 3; i++) {
            if (Set_In(Set_FooterButton(w, i), x, y)) {
                h.kind = SetHit::Footer;
                h.a = i;
                h.id = 10 + i;
                return h;
            }
        }
        return h;
    }
    if (y >= Set_ViewH(w)) return h;
    D2D1_RECT_F th = Set_ScrollThumb(w);
    if (th.bottom > th.top && x >= w.cw - 16 && y >= th.top && y <= th.bottom) {
        h.kind = SetHit::Scroll;
        return h;
    }
    float cy = y + w.scrollY;
    for (size_t i = 0; i < w.items.size(); i++) {
        const SetItem& it = w.items[i];
        D2D1_RECT_F row = Set_RowRect(w, it);
        if (!Set_In(row, x, cy)) continue;
        if (it.type == SetItem::Link) {
            h.kind = SetHit::Link;
            h.a = (int)i;
            h.id = 3000 + (int)i;
        } else if (it.type == SetItem::Support) {
            h.id = 3700 + (int)i;  // The card lights up; only its button clicks.
            if (Set_In(Set_SupportButton(w, it), x, cy)) {
                h.kind = SetHit::Link;
                h.a = (int)i;
                h.id = 3500 + (int)i;
            }
        } else if (it.type == SetItem::About) {
            h.id = 3800 + (int)i;
        } else if (it.type == SetItem::Actions) {
            for (int b = 0; b < 4; b++)
                if (Set_In(Set_ActionRect(w, it, b), x, cy)) {
                    h.kind = SetHit::Action;
                    h.a = b;
                    h.id = 4100 + b;
                }
        } else if (it.type == SetItem::Row) {
            const SetField& f = w.fields[it.field];
            SetParts pt = Set_Parts(f, row);
            h.a = it.field;
            h.id = 1000 + it.field;
            h.kind = f.kind == SetField::Check ? SetHit::Control : SetHit::Row;
            if (it.powerPosition >= 0) {
                for (int i = 0; i < 2; ++i) {
                    if (!Set_In(pt.powerMove[i], x, cy)) continue;
                    h.kind = Set_CanMovePower(it.powerPosition, i == 0 ? -1 : 1) ? SetHit::PowerMove : SetHit::Row;
                    h.b = i;
                    return h;
                }
            }
            if (f.kind == SetField::Hotkey && !f.cur.empty() && w.recording != it.field && Set_In(pt.clear, x, cy)) {
                h.kind = SetHit::Clear;
            } else if (f.kind == SetField::Color) {
                for (int c = 0; c < kSetChipCount; c++) {
                    if (Set_In(pt.chip[c], x, cy)) {
                        h.kind = SetHit::Chip;
                        h.b = c;
                    }
                }
                if (h.kind != SetHit::Chip && Set_In(pt.box, x, cy)) h.kind = SetHit::Control;
            } else if (pt.slider &&
                       Set_In({pt.track.left - 8, pt.track.top - 10, pt.track.right + 8, pt.track.bottom + 10}, x, cy)) {
                h.kind = SetHit::Slider;
            } else if (Set_In(pt.box, x, cy)) {
                h.kind = SetHit::Control;
            }
        }
        return h;
    }
    return h;
}

static bool Set_Clickable(const SetHit& h) {
    return h.kind == SetHit::Rail || h.kind == SetHit::Footer || h.kind == SetHit::Control || h.kind == SetHit::Slider ||
           h.kind == SetHit::Clear || h.kind == SetHit::Chip || h.kind == SetHit::Drop || h.kind == SetHit::Link ||
           h.kind == SetHit::Scroll || h.kind == SetHit::RailToggle || h.kind == SetHit::PowerMove || h.kind == SetHit::Action;
}

// Sidebar width from a drag: below the snap point it collapses to the icon rail.
static void Set_RailDragTo(SetWindow& w, float x) {
    float width = x - kSetDockGap;
    if (width < kSetRailSnap) {
        w.railCollapsed = true;
    } else {
        w.railCollapsed = false;
        w.railW = std::clamp(width, kSetRailMin, kSetRailMax);
        w.railCur = w.railW;
    }
    Set_BuildPage(w);
    Set_Invalidate(w);
}

static void Set_ToggleRail(SetWindow& w) {
    w.railCollapsed = !w.railCollapsed;
    if (!w.railCollapsed && w.railCur < kSetRailMini) w.railCur = kSetRailMini;
    Set_Invalidate(w);
}

// ---- Editing, recording, choosing -----------------------------------------------

static std::wstring Set_FormatHotkey(UINT mods, UINT vk) {
    std::wstring text = std::to_wstring(vk);
    if (!text.empty()) return text;
    // Fallback while the capture part is a stub.
    if (mods & MOD_CONTROL) text += L"Ctrl+";
    if (mods & MOD_ALT) text += L"Alt+";
    if (mods & MOD_SHIFT) text += L"Shift+";
    if (mods & MOD_WIN) text += L"Win+";
    WCHAR name[64] = {};
    LONG scan = (LONG)MapVirtualKeyW(vk, MAPVK_VK_TO_VSC) << 16;
    if (GetKeyNameTextW(scan, name, ARRAYSIZE(name)) > 0) {
        text += name;
    } else {
        text += std::to_wstring(vk);
    }
    return text;
}

static bool Set_IsModifier(UINT vk) {
    switch (vk) {
        case VK_SHIFT: case VK_LSHIFT: case VK_RSHIFT:
        case VK_CONTROL: case VK_LCONTROL: case VK_RCONTROL:
        case VK_MENU: case VK_LMENU: case VK_RMENU:
        case VK_LWIN: case VK_RWIN:
            return true;
    }
    return false;
}

static void Set_StopRecording(SetWindow& w) {
    if (w.recording < 0) return;
    w.recording = -1;
    if (w.status.rfind(Tr(L"Press the keys"), 0) == 0) Set_SetStatus(w, L"");
    
    Set_Invalidate(w);
}

static void Set_StartRecording(SetWindow& w, int k) {
    Set_StopRecording(w);
    w.recording = k;
    // Our own keys would fire instead of reaching the window while it listens.
    if (!w.render) {
        SetFocus(w.hwnd);
    }
    Set_SetStatus(w, Tr(L"Press the keys you want. Esc cancels, Backspace clears."));
}

static void Set_ClosePopup(SetWindow& w) {
    if (w.openSel < 0) return;
    w.openSel = -1;
    w.dropHover = -1;
    w.dropTop = 0;
    Set_Invalidate(w);
}

static void Set_OpenPopup(SetWindow& w, int k) {
    const SetItem* it = Set_ItemOfField(w, k);
    if (!it) return;
    const SetField& f = w.fields[k];
    SetParts pt = Set_Parts(f, Set_RowRect(w, *it));
    float boxTop = pt.box.top - w.scrollY, boxBottom = pt.box.bottom - w.scrollY;
    const int n = (int)f.options.size(), vis = std::min(n, kSetDropMax);
    float h = (float)vis * kSetDropItem + 8;
    float top = boxBottom + 4;
    if (top + h > Set_ViewH(w) && boxTop - 4 - h > 0) top = boxTop - 4 - h;  // No room below: open upward.
    // As wide as the longest option (text, check mark, scrollbar), growing leftward from the box.
    float textW = 0;
    for (const auto& o : f.options) textW = std::max(textW, Set_TextW(w.fValue.get(), Tr(o.second.c_str())));
    float width = std::max(pt.box.right - pt.box.left, std::ceil(textW) + 52 + (n > vis ? 8 : 0));
    float left = std::max(pt.box.right - width, Set_RailSpace(w) + 8);
    w.dropRect = {left, top, std::max(pt.box.right, left + width), top + h};
    w.openSel = k;
    w.dropHover = -1;
    w.dropTop = 0;
    for (int i = 0; i < n; i++)
        if (f.cur == f.options[i].first) w.dropHover = i;
    Set_DropShow(w, w.dropHover);
    Set_Invalidate(w);
}

static void Set_CommitEdit(SetWindow& w, bool apply) {
    if (w.editing < 0) return;
    int k = w.editing;
    w.editing = -1;  // Hiding the box ends its focus; this stops that from coming back here.
    SetField& f = w.fields[k];
    std::wstring text = w.edit ? Set_WindowText(w.edit) : L"";
    if (w.edit) ShowWindow(w.edit, SW_HIDE);
    if (apply) {
        switch (f.kind) {
            case SetField::Number:
                if (!text.empty()) f.cur = std::to_wstring(std::clamp(_wtoi(text.c_str()), f.min, f.max));
                break;
            case SetField::Color: {
                while (!text.empty() && text.back() == L' ') text.pop_back();
                while (!text.empty() && text.front() == L' ') text.erase(0, 1);
                if (!text.empty() && text[0] != L'#') text = L"#" + text;
                UINT32 rgb;
                if (Set_ParseHex(text, rgb)) {
                    f.cur = Set_HexFromRgb(rgb);
                    Set_SetStatus(w, L"");
                } else {
                    Set_SetStatus(w, Tr(L"Use a color like #89B4FA."), true);
                }
                break;
            }
            default:
                f.cur = text;
        }
    }
    if (!w.render && w.hwnd) SetFocus(w.hwnd);
    Set_Invalidate(w);
}

static void Set_PlaceEdit(SetWindow& w) {
    if (w.editing < 0 || !w.edit) return;
    const SetItem* it = Set_ItemOfField(w, w.editing);
    if (!it) return;
    SetParts pt = Set_Parts(w.fields[w.editing], Set_RowRect(w, *it));
    float left = pt.box.left + (w.fields[w.editing].kind == SetField::Color ? 30 : 10), right = pt.box.right - 8;
    float mid = (pt.box.top + pt.box.bottom) / 2 - w.scrollY + kSetCapH;
    float s = w.dpi / 96.0f;
    SetWindowPos(w.edit, HWND_TOP, (int)(left * s), (int)((mid - 10) * s), (int)((right - left) * s), (int)(20 * s),
                 SWP_NOACTIVATE | SWP_SHOWWINDOW);
}

static LRESULT CALLBACK Set_EditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
    SetWindow* w = g_setWin.get();
    if (w && w->edit == hwnd) {
        switch (msg) {
            case WM_KEYDOWN:
                if (wParam == VK_RETURN || wParam == VK_TAB) {
                    Set_CommitEdit(*w, true);
                    return 0;
                }
                if (wParam == VK_ESCAPE) {
                    Set_CommitEdit(*w, false);
                    return 0;
                }
                break;
            case WM_CHAR:
                if (wParam == VK_RETURN || wParam == VK_ESCAPE || wParam == VK_TAB) return 0;  // No beep.
                break;
            case WM_KILLFOCUS:
                if (w->editing >= 0) Set_CommitEdit(*w, true);
                break;
        }
    }
    return DefSubclassProc(hwnd, msg, wParam, lParam);
}

static void Set_EnsureVisible(SetWindow& w, float top, float bottom);

static void Set_BeginEdit(SetWindow& w, int k) {
    Set_CommitEdit(w, true);
    Set_StopRecording(w);
    Set_ClosePopup(w);
    SetField& f = w.fields[k];
    if (w.edit) DestroyWindow(w.edit);
    if (w.editFont) DeleteObject(w.editFont);
    DWORD style = WS_CHILD | ES_AUTOHSCROLL | (f.kind == SetField::Number ? ES_NUMBER | ES_CENTER : 0);
    // Over the extended frame GDI text would come out see-through; a layered child keeps it solid.
    w.edit = CreateWindowExW(w.glass ? WS_EX_LAYERED : 0, L"EDIT", L"", style, 0, 0, 10, 10, w.hwnd, nullptr,
                             GetModuleHandleW(nullptr), nullptr);
    if (!w.edit) return;
    if (w.glass) SetLayeredWindowAttributes(w.edit, 0, 255, LWA_ALPHA);
    w.editFont = CreateFontW(-MulDiv(13, w.dpi, 96), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, w.family.c_str());
    SendMessageW(w.edit, WM_SETFONT, (WPARAM)w.editFont, TRUE);
    SendMessageW(w.edit, EM_SETLIMITTEXT, f.kind == SetField::Number ? 6 : f.kind == SetField::Color ? 7 : 260, 0);
    SetWindowTextW(w.edit, f.kind == SetField::Number ? Set_FieldValue(f).c_str() : f.cur.c_str());
    SetWindowSubclass(w.edit, Set_EditProc, 1, 0);
    w.editing = k;
    if (const SetItem* item = Set_ItemOfField(w, k)) {
        Set_EnsureVisible(w, item->y, item->y + item->h);
        w.scrollY = w.scrollTarget;
    }
    Set_PlaceEdit(w);
    if (!w.render) SetFocus(w.edit);
    SendMessageW(w.edit, EM_SETSEL, 0, -1);
    Set_Invalidate(w);
}

// Fills the material controls from a built-in preset. "custom" leaves them alone.
static void Set_ApplyMaterialPreset(SetWindow& w, const std::wstring& id) {
    const launchpad_material::Preset* preset = nullptr;
    for (const auto& p : launchpad_material::Presets())
        if (id == p.id) {
            preset = &p;
            break;
        }
    launchpad_material::Preset saved;
    bool own = false;
    if (!preset)
        for (const auto& p : w.userPresets)
            if (id == p.name) {
                saved = p.values;
                preset = &saved;
                own = true;
                break;
            }
    if (!preset) return;
    auto put = [&](PCWSTR key, std::wstring value) {
        for (auto& f : w.fields)
            if (wcscmp(f.key, key) == 0) f.cur = std::move(value);
    };
    put(L"materialBlur", std::to_wstring(preset->blur));
    put(L"materialSaturation", std::to_wstring(preset->saturation));
    put(L"materialBrightness", std::to_wstring(preset->brightness));
    put(L"materialContrast", std::to_wstring(preset->contrast));
    put(L"backgroundOpacity", std::to_wstring(preset->tintOpacity));
    put(L"materialLuminosity", std::to_wstring(preset->luminosity));
    put(L"materialNoise", std::to_wstring(preset->noise));
    put(L"backgroundDim", std::to_wstring(preset->dim));
    // Deliberately not tintColor: a preset is a texture, not a colour scheme.
    // The tint row keeps whatever the user picked.
    if (own) put(L"materialPresetName", id);  // So Save/Delete act on the pick.
    w.previewKey.clear();  // Force a fresh bake on the next draw.
}

static void Set_Choose(SetWindow& w, int k, int option) {
    SetField& f = w.fields[k];
    if (option >= 0 && option < (int)f.options.size()) f.cur = f.options[option].first;
    if (option >= 0 && wcscmp(f.key, L"materialPreset") == 0)
        Set_ApplyMaterialPreset(w, f.cur);
    Set_ClosePopup(w);
}

static void Set_SliderTo(SetWindow& w, int k, float x) {
    const SetItem* it = Set_ItemOfField(w, k);
    if (!it) return;
    SetField& f = w.fields[k];
    SetParts pt = Set_Parts(f, Set_RowRect(w, *it));
    float t = std::clamp((x - pt.track.left) / (pt.track.right - pt.track.left), 0.0f, 1.0f);
    f.cur = std::to_wstring(f.min + (int)(t * (f.max - f.min) + 0.5f));
    Set_Invalidate(w);
}

static void Set_Save(std::shared_ptr<SetWindow> keep);

static void Set_ResetToSuggested(SetWindow& w) {
    Set_CommitEdit(w, false);
    Set_StopRecording(w);
    Set_ClosePopup(w);
    for (auto& f : w.fields) Set_PutValue(f, f.def);
    Set_BuildPage(w);
    Set_SetStatus(w, Tr(L"Suggested values are filled in. Press Save to keep them."));
}

static void Set_BuildAndClamp(SetWindow& w) {
    Set_BuildPage(w);
    Set_PlaceEdit(w);
    Set_Invalidate(w);
}

static bool Set_MovePower(SetWindow& w, int field, int step) {
    if (field < 0 || field >= (int)w.fields.size()) return false;
    const int action = Set_PowerAction(w.fields[field]);
    if (action < 0) return false;
    auto orderField = Set_PowerOrderField(w);
    if (orderField == w.fields.end()) return false;
    auto order = ParsePowerOrder(orderField->cur);
    const int position = (int)(std::find(order.begin(), order.end(), action) - order.begin());
    if (!Set_CanMovePower(position, step)) return true;
    std::swap(order[position], order[position + step]);
    orderField->cur = FormatPowerOrder(order);
    w.focus = field;
    Set_BuildPage(w);
    if (const auto* item = Set_ItemOfField(w, field)) {
        Set_EnsureVisible(w, item->y, item->y + item->h);
        w.scrollY = w.scrollTarget;
    }
    w.hover = Set_Hit(w, w.mouseX, w.mouseY);
    Set_SetStatus(w, L"");
    return true;
}

static void Set_GoPage(SetWindow& w, int page) {
    page = std::clamp(page, 0, kSetPageCount - 1);
    Set_CommitEdit(w, true);
    Set_StopRecording(w);
    Set_ClosePopup(w);
    if (page == w.page) return;
    w.page = page;
    w.scrollY = w.scrollTarget = 0;
    w.focus = -1;
    w.pageT = Set_Anim(w) ? 0.0f : 1.0f;
    Set_BuildAndClamp(w);
}

static void Set_OpenLink(PCWSTR url) {
#ifdef SCM_HARNESS
    Wh_Log(L"settings: would open %s", url);
#else
    ShellExecuteW(nullptr, L"open", url, nullptr, nullptr, SW_SHOWNORMAL);
#endif
}

// A click (or Space or Enter) on a field's control.
static void Set_ActivateField(SetWindow& w, int k, const SetHit* hit) {
    SetField& f = w.fields[k];
    if (w.recording >= 0 && w.recording != k) Set_StopRecording(w);
    switch (f.kind) {
        case SetField::Check:
            f.cur = f.cur == L"1" ? L"0" : L"1";
            Set_Invalidate(w);
            break;
        case SetField::Choice:
            if (w.openSel == k) {
                Set_ClosePopup(w);
            } else {
                Set_CommitEdit(w, true);
                Set_OpenPopup(w, k);
            }
            break;
        case SetField::Number:
        case SetField::Text:
            Set_BeginEdit(w, k);
            break;
        case SetField::Color:
            if (hit && hit->kind == SetHit::Chip) {
                Set_CommitEdit(w, false);
                f.cur = Set_HexFromRgb(kSetChips[hit->b]);
                Set_SetStatus(w, L"");
            } else {
                Set_BeginEdit(w, k);
            }
            break;
        case SetField::Hotkey:
            if (hit && hit->kind == SetHit::Clear) {
                f.cur.clear();
                Set_Invalidate(w);
            } else if (w.recording == k) {
                Set_StopRecording(w);
            } else {
                Set_CommitEdit(w, true);
                Set_StartRecording(w, k);
            }
            break;
    }
}

static void Set_FooterAction(SetWindow& w, int which) {
    if (which == 0) {
        Set_ResetToSuggested(w);
    } else if (which == 1) {
        if (!w.render) DestroyWindow(w.hwnd);  // w is gone after this.
    } else {
        Set_CommitEdit(w, true);
        if (g_setWin && g_setWin->hwnd == w.hwnd) Set_Save(g_setWin);
    }
}

// ---- Saving ---------------------------------------------------------------------

static void Set_ApplyFrame(SetWindow& w);
static void Set_Relayout(SetWindow& w);

static void Set_Save(std::shared_ptr<SetWindow> keep) {
    SetWindow& w = *keep;
    if (w.saving) return;
    Set_StopRecording(w);
    int changed = 0;
    for (const auto& field : w.fields) changed += Set_FieldValue(field) != field.value;
    if (!changed) {
        Set_SetStatus(w, Tr(L"Nothing changed"));
        return;
    }
#ifdef SEAMLESS_PREVIEW
    bool ok = true;
    for (auto& f : w.fields) {
        auto value = Set_FieldValue(f);
        if (value != f.value) {
            ok = Preview::PutSetting(f.key, value) && ok;
            if (wcscmp(f.key, L"interfaceLanguage") == 0) Preview::languageOverride.clear();
        }
    }
    if (!ok) { Set_SetStatus(w, Tr(L"Preview settings could not be saved."), true); return; }
    launchpad_i18n::Select(GetStringSetting(L"interfaceLanguage"));
    Set_MakeFormats(w);
    Set_LoadFields(w);
    Set_ApplyFrame(w);
    Set_Relayout(w);
    SetWindowTextW(w.hwnd, Tr(L"Lumiflow settings"));
    Set_SetStatus(w, Tr(L"Saved"));
    if (g_wnd) PostMessageW(g_wnd, WM_APP_RELOAD, 0, 0);
    return;
#else
    std::vector<launchpad_settings_store::Change> changes;
    for (const auto& field : w.fields) {
        auto value=Set_FieldValue(field);
        if (value!=field.value) {
            std::wstring host=WindhawkUtils::StringSetting::make(field.key).get();
            changes.push_back({field.key,value,host});
        }
    }
    if (!launchpad_settings_store::Save(StoragePath(L"ui-settings.ini"),changes)) {
        Set_SetStatus(w,Tr(L"Could not save private settings. The previous file was kept."),true);
        return;
    }
    launchpad_i18n::Select(GetStringSetting(L"interfaceLanguage"));
    Set_MakeFormats(w); Set_LoadFields(w); Set_ApplyFrame(w); Set_Relayout(w);
    SetWindowTextW(w.hwnd,Tr(L"Lumiflow settings"));
    Set_SetStatus(w,Tr(L"Saved"));
    if (g_wnd) PostMessageW(g_wnd,WM_APP_RELOAD,0,0);

#endif
}

// ---- Material preset actions --------------------------------------------------

static bool Set_WriteUtf8File(const std::wstring& path, const std::wstring& text) {
    HANDLE file = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) return false;
    std::string bytes(3, '\0');
    bytes[0] = '\xEF'; bytes[1] = '\xBB'; bytes[2] = '\xBF';  // BOM: Notepad friendly.
    if (!text.empty()) {
        int n = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), (int)text.size(), nullptr, 0,
                                    nullptr, nullptr);
        size_t at = bytes.size();
        bytes.resize(at + n);
        WideCharToMultiByte(CP_UTF8, 0, text.c_str(), (int)text.size(), bytes.data() + at, n,
                            nullptr, nullptr);
    }
    DWORD wrote = 0;
    bool ok = WriteFile(file, bytes.data(), (DWORD)bytes.size(), &wrote, nullptr) &&
              wrote == bytes.size();
    CloseHandle(file);
    return ok;
}

static bool Set_ReadUtf8File(const std::wstring& path, std::wstring& text) {
    // Material presets are a few kilobytes. Refuse an arbitrarily large file
    // instead of reading it into one string: a wrong path (or a huge JSON)
    // must not turn into a memory spike in Explorer.
    constexpr LONGLONG kMaxBytes = 8 * 1024 * 1024;
    HANDLE file = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) return false;
    LARGE_INTEGER size{};
    if (!GetFileSizeEx(file, &size) || size.QuadPart < 0 || size.QuadPart > kMaxBytes) {
        CloseHandle(file);
        return false;
    }
    std::string bytes;
    bytes.reserve((size_t)size.QuadPart);
    char buffer[4096];
    DWORD read = 0;
    while (ReadFile(file, buffer, sizeof(buffer), &read, nullptr) && read)
        bytes.append(buffer, read);
    CloseHandle(file);
    size_t start = bytes.size() >= 3 && bytes.compare(0, 3, "\xEF\xBB\xBF") == 0 ? 3 : 0;
    int n = MultiByteToWideChar(CP_UTF8, 0, bytes.data() + start, (int)(bytes.size() - start),
                                nullptr, 0);
    text.assign(n > 0 ? (size_t)n : 0, 0);
    if (n > 0)
        MultiByteToWideChar(CP_UTF8, 0, bytes.data() + start, (int)(bytes.size() - start),
                            text.data(), n);
    return true;
}

static bool Set_PickMaterialFile(SetWindow& w, bool save, std::wstring& path) {
    WCHAR file[1024] = L"";
    if (save && !w.userPresets.empty()) {
        std::wstring suggest = w.userPresets.front().name + L".ryjsm";
        wcscpy_s(file, suggest.c_str());
    }
    // Each embedded NUL is appended explicitly: std::wstring(c_str()) would
    // stop at the first one. c_str() supplies the final extra NUL.
    std::wstring filter = Tr(L"Lumiflow material (*.ryjsm)");
    auto filterPart = [&filter](const wchar_t* text) {
        filter.append(text);
        filter.push_back(L'\0');
    };
    filterPart(L"*.ryjsm");
    filterPart(L"JSON (*.json)");
    filterPart(L"*.json");
    OPENFILENAMEW ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = w.owner ? w.owner : w.hwnd;
    ofn.lpstrFilter = filter.c_str();
    ofn.lpstrFile = file;
    ofn.nMaxFile = ARRAYSIZE(file);
    ofn.lpstrDefExt = L"ryjsm";
    ofn.lpstrTitle = save ? Tr(L"Export material") : Tr(L"Import material");
    ofn.Flags = OFN_PATHMUSTEXIST | (save ? OFN_OVERWRITEPROMPT : OFN_FILEMUSTEXIST);
    bool picked = save ? GetSaveFileNameW(&ofn) : GetOpenFileNameW(&ofn);
    if (!picked) return false;
    path = file;
    return true;
}

static bool Set_StoreUserPresets(SetWindow& w) {
#ifndef SEAMLESS_PREVIEW
    std::wstring host = WindhawkUtils::StringSetting::make(L"materialUserPresets").get();
    return launchpad_settings_store::Save(
        StoragePath(L"ui-settings.ini"),
        {{L"materialUserPresets", launchpad_material::UserPresetsJson(w.userPresets), host}});
#else
    std::wstring json = launchpad_material::UserPresetsJson(w.userPresets);
    for (auto& c : json)
        if (c == L'\n' || c == L'\r') c = L' ';  // The preview INI cannot hold line breaks.
    return Preview::PutSetting(L"materialUserPresets", json);
#endif
}

static void Set_SaveMaterialPreset(SetWindow& w) {
    Set_CommitEdit(w, true);
    std::wstring name;
    if (auto* field = Set_FieldByKey(w, L"materialPresetName")) name = field->cur;
    name = launchpad_material::SanitizePresetName(name);
    auto& list = w.userPresets;
    if (name.empty()) {
        for (int i = 1; i <= 99; i++) {
            std::wstring candidate = std::wstring(Tr(L"My preset")) + L" " + std::to_wstring(i);
            bool taken = false;
            for (const auto& preset : list) taken = taken || preset.name == candidate;
            for (const auto& preset : launchpad_material::Presets())
                taken = taken || wcscmp(preset.id, candidate.c_str()) == 0;
            if (!taken) { name = candidate; break; }
        }
    }
    launchpad_material::Preset values = Set_PresetFromParams(Set_MaterialParams(w));
    for (const auto& preset : launchpad_material::Presets())
        if (wcscmp(preset.id, name.c_str()) == 0 || name == L"custom") {
            Set_SetStatus(w, Tr(L"That name belongs to a built-in preset. Pick another."), true);
            return;
        }
    bool replaced = false;
    for (auto& preset : list) {
        if (preset.name == name) {
            preset.values = values;
            replaced = true;
        }
    }
    if (!replaced) {
        if (list.size() >= launchpad_material::kMaxUserPresets) {
            Set_SetStatus(w, Tr(L"All preset slots are taken. Delete one first."), true);
            return;
        }
        list.push_back({name, values});
    }
    Wh_Log(L"save preset list now %zu, name=%ls", list.size(), name.c_str());
    if (!Set_StoreUserPresets(w)) {
        Set_SetStatus(w, Tr(L"Could not save presets. The previous file was kept."), true);
        return;
    }
    if (auto* field = Set_FieldByKey(w, L"materialPresetName")) Set_PutValue(*field, name);
    Set_RefreshPresetOptions(w);
    if (auto* field = Set_FieldByKey(w, L"materialPreset")) field->cur = name;
    w.previewKey.clear();
    Set_SetStatus(w, replaced ? Tr(L"Preset updated. Press Save to keep the material.")
                              : Tr(L"Preset saved. Press Save to keep the material."));
}

static void Set_DeleteMaterialPreset(SetWindow& w) {
    Set_CommitEdit(w, true);
    SetField* field = Set_FieldByKey(w, L"materialPreset");
    if (!field) return;
    std::wstring id = field->cur;
    auto& list = w.userPresets;
    auto at = std::find_if(list.begin(), list.end(),
                           [&](const launchpad_material::UserPreset& p) { return p.name == id; });
    if (at == list.end()) {
        Set_SetStatus(w, Tr(L"Only your own presets can be deleted."), true);
        return;
    }
    list.erase(at);
    if (!Set_StoreUserPresets(w)) {
        Set_SetStatus(w, Tr(L"Could not save presets. The previous file was kept."), true);
        return;
    }
    field->cur = L"custom";
    Set_RefreshPresetOptions(w);
    Set_SetStatus(w, Tr(L"Preset deleted."));
}

static std::wstring Set_MaterialDocument(SetWindow& w, const std::wstring& name) {
    return L"{\n  \"format\": \"ryjs-launchpad-material\",\n  \"version\": 1,\n  \"name\": \"" +
           launchpad_material::JsonEscape(name) + L"\",\n  \"params\": " +
           launchpad_material::ParamsJson(Set_MaterialParams(w), L"  ") + L"\n}\n";
}

// Returns nullptr on success, else the message to show.
static const wchar_t* Set_ApplyMaterialDocument(SetWindow& w, const std::wstring& text) {
    launchpad_material::Json doc;
    if (!launchpad_material::JsonReader(text).Read(doc) ||
        doc.type != launchpad_material::Json::Object)
        return L"That file is not a material file.";
    const launchpad_material::Json* format = doc.Get(L"format");
    if (!format || format->type != launchpad_material::Json::String ||
        format->text != L"ryjs-launchpad-material")
        return L"That file is not a material file.";
    const launchpad_material::Json* params = doc.Get(L"params");
    launchpad_material::Params parsed;
    int bad = 0;
    if (!params || !launchpad_material::ParseParams(*params, parsed, &bad))
        return L"Those material values could not be read.";
    Set_ApplyMaterialParams(w, parsed);
    if (const launchpad_material::Json* imported = doc.Get(L"name"))
        if (imported->type == launchpad_material::Json::String)
            if (auto* field = Set_FieldByKey(w, L"materialPresetName"))
                Set_PutValue(*field, launchpad_material::SanitizePresetName(imported->text));
    if (auto* field = Set_FieldByKey(w, L"materialPreset")) field->cur = L"custom";
    Set_RefreshPresetOptions(w);
    return nullptr;
}

static void Set_ExportMaterialPreset(SetWindow& w) {
    Set_CommitEdit(w, true);
    std::wstring name;
    if (auto* field = Set_FieldByKey(w, L"materialPreset")) name = field->cur;
    if (name.empty() || name == L"custom") name = L"material";
    std::wstring path;
    if (!Set_PickMaterialFile(w, true, path)) return;
    if (!Set_WriteUtf8File(path, Set_MaterialDocument(w, name))) {
        Set_SetStatus(w, Tr(L"Could not write that file."), true);
        return;
    }
    Set_SetStatus(w, Tr(L"Material exported."));
}

static void Set_ImportMaterialPreset(SetWindow& w) {
    Set_CommitEdit(w, true);
    std::wstring path;
    if (!Set_PickMaterialFile(w, false, path)) return;
    std::wstring text;
    if (!Set_ReadUtf8File(path, text)) {
        Set_SetStatus(w, Tr(L"Could not read that file."), true);
        return;
    }
    if (const wchar_t* error = Set_ApplyMaterialDocument(w, text))
        Set_SetStatus(w, Tr(error), true);
    else
        Set_SetStatus(w, Tr(L"Material imported. Press Save to keep it."));
}

static void Set_MaterialAction(SetWindow& w, int which) {
    if (which == 0) Set_SaveMaterialPreset(w);
    else if (which == 1) Set_DeleteMaterialPreset(w);
    else if (which == 2) Set_ExportMaterialPreset(w);
    else if (which == 3) Set_ImportMaterialPreset(w);
}

// ---- Animation tick ----------------------------------------------------------------

static bool Set_Tick(SetWindow& w) {
    bool moved = false, anim = Set_Anim(w);
    ULONGLONG tickNow = GetTickCount64();
    float dt = w.lastTick ? (float)std::clamp((tickNow - w.lastTick) / 1000.0, 0.0, 0.1) : 0.015f;
    w.lastTick = tickNow;
    auto step = [&](float& v, float target, float rate) {
        float d = target - v;
        if (!anim || fabsf(d) < 0.02f) {
            if (v != target) {
                v = target;
                moved = true;
            }
            return;
        }
        v += d * (1 - std::pow(1 - rate, dt / 0.015f));
        moved = true;
    };
    int ids[2];
    Set_HoverIds(w.hover, ids);
    for (int id : ids)
        if (id >= 0 && !w.glow.count(id)) w.glow[id] = 0;
    for (auto it = w.glow.begin(); it != w.glow.end();) {
        bool on = it->first == ids[0] || it->first == ids[1];
        step(it->second, on ? 1.0f : 0.0f, 0.3f);
        if (!on && it->second == 0) {
            it = w.glow.erase(it);
        } else {
            ++it;
        }
    }
    for (size_t k = 0; k < w.fields.size(); k++) {
        if (w.fields[k].kind != SetField::Check) continue;
        float target = w.fields[k].cur == L"1" ? 1.0f : 0.0f;
        if (!w.knob.count((int)k)) w.knob[(int)k] = target;
        step(w.knob[(int)k], target, 0.35f);
    }
    if (w.scrollY != w.scrollTarget) {
        float d = w.scrollTarget - w.scrollY;
        if (!anim || fabsf(d) < 0.5f) {
            w.scrollY = w.scrollTarget;
        } else {
            // 19 was a 52 ms time constant: correct for an idle fade, but it
            // reads as rubbery under the wheel. 32 settles in a few frames.
            w.scrollY += d * (1 - std::exp(-dt * 32));
        }
        moved = true;
        Set_PlaceEdit(w);
    }
    if (w.pageT < 1) {
        w.pageT = anim ? std::min(1.0f, w.pageT + dt / 0.22f) : 1.0f;
        moved = true;
    }
    // Sidebar open and collapse.
    if (!w.dragRail) {
        float target = w.railCollapsed ? kSetRailMini : w.railW, d = target - w.railCur;
        if (d != 0) {
            w.railCur = (!anim || fabsf(d) < 0.6f) ? target : w.railCur + d * 0.28f;
            Set_BuildPage(w);
            Set_PlaceEdit(w);
            moved = true;
        }
    }
    return moved;
}

// ---- Keyboard focus ----------------------------------------------------------------

// Focus order: the page's rows, then Reset, Cancel, Save (-1, -2, -3).
static std::vector<int> Set_FocusOrder(const SetWindow& w) {
    std::vector<int> order;
    for (auto& it : w.items)
        if (it.type == SetItem::Row) order.push_back(it.field);
    order.push_back(-1);
    order.push_back(-2);
    order.push_back(-3);
    return order;
}

static void Set_EnsureVisible(SetWindow& w, float top, float bottom) {
    float view = Set_ViewH(w);
    if (top - 16 < w.scrollTarget) {
        w.scrollTarget = top - 16;
    } else if (bottom + 16 > w.scrollTarget + view) {
        w.scrollTarget = bottom + 16 - view;
    }
    w.scrollTarget = std::clamp(w.scrollTarget, 0.0f, std::max(0.0f, w.contentH - view));
}

static void Set_MoveFocus(SetWindow& w, int dir) {
    Set_CommitEdit(w, true);
    Set_ClosePopup(w);
    std::vector<int> order = Set_FocusOrder(w);
    int index = -1;
    for (size_t i = 0; i < order.size(); i++)
        if (order[i] == w.focus) index = (int)i;
    index = index < 0 ? (dir > 0 ? 0 : (int)order.size() - 1) : (index + dir + (int)order.size()) % (int)order.size();
    w.focus = order[index];
    w.kbd = true;
    if (w.focus >= 0) {
        if (const SetItem* it = Set_ItemOfField(w, w.focus)) Set_EnsureVisible(w, it->y, it->y + it->h);
    }
    Set_Invalidate(w);
}

static void Set_Adjust(SetWindow& w, int dir) {
    if (w.focus < 0) return;
    SetField& f = w.fields[w.focus];
    if (f.kind == SetField::Number) {
        int span = f.max - f.min, step = span <= 100 ? 1 : span <= 3000 ? 10 : 100;
        f.cur = std::to_wstring(std::clamp(_wtoi(Set_FieldValue(f).c_str()) + dir * step, f.min, f.max));
    } else if (f.kind == SetField::Choice && !f.options.empty()) {
        int n = (int)f.options.size(), at = 0;
        for (int i = 0; i < n; i++)
            if (f.cur == f.options[i].first) at = i;
        f.cur = f.options[(at + dir + n) % n].first;
    } else if (f.kind == SetField::Check) {
        f.cur = dir > 0 ? L"1" : L"0";
    }
    Set_Invalidate(w);
}

static bool Set_OnKey(SetWindow& w, UINT vk) {
    if (w.saving) return true;
    if (w.recording >= 0) {
        if (Set_IsModifier(vk)) return true;
        UINT mods = 0;
        if (GetKeyState(VK_CONTROL) & 0x8000) mods |= MOD_CONTROL;
        if (GetKeyState(VK_MENU) & 0x8000) mods |= MOD_ALT;
        if (GetKeyState(VK_SHIFT) & 0x8000) mods |= MOD_SHIFT;
        if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000) mods |= MOD_WIN;
        SetField& f = w.fields[w.recording];
        if (vk == VK_ESCAPE && !mods) {
            // Cancel: keep what was there.
        } else if ((vk == VK_BACK || vk == VK_DELETE) && !mods) {
            f.cur.clear();
        } else {
            f.cur = Set_FormatHotkey(mods, vk);
        }
        Set_StopRecording(w);
        return true;
    }
    if (w.openSel >= 0) {
        const SetField& f = w.fields[w.openSel];
        int n = (int)f.options.size();
        if (vk == VK_ESCAPE) Set_ClosePopup(w);
        else if (vk == VK_UP) Set_DropShow(w, w.dropHover = w.dropHover < 0 ? n - 1 : (w.dropHover - 1 + n) % n);
        else if (vk == VK_DOWN) Set_DropShow(w, w.dropHover = w.dropHover < 0 ? 0 : (w.dropHover + 1) % n);
        else if (vk == VK_RETURN || vk == VK_SPACE) Set_Choose(w, w.openSel, w.dropHover);
        Set_Invalidate(w);
        return true;
    }
    bool ctrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0, shift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
    if (ctrl && (vk == VK_UP || vk == VK_DOWN) && Set_MovePower(w, w.focus, vk == VK_UP ? -1 : 1)) {
        w.kbd = true;
        return true;
    }
    if (ctrl && vk == 'B') {  // Same shortcut as T3's sidebar.
        Set_ToggleRail(w);
        return true;
    }
    float view = Set_ViewH(w), maxScroll = std::max(0.0f, w.contentH - view);
    switch (vk) {
        case VK_TAB:
            if (ctrl) Set_GoPage(w, (w.page + (shift ? kSetPageCount - 1 : 1)) % kSetPageCount);
            else Set_MoveFocus(w, shift ? -1 : 1);
            return true;
        case VK_SPACE:
        case VK_RETURN:
            w.kbd = true;
            if (w.focus >= 0) Set_ActivateField(w, w.focus, nullptr);
            else if (w.focus <= -1 && w.focus >= -3) Set_FooterAction(w, -1 - w.focus);
            return true;
        case VK_LEFT:
        case VK_RIGHT:
            w.kbd = true;
            Set_Adjust(w, vk == VK_RIGHT ? 1 : -1);
            return true;
        case VK_UP: w.scrollTarget = std::clamp(w.scrollTarget - 60, 0.0f, maxScroll); break;
        case VK_DOWN: w.scrollTarget = std::clamp(w.scrollTarget + 60, 0.0f, maxScroll); break;
        case VK_PRIOR: w.scrollTarget = std::clamp(w.scrollTarget - view * 0.9f, 0.0f, maxScroll); break;
        case VK_NEXT: w.scrollTarget = std::clamp(w.scrollTarget + view * 0.9f, 0.0f, maxScroll); break;
        case VK_HOME: w.scrollTarget = 0; break;
        case VK_END: w.scrollTarget = maxScroll; break;
        case VK_ESCAPE:
            if (!w.render) DestroyWindow(w.hwnd);
            return true;
        default:
            if (ctrl && vk >= '1' && vk <= '6') {
                Set_GoPage(w, (int)(vk - '1'));
                return true;
            }
            return false;
    }
    if (!Set_Anim(w)) w.scrollY = w.scrollTarget;
    Set_PlaceEdit(w);
    Set_Invalidate(w);
    return true;
}

// ---- Window ---------------------------------------------------------------------

static void Set_ReleaseRt(SetWindow& w) {
    // The preview is a bitmap on this render target. Its key does not include
    // the height, so a resize that keeps the width would otherwise reuse a
    // bitmap belonging to the destroyed target.
    w.preview.reset();
    w.previewKey.clear();
    w.rt.reset();
    w.dcRt.reset();
    w.deskBitmap.reset();
    w.mark.reset();
    w.roundLayer.reset();
    w.roundGeo.reset();
    w.roundTarget = nullptr;
    w.roundSize = {};
    if (w.memDC) {
        if (w.memOldBitmap) SelectObject(w.memDC, w.memOldBitmap);
        w.memOldBitmap = nullptr;
    }
    if (w.memBitmap) {
        DeleteObject(w.memBitmap);
        w.memBitmap = nullptr;
    }
    if (w.memDC) {
        DeleteDC(w.memDC);
        w.memDC = nullptr;
    }
    w.memBits = nullptr;
    w.memW = w.memH = 0;
}

static bool Set_EnsureRt(SetWindow& w) {
    if (w.rt) return true;
    if (w.render || !w.hwnd) return false;
    if (!Set_EnsureFactories()) return false;
    RECT rc;
    GetClientRect(w.hwnd, &rc);
    const int width = std::max(1, (int)rc.right), height = std::max(1, (int)rc.bottom);
    HDC screen = GetDC(nullptr);
    w.memDC = CreateCompatibleDC(screen);
    ReleaseDC(nullptr, screen);
    if (!w.memDC) return false;
    BITMAPINFO bi{};
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = width;
    bi.bmiHeader.biHeight = -height;  // top-down, the same orientation D2D writes.
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    w.memBitmap = CreateDIBSection(w.memDC, &bi, DIB_RGB_COLORS, &w.memBits, nullptr, 0);
    if (!w.memBitmap) {
        Set_ReleaseRt(w);
        return false;
    }
    w.memOldBitmap = (HBITMAP)SelectObject(w.memDC, w.memBitmap);
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_SOFTWARE,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
        (float)w.dpi, (float)w.dpi);
    if (FAILED(g_setD2d->CreateDCRenderTarget(&props, w.dcRt.put()))) {
        Set_ReleaseRt(w);
        return false;
    }
    RECT dc{0, 0, width, height};
    if (FAILED(w.dcRt->BindDC(w.memDC, &dc))) {
        Set_ReleaseRt(w);
        return false;
    }
    w.rt.p = w.dcRt.p;
    w.rt.p->AddRef();
    w.memW = width;
    w.memH = height;
    return true;
}

static void Set_BoxBlurH(const uint8_t* src, uint8_t* dst, int width, int height, int radius) {
    const int window = radius * 2 + 1;
    for (int y = 0; y < height; ++y) {
        const uint8_t* row = src + (size_t)y * width * 4;
        uint8_t* out = dst + (size_t)y * width * 4;
        int sum[3] = {};
        for (int k = -radius; k <= radius; ++k) {
            const int x = std::clamp(k, 0, width - 1);
            for (int c = 0; c < 3; ++c) sum[c] += row[x * 4 + c];
        }
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < 3; ++c) out[x * 4 + c] = (uint8_t)(sum[c] / window);
            out[x * 4 + 3] = 255;
            const int add = std::clamp(x + radius + 1, 0, width - 1);
            const int sub = std::clamp(x - radius, 0, width - 1);
            for (int c = 0; c < 3; ++c) sum[c] += row[add * 4 + c] - row[sub * 4 + c];
        }
    }
}

static void Set_BoxBlurV(const uint8_t* src, uint8_t* dst, int width, int height, int radius) {
    const int window = radius * 2 + 1;
    for (int x = 0; x < width; ++x) {
        int sum[3] = {};
        for (int k = -radius; k <= radius; ++k) {
            const int y = std::clamp(k, 0, height - 1);
            const uint8_t* p = src + ((size_t)y * width + x) * 4;
            for (int c = 0; c < 3; ++c) sum[c] += p[c];
        }
        for (int y = 0; y < height; ++y) {
            uint8_t* out = dst + ((size_t)y * width + x) * 4;
            for (int c = 0; c < 3; ++c) out[c] = (uint8_t)(sum[c] / window);
            out[3] = 255;
            const int add = std::clamp(y + radius + 1, 0, height - 1);
            const int sub = std::clamp(y - radius, 0, height - 1);
            const uint8_t* ap = src + ((size_t)add * width + x) * 4;
            const uint8_t* sp = src + ((size_t)sub * width + x) * 4;
            for (int c = 0; c < 3; ++c) sum[c] += ap[c] - sp[c];
        }
    }
}

// Capture the whole virtual desktop once, while the settings HWND is still
// hidden. A 1/4-size copy plus a box blur is enough: the page tint sits over it,
// and it avoids a per-frame desktop capture. Moving inside the virtual screen
// crops the same snapshot; the glass never has to be re-captured for a drag.
static void Set_CaptureDesktop(SetWindow& w) {
    if (w.render || !w.hwnd) return;
    const int vx = GetSystemMetrics(SM_XVIRTUALSCREEN);
    const int vy = GetSystemMetrics(SM_YVIRTUALSCREEN);
    const int vw = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    const int vh = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    if (vw <= 0 || vh <= 0) return;
    const int dw = std::max(1, vw / 4), dh = std::max(1, vh / 4);
    HDC screen = GetDC(nullptr);
    HDC mem = CreateCompatibleDC(screen);
    if (!screen || !mem) {
        if (mem) DeleteDC(mem);
        if (screen) ReleaseDC(nullptr, screen);
        return;
    }
    BITMAPINFO bi{};
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = dw;
    bi.bmiHeader.biHeight = -dh;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    void* bits = nullptr;
    HBITMAP dib = CreateDIBSection(mem, &bi, DIB_RGB_COLORS, &bits, nullptr, 0);
    HBITMAP old = dib ? (HBITMAP)SelectObject(mem, dib) : nullptr;
    const int oldMode = SetStretchBltMode(mem, HALFTONE);
    SetBrushOrgEx(mem, 0, 0, nullptr);
    const BOOL copied = dib && StretchBlt(mem, 0, 0, dw, dh, screen, vx, vy, vw, vh,
                                          SRCCOPY | CAPTUREBLT);
    SetStretchBltMode(mem, oldMode);
    if (copied) {
        w.deskBits.assign((uint8_t*)bits, (uint8_t*)bits + (size_t)dw * dh * 4);
        for (size_t i = 3; i < w.deskBits.size(); i += 4) w.deskBits[i] = 255;
        const int blur = std::max(2, (int)std::lround(18.f * w.dpi / 96.f * dw / vw));
        std::vector<uint8_t> tmp(w.deskBits.size());
        for (int pass = 0; pass < 3; ++pass) {
            Set_BoxBlurH(w.deskBits.data(), tmp.data(), dw, dh, blur);
            Set_BoxBlurV(tmp.data(), w.deskBits.data(), dw, dh, blur);
        }
        w.deskW = dw;
        w.deskH = dh;
        w.deskVirtual = RECT{vx, vy, vx + vw, vy + vh};
        w.deskBitmap.reset();
    }
    if (old) SelectObject(mem, old);
    if (dib) DeleteObject(dib);
    DeleteDC(mem);
    ReleaseDC(nullptr, screen);
}

static bool Set_EnsureDeskBitmap(SetWindow& w) {
    if (w.deskBitmap) return true;
    if (!w.rt || w.deskBits.empty() || !w.deskW || !w.deskH) return false;
    D2D1_BITMAP_PROPERTIES props = D2D1::BitmapProperties(
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE), 96, 96);
    return SUCCEEDED(w.rt->CreateBitmap(D2D1::SizeU((UINT)w.deskW, (UINT)w.deskH),
                                        w.deskBits.data(), (UINT)w.deskW * 4, props,
                                        w.deskBitmap.put()));
}

static bool Set_DrawDesktop(SetWindow& w, ID2D1RenderTarget* rt, float total, bool live) {
    if (!live || !w.alphaComposition || !w.glass || !w.hwnd || !Set_EnsureDeskBitmap(w)) return false;
    RECT wr{};
    GetWindowRect(w.hwnd, &wr);
    const float sx = (float)w.deskW / std::max(1, (int)(w.deskVirtual.right - w.deskVirtual.left));
    const float sy = (float)w.deskH / std::max(1, (int)(w.deskVirtual.bottom - w.deskVirtual.top));
    D2D1_RECT_F src{(wr.left - w.deskVirtual.left) * sx, (wr.top - w.deskVirtual.top) * sy,
                    (wr.right - w.deskVirtual.left) * sx, (wr.bottom - w.deskVirtual.top) * sy};
    src.left = std::clamp(src.left, 0.f, (float)w.deskW);
    src.top = std::clamp(src.top, 0.f, (float)w.deskH);
    src.right = std::clamp(src.right, 0.f, (float)w.deskW);
    src.bottom = std::clamp(src.bottom, 0.f, (float)w.deskH);
    if (src.right <= src.left || src.bottom <= src.top) return false;
    rt->DrawBitmap(w.deskBitmap.get(), D2D1::RectF(0, 0, w.cw, total), 1.f,
                   D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &src);
    return true;
}

static bool Set_EnsureRoundClip(SetWindow& w, ID2D1RenderTarget* rt, float total) {
    if (w.roundTarget == rt && w.roundGeo && w.roundLayer &&
        std::abs(w.roundSize.width - w.cw) < .01f && std::abs(w.roundSize.height - total) < .01f)
        return true;
    w.roundGeo.reset();
    w.roundLayer.reset();
    w.roundTarget = nullptr;
    const float radius = std::min(kSetCornerDip, std::min(w.cw, total) * .5f);
    if (FAILED(g_setD2d->CreateRoundedRectangleGeometry(
            D2D1::RoundedRect(D2D1::RectF(0, 0, w.cw, total), radius, radius),
            w.roundGeo.put())) ||
        FAILED(rt->CreateLayer(w.roundLayer.put())))
        return false;
    w.roundTarget = rt;
    w.roundSize = {w.cw, total};
    return true;
}

static void Set_Relayout(SetWindow& w) {
    RECT rc;
    GetClientRect(w.hwnd, &rc);
    w.cw = rc.right * 96.0f / w.dpi;
    w.ch = std::max(0.0f, rc.bottom * 96.0f / w.dpi - kSetCapH);
    if (!w.render && w.rt && ((int)rc.right != w.memW || (int)rc.bottom != w.memH))
        Set_ReleaseRt(w);
    Set_BuildAndClamp(w);
}

// A switch's stored value, found by the end of its key so a "SettingsWindow." prefix still matches.
static bool Set_FieldOn(const SetWindow& w, PCWSTR key, bool def) {
    const size_t n = wcslen(key);
    for (const auto& f : w.fields) {
        const size_t m = wcslen(f.key);
        if (m >= n && wcscmp(f.key + m - n, key) == 0 && (m == n || f.key[m - n - 1] == L'.')) return f.value == L"1";
    }
    return def;
}

// The live value, including a toggle the user has flipped but not saved yet.
// Preview surfaces use this; persistence/frame decisions keep Set_FieldOn.
static bool Set_FieldCurOn(const SetWindow& w, PCWSTR key, bool def) {
    const size_t n = wcslen(key);
    for (const auto& f : w.fields) {
        const size_t m = wcslen(f.key);
        if (m >= n && wcscmp(f.key + m - n, key) == 0 && (m == n || f.key[m - n - 1] == L'.')) return f.cur == L"1";
    }
    return def;
}

// The dark-mode flag, caption and caption text follow the settings palette, so
// a light settings window also gets a light title bar.
static void ApplyWindowColors(HWND hwnd, bool nativeCaption) {
    if (!hwnd) return;
    BOOL dark = pal().light ? FALSE : TRUE;
    DwmSetWindowAttribute(hwnd, 20 /* DWMWA_USE_IMMERSIVE_DARK_MODE */, &dark, sizeof(dark));
    // The custom title bar uses a per-pixel-alpha composition and its own 20
    // DIP rounded mask, so DWM must not add its 8 DIP corner on top. Native
    // title mode keeps the system corner. Set_ApplyFrame resets this to ROUND
    // if the Windows 11 26100 alpha attribute is unavailable.
    int corner = nativeCaption ? 2 : 1;  // DWMWCP_ROUND / DONOTROUND
    DwmSetWindowAttribute(hwnd, kDwmWindowCornerPreference, &corner, sizeof(corner));
    // No DWM hairline: on the light palette its default colour reads as a dark outline, and
    // the page paints its own frame ring just inside the edge.
    COLORREF noBorder = 0xFFFFFFFE;  // DWMWA_COLOR_NONE
    DwmSetWindowAttribute(hwnd, 34 /* DWMWA_BORDER_COLOR */, &noBorder, sizeof(noBorder));
    if (!nativeCaption) return;
    COLORREF caption = RefFromRgb(pal().crust), text = RefFromRgb(pal().text);
    DwmSetWindowAttribute(hwnd, 35 /* DWMWA_CAPTION_COLOR */, &caption, sizeof(caption));
    DwmSetWindowAttribute(hwnd, 36 /* DWMWA_TEXT_COLOR */, &text, sizeof(text));
}

// "Native Windows title bar" and "Blurred settings window", applied to the live window: on first
// open and again whenever a save or a reload changes them, so neither needs a reopen.
static void Set_ApplyFrame(SetWindow& w) {
    if (w.render || !w.hwnd) return;
    const HWND hwnd = w.hwnd;
    // The title bar follows the palette, not a fixed dark flag, so switching
    // the theme repaints the chrome even when the frame options are untouched.
    const bool native = Set_FieldOn(w, L"nativeSettingsTitleBar", false);
    const bool glass = Set_FieldOn(w, L"settingsGlass", true);
    ApplyWindowColors(hwnd, native);
    const bool first = !w.frameApplied;
    if (!first && native == w.nativeTitle && glass == w.glass) return;
    w.frameApplied = true;
    w.nativeTitle = native;
    w.glass = glass;
    kSetCapH = native ? 0.0f : 40.0f;
    struct SetAccentPolicy { int state, flags, abgr, animation; };
    struct SetCompositionData { DWORD attribute; void* data; SIZE_T size; };
    using SetComposition_t = BOOL(WINAPI*)(HWND, SetCompositionData*);
    auto setComposition =
        (SetComposition_t)(void*)GetProcAddress(GetModuleHandleW(L"user32.dll"), "SetWindowCompositionAttribute");

    // No acrylic and no extended frame: the settings window owns its whole
    // surface. The custom title bar asks DWM to honour the premultiplied alpha
    // in its redirection bitmap (Windows 11 26100), which is what makes a real
    // 20 DIP rounded clip possible without a rectangular acrylic layer.
    if (setComposition) {
        SetAccentPolicy policy{0, 0, 0, 0};
        SetCompositionData data{19 /* WCA_ACCENT_POLICY */, &policy, sizeof(policy)};
        setComposition(hwnd, &data);
    }
    DWM_BLURBEHIND bb{};
    bb.dwFlags = DWM_BB_ENABLE;
    bb.fEnable = FALSE;
    DwmEnableBlurBehindWindow(hwnd, &bb);
    MARGINS margins{0, 0, 0, 0};
    DwmExtendFrameIntoClientArea(hwnd, &margins);
    int backdrop = 1;  // DWMSBT_NONE
    DwmSetWindowAttribute(hwnd, kDwmSystemBackdropType, &backdrop, sizeof(backdrop));

    BOOL alpha = native ? FALSE : TRUE;
    HRESULT alphaHr = DwmSetWindowAttribute(hwnd, kDwmRedirectionBitmapAlpha, &alpha, sizeof(alpha));
    w.alphaComposition = !native && SUCCEEDED(alphaHr);
    if (!native && !w.alphaComposition) {
        // Older Windows 11: keep the system corner and an opaque page rather
        // than showing black wedges where the custom mask would be transparent.
        int corner = 2;  // DWMWCP_ROUND
        DwmSetWindowAttribute(hwnd, kDwmWindowCornerPreference, &corner, sizeof(corner));
    }
    if (!native) {
        COLORREF none = 0xFFFFFFFE;  // DWMWA_COLOR_NONE
        DwmSetWindowAttribute(hwnd, 35 /* DWMWA_CAPTION_COLOR */, &none, sizeof(none));
    }
    // With the frame extended DWM would draw its own minimize, maximize and close buttons over the
    // built-in title bar; without a system menu it draws none. The Windows title bar keeps it.
    LONG_PTR style = GetWindowLongPtrW(hwnd, GWL_STYLE);
    SetWindowLongPtrW(hwnd, GWL_STYLE, native ? (style | WS_SYSMENU) : (style & ~WS_SYSMENU));
    // Set_WndProc's WM_NCCALCSIZE drops or keeps the system title bar to match.
    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    if (!first) {
        Set_Relayout(w);
        RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_FRAME | RDW_UPDATENOW);
    }
}

static void Set_RememberRect(SetWindow& w) {
    if (w.render || IsIconic(w.hwnd) || IsZoomed(w.hwnd)) return;
    GetWindowRect(w.hwnd, &w.normalRect);
}

static LRESULT CALLBACK Set_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    SetWindow* w = g_setWin && g_setWin->hwnd == hwnd ? g_setWin.get() : nullptr;
    if (!w) return DefWindowProcW(hwnd, msg, wParam, lParam);
    if (w->nativeTitle) {
        LRESULT r = 0;
        if (DwmDefWindowProc(hwnd, msg, wParam, lParam, &r)) return r;  // Hover and clicks on DWM's buttons.
    }
    auto dipX = [&](LPARAM l) { return (float)(short)LOWORD(l) * 96.0f / w->dpi; };
    auto dipY = [&](LPARAM l) { return (float)(short)HIWORD(l) * 96.0f / w->dpi - kSetCapH; };
    switch (msg) {
        case WM_APP_SETTINGS_RELOAD:
            if (!w->saving) {
                launchpad_i18n::Select(GetStringSetting(L"interfaceLanguage"));
                Set_MakeFormats(*w);
                Set_LoadFields(*w);
                Set_ApplyFrame(*w);
                Set_Relayout(*w);
                SetWindowTextW(hwnd, Tr(L"Lumiflow settings"));
            }
            return 0;
        case WM_APP_SETTINGS_ACTIVATE: ShowWindow(hwnd, SW_RESTORE); ForceForeground(hwnd); return 0;
        case WM_NCCALCSIZE:
            // The custom frame owns the entire window rectangle; the rounded mask
            // in Set_Draw handles the outside shape and WM_NCHITTEST supplies
            // the resize edges. Native title mode keeps the system non-client.
            if (wParam && !w->render && !w->nativeTitle) {
                auto* nc = (NCCALCSIZE_PARAMS*)lParam;
                if (IsZoomed(hwnd)) {
                    // A maximized custom frame covers the work area, not the monitor:
                    // the taskbar and the outer edge stay outside the client.
                    MONITORINFO mi{sizeof(mi)};
                    if (GetMonitorInfoW(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi))
                        nc->rgrc[0] = mi.rcWork;
                    else
                        DefWindowProcW(hwnd, msg, wParam, lParam);
                }
                // Normal windows: client == window rectangle. All resize
                // edges come from WM_NCHITTEST, not a system frame.
                return 0;
            }
            break;
        case WM_NCHITTEST: {
            if (w->render || w->nativeTitle) break;
            POINT pt{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            ScreenToClient(hwnd, &pt);
            RECT rc;
            GetClientRect(hwnd, &rc);
            if (pt.x < 0 || pt.y < 0 || pt.x >= rc.right || pt.y >= rc.bottom) break;
            if (w->alphaComposition) {
                const int r = MulDiv((int)kSetCornerDip, (int)w->dpi, 96);
                auto outside = [&](int dx, int dy) { return (long long)dx * dx + (long long)dy * dy > (long long)r * r; };
                if (pt.x < r && pt.y < r && outside(r - pt.x, r - pt.y)) return HTTRANSPARENT;
                if (pt.x >= rc.right - r && pt.y < r && outside(pt.x - (rc.right - r), r - pt.y)) return HTTRANSPARENT;
                if (pt.x < r && pt.y >= rc.bottom - r && outside(r - pt.x, pt.y - (rc.bottom - r)))
                    return HTTRANSPARENT;
                if (pt.x >= rc.right - r && pt.y >= rc.bottom - r &&
                    outside(pt.x - (rc.right - r), pt.y - (rc.bottom - r)))
                    return HTTRANSPARENT;
            }
            int frame = GetSystemMetricsForDpi(SM_CXSIZEFRAME, w->dpi) +
                        GetSystemMetricsForDpi(SM_CXPADDEDBORDER, w->dpi);
            if (!IsZoomed(hwnd)) {
                const bool left = pt.x < frame, right = pt.x >= rc.right - frame;
                const bool top = pt.y < frame, bottom = pt.y >= rc.bottom - frame;
                if (top && left) return HTTOPLEFT;
                if (top && right) return HTTOPRIGHT;
                if (bottom && left) return HTBOTTOMLEFT;
                if (bottom && right) return HTBOTTOMRIGHT;
                if (left) return HTLEFT;
                if (right) return HTRIGHT;
                if (top) return HTTOP;
                if (bottom) return HTBOTTOM;
            }
            float x = pt.x * 96.0f / w->dpi, y = pt.y * 96.0f / w->dpi - kSetCapH;
            if (y < 0) return Set_CapButtonAt(*w, x, y) >= 0 ? HTCLIENT : HTCAPTION;
            return HTCLIENT;
        }
        case WM_ACTIVATE:
            w->active = LOWORD(wParam) != WA_INACTIVE;
            Set_Invalidate(*w);
            break;
        case WM_ERASEBKGND:
            return 1;
        case WM_PAINT:
        case WM_PRINTCLIENT: {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            if (!w->render && Set_EnsureRt(*w)) {
                Set_Draw(*w, w->rt.get());
                if (w->memDC) BitBlt(ps.hdc, 0, 0, w->memW, w->memH, w->memDC, 0, 0, SRCCOPY);
            }
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_CTLCOLOREDIT: {
            HDC dc = (HDC)wParam;
            SetTextColor(dc, RefFromRgb(pal().text));
            SetBkColor(dc, RefFromRgb(SetBoxFill()));
            return (LRESULT)w->editBrush;
        }
        case WM_SIZE:
            if (wParam != SIZE_MINIMIZED && !w->render) {
                Set_Relayout(*w);
                Set_RememberRect(*w);
                RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);  // No stale title bar after a snap.
            }
            return 0;
        case WM_MOVE:
            Set_RememberRect(*w);
            // The glass is a crop of a fixed desktop snapshot. Repaint as the
            // window moves so the crop follows instead of showing a stale patch.
            if (!w->render && w->alphaComposition)
                RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
            return 0;
        case WM_GETMINMAXINFO: {
            auto* mm = (MINMAXINFO*)lParam;
            // Client == window rectangle in the custom frame, so the minimum
            // track size is the requested client size without frame padding.
            mm->ptMinTrackSize = {MulDiv(760, w->dpi, 96), MulDiv(520, w->dpi, 96)};
            return 0;
        }
        case WM_DPICHANGED: {
            if (g_setForceDpi) return 0;
            Set_CommitEdit(*w, true);
            w->dpi = HIWORD(wParam);
            if (w->rt) w->rt->SetDpi((float)w->dpi, (float)w->dpi);
            auto* r = (RECT*)lParam;
            SetWindowPos(hwnd, nullptr, r->left, r->top, r->right - r->left, r->bottom - r->top,
                         SWP_NOZORDER | SWP_NOACTIVATE);
            Set_Relayout(*w);
            return 0;
        }
        case WM_TIMER:
            if (wParam == kSetTimerId) {
                if (Set_Tick(*w)) Set_Invalidate(*w);
                return 0;
            }
            break;
        case WM_APP_SETTINGS_TICK:
            InterlockedExchange(&g_setFramePending, 0);
            if (Set_Tick(*w)) Set_Invalidate(*w);
            return 0;
        case WM_MOUSEMOVE: {
            if (w->saving) return 0;
#ifdef LP_TEST_HOVER  // Only the test script's moves (marked 0x8000), never a real cursor passing over.
            if (!(wParam & 0x8000)) return 0;
#endif
            float x = dipX(lParam), y = dipY(lParam);
#ifndef LP_TEST_HOVER  // The test build feeds pointer moves by message; a leave would undo them.
            const bool startTracking = !w->tracking && !w->render;
#else
            const bool startTracking = false;
#endif
            if (startTracking) {
                TRACKMOUSEEVENT t{sizeof(t), TME_LEAVE, hwnd, 0};
                TrackMouseEvent(&t);
                w->tracking = true;
            }
            const bool pointerMoved = w->mouseX != x || w->mouseY != y;
            w->mouseX = x;
            w->mouseY = y;
            if (w->dragRail) {
                Set_RailDragTo(*w, x - w->dragOff);
                return 0;
            }
            if (w->dragSlider >= 0) {
                Set_SliderTo(*w, w->dragSlider, x);
                return 0;
            }
            if (w->dragScroll) {
                D2D1_RECT_F th = Set_ScrollThumb(*w);
                float view = Set_ViewH(*w), thh = th.bottom - th.top, range = view - thh - 8;
                float t = range > 0 ? std::clamp((y - w->dragOff - 4) / range, 0.0f, 1.0f) : 0;
                w->scrollTarget = w->scrollY = t * std::max(0.0f, w->contentH - view);
                Set_PlaceEdit(*w);
                Set_Invalidate(*w);
                return 0;
            }
            SetHit h = Set_Hit(*w, x, y);
            int dh = h.kind == SetHit::Drop ? h.b : -1;
            if (h.kind != w->hover.kind || h.id != w->hover.id || h.a != w->hover.a || h.b != w->hover.b ||
                dh != w->dropHover) {
                w->hover = h;
                if (w->openSel >= 0) w->dropHover = dh;
                Set_Invalidate(*w);
            } else if (pointerMoved && (h.id >= 0 || h.kind == SetHit::Drop)) {
                Set_Invalidate(*w);
            }
            return 0;
        }
        case WM_MOUSELEAVE: {
            POINT pointer; GetCursorPos(&pointer);
            w->tracking = false;
            w->hover = SetHit();
            if (!w->dragRail) w->mouseX = w->mouseY = -1;
            Set_Invalidate(*w);
            return 0;
        }
        case WM_SETCURSOR:
            if (LOWORD(lParam) == HTCLIENT) {
                PCWSTR cur = w->dragRail || w->hover.kind == SetHit::RailEdge ? IDC_SIZEWE
                             : Set_Clickable(w->hover)                         ? IDC_HAND
                                                                               : IDC_ARROW;
                SetCursor(LoadCursorW(nullptr, cur));
                return TRUE;
            }
            break;
        case WM_LBUTTONDOWN: {
            if (w->saving) return 0;
            if (!w->render) SetFocus(hwnd);
            w->kbd = false;
            float x = dipX(lParam), y = dipY(lParam);
            SetHit h = Set_Hit(*w, x, y);
            if (w->openSel >= 0) {  // The popup eats the click, whatever it hits.
                if (h.kind == SetHit::Drop) Set_Choose(*w, h.a, h.b);
                else Set_ClosePopup(*w);
                return 0;
            }
            Set_CommitEdit(*w, true);
            if (w->recording >= 0 && !(h.kind == SetHit::Control && h.a == w->recording)) Set_StopRecording(*w);
            w->hover = h;
            switch (h.kind) {
                case SetHit::Rail:
                    Set_GoPage(*w, h.a);
                    break;
                case SetHit::RailToggle:
                    Set_ToggleRail(*w);
                    break;
                case SetHit::RailEdge:
                    w->dragRail = true;
                    w->dragOff = x - Set_RailDock(*w).right;
                    SetCapture(hwnd);
                    Set_Invalidate(*w);
                    break;
                case SetHit::Action:
                    Set_MaterialAction(*w, h.a);
                    break;
                case SetHit::Footer:
                    Set_FooterAction(*w, h.a);
                    return 0;  // The window may be gone.
                case SetHit::PowerMove:
                    Set_MovePower(*w, h.a, h.b == 0 ? -1 : 1);
                    break;
                case SetHit::Control:
                case SetHit::Clear:
                case SetHit::Chip:
                    w->focus = h.a;
                    Set_ActivateField(*w, h.a, &h);
                    break;
                case SetHit::Slider:
                    w->focus = h.a;
                    w->dragSlider = h.a;
                    SetCapture(hwnd);
                    Set_SliderTo(*w, h.a, x);
                    break;
                case SetHit::Scroll: {
                    w->dragScroll = true;
                    w->dragOff = y - Set_ScrollThumb(*w).top;
                    SetCapture(hwnd);
                    Set_Invalidate(*w);
                    break;
                }
                case SetHit::Link:
                    Set_OpenLink(w->items[h.a].url.c_str());
                    break;
                case SetHit::Caption:
                    w->capDown = h.a + 1;
                    SetCapture(hwnd);
                    Set_Invalidate(*w);
                    break;
                default:
                    break;
            }
            return 0;
        }
        case WM_LBUTTONUP:
        case WM_CAPTURECHANGED:
            if (w->capDown) {
                int b = w->capDown - 1;
                w->capDown = 0;
                if (msg == WM_LBUTTONUP) {
                    if (GetCapture() == hwnd) ReleaseCapture();
                    SetHit h = Set_Hit(*w, dipX(lParam), dipY(lParam));
                    if (h.kind == SetHit::Caption && h.a == b) {
                        if (b == 0) ShowWindow(hwnd, SW_MINIMIZE);
                        else if (b == 1) ShowWindow(hwnd, IsZoomed(hwnd) ? SW_RESTORE : SW_MAXIMIZE);
                        else PostMessageW(hwnd, WM_CLOSE, 0, 0);
                    }
                }
                Set_Invalidate(*w);
                return 0;
            }
            if (w->dragSlider >= 0 || w->dragScroll || w->dragRail) {
                w->dragSlider = -1;
                w->dragScroll = false;
                w->dragRail = false;
                if (msg == WM_LBUTTONUP && GetCapture() == hwnd) ReleaseCapture();
                Set_Invalidate(*w);
            }
            return 0;
        case WM_MOUSEWHEEL: {
            if (w->openSel >= 0 && Set_In(w->dropRect, w->mouseX, w->mouseY)) {
                int n = (int)w->fields[w->openSel].options.size(), vis = Set_DropVisible(*w);
                int step = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? -3 : 3;
                w->dropTop = std::clamp(w->dropTop + step, 0, std::max(0, n - vis));
                SetHit h = Set_Hit(*w, w->mouseX, w->mouseY);
                w->hover = h;
                w->dropHover = h.kind == SetHit::Drop ? h.b : w->dropHover;
                Set_Invalidate(*w);
                return 0;
            }
            Set_CommitEdit(*w, true);
            Set_ClosePopup(*w);
            float delta = (float)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
            float maxScroll = std::max(0.0f, w->contentH - Set_ViewH(*w));
            w->scrollTarget = std::clamp(w->scrollTarget - delta * 72, 0.0f, maxScroll);
            if (!Set_Anim(*w)) w->scrollY = w->scrollTarget;
            Set_Invalidate(*w);
            return 0;
        }
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if (Set_OnKey(*w, (UINT)wParam)) return 0;
            // Without a system menu (glass title bar) Windows no longer turns Alt+F4 into a close.
            if (msg == WM_SYSKEYDOWN && wParam == VK_F4 && w->recording < 0) {
                PostMessageW(hwnd, WM_CLOSE, 0, 0);
                return 0;
            }
            break;
        case WM_CHAR:
        case WM_SYSCHAR:
            if (w->recording >= 0) return 0;
            break;
        case WM_KILLFOCUS:
            if (w->recording >= 0 && (HWND)wParam != w->edit) Set_StopRecording(*w);
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY: {
            Set_StopRecording(*w);
            if (w->timer) KillTimer(hwnd, kSetTimerId);
            if (!w->render && w->normalRect.right > w->normalRect.left) {
                const RECT& r = w->normalRect;
                SetState(StateKey(L"Settings", L"X").c_str(), r.left);
                SetState(StateKey(L"Settings", L"Y").c_str(), r.top);
                SetState(StateKey(L"Settings", L"CX").c_str(), r.right - r.left);
                SetState(StateKey(L"Settings", L"CY").c_str(), r.bottom - r.top);
            }
            if (!w->render) {
                SetState(StateKey(L"Settings", L"RailW").c_str(), (int)w->railW);
                SetState(StateKey(L"Settings", L"RailCollapsed").c_str(), w->railCollapsed ? 1 : 0);
            }
            return 0;
        }
        case WM_NCDESTROY: {
            if (w->editFont) DeleteObject(w->editFont);
            if (w->editBrush) DeleteObject(w->editBrush);
            if (w->iconSmall) DestroyIcon(w->iconSmall);
            if (w->iconBig) DestroyIcon(w->iconBig);
            if (w->owner) DestroyWindow(w->owner);
            w->destroyed = true;
            w->hwnd = nullptr;
            w->edit = nullptr;
            // Releases the DIB + memory DC as well as the render target and
            // the preview. WM_NCDESTROY is the only path that runs for a real
            // settings window, so resetting w->rt alone would leak the DIB
            // (about width * height * 4 bytes) on every open/close cycle.
            Set_ReleaseRt(*w);
            g_settingsWnd = nullptr; PostQuitMessage(0);
            g_setWin.reset();
            Set_ReleaseFactories();
            return DefWindowProcW(hwnd, msg, wParam, lParam);
        }
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

// The supplied glass sheet is the mark; no second, hand-drawn substitute.
static HICON Set_MakeIcon(const std::wstring&, int px) {
    return CreateAppIconHicon(px);
}

// The class belongs to this DLL and goes away with the settings thread. Registered under
// Explorer's handle it outlived a mod update, and the next window ran the unloaded code.
static void Set_RegisterClass() {
    UnregisterClassW(kSettingsClass, GetModuleHandleW(nullptr));  // left by builds before 2.3.0
    WNDCLASSEXW wc{sizeof(wc)};
    wc.lpfnWndProc = Set_WndProc;
    wc.hInstance = (HINSTANCE)&__ImageBase;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.lpszClassName = kSettingsClass;
    RegisterClassExW(&wc);
}

// Builds the window, hidden. render: an off-screen copy that is never shown (tests and PNG output).
static bool Set_Create(bool render) {
    if (!Set_EnsureFactories()) return false;
    Set_RegisterClass();
    auto w = std::make_shared<SetWindow>();
    w->render = render;
    w->editBrush = CreateSolidBrush(RefFromRgb(SetBoxFill()));
    g_setWin = w;
    if (render) {
        // An owned window gets no taskbar button.
        w->owner = CreateWindowExW(WS_EX_TOOLWINDOW, L"STATIC", L"", WS_POPUP, 0, 0, 0, 0, nullptr, nullptr,
                                   GetModuleHandleW(nullptr), nullptr);
    }
    const int offX = GetSystemMetrics(SM_XVIRTUALSCREEN) - 6000, offY = GetSystemMetrics(SM_YVIRTUALSCREEN);
    // The built-in title bar until Set_ApplyFrame reads the switches.
    kSetCapH = 40.0f;
    const DWORD style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
    HWND hwnd = CreateWindowExW(0, kSettingsClass, Tr(L"Lumiflow settings"), style,
                                render ? offX : CW_USEDEFAULT, render ? offY : CW_USEDEFAULT, 800, 600, w->owner,
                                nullptr, (HINSTANCE)&__ImageBase, nullptr);
    if (!hwnd) {
        // No WM_NCDESTROY follows a failed CreateWindowExW, so release the
        // objects this function already created. Otherwise each failed open
        // leaks a brush and an owned message window.
        if (w->editBrush) {
            DeleteObject(w->editBrush);
            w->editBrush = nullptr;
        }
        if (w->owner) {
            DestroyWindow(w->owner);
            w->owner = nullptr;
        }
        g_setWin.reset();
        return false;
    }
    w->hwnd = hwnd;
    w->dpi = g_setForceDpi ? g_setForceDpi : GetDpiForWindow(hwnd);
    ApplyWindowColors(hwnd, false);
    Set_MakeFormats(*w);
    Set_LoadFields(*w);
    if (!render) {
        w->railW = std::clamp((float)GetState(StateKey(L"Settings", L"RailW").c_str(), (int)kSetRailDef), kSetRailMin, kSetRailMax);
        w->railCollapsed = GetState(StateKey(L"Settings", L"RailCollapsed").c_str(), 0) != 0;
        w->railCur = w->railCollapsed ? kSetRailMini : w->railW;
    }
    if (!render) {
        w->iconSmall = Set_MakeIcon(w->iconFamily, GetSystemMetricsForDpi(SM_CXSMICON, w->dpi));
        w->iconBig = Set_MakeIcon(w->iconFamily, GetSystemMetricsForDpi(SM_CXICON, w->dpi));
        if (w->iconSmall) SendMessageW(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)w->iconSmall);
        if (w->iconBig) SendMessageW(hwnd, WM_SETICON, ICON_BIG, (LPARAM)w->iconBig);
        Set_ApplyFrame(*w);  // Title bar and glass; also drops the system title bar the first WM_NCCALCSIZE kept.
    }

    auto outer = [&](float cwDip, float chDip, SIZE& size) {
        // The custom frame owns the whole window rectangle; no non-client
        // padding is added, so the glass tint reaches every edge.
        size = {MulDiv((int)cwDip, w->dpi, 96), MulDiv((int)chDip, w->dpi, 96)};
    };
    SIZE size;
    if (render) {
        w->cw = 980;
        w->ch = 700;
        outer(w->cw, w->ch, size);
        SetWindowPos(hwnd, nullptr, offX, offY, size.cx, size.cy, SWP_NOZORDER | SWP_NOACTIVATE);
        Set_BuildPage(*w);
        return true;
    }
    outer(980, 720, size);
    POINT cursor;
    GetCursorPos(&cursor);
    MONITORINFO mi{sizeof(mi)};
    GetMonitorInfoW(MonitorFromPoint(cursor, MONITOR_DEFAULTTONEAREST), &mi);
    const RECT& work = mi.rcWork;
    int width = std::min((int)size.cx, (int)(work.right - work.left) * 9 / 10);
    int height = std::min((int)size.cy, (int)(work.bottom - work.top) * 9 / 10);
    RECT r{(work.left + work.right - width) / 2, (work.top + work.bottom - height) / 2, 0, 0};
    r.right = r.left + width;
    r.bottom = r.top + height;
    int cx = GetState(StateKey(L"Settings", L"CX").c_str(), 0);
    int cy = GetState(StateKey(L"Settings", L"CY").c_str(), 0);
    if (cx > 0 && cy > 0) {
        RECT saved{GetState(StateKey(L"Settings", L"X").c_str(), 0), GetState(StateKey(L"Settings", L"Y").c_str(), 0),
                   0, 0};
        saved.right = saved.left + cx;
        saved.bottom = saved.top + cy;
        if (MonitorFromRect(&saved, MONITOR_DEFAULTTONULL)) r = saved;
    }
    SetWindowPos(hwnd, nullptr, r.left, r.top, r.right - r.left, r.bottom - r.top, SWP_NOZORDER | SWP_NOACTIVATE);
    // The monitor it landed on may have a different scale.
    UINT dpi = GetDpiForWindow(hwnd);
    if (!g_setForceDpi && dpi != w->dpi) w->dpi = dpi;
    Set_Relayout(*w);
    // Capture the desktop while the window is still hidden. This is the only
    // full-screen grab; dragging later crops the same blurred snapshot.
    Set_EnsureRt(*w);
    Set_CaptureDesktop(*w);
    w->pageT = Set_Anim(*w) ? 0.0f : 1.0f;
    return true;
}

void Settings_Open() {
    if (g_setWin && g_setWin->hwnd && !g_setWin->render) {
        HWND hwnd = g_setWin->hwnd;
        if (IsIconic(hwnd)) ShowWindow(hwnd, SW_RESTORE);
        SetForegroundWindow(hwnd);
        return;
    }
    if (g_setWin) Settings_Close();
    if (!Set_Create(false)) return;
    HWND hwnd = g_setWin->hwnd;
    ShowWindow(hwnd, SW_SHOW);
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);
}

void Settings_Close() {
    if (g_setWin && g_setWin->hwnd) DestroyWindow(g_setWin->hwnd);
    g_setWin.reset();
}

// 120 Hz high-resolution waitable timer for the settings window. The launcher
// has its own GPU frame loop; this keeps the settings page from being pinned
// to SetTimer's 15.6 ms tick while it is open.
DWORD WINAPI SetFrameThread(void*) {
    while (!g_setFrameStop.load()) {
        LARGE_INTEGER due{};
        due.QuadPart = -83333LL;  // 8.33 ms, 120 Hz in 100 ns units
        if (!SetWaitableTimer(g_setFrameWake, &due, 0, nullptr, nullptr, FALSE)) break;
        DWORD wait = WaitForSingleObject(g_setFrameWake, 50);
        if (wait != WAIT_OBJECT_0) continue;
        if (g_setFrameStop.load()) break;
        HWND hwnd = g_settingsWnd.load();
        if (hwnd && InterlockedCompareExchange(&g_setFramePending, 1, 0) == 0) {
            if (!PostMessageW(hwnd, WM_APP_SETTINGS_TICK, 0, 0))
                InterlockedExchange(&g_setFramePending, 0);  // Do not stop ticking on a transient failure.
        }
    }
    return 0;
}

static bool SetFrameStart() {
    g_setFramePending = 0;
    g_setFrameStop = false;
    g_setFrameWake = CreateWaitableTimerExW(nullptr, nullptr, 0x2 /* high resolution */, TIMER_ALL_ACCESS);
    if (!g_setFrameWake) return false;
    g_setFrameThread = CreateThread(nullptr, 0, SetFrameThread, nullptr, 0, nullptr);
    if (!g_setFrameThread) {
        CloseHandle(g_setFrameWake);
        g_setFrameWake = nullptr;
        return false;
    }
    return true;
}

static void SetFrameStop() {
    g_setFrameStop = true;
    if (g_setFrameWake) CancelWaitableTimer(g_setFrameWake);
    if (g_setFrameThread) {
        // The thread only wakes every 50 ms and has no blocking call that
        // could outlive the window, so wait for it without a timeout. Closing
        // the handle after 100 ms (the old behaviour) could unload the module
        // while the thread was still executing its loop.
        if (WaitForSingleObject(g_setFrameThread, 5000) == WAIT_TIMEOUT)
            Wh_Log(L"Settings frame thread did not stop within 5 s; waiting for it before unload");
        WaitForSingleObject(g_setFrameThread, INFINITE);
        CloseHandle(g_setFrameThread);
        g_setFrameThread = nullptr;
    }
    if (g_setFrameWake) {
        CloseHandle(g_setFrameWake);
        g_setFrameWake = nullptr;
    }
    g_setFramePending = 0;
}

// Runs on the settings thread, after its message loop or its early exit.
void SettingsThreadTeardown() {
    SetFrameStop();
    Settings_Close();
    UnregisterClassW(kSettingsClass, (HINSTANCE)&__ImageBase);
    Set_ReleaseFactories();
    CoUninitialize();
}

DWORD WINAPI SettingsThread(void*) {
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    Settings_Open();
    // CloseSettingsWindow may have run while the window was being built; the
    // stop flag is what tells this thread to leave instead of entering the
    // loop, where the posted WM_QUIT would already have been consumed.
    if (g_settingsStop) {
        SettingsThreadTeardown();
        return 0;
    }
    if (g_setWin) g_settingsWnd = g_setWin->hwnd;
    if (g_setWin && !SetFrameStart())
        g_setWin->timer = SetTimer(g_settingsWnd, kSetTimerId, 15, nullptr) != 0;
    MSG msg;
    while (g_settingsWnd && GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    SettingsThreadTeardown();
    return 0;
}

void OpenSettingsWindow() {
    if (g_fullImportBusy) return;
    if (HWND hwnd = g_settingsWnd) {
        PostMessageW(hwnd, WM_APP_SETTINGS_ACTIVATE, 0, 0);
        return;
    }
    if (g_settingsThread) {
        // A previous window was closed: reap that thread before starting one.
        if (WaitForSingleObject(g_settingsThread, 0) == WAIT_TIMEOUT) return;
        CloseHandle(g_settingsThread);
        g_settingsThread = nullptr;
        g_settingsThreadId = 0;
    }
    g_settingsStop = false;
    g_settingsThread = CreateThread(nullptr, 0, SettingsThread, nullptr, 0,
                                    &g_settingsThreadId);
}

// The settings window lives on its own thread, and the DLL must not unload
// while either the window or the thread is still alive. The thread is asked
// to quit through its message queue and then joined without a bound: the
// thread's own exit path is what guarantees the join returns, so waiting for
// it is the only correct order. g_settingsStop covers the window that has not
// finished building yet.
void CloseSettingsWindow() {
    g_settingsStop = true;
    if (HWND hwnd = g_settingsWnd) PostMessageW(hwnd, WM_CLOSE, 0, 0);
    if (g_settingsThread) {
        if (g_settingsThreadId) {
            // The queue may not exist yet while the window is being built, so
            // post repeatedly until the thread is gone.
            do {
                PostThreadMessageW(g_settingsThreadId, WM_QUIT, 0, 0);
            } while (WaitForSingleObject(g_settingsThread, 100) == WAIT_TIMEOUT);
        } else {
            WaitForSingleObject(g_settingsThread, INFINITE);
        }
        CloseHandle(g_settingsThread);
        g_settingsThread = nullptr;
        g_settingsThreadId = 0;
    }
    g_settingsWnd = nullptr;
}

void EnsureVisible(int index) {
    if (index < 0) {
        return;
    }
    if (g_openFolder) {
        FolderLayout F = GetFolderLayout();
        int row = index / F.columns;
        g_folderRow = std::clamp(g_folderRow, row - F.visibleRows + 1, row);
        return;
    }
    const Layout& L = g_layout;
    if (GroupedView() && index < (int)g_groupCells.size()) {
        EnsureFreeGroupVisible(index);
        auto cell = g_groupCells[index];
        if (g_settings.pages) {
            int page=std::max(0,(int)std::floor(cell.icon.left/L.width));
            float target=page*(float)L.width;
            if(target!=g_scrollTarget)g_groupPagePan=0;
            g_scrollTarget=target;
            if(g_squareGridColumns) {
                if(cell.icon.top-g_groupPagePan<L.viewTop)g_groupPagePan=cell.icon.top-L.viewTop;
                if(cell.label.bottom-g_groupPagePan>L.viewBottom)g_groupPagePan=cell.label.bottom-L.viewBottom;
                g_groupPagePan=std::clamp(g_groupPagePan,0.f,MaxPagePan(page));
            }
        }
        else {
            if (cell.icon.top - g_scrollTarget < L.viewTop) g_scrollTarget = cell.icon.top - L.viewTop;
            if (cell.label.bottom - g_scrollTarget > L.viewBottom) g_scrollTarget = cell.label.bottom - L.viewBottom;
            g_scrollTarget = std::clamp(g_scrollTarget, 0.0f, MaxScroll());
        }
        return;
    }
    if (g_settings.pages) {
        float target=(index/L.perPage)*(float)L.width;
        if(target!=g_scrollTarget)g_groupPagePan=0;
        g_scrollTarget=target;
        auto cell=MakeSquareCell(0,L.viewTop+((index%L.perPage)/L.columns)*L.cellHeight,L.cellWidth);
        if(cell.icon.top-g_groupPagePan<L.viewTop)g_groupPagePan=cell.icon.top-L.viewTop;
        if(cell.label.bottom-g_groupPagePan>L.viewBottom)g_groupPagePan=cell.label.bottom-L.viewBottom;
        g_groupPagePan=std::clamp(g_groupPagePan,0.f,MaxPagePan(index/L.perPage));
        return;
    }
    int row = index / L.columns;
    int firstRow = (int)std::lround(g_scrollTarget / L.cellHeight);
    if (row < firstRow) {
        g_scrollTarget = row * L.cellHeight;
    } else if (row >= firstRow + L.visibleRows) {
        g_scrollTarget = (row - L.visibleRows + 1) * L.cellHeight;
    }
    g_scrollTarget = std::clamp(g_scrollTarget, 0.0f, MaxScroll());
}

// With pages, any number of rows flips one page that way.
void ScrollRows(int rows) {
    if (g_settings.pages) {
        float width = (float)g_layout.width;
        float target =
            (std::lround(g_scrollTarget / width) + (rows > 0 ? 1 : -1)) * width;
        float previous=g_scrollTarget;
        g_scrollTarget = std::clamp(target, 0.0f, MaxScroll());
        if(previous!=g_scrollTarget)g_groupPagePan=0;
        StartFrames();
        return;
    }
    float cell = g_layout.cellHeight;
    float target = (std::lround(g_scrollTarget / cell) + rows) * cell;
    g_scrollTarget = std::clamp(target, 0.0f, MaxScroll());
    StartFrames();
}

////////////////////////////////////////////////////////////////////////////////
// Folders, dragging and the right-click menus

// After the arrangement changed.
void Refresh() {
    UpdateFilter();
    ComputeLayout();
    Render();
}

int IndexOf(const Tiles& tiles, const Tile& tile) {
    for (int i = 0; i < (int)tiles.size(); i++) {
        if (tiles[i] == tile) {
            return i;
        }
    }
    return -1;
}

// The stable layout order is shared by drags and the group menu.
Tiles SelectedTiles() {
    Tiles tiles;
    auto collect = [&](const Tile& tile) {
        if (tile.app && AppSelected(*tile.app)) tiles.push_back(tile);
    };
    for (const auto& tile : g_tiles) {
        if (tile.folder) {
            for (const auto& app : tile.folder->apps) collect(app);
        } else collect(tile);
    }
    return tiles;
}

void EraseTiles(Tiles& source, const Tiles& moving) {
    std::erase_if(source, [&](const Tile& tile) { return IndexOf(moving, tile) >= 0; });
}

#include "launchpad_groups_actions.inc"

// After a change that may have left the open folder with one app or none.
void DropGoneFolder() {
    TidyTiles();
    if (g_openFolder && IndexOf(g_tiles, {nullptr, g_openFolder}) < 0) {
        g_openFolder = nullptr;
    }
    g_hover = g_selected = g_pressed = -1;
}

// Used by the menu, Delete and the selection toolbar.
void RemoveAppsFromGrid(const AppSelection& selection) {
    if (selection.empty() || g_layoutReadOnly || g_dataReadOnly || g_entriesReadOnly) return;
    bool unpin = PinnedOnly();
    std::unordered_set<std::wstring> keys;
    for (const auto& app : selection) {
        auto key = AppKey(*app);
        keys.insert(key);
        if (unpin) g_pins.erase(key);
        else g_hidden.insert(key);
    }
    if (unpin) SavePins();
    auto selected = [&](const Tile& tile) {
        return tile.app && keys.contains(AppKey(*tile.app));
    };
    std::erase_if(g_tiles, selected);
    for (auto& tile : g_tiles) {
        if (tile.folder)
            std::erase_if(tile.folder->apps, selected);
    }
    if (std::any_of(g_appSelection.begin(), g_appSelection.end(),
                    [&](const auto& app) { return keys.contains(AppKey(*app)); }))
        ClearAppSelection();
    DropGoneFolder();
    SaveTiles();
    Refresh();
}

void DeleteSelectedApps() {
    if (!g_batchLaunching) RemoveAppsFromGrid(g_appSelection);
}

void OpenFolder(std::shared_ptr<Folder> folder) {
    CancelImeComposition(g_wnd);
    if (!g_drag.active) ClearAppSelection();
    FinishMetrics();
    if (g_settings.performanceLogging) g_metrics.Begin("folder-open", Now(), g_layout.width, g_layout.height, g_layout.scale, g_refreshHz);
    g_folderRenaming = false;
    g_folderTitleHover = false;
    g_openFolder = std::move(folder);
    g_folderOriginalName=g_openFolder->name;
    g_closingFolder = nullptr;
    g_folderAnim = Animate() ? 0.0f : 1.0f;
    if (g_folderAnim < 1) {
        StartFrames();
    }
    g_folderRow = 0;
    g_hover = g_selected = g_pressed = -1;
    g_searchDrag = false;
    g_hist[1] = {};  // Fresh history per folder.
    SetCaret(EditText().size(), false);  // At the end of the name.
    Render();
}

void CloseFolder() {
    if (!g_openFolder) {
        return;
    }
    CancelImeComposition(g_wnd);
    ClearAppSelection();
    FinishMetrics();
    if (g_settings.performanceLogging) g_metrics.Begin("folder-close", Now(), g_layout.width, g_layout.height, g_layout.scale, g_refreshHz);
    if (Animate()) {
        g_closingFolder = g_openFolder;
        StartFrames();
    }
    g_folderRenaming = false;
    g_folderTitleHover = false;
    SaveFolderName();
    g_openFolder = nullptr;
    g_hover = g_selected = g_pressed = -1;
    g_searchDrag = false;
    SetCaret(g_query.size(), false);
    Refresh();
}

void ActivateItem(int index) {
    const Tiles& items = Items();
    if (index < 0 || index >= (int)items.size()) {
        return;
    }
    if (items[index].folder) {
        OpenFolder(items[index].folder);
    } else {
        LaunchTarget(items[index].app->target,nullptr,items[index].app.get());
    }
}

// Where the click doesn't count as one on empty space.
bool ClickKeepsOpen(int x, int y) {
    if(ToolBarConsumes(x,y))return true;
    // A section is a caption and a run of icons, not a panel: its empty space
    // is empty space, and clicking there closes the grid like anywhere else.
    // Only the caption itself stays a target, since it starts a drag.
    if (GroupedView() && GroupAtPoint(x, y, true) >= 0) return true;
    return g_openFolder ? InRect(GetFolderLayout().panel, (float)x, (float)y)
                        : InRect(g_layout.search, (float)x, (float)y) ||
                              PowerAt(x, y) >= 0;
}

// The words both names start with ("Microsoft Edge" and "Microsoft Teams"
// give "Microsoft"), or "Folder".
std::wstring FolderName(const std::wstring& a, const std::wstring& b) {
    size_t same = 0;
    for (size_t i = 0;; i++) {
        bool endA = i == a.size() || a[i] == L' ';
        bool endB = i == b.size() || b[i] == L' ';
        if (endA && endB) {
            same = i;
            if (i == a.size() || i == b.size()) {
                break;
            }
            continue;
        }
        if (endA || endB || towlower(a[i]) != towlower(b[i])) {
            break;
        }
    }
    return same ? a.substr(0, same) : Tr(L"Folder");
}

void StartDrag() {
    const Tiles& items = Items();
    int i = g_pressed;
    g_drag.pending = false;
    if (i < 0 || i >= (int)items.size() || !g_query.empty() ||
        g_layoutReadOnly || g_dataReadOnly || g_entriesReadOnly) {
        return;
    }
    Cell c = ItemCell(i);
    g_drag.active = true;
    g_drag.tile = items[i];
    if (g_drag.tile.app && AppSelected(*g_drag.tile.app)) {
        g_drag.tiles = SelectedTiles();
    } else {
        ClearAppSelection();
    }
    if (g_drag.tiles.empty()) g_drag.tiles.push_back(g_drag.tile);
    g_selectionPointer = {};
    g_drag.at = g_drag.start;
    g_drag.iconSize = c.icon.right - c.icon.left;
    g_drag.grab = D2D1::Point2F(g_drag.start.x - c.icon.left,
                                g_drag.start.y - c.icon.top);
    g_drag.target = g_drag.slot = -1;
    g_drag.edge = 0;
    g_drag.inPanel = g_openFolder != nullptr;
    g_drag.backup = ArrangementLines();
    g_hover = g_selected = g_pressed = -1;
    StartFrames();
}

void DragMove(int x, int y) {
    const Layout& L = g_layout;
    double now = Now();
    g_drag.at = {x, y};
    if (!g_drag.moved && x == g_drag.start.x && y == g_drag.start.y) return;
    g_drag.moved = true;
    if (GroupedView() && !g_openFolder) {
        GroupDragMove(x,y);
        int edge = g_settings.pages ? (x < 36 * L.scale ? -1 : x > L.width - 36 * L.scale ? 1 : 0) :
            (y < L.viewTop ? -1 : y >= L.viewBottom ? 1 : 0);
        if (edge != g_drag.edge) { g_drag.edge = edge; g_drag.edgeTime = now; }
        return;
    }
    if (g_openFolder) {
        if (InRect(GetFolderLayout().panel, (float)x, (float)y)) {
            g_drag.inPanel = true;
        } else if (g_drag.inPanel) {
            // Out of the folder, into the grid right after it.
            if (IndexOf(g_openFolder->apps, g_drag.tile) >= 0) {
                EraseTiles(g_openFolder->apps, g_drag.tiles);
                int f = IndexOf(g_tiles, {nullptr, g_openFolder});
                g_tiles.insert(g_tiles.begin() + f + 1, g_drag.tiles.begin(), g_drag.tiles.end());
            }
            g_openFolder = nullptr;
            UpdateFilter();
            ComputeLayout();
            if (GroupedView()) {
                DragMove(x, y);
                return;
            }
        }
    }

    // Over the middle of another icon: dropping there makes or fills a folder.
    // The rest of that icon moves nothing, so reaching the middle doesn't
    // shuffle the icons first.
    int target = -1;
    bool overIcon = false;
    if (!g_openFolder && g_drag.tile.app) {
        int i = ItemAt(x, y);
        if (i >= 0 && !IsDragged(g_visible[i])) {
            D2D1_RECT_F r = CellAt(i).icon;
            overIcon = InRect(r, (float)x, (float)y);
            float in = (r.right - r.left) * 0.2f;
            if (InRect(D2D1::RectF(r.left + in, r.top + in, r.right - in,
                                   r.bottom - in),
                       (float)x, (float)y)) {
                target = i;
            }
        }
    }
    if (target != g_drag.target) {
        g_drag.target = target;
        g_drag.targetTime = now;
    }
    int slot = overIcon ? -1 : SlotAt(x, y);
    if (slot != g_drag.slot) {
        g_drag.slot = slot;
        g_drag.slotTime = now;
    }

    int edge = 0;
    if (!g_openFolder) {
        if (g_settings.pages) {
            float zone =
                std::max(36 * L.scale, std::min(L.gridLeft, L.width * 0.06f));
            edge = x < zone ? -1 : x >= L.width - zone ? 1 : 0;
        } else {
            edge = y < L.viewTop ? -1 : y >= L.viewBottom ? 1 : 0;
        }
    }
    if (edge != g_drag.edge) {
        g_drag.edge = edge;
        g_drag.edgeTime = now;
    }
}

void ReorderDraggedTiles() {
    if (g_drag.slot < 0) return;
    Tiles& tiles = g_openFolder ? g_openFolder->apps : g_tiles;
    if (IndexOf(tiles, g_drag.tile) < 0) return;
    int to = std::clamp(g_drag.slot, 0, std::max(0, (int)tiles.size() - (int)g_drag.tiles.size()));
    auto at = tiles.begin() + to;
    if (g_drag.tiles.size() <= tiles.size() - to &&
        std::equal(g_drag.tiles.begin(), g_drag.tiles.end(), at)) return;
    CaptureTileMotion();
    EraseTiles(tiles, g_drag.tiles);
    tiles.insert(tiles.begin() + std::min(to, (int)tiles.size()),
                 g_drag.tiles.begin(), g_drag.tiles.end());
    if (!g_openFolder) {
        UpdateFilter();
        ComputeLayout();
    }
}

// Each frame of a drag: what happens when the pointer rests somewhere.
void DragTick() {
    // The page or rows under a still pointer move while scrolling.
    DragMove(g_drag.at.x, g_drag.at.y);
    double now = Now();
    if (g_drag.edge &&
        now - g_drag.edgeTime >= (g_settings.pages ? 0.6 : 0.3)) {
        g_drag.edgeTime = now;
        ScrollRows(g_drag.edge);
    }
    if (GroupedView() && !g_openFolder) return;

    int t = g_drag.target;
    if (t >= 0 && t < (int)g_visible.size() && g_visible[t].folder &&
        now - g_drag.targetTime >= 0.7) {
        // Resting on a folder opens it, to place the app inside.
        std::shared_ptr<Folder> folder = g_visible[t].folder;
        EraseTiles(g_tiles, g_drag.tiles);
        folder->apps.insert(folder->apps.end(), g_drag.tiles.begin(), g_drag.tiles.end());
        UpdateFilter();
        ComputeLayout();
        OpenFolder(folder);
        FolderLayout F = GetFolderLayout();
        g_folderRow = F.rows - F.visibleRows;
        g_drag.target = g_drag.slot = -1;
        g_drag.edge = 0;
        g_drag.inPanel = false;
        return;
    }

    if (g_drag.slot >= 0 && now - g_drag.slotTime >= 0.2) {
        ReorderDraggedTiles();
    }
}

// drop: keep the new place (and make or fill a folder); otherwise everything
// goes back.
void EndDrag(bool drop) {
    g_drag.pending = false;
    if (!g_drag.active) {
        return;
    }
    if (GroupedView() && !g_openFolder) {
        drop = drop && g_drag.tile.app && g_groupDropFolder;
        CaptureTileMotion();
        if (drop) MoveAppsToGroup(g_drag.tiles, g_groupDropFolder, g_groupDropSlot);
        if (!drop && ArrangementLines() != g_drag.backup) ArrangeTiles(g_drag.backup);
        g_drag = {}; g_groupDropFolder.reset(); g_groupDropSlot = -1;
        if (drop) SaveTiles();
        Refresh(); return;
    }
    if (drop) ReorderDraggedTiles();
    CaptureTileMotion();
    if (drop) {
        for (auto& motion : g_tileMotion) {
            if (motion.key == MotionKey(g_drag.tile)) {
                float outset = (std::round(g_drag.iconSize * 1.06f) - g_drag.iconSize) / 2;
                motion.from = {g_drag.at.x - g_drag.grab.x - outset,
                               g_drag.at.y - g_drag.grab.y - outset};
            }
        }
    }
    auto drag = std::exchange(g_drag, {});
    int target = drag.target;
    if (!drop && ArrangementLines() != drag.backup) {
        g_openFolder = nullptr;
        ArrangeTiles(drag.backup);
    } else if (drop && target >= 0 && target < (int)g_tiles.size()) {
        Tile onto = g_tiles[target];
        if (onto.folder) {
            onto.folder->apps.insert(onto.folder->apps.end(), drag.tiles.begin(), drag.tiles.end());
        } else {
            auto folder = std::make_shared<Folder>();
            folder->name = FolderName(onto.app->name, drag.tile.app->name);
            folder->apps = {onto};
            folder->apps.insert(folder->apps.end(), drag.tiles.begin(), drag.tiles.end());
            g_tiles[target] = {nullptr, folder};
        }
        EraseTiles(g_tiles, drag.tiles);
    }
    DropGoneFolder();
    if (drop) {
        SaveTiles();
    }
    Refresh();
}

// What an app's menu needs.
struct AppPaths {
    std::wstring file;      // The program it starts, when that's a file.
    std::wstring folder;    // A Store app's install folder.
    std::wstring shortcut;  // Its shortcut in the Start menu or on the desktop.
};

std::wstring ItemString(IShellItem2* item, PCWSTR property) {
    PROPERTYKEY key;
    PWSTR value = nullptr;
    std::wstring result;
    if (SUCCEEDED(PSGetPropertyKeyFromName(property, &key)) &&
        SUCCEEDED(item->GetString(key, &value))) {
        result = value;
        CoTaskMemFree(value);
    }
    return result;
}

bool FileExists(const std::wstring& path) {
    return !path.empty() &&
           GetFileAttributesW(path.c_str()) != INVALID_FILE_ATTRIBUTES;
}

// Start lists the shortcuts in these two folders and their subfolders. The
// one named like the app, or else the one that starts the same file.
std::wstring FindStartShortcut(const std::wstring& name,
                               const std::wstring& file) {
    std::vector<std::wstring> dirs, links;
    for (REFKNOWNFOLDERID id : {FOLDERID_Programs, FOLDERID_CommonPrograms}) {
        PWSTR root = nullptr;
        if (SUCCEEDED(SHGetKnownFolderPath(id, 0, nullptr, &root))) {
            dirs.push_back(root);
        }
        CoTaskMemFree(root);
    }
    while (!dirs.empty() && links.size() < 5000) {
        std::wstring dir = std::move(dirs.back());
        dirs.pop_back();
        WIN32_FIND_DATAW fd;
        HANDLE find = FindFirstFileW((dir + L"\\*").c_str(), &fd);
        if (find == INVALID_HANDLE_VALUE) {
            continue;
        }
        do {
            std::wstring path = dir + L"\\" + fd.cFileName;
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (fd.cFileName[0] != L'.' &&
                    !(fd.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)) {
                    dirs.push_back(path);
                }
            } else if (EndsWith(ToLower(path), L".lnk")) {
                links.push_back(path);
            }
        } while (FindNextFileW(find, &fd));
        FindClose(find);
    }

    std::wstring wanted = L"\\" + ToLower(name) + L".lnk";
    for (const auto& link : links) {
        if (EndsWith(ToLower(link), wanted)) {
            return link;
        }
    }
    ComPtr<IShellLinkW> shellLink;
    ComPtr<IPersistFile> persist;
    if (file.empty() ||
        FAILED(CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER,
                                IID_PPV_ARGS(&shellLink))) ||
        FAILED(shellLink.As(&persist))) {
        return {};
    }
    std::wstring lowerFile = ToLower(file);
    for (const auto& link : links) {
        WCHAR target[MAX_PATH];
        if (SUCCEEDED(persist->Load(link.c_str(), STGM_READ)) &&
            shellLink->GetPath(target, ARRAYSIZE(target), nullptr, 0) == S_OK &&
            ToLower(target) == lowerFile) {
            return link;
        }
    }
    return {};
}

AppPaths GetAppPaths(const App& app) {
    AppPaths paths;
    std::wstring lower = ToLower(app.target);
    bool fromAppsFolder = lower.rfind(ToLower(kAppsFolder), 0) == 0;
    ComPtr<IShellItem2> item;
    if (SUCCEEDED(SHCreateItemFromParsingName(app.target.c_str(), nullptr,
                                              IID_PPV_ARGS(&item)))) {
        paths.file = ItemString(item.Get(), L"System.Link.TargetParsingPath");
        paths.folder =
            ItemString(item.Get(), L"System.AppUserModel.PackageInstallPath");
    }
    if (!fromAppsFolder) {
        if (EndsWith(lower, L".lnk")) {
            paths.shortcut = app.target;
        } else if (paths.file.empty()) {
            paths.file = app.target;
        }
    }
    // Control Panel and the like point to "::{...}", not a file.
    if (paths.file.rfind(L"::", 0) == 0 || !FileExists(paths.file)) {
        paths.file.clear();
    }
    if (paths.shortcut.empty() && fromAppsFolder && paths.folder.empty()) {
        paths.shortcut = FindStartShortcut(app.name, paths.file);
    }
    return paths;
}

void ShowInFolder(const std::wstring& path) {
    PIDLIST_ABSOLUTE pidl = nullptr;
    if (SUCCEEDED(SHParseDisplayName(path.c_str(), nullptr, &pidl, 0,
                                     nullptr))) {
        SHOpenFolderAndSelectItems(pidl, 0, nullptr, 0);
        CoTaskMemFree(pidl);
    }
}

bool PickIconFile(HWND hwnd, std::wstring& path) {
    WCHAR file[MAX_PATH] = L"";
    OPENFILENAMEW ofn{sizeof(ofn)};
    ofn.hwndOwner = hwnd;
    auto filter = launchpad_i18n::IconFileFilter();
    ofn.lpstrFilter = filter.c_str();
    ofn.lpstrFile = file;
    ofn.nMaxFile = ARRAYSIZE(file);
    ofn.lpstrTitle = Tr(L"Choose a picture for the icon");
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST |
                OFN_NOCHANGEDIR;
    bool previousModal = g_modal;
    g_modal = true;
    BOOL ok = GetOpenFileNameW(&ofn);
    g_modal = previousModal;
    if (g_shown && !g_closing) {
        ForceForeground(hwnd);
    }
    if (ok) {
        path = file;
    }
    return ok;
}

void TakeOutOfFolder(const Tile& tile) {
    if (!g_openFolder) {
        return;
    }
    Tiles& apps = g_openFolder->apps;
    int i = IndexOf(apps, tile);
    if (i < 0) {
        return;
    }
    apps.erase(apps.begin() + i);
    int f = IndexOf(g_tiles, {nullptr, g_openFolder});
    g_tiles.insert(g_tiles.begin() + (f + 1), tile);
    DropGoneFolder();
    SaveTiles();
    Refresh();
}

void Ungroup(const std::shared_ptr<Folder>& folder) {
    int f = IndexOf(g_tiles, {nullptr, folder});
    if (f < 0) {
        return;
    }
    Tiles apps = folder->apps;
    g_tiles.erase(g_tiles.begin() + f);
    g_tiles.insert(g_tiles.begin() + f, apps.begin(), apps.end());
    DropGoneFolder();
    SaveTiles();
    Refresh();
}

// An "&" in a menu would underline the next letter.
std::wstring MenuText(const std::wstring& text) {
    std::wstring out;
    for (WCHAR c : text) {
        if (c == L'&') {
            out += L'&';
        }
        out += c;
    }
    return out;
}

// Store apps and shell-resolved exes live behind an AppsFolder target; their
// elevation goes through a temporary .lnk, never the raw target.
bool IsPackagedTarget(const std::wstring& target) {
    return ToLower(target).rfind(ToLower(kAppsFolder), 0) == 0;
}

#include "launchpad_web_icons.inc"
#include "launchpad_entry_editor.inc"
#include "launchpad_full_backup.inc"

void ShowTileMenu(HWND hwnd, int index, POINT pt) {
    enum {
        kOpen = 1,
        kRunAs,
        kFileLocation,
        kShortcutLocation,
        kChangeIcon,
        kRestoreIcon,
        kOutOfFolder,
        kRemove,
        kProperties,
        kUngroup,
        kPin,
    };
    Tile tile = Items()[index];
    bool useSelection = tile.app && AppSelected(*tile.app) && g_appSelection.size() > 1;
    g_selected = index;
    Render();
    HMENU menu = CreatePopupMenu();
    AppPaths paths;
    std::wstring key;
    auto appendGroups = [&] {
        if (!g_settings.groups) return;
        HMENU groups = CreatePopupMenu();
        for (int i = 0; i < (int)g_tiles.size(); i++) if (g_tiles[i].folder)
            AppendMenuW(groups, MF_STRING, 1000+i, MenuText(g_tiles[i].folder->name).c_str());
        AppendMenuW(menu, MF_POPUP, (UINT_PTR)groups, Tr(L"Move to group"));
        AppendMenuW(menu, MF_STRING, 91, Tr(L"Remove from group"));
    };
    if (tile.folder) {
        AppendMenuW(menu, MF_STRING, kOpen, Tr(L"Open"));
        AppendMenuW(menu, MF_STRING, kUngroup, Tr(L"Take the apps out"));
    } else if (useSelection) {
        AppendMenuW(menu, MF_STRING, kOpen, CountLabel(L"Open {0}", g_appSelection.size()).c_str());
        appendGroups();
        AppendMenuW(menu, MF_STRING, kRemove, Tr(PinnedOnly() ? L"Unpin from this grid" : L"Remove from this grid"));
    } else {
        paths = GetAppPaths(*tile.app);
        key = AppKey(*tile.app);
        AppendMenuW(menu, MF_STRING, kOpen, Tr(L"Open"));
        appendGroups();
        AppendMenuW(menu, MF_STRING, 95, Tr(L"Rename"));
        if(g_displayNames.count(key)&&!g_displayNames[key].custom.empty())AppendMenuW(menu,MF_STRING,96,Tr(L"Restore original name"));
        AppendMenuW(menu, MF_STRING, 92, Tr(L"Edit entry…"));
        if (tile.app->manual) AppendMenuW(menu, MF_STRING, 93, Tr(L"Delete manual entry"));
        if (!tile.app->websiteUrl.empty() && !g_customIcons.count(key))
            AppendMenuW(menu, MF_STRING, 94, Tr(L"Refresh website icon"));
        // Anything with a launchable file can be elevated: a classic .exe,
        // any Start-menu shortcut, or a packaged app through a temporary
        // .lnk aimed at its AppsFolder target.
        if (!paths.shortcut.empty() || !paths.folder.empty() ||
            IsPackagedTarget(tile.app->target) ||
            (!paths.file.empty() && EndsWith(ToLower(paths.file), L".exe"))) {
            AppendMenuW(menu, MF_STRING, kRunAs, Tr(L"Run as administrator"));
        }
        AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
        AppendMenuW(menu,
                    MF_STRING | (paths.file.empty() && paths.folder.empty()
                                     ? MF_GRAYED
                                     : 0),
                    kFileLocation, Tr(L"Open file location"));
        AppendMenuW(menu, MF_STRING | (paths.shortcut.empty() ? MF_GRAYED : 0),
                    kShortcutLocation, Tr(L"Open shortcut location"));
        AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
        AppendMenuW(menu, MF_STRING, kChangeIcon, Tr(L"Change icon\u2026"));
        if (g_customIcons.count(key)) {
            AppendMenuW(menu, MF_STRING, kRestoreIcon,
                        Tr(L"Restore original icon"));
        }
        AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
        if (g_openFolder) {
            AppendMenuW(menu, MF_STRING, kOutOfFolder, Tr(L"Take out of folder"));
        }
        if (!PinnedOnly()) {
            AppendMenuW(menu, MF_STRING, kRemove, Tr(L"Remove from this grid"));
        } else if (g_pins.count(key)) {
            AppendMenuW(menu, MF_STRING, kRemove, Tr(L"Unpin from this grid"));
        } else {
            AppendMenuW(menu, MF_STRING, kPin, Tr(L"Pin to this grid"));
        }
        AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
        AppendMenuW(menu,
                    MF_STRING | (paths.shortcut.empty() && paths.file.empty()
                                     ? MF_GRAYED
                                     : 0),
                    kProperties, Tr(L"Properties"));
    }
    SetMenuDefaultItem(menu, kOpen, FALSE);
    g_modal = true;
    int command = TrackLaunchpadPopupMenu(
        menu, TPM_RETURNCMD | TPM_RIGHTBUTTON | TPM_NONOTIFY, pt.x, pt.y, hwnd,
        nullptr);
    g_modal = false;
    DestroyMenu(menu);

    int size = g_appsKey & 0xFFFF;
    if(tile.app&&(command==95||command==96)) {
        auto name=command==95?PromptGroupName(hwnd,tile.app->name,L"Application name",160):std::wstring();
        if(command==95&&name.empty())return;
        if(!SetDisplayName(tile.app,name))MessageBoxW(hwnd,Tr(L"Could not save entry."),Tr(L"Application name"),MB_OK|MB_ICONWARNING);
        g_labelCache.clear();Refresh();return;
    }
    if (tile.app && command==92) {EditEntry(hwnd,tile.app);return;}
    if (tile.app && command==93) {DeleteManualEntry(tile.app);return;}
    if (tile.app && command==94) {StartWebIconFetch(*tile.app,true,true);return;}
    if (g_settings.groups && tile.app && (command == 91 || (command >= 1000 && command < 1000 + (int)g_tiles.size()))) {
        if (g_layoutReadOnly || g_dataReadOnly || g_entriesReadOnly) return;
        auto destination = command == 91 ? InboxGroup() : g_tiles[command - 1000].folder;
        Tiles moving = useSelection ? SelectedTiles() : Tiles{tile};
        if (useSelection) {
            for (const auto& app : g_appSelection) {
                Tile item{app, nullptr};
                if (std::none_of(moving.begin(), moving.end(), [&](const Tile& current) {
                    return AppKey(*current.app) == AppKey(*app);
                })) moving.push_back(std::move(item));
            }
        }
        MoveAppsToGroup(moving, destination, destination ? (int)destination->apps.size() : 0);
        SaveTiles(); Refresh(); return;
    }
    const std::wstring& linkOrFile =
        paths.shortcut.empty() ? paths.file : paths.shortcut;
    switch (command) {
        case kOpen:
            if (useSelection) LaunchSelectedApps();
            else if (tile.folder) OpenFolder(tile.folder);
            else LaunchTarget(tile.app->target, nullptr, tile.app.get());
            break;
        case kUngroup:
            Ungroup(tile.folder);
            break;
        case kRunAs:
            // A Store app's .lnk brokers an elevated launch; the raw
            // AppsFolder target — with or without a real exe inside it — is
            // a silent no-op. Packaged apps go through the temporary .lnk.
            if (IsPackagedTarget(tile.app->target)) {
                ElevateViaTempLnk(tile.app->target);
            } else {
                LaunchTarget(linkOrFile, L"runas",tile.app.get());
            }
            break;
        case kFileLocation:
            if (paths.file.empty()) {
                LaunchTarget(paths.folder);
            } else {
                Close();
                ShowInFolder(paths.file);
            }
            break;
        case kShortcutLocation:
            Close();
            ShowInFolder(paths.shortcut);
            break;
        case kChangeIcon: {
            std::wstring picture;
            if (PickIconFile(hwnd, picture)) {
                if (SetCustomIcon(key, picture)) {
                    LoadIconFile(CustomIconPath(g_customIcons.at(key)), size, *tile.app, false);
                } else {
                    MessageBoxW(hwnd,Tr(L"Could not save the selected icon."),Tr(L"Change icon…"),MB_OK|MB_ICONWARNING);
                }
            }
            Render();
            break;
        }
        case kRestoreIcon:
            if (!SetCustomIcon(key, {})) {
                MessageBoxW(hwnd,Tr(L"Could not save the selected icon."),Tr(L"Restore original icon"),MB_OK|MB_ICONWARNING);
                break;
            }
            ReloadEntryIcon(size, *tile.app);
            if (tile.app->pixels.empty()) StartWebIconFetch(*tile.app,true);
            Render();
            break;
        case kOutOfFolder:
            TakeOutOfFolder(tile);
            break;
        case kRemove:
            if (useSelection) DeleteSelectedApps();
            else RemoveAppsFromGrid({tile.app});
            break;
        case kPin:
            if(g_settings.groups) {
                auto destination=InboxGroup();
                MoveAppToGroup(tile,destination,(int)destination->apps.size());
            } else {
                g_pins.insert(key);
                SavePins();
                g_hidden.erase(key);
                g_tiles.push_back(tile);
            }
            SaveTiles();
            Refresh();
            break;
        case kProperties:
            Close();
            SHObjectProperties(nullptr, SHOP_FILEPATH, linkOrFile.c_str(),
                               nullptr);
            break;
    }
}

// The menu key: the menu of the selected icon.
void ShowSelectedMenu() {
    if (g_drag.active) return;
    int index = g_selected;
    if (!g_appSelection.empty()) {
        const auto& items = Items();
        auto selected = std::find_if(items.begin(), items.end(), [](const Tile& tile) {
            return tile.app && AppSelected(*tile.app);
        });
        if (selected != items.end()) index = (int)(selected - items.begin());
    }
    if (index < 0 || index >= (int)Items().size()) return;
    Cell c = ItemCell(index);
    POINT pt{(LONG)c.icon.left, (LONG)c.icon.bottom};
    ClientToScreen(g_wnd, &pt);
    ShowTileMenu(g_wnd, index, pt);
}

// The "..." menu, below the button, or at the pointer after a right-click on
// empty space.
void ShowOptionsMenu(HWND hwnd, POINT pt) {
    enum { kOpenSettings = 1, kRestoreAll, kSort, kRestoreFirst = 100 };
    // Removed apps that are still installed, A to Z like the list.
    std::vector<std::shared_ptr<App>> removed;
    for (const auto& app : g_apps) {
        if (g_hidden.count(AppKey(*app)) && removed.size() < 500) {
            removed.push_back(app);
        }
    }
    HMENU menu = CreatePopupMenu();
    #ifdef SEAMLESS_PREVIEW
    AppendMenuW(menu, MF_STRING, 90, Tr(L"Exit preview"));
#endif
    AppendMenuW(menu, MF_STRING, kOpenSettings, Tr(L"Open settings"));
    AppendMenuW(menu, MF_STRING, 54, Tr(L"Add entry…"));
    AppendMenuW(menu, MF_STRING, 56, Tr(L"Create complete backup…"));
    AppendMenuW(menu, MF_STRING, 57, Tr(L"Import complete backup…"));
    if(g_settings.groups)AppendMenuW(menu,MF_STRING,55,Tr(g_groupEditMode?L"Finish group editing":L"Edit group layout"));
    if(g_groupEditMode)AppendMenuW(menu,MF_STRING,58,Tr(L"Cancel group editing"));
    if (g_settings.groups) AppendMenuW(menu, MF_STRING, 50, Tr(L"New group"));
    AppendMenuW(menu, MF_STRING, 51, Tr(L"Restore layout backup"));
    if(g_settings.groups&&GetFileAttributesW((GroupPositionFile()+L".bak").c_str())!=INVALID_FILE_ATTRIBUTES)
        AppendMenuW(menu,MF_STRING,59,Tr(L"Restore previous group geometry"));
    AppendMenuW(menu, MF_STRING, 52, Tr(L"Import layout file"));
    AppendMenuW(menu, MF_STRING, 53, Tr(L"Export layout file"));
    HMENU sub = CreatePopupMenu();
    for (int i = 0; i < (int)removed.size(); i++) {
        AppendMenuW(sub, MF_STRING, kRestoreFirst + i,
                    MenuText(removed[i]->name).c_str());
    }
    if (!removed.empty()) {
        AppendMenuW(sub, MF_SEPARATOR, 0, nullptr);
        AppendMenuW(sub, MF_STRING, kRestoreAll, Tr(L"Bring them all back"));
    }
    AppendMenuW(menu, MF_POPUP | (removed.empty() ? MF_GRAYED : 0),
                (UINT_PTR)sub, Tr(L"Bring back a removed app"));
    AppendMenuW(menu,
                MF_STRING | (!g_appsLoaded ? MF_GRAYED : 0),
                kSort, Tr(g_settings.groups ? L"Sort apps within groups" : L"Sort A to Z"));
    UINT flags = TPM_RETURNCMD | TPM_RIGHTBUTTON | TPM_NONOTIFY;
    g_modal = true;
    int command = TrackLaunchpadPopupMenu(menu, flags, pt.x, pt.y, hwnd, nullptr);
    g_modal = false;
    DestroyMenu(menu);

#ifdef SEAMLESS_PREVIEW
    if (command == 90) { if(ConfirmGroupEditClose())PostMessageW(hwnd, WM_APP_QUIT, 0, 0); return; }
#endif
    if(command==58){CancelGroupEdit();return;}
    if(g_groupEditMode&&command!=55)return;
    if (command == 50) { CreateGroup(hwnd); return; }
    if (command == 51 || command == 52) { ImportOrRestoreLayout(hwnd, command == 51); return; }
    if (command == 53) { ExportLayout(hwnd); return; }
    if (command == 59) { RestoreGroupGeometry(hwnd); return; }
    if (command == 54) { EditEntry(hwnd); return; }
    if (command == 55) { ToggleGroupEdit(); return; }
    if (command == 56 || command == 57) { ShowFullBackupDialog(hwnd,command==57); return; }
    if (command == kOpenSettings) {
        HideNow();
        OpenSettingsWindow();
    } else if (command == kRestoreAll ||
               (command >= kRestoreFirst &&
                command < kRestoreFirst + (int)removed.size())) {
        if (command == kRestoreAll) {
            for (const auto& app : removed) {
                g_hidden.erase(AppKey(*app));
            }
        } else {
            g_hidden.erase(AppKey(*removed[command - kRestoreFirst]));
        }
        ArrangeTiles(ArrangementLines(),true);  // Explicit restore puts them at the end.
        DropGoneFolder();
        SaveTiles();
        Refresh();
    } else if (command == kSort) {
        if (g_settings.groups) {
            for (auto& group : g_tiles) if (group.folder)
                std::stable_sort(group.folder->apps.begin(), group.folder->apps.end(), [](const Tile& a, const Tile& b) { return NameLess(a.app->name,b.app->name); });
            SaveTiles(); Refresh(); return;
        }
        g_modal = true;
        int answer = MessageBoxW(
            hwnd,
            Tr(L"Put every app back in A to Z order and out of its folder? Removed apps stay removed."),
            Tr(L"Sort A to Z"), MB_OKCANCEL | MB_ICONQUESTION);
        g_modal = false;
        if (g_shown && !g_closing) {
            ForceForeground(hwnd);
        }
        if (answer == IDOK) {
            std::vector<std::wstring> lines;
            for (const auto& key : g_hidden) {
                lines.push_back(L"hidden\t" + key);
            }
            ArrangeTiles(lines);
            DropGoneFolder();
            g_scroll = g_scrollTarget = g_groupPagePan = 0;
            SaveTiles();
            Refresh();
        }
    }
}

void QueryChanged() {
    const bool showingResults = !g_query.empty();
    if (g_showingSearchResults != showingResults) {
        g_showingSearchResults = showingResults;
        g_searchGridTransition = Animate() ? 0.0f : 1.0f;
    }
    UpdateFilter();
    g_hover = -1;
    g_selected = g_query.empty() || g_visible.empty() ? -1 : 0;
    g_scroll = g_scrollTarget = g_groupPagePan = 0;
    ComputeLayout();
    StartFrames();
    Render();
}

void DismissSearch() {
    CancelImeComposition(g_wnd);
    if (!g_query.empty()) EditPush(0);
    g_query.clear();
    g_searchActivated = false;
    g_clearHover = false;
    SetCaret(0, false);
    QueryChanged();
}

void PasteText() {
    if (!OpenClipboard(g_wnd)) {
        return;
    }
    if (HANDLE data = GetClipboardData(CF_UNICODETEXT)) {
        if (auto* text = (PCWSTR)GlobalLock(data)) {
            std::wstring line = text;
            GlobalUnlock(data);
            line = line.substr(0, line.find_first_of(L"\r\n"));
            // Tabs would split the saved folder name; typed ones never get in.
            std::erase_if(line, [](WCHAR c) { return c < 32; });
            EditPush(0);
            InsertAtCaret(line);
            EditDone();
        }
    }
    CloseClipboard();
    EditChanged();
}

void OnChar(WCHAR ch) {
    if(g_groupEditMode)return;
    if (g_openFolder && !g_folderRenaming) return;
    if (g_drag.active || g_drag.pending) {
        return;
    }
    switch (ch) {
        case 8: {  // Backspace: the selection, or a code point before the caret.
            size_t from, to;
            if (SearchSelection(&from, &to) || g_caret > 0) {
                EditPush(2);
                if (!DeleteSelection()) {
                    size_t prev = CaretStep(g_caret, -1);
                    EditText().erase(prev, g_caret - prev);
                    g_caret = g_anchor = prev;
                }
                EditChanged();
                EditDone();
            }
            TouchCaret();
            return;
        }
        case 0x7F: {  // Ctrl+Backspace: the word before the caret.
            size_t from, to;
            if (SearchSelection(&from, &to) || g_caret > 0) {
                EditPush(2);
                if (!DeleteSelection()) {
                    size_t start = WordStart(g_caret);
                    EditText().erase(start, g_caret - start);
                    g_caret = g_anchor = start;
                }
                EditChanged();
                EditDone();
            }
            TouchCaret();
            return;
        }
        case 0x01:  // Ctrl+A: select all.
            g_anchor = 0;
            g_caret = EditText().size();
            TouchCaret();
            Render();
            return;
        case 0x03:  // Ctrl+C
            CopySelection(false);
            return;
        case 0x18:  // Ctrl+X
            CopySelection(true);
            return;
        case 0x16:  // Ctrl+V
            PasteText();
            return;
        case 0x1A:  // Ctrl+Z: undo; Ctrl+Shift+Z: redo.
            if (GetKeyState(VK_SHIFT) < 0) {
                RedoEdit();
            } else {
                UndoEdit();
            }
            return;
        case 0x19:  // Ctrl+Y: redo.
            RedoEdit();
            return;
    }
    if (ch >= 32 && EditText().size() < MaxEditChars()) {
        EditPush(1);
        InsertAtCaret(std::wstring(1, ch));
        EditDone();
        EditChanged();
        // Typing is how the field opens now: there is no resting icon to
        // click, so a keystroke both fills and reveals the field.
        if (!g_openFolder) g_searchActivated = true;
    }
}

void MoveSelection(int delta) {
    const Tiles& items = Items();
    if (items.empty()) {
        return;
    }
    int count = (int)items.size();
    if (g_selected < 0) {
        g_selected = 0;
    } else {
        int next = g_selected + delta;
        if (next >= 0 && next < count) {
            g_selected = next;
        } else if (delta > 0 && delta == Columns()) {
            g_selected = count - 1;  // Down on the last incomplete row.
        }
    }
    EnsureVisible(g_selected);
    Render();
    StartFrames();
}

void MoveGroupSelection(WPARAM key) {
    if (g_visible.empty()) return;
    if (g_selected<0 || g_selected>=(int)g_groupCells.size()) {
        g_selected=0;
        for (int i=0;i<(int)g_groupCells.size();i++) {
            auto c=GroupCellAt(i);
            if (c.icon.left>=0 && c.icon.right<=g_layout.width && c.icon.top>=g_layout.viewTop && c.label.bottom<=g_layout.viewBottom) {g_selected=i;break;}
        }
    } else {
        auto current=g_groupCells[g_selected].icon;
        float cx=(current.left+current.right)/2, cy=(current.top+current.bottom)/2;
        float best=1.0e30f; int next=g_selected;
        for (int i=0;i<(int)g_groupCells.size();i++) if (i!=g_selected) {
            auto c=g_groupCells[i].icon;
            float dx=(c.left+c.right)/2-cx,dy=(c.top+c.bottom)/2-cy;
            float forward=key==VK_LEFT?-dx:key==VK_RIGHT?dx:key==VK_UP?-dy:dy;
            float cross=key==VK_LEFT||key==VK_RIGHT?std::abs(dy):std::abs(dx);
            float score=forward+cross*4;
            if (forward>0.5f && score<best) {best=score;next=i;}
        }
        g_selected=next;
    }
    EnsureVisible(g_selected); Render(); StartFrames();
}

void OnKeyDown(WPARAM key) {
    if(g_groupEditMode&&key==VK_RETURN){ToggleGroupEdit();return;}
    if(key==VK_ESCAPE&&g_groupEditMode) {
        if(g_groupGesture.group || g_groupMarquee.pending) {
            ResetGroupGesture(true);
            Refresh();
        } else if (!g_groupSelection.empty()) {
            g_groupSelection.clear();
            Render();
        } else CancelGroupEdit();
        return;
    }
    if(g_groupEditMode||g_batchLaunching)return;
    if (g_drag.active) {
        if (key == VK_ESCAPE) {
            EndDrag(false);
            if (GetCapture() == g_wnd) ReleaseCapture();
        }
        return;
    }
    if(key==VK_ESCAPE&&(!g_appSelection.empty()||g_selectionPointer.pending||g_selectionPointer.app)) {
        ClearAppSelection();
        g_pressed=-1;g_pressedEmpty=false;
        if(GetCapture()==g_wnd)ReleaseCapture();
        Render();return;
    }
    if (key == VK_ESCAPE && g_groupHeaderPressed) {
        CancelGroupPointer(); if (GetCapture() == g_wnd) ReleaseCapture(); Render(); return;
    }
    if (GroupedView() && (key==VK_LEFT || key==VK_RIGHT || key==VK_UP || key==VK_DOWN)) {
        MoveGroupSelection(key); return;
    }
    if (g_openFolder && g_folderRenaming && (key == VK_RETURN || key == VK_ESCAPE)) {
        g_folderRenaming = false;
        g_searchDrag = false;
        g_anchor = g_caret;
        SaveFolderName();
        Render();
        return;
    }
    if (key == VK_DELETE && !g_folderRenaming) {
        if (!g_appSelection.empty()) {
            DeleteSelectedApps();
            return;
        }
        if ((g_openFolder || g_query.empty()) && g_selected >= 0 && g_selected < (int)Items().size() &&
            Items()[g_selected].app) {
            RemoveAppsFromGrid({Items()[g_selected].app});
            return;
        }
    }
    if (key == VK_RETURN) {
        if (!g_appSelection.empty()) LaunchSelectedApps();
        else if (g_selected >= 0) ActivateItem(g_selected);
        else if (!g_openFolder && !g_query.empty() && !g_visible.empty()) ActivateItem(0);
        return;
    }
    if (g_openFolder && !g_folderRenaming) {
        switch (key) {
            case VK_LEFT: MoveSelection(-1); break;
            case VK_RIGHT: MoveSelection(1); break;
            case VK_UP: MoveSelection(-Columns()); break;
            case VK_DOWN: MoveSelection(Columns()); break;
            case VK_ESCAPE: break;
            case VK_APPS: ShowSelectedMenu(); break;
        }
        if (key != VK_ESCAPE) return;
    }
    switch (key) {
        case VK_ESCAPE:
            if (g_powerArmed >= 0) {
                DisarmPower();
                Render();
            } else if (g_openFolder) {
                CloseFolder();
            } else if (SearchExpanded()) {
                DismissSearch();
            } else {
                Close();
            }
            break;
        case VK_APPS:
            ShowSelectedMenu();
            break;
        case VK_LEFT:
            if (!EditText().empty()) {
                bool word = GetKeyState(VK_CONTROL) < 0;
                SetCaret(word ? WordStart(g_caret) : CaretStep(g_caret, -1),
                         GetKeyState(VK_SHIFT) < 0);
                Render();
            } else {
                MoveSelection(-1);
            }
            break;
        case VK_RIGHT:
            if (!EditText().empty()) {
                bool word = GetKeyState(VK_CONTROL) < 0;
                SetCaret(word ? WordEnd(g_caret) : CaretStep(g_caret, 1),
                         GetKeyState(VK_SHIFT) < 0);
                Render();
            } else {
                MoveSelection(1);
            }
            break;
        case VK_UP:
            MoveSelection(-Columns());
            break;
        case VK_DOWN:
            MoveSelection(Columns());
            break;
        case VK_PRIOR:
            if (!g_openFolder) {
                ScrollRows(-g_layout.visibleRows);
            }
            break;
        case VK_NEXT:
            if (!g_openFolder) {
                ScrollRows(g_layout.visibleRows);
            }
            break;
        case VK_HOME:
            if (!EditText().empty()) {
                SetCaret(0, GetKeyState(VK_SHIFT) < 0);
                Render();
            }
            break;
        case VK_END:
            if (!EditText().empty()) {
                SetCaret(EditText().size(), GetKeyState(VK_SHIFT) < 0);
                Render();
            }
            break;
        case VK_DELETE:
            if (!EditText().empty()) {
                size_t dfrom, dto;
                if (SearchSelection(&dfrom, &dto) || g_caret < EditText().size()) {
                    EditPush(2);
                    bool changed = DeleteSelection();
                    if (!changed) {
                        size_t next = CaretStep(g_caret, 1);
                        if (next != g_caret) {
                            EditText().erase(g_caret, next - g_caret);
                            changed = true;
                        }
                    }
                    if (changed) {
                        EditChanged();
                        EditDone();
                    }
                }
                TouchCaret();
            }
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Opening instead of the Windows Start
//
// Every way of opening Start (Windows key, Ctrl+Esc, the taskbar's Start
// button, WM_SYSCOMMAND/SC_TASKLIST from the Dock) goes through
// XamlLauncher::ShowStartView in twinui.pcshell.dll. The hook asks the mod's
// window to toggle and doesn't call the original, so the Windows Start never
// opens.

// Observed on Windows 11 build 26300: the Windows key and Ctrl+Esc arrive as
// method 1 with flag 8; WM_SYSCOMMAND/SC_TASKLIST (the Dock's Start button)
// also arrives as method 1, without that flag.
constexpr int kShowMethodKeyboard = 1;
constexpr int kShowFlagKeyboard = 0x8;

bool IsKeyboardShow(int showMethod, int showFlags) {
    return showMethod == kShowMethodKeyboard && (showFlags & kShowFlagKeyboard);
}

// Shift + Windows key
//
// Windows doesn't open Start for the Windows key pressed with Shift, so
// ShowStartView never sees it. A low-level keyboard hook on its own thread
// (the window thread can be busy drawing) watches for a Windows key press
// with Shift down during it, and no other key pressed in between (Shift+Win+S and
// the like stay Windows shortcuts). For the grid and the original Start, the
// window presses Ctrl+Esc once the keys are up, and the ShowStartView hook
// gives that one the Shift + Windows key action.
//
// Windows keeps keys typed in an administrator window from this hook, and
// drops the Ctrl+Esc it would press there. The optional helper
// (helper/launchpad-keys.cpp) runs as administrator with the same hook. While its
// window exists, this hook leaves the key to it, and the helper sends
// WM_APP_SHIFT_WIN with wParam 1, then presses Ctrl+Esc itself if the answer
// is 1.
constexpr WCHAR kHelperClass[] = L"RYJSLaunchpadKeys";

std::atomic<ULONGLONG> g_shiftWinUntil;
ULONGLONG g_shiftWinReleaseDeadline;
HANDLE g_keyThread;
DWORD g_keyThreadId;

LRESULT CALLBACK KeyboardHook(int code, WPARAM wParam, LPARAM lParam) {
    // Keyboards that send Shift+Win from one key (macro keys) let go of Shift
    // a moment before Win, so Shift counts if it was down at any point.
    static bool winDown, otherKey, withShift;
    if (code == HC_ACTION) {
        DWORD vk = ((KBDLLHOOKSTRUCT*)lParam)->vkCode;
        bool up = wParam == WM_KEYUP || wParam == WM_SYSKEYUP;
        bool shift = vk == VK_SHIFT || vk == VK_LSHIFT || vk == VK_RSHIFT;
        if (vk == VK_LWIN || vk == VK_RWIN) {
            if (!up && !winDown) {
                winDown = true;
                otherKey = false;
                withShift = GetAsyncKeyState(VK_SHIFT) < 0;
            } else if (up && winDown) {
                winDown = false;
                HWND wnd = g_wnd;
                if (!otherKey && withShift && wnd &&
                    !FindWindowW(kHelperClass, nullptr)) {
                    PostMessageW(wnd, WM_APP_SHIFT_WIN, 0, 0);
                } else if (!otherKey && !withShift && wnd &&
                           g_winKeyAction.load() == KeyAction::Grid) {
                    // A plain tap: Windows would open Start here, and the mod
                    // answers that in the ShowStartView hook. A second tap never
                    // reaches that hook, so the window decides by its own state
                    // whether this one closes the grid. The key only drives the
                    // grid when it is set to: on Start or on a program, Windows
                    // keeps this tap.
                    PostMessageW(wnd, WM_APP_WIN_TAP, 0, 0);
                }
            }
        } else if (shift && !up && winDown) {
            withShift = true;
        } else if (!up && !shift && winDown) {
            otherKey = true;
        }
    }
    return CallNextHookEx(nullptr, code, wParam, lParam);
}

DWORD WINAPI KeyThread(void*) {
    HHOOK hook = SetWindowsHookExW(WH_KEYBOARD_LL, KeyboardHook,
                                   GetModuleHandleW(nullptr), 0);
    if (!hook) {
        Wh_Log(L"Keyboard hook failed: %u", GetLastError());
        return 0;
    }
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
    }
    UnhookWindowsHookEx(hook);
    return 0;
}

bool OwnsTaskbar() {
#ifdef SEAMLESS_PREVIEW
    return true;
#else
    DWORD pid = 0;
    HWND tray = FindWindowW(L"Shell_TrayWnd", nullptr);
    return tray && GetWindowThreadProcessId(tray, &pid) &&
           pid == GetCurrentProcessId();
#endif
}

#ifndef SEAMLESS_PREVIEW
#include "launchpad_start_button.h"
#include "launchpad_start_xaml.h"
#endif

#ifndef SEAMLESS_PREVIEW
// The ExperienceToggleButton hooks install when the taskbar's XAML module is
// already mapped, or from the kernelbase LoadLibraryExW hook when it loads
// later. A ShowStartView request runs the same check as a last-resort
// self-heal, so no polling timer is needed.
void EnsureXamlStartHooks() {
    if (!GetModuleHandleW(L"Taskbar.View.dll") &&
        !GetModuleHandleW(L"ExplorerExtensions.dll"))
        return;
    if (HookXamlStartVisibility()) {
        const bool applied = Wh_ApplyHookOperations();
        Wh_Log(L"Deferred Start button hook application succeeded=%d", applied);
    }
}
#endif

using XamlLauncher_ShowStartView_t = HRESULT(WINAPI*)(void* pThis,
                                                      int showMethod,
                                                      int showFlags);
XamlLauncher_ShowStartView_t XamlLauncher_ShowStartView_Original;
HRESULT WINAPI XamlLauncher_ShowStartView_Hook(void* pThis,
                                               int showMethod,
                                               int showFlags) {
    Wh_Log(L"> method %d, flags %d", showMethod, showFlags);
    const bool keyboard = IsKeyboardShow(showMethod, showFlags);
    const bool shiftWin = keyboard && GetTickCount64() < g_shiftWinUntil.exchange(0);
    KeyAction action = shiftWin
                           ? g_shiftWinKeyAction.load()
                       : keyboard
                           ? g_winKeyAction.load()
                           : g_startButtonAction.load();
#ifndef SEAMLESS_PREVIEW
    // The keyboard path never reaches the toggle or pointer hooks, so the
    // show request is the only record of it in the state trace.
    {
        std::wstring note = std::wstring(L"show-") +
                            (shiftWin ? L"shiftwin" : keyboard ? L"key" : L"button") +
                            L"-m" + std::to_wstring(showMethod) +
                            L"-f" + std::to_wstring(showFlags) +
                            (action == KeyAction::Grid ? L"-grid"
                             : action == KeyAction::Start ? L"-start"
                                                          : L"-program");
        StartStateTrace(note.c_str());
    }
    // A mouse click reaches here even when the XAML hooks never installed.
    // Install them now so the latch this click left on the button is the
    // last one; every later click is intercepted inside OnToggle itself.
    if (action != KeyAction::Start && !keyboard) EnsureXamlStartHooks();
#endif
    HWND wnd = g_wnd;
    if (action == KeyAction::Start || !wnd) {
#ifndef SEAMLESS_PREVIEW
        g_startButtonReplacementActive = false;
        g_nativeStartPassthrough=true;
#endif
        const auto result = XamlLauncher_ShowStartView_Original(pThis, showMethod, showFlags);
#ifndef SEAMLESS_PREVIEW
        if (FAILED(result)) g_nativeStartPassthrough = false;
#endif
        return result;
    }
#ifndef SEAMLESS_PREVIEW
    g_nativeStartPassthrough=false;
    g_startButtonReplacementActive=true;
#endif
    PostMessageW(wnd, action == KeyAction::Program ? WM_APP_RUN
                                                   : WM_APP_TOGGLE,
                 !shiftWin && !keyboard, 0);
    return S_OK;
}

////////////////////////////////////////////////////////////////////////////////
// Windows


void OnAppsLoaded(AppList* list, int key) {
    if(g_groupEditMode){delete list;g_appsTime=0;return;}
    if (g_fullImportBusy) { delete list; return; }
    if (g_drag.active || g_modal || g_groupHeaderPressed || !g_appSelection.empty() || g_selectionPointer.pending) {
        // Not under a dragged icon or an open menu: read again on the next
        // open.
        delete list;
        g_appsTime = 0;
        return;
    }
    if (g_openFolder) {
        SaveFolderName();
        g_openFolder = nullptr;
    }
    g_apps = list ? std::move(*list) : AppList{};
    g_labelCache.clear();
    delete list;
    g_appsLoaded = true;
    g_appsKey = key;
    g_appsNormalized = g_settings.normalizeIcons;
    ApplyEntries(key & 0xFFFF);
    for (const auto& app : g_apps) if (!g_entries.count(AppKey(*app))) {
        app->websiteUrl=ResolveWebsiteUrl(*app);
        if (!app->websiteUrl.empty()) LoadWebsiteIcon(key & 0xFFFF,*app);
    }
    ApplyDisplayNames();
    LoadTiles();
    ApplyCustomIcons(key & 0xFFFF);
    for (const auto& app : g_apps)
        if (g_settings.webIconBackgroundFetch && !g_webStop && app->pixels.empty() &&
            !g_customIcons.count(AppKey(*app))) StartWebIconFetch(*app);
    if (key != AppsKey()) {
        // The settings changed while this list was read: the reload then
        // found the worker busy. It has posted and is about to end.
        WaitForSingleObject(g_worker, 1000);
        StartLoadingApps();
    }
    if (g_shown) {
        UpdateFilter();
        int count = (int)g_visible.size();
        if (g_selected >= count) {
            g_selected = count ? 0 : -1;
        }
        g_hover = g_pressed = -1;
        ComputeLayout();
        Render();
    }
}

// The tray icon (Look > Tray icon): click opens the settings, right-click shows a menu.
// It lives on the grid's window and thread, like the rest of Launchpad.
constexpr UINT WM_APP_TRAY = WM_APP + 21;
constexpr UINT kTrayId = 1;
static HICON g_trayIcon;
static bool g_trayShown;
static UINT g_taskbarCreated;

// The same supplied sheet as the window icon; a separate dot-grid vector was
// the old mark, not what the icon concept asked for.
static HICON MakeTrayIcon(int size) {
    return CreateAppIconHicon(size);
}

static void UpdateTrayIcon(HWND hwnd) {
    NOTIFYICONDATAW nid{sizeof(nid)};
    nid.hWnd = hwnd;
    nid.uID = kTrayId;
    if (!g_settings.trayIcon) {
        if (g_trayShown) Shell_NotifyIconW(NIM_DELETE, &nid);
        g_trayShown = false;
        return;
    }
    if (!g_trayIcon) g_trayIcon = MakeTrayIcon(GetSystemMetricsForDpi(SM_CXSMICON, GetDpiForSystem()));
    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_SHOWTIP;
    nid.uCallbackMessage = WM_APP_TRAY;
    nid.hIcon = g_trayIcon;
    wcscpy_s(nid.szTip, Tr(L"Lumiflow"));
    if (g_trayShown && Shell_NotifyIconW(NIM_MODIFY, &nid)) return;
    g_trayShown = Shell_NotifyIconW(NIM_ADD, &nid) != FALSE;
    if (g_trayShown) {
        nid.uVersion = NOTIFYICON_VERSION_4;
        Shell_NotifyIconW(NIM_SETVERSION, &nid);
    }
}

static void RemoveTrayIcon(HWND hwnd) {
    if (g_trayShown) {
        NOTIFYICONDATAW nid{sizeof(nid)};
        nid.hWnd = hwnd;
        nid.uID = kTrayId;
        Shell_NotifyIconW(NIM_DELETE, &nid);
        g_trayShown = false;
    }
    if (g_trayIcon) {
        DestroyIcon(g_trayIcon);
        g_trayIcon = nullptr;
    }
}

static void OnTrayMessage(HWND hwnd, LPARAM lParam) {
    switch (LOWORD(lParam)) {
        case NIN_SELECT:
        case NIN_KEYSELECT:
            OpenSettingsWindow();
            break;
        case WM_CONTEXTMENU: {
            HMENU menu = CreatePopupMenu();
            AppendMenuW(menu, MF_STRING, 1, Tr(L"Settings"));
            AppendMenuW(menu, MF_STRING, 2, Tr(L"Open Lumiflow"));
#ifdef SEAMLESS_PREVIEW
            AppendMenuW(menu, MF_STRING, 90, Tr(L"Exit preview"));
#endif
            SetMenuDefaultItem(menu, 1, FALSE);
            POINT pt;
            GetCursorPos(&pt);
            SetForegroundWindow(hwnd);
            int cmd = TrackLaunchpadPopupMenu(menu, TPM_RETURNCMD | TPM_NONOTIFY | TPM_RIGHTBUTTON, pt.x, pt.y, hwnd, nullptr);
            DestroyMenu(menu);
#ifdef SEAMLESS_PREVIEW
            if (cmd == 90 && ConfirmGroupEditClose()) PostMessageW(hwnd, WM_APP_QUIT, 0, 0);
#endif
            if (cmd == 1) OpenSettingsWindow();
            if (cmd == 2) PostMessageW(hwnd, WM_APP_TOGGLE, 0, 0);
            break;
        }
    }
}

#ifndef SEAMLESS_PREVIEW
void NormalizeMainPointer(HWND hwnd, int& x, int& y) {
    RECT rc{};
    if (!GetClientRect(hwnd, &rc) || rc.right <= 0 || rc.bottom <= 0 ||
        g_layout.width <= 0 || g_layout.height <= 0) return;
    // The injected window can briefly expose a DWM-scaled client while the
    // layout is already in physical pixels. Map the message once so the hit
    // test follows the tile that is actually painted under the cursor.
    if (rc.right != g_layout.width) x = (int)std::lround((float)x * g_layout.width / rc.right);
    if (rc.bottom != g_layout.height) y = (int)std::lround((float)y * g_layout.height / rc.bottom);
}
#define NORMALIZE_POINTER(hwnd,x,y) NormalizeMainPointer(hwnd,x,y)
#else
#define NORMALIZE_POINTER(hwnd,x,y) ((void)0)
#endif

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (g_taskbarCreated && msg == g_taskbarCreated) {
        g_trayShown = false;  // The new taskbar has no icons yet.
        UpdateTrayIcon(hwnd);
        return 0;
    }
    switch (msg) {
        case WM_APP + 20: OpenSettingsWindow(); return 0;
        case WM_APP + 62:
            if (g_shown) {
                if(!ConfirmGroupEditClose())return 0;
                auto query = g_query;
                bool searchActivated = g_searchActivated;
                HideNow();
                Open();
                g_query = std::move(query);
                g_searchActivated = searchActivated;
                // Open() starts the field collapsed; a reload keeps the text,
                // so the field starts where that text says it belongs instead
                // of folding up and growing again.
                g_searchExpand = SearchExpanded() ? 1.f : 0.f;
                g_caret = g_anchor = g_query.size();
                UpdateFilter();
                ComputeLayout();
                Render();
            }
            return 0;
#ifdef SEAMLESS_PREVIEW
        case WM_APP + 60: Preview_RunSmoke(hwnd); return 0;
        case WM_APP + 61: Open(); return 0;
#endif
        case WM_APP_TRAY: OnTrayMessage(hwnd, lParam); return 0;
        case WM_APP_TOGGLE:
#ifndef SEAMLESS_PREVIEW
            ResetTaskbarStartButtons(true);
#endif
            // The keyboard hook reports the same Windows key tap that produced
            // this request, so only the first of the pair toggles.
            if (!FirstOfKeystroke()) {
                return 0;
            }
            // Clicking the taskbar's Start button while this one is open first
            // takes the focus (which closes it) and then asks for Start: that
            // click was meant to close it.
            if ((!g_settings.iconFlyIn || wParam) && (!g_shown || g_closing) &&
                Now() - g_lastClosed < 0.5) {
                if (g_settings.iconFlyIn) g_lastClosed = 0;
                return 0;
            }
            Toggle();
            return 0;

        case WM_APP_WIN_TAP:
            // A lone Windows key tap. The grid follows the key itself rather
            // than Windows' idea of Start: once the mod answered the first
            // show, Windows believes Start is up and says nothing on the next
            // tap, so the key is the only reliable source for closing.
            if (!FirstOfKeystroke()) {
                return 0;
            }
#ifndef SEAMLESS_PREVIEW
            ResetTaskbarStartButtons(true);
#endif
            Toggle();
            return 0;

        case WM_APP_APPS:
            OnAppsLoaded((AppList*)lParam, (int)wParam);
            return 0;
        case kWebIconReady: OnWebIconReady((WebIconResult*)lParam); return 0;
        case kFullBackupDone: OnFullBackupDone(hwnd); return 0;

        case WM_APP_RELOAD:
            if (g_fullImportBusy || !ConfirmGroupEditClose()) return 0;
            KillTimer(hwnd, kTimerShiftWin);
            g_shiftWinUntil = 0;
            if (g_shown) HideNow(); // Save an edited folder using the previous layout mode.
            LoadSettings();
            g_labelFormat.Reset();
            UpdateTrayIcon(hwnd);
            if (g_shown) {
                HideNow();
            }
            if (g_appsKey != AppsKey() ||
                g_appsNormalized != g_settings.normalizeIcons) {
                StartLoadingApps();
            } else if (g_appsLoaded) {
                LoadTiles();
            }
            return 0;

        case WM_APP_RUN:
            if (g_fullImportBusy || !ConfirmGroupEditClose()) return 0;
#ifndef SEAMLESS_PREVIEW
            ResetTaskbarStartButtons(true);
#endif
            if (g_shown) {
                HideNow();
            }
            RunProgram();
            return 0;

        case WM_APP_SHIFT_WIN:
            // Explorer runs this mod in each of its processes; only the
            // taskbar's one answers the key.
            if (!OwnsTaskbar()) {
                return 0;
            }
            if (g_shiftWinKeyAction == KeyAction::Program) {
                PostMessageW(hwnd, WM_APP_RUN, 0, 0);
                return 0;
            }
            if (g_shown && g_shiftWinKeyAction == KeyAction::Start) {
                if(!ConfirmGroupEditClose())return 0;
                // Native Start owns the button state for this handoff.
                HideNow(false);
            }
            if (wParam == 1) {
                // The helper presses Ctrl+Esc once the keys are up.
                g_shiftWinUntil = GetTickCount64() + 3000;
                return 1;
            }
            // The grid and the original Start open through Ctrl+Esc, once
            // Shift is up (with Shift it's Task Manager): only a Start that
            // Windows opens may take the foreground. SC_TASKLIST to the
            // taskbar does nothing here.
            g_shiftWinReleaseDeadline = GetTickCount64() + 3000;
            SetTimer(hwnd, kTimerShiftWin, 20, nullptr);
            return 0;

        case kPowerUpdateStatusReady:
            if (wParam <= static_cast<WPARAM>(PowerUpdateStatus::RestartRequired)) {
                const auto status = static_cast<PowerUpdateStatus>(wParam);
                if (status != PowerUpdateStatus::Unknown && status != g_powerUpdateStatus) {
                    g_powerUpdateStatus = status;
                    if (g_shown && !g_closing) Render();
                }
            }
            return 0;

        case WM_APP_QUIT:
#ifndef SEAMLESS_PREVIEW
            StopPowerUpdateProbe();
#endif
            RemoveTrayIcon(hwnd);
            DestroyWindow(hwnd);
            return 0;

        case WM_TIMER:
            switch (wParam) {
                case kTimerFrame:
                    OnFrame();
                    break;
                case kTimerShiftWin:
                    if (GetTickCount64() >= g_shiftWinReleaseDeadline) {
                        KillTimer(hwnd, kTimerShiftWin);
                    } else if (GetAsyncKeyState(VK_SHIFT) >= 0 &&
                               GetAsyncKeyState(VK_LWIN) >= 0 &&
                               GetAsyncKeyState(VK_RWIN) >= 0 &&
                               GetAsyncKeyState(VK_CONTROL) >= 0 &&
                               GetAsyncKeyState(VK_MENU) >= 0) {
                        KillTimer(hwnd, kTimerShiftWin);
                        g_shiftWinUntil = GetTickCount64() + 1000;
                        INPUT keys[4] = {};
                        WORD vks[4] = {VK_CONTROL, VK_ESCAPE, VK_ESCAPE,
                                       VK_CONTROL};
                        for (int i = 0; i < 4; i++) {
                            keys[i].type = INPUT_KEYBOARD;
                            keys[i].ki.wVk = vks[i];
                            keys[i].ki.dwFlags = i < 2 ? 0 : KEYEVENTF_KEYUP;
                        }
                        if (SendInput(4, keys, sizeof(INPUT)) != 4) g_shiftWinUntil = 0;
                    }
                    break;
#ifndef SEAMLESS_PREVIEW
                case kTimerPowerUpdate:
                    if (g_shown && !g_closing) RefreshPowerUpdateStatus();
                    break;
#endif
                case kTimerPowerArm:
                    DisarmPower();
                    Render();
                    break;
                case kTimerCaret:
                    // The search caret blinks by plain redraws.
                    if (g_shown && (g_openFolder ? g_folderRenaming : SearchExpanded())) {
                        Render();
                    }
                    break;
                case kTimerFirstLoad:
                    // Only in the process that owns the taskbar: short-lived
                    // explorer.exe processes (explorer.exe /select,...) get
                    // the mod too, and exit before they'd need the list.
                    if (OwnsTaskbar()) {
                        KillTimer(hwnd, kTimerFirstLoad);
                        if (!g_appsLoaded) {
                            StartLoadingApps();
                        }
                    }
                    break;
            }
            return 0;

        case WM_ACTIVATE:
#ifdef SEAMLESS_PREVIEW
            if(Preview::uiCheck)return 0;
#endif
            if (LOWORD(wParam) == WA_INACTIVE && g_shown && !g_closing &&
                !g_modal) {
                g_lastClosed = Now();
                Close();
            }
            return 0;

        case WM_SETCURSOR:
            if (LOWORD(lParam) == HTCLIENT) {
                POINT pt{};
                GetCursorPos(&pt);
                ScreenToClient(hwnd, &pt);
                if (GroupEditButtonAt(pt.x, pt.y) >= 0 || (!g_openFolder && PowerAt(pt.x, pt.y) >= 0)) {
                    SetCursor(LoadCursorW(nullptr, IDC_HAND));
                    return TRUE;
                }
                if ((g_groupGesture.group && g_groupGesture.resize) || GroupResizeAt(pt.x, pt.y) >= 0) {
                    SetCursor(LoadCursorW(nullptr,IDC_SIZENWSE));return TRUE;
                }
            }
            break;
        case WM_MOUSEMOVE: {
            if (!g_trackingMouse) {
                TRACKMOUSEEVENT tme{sizeof(tme), TME_LEAVE, hwnd, 0};
                g_trackingMouse = TrackMouseEvent(&tme);
            }
            int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
            NORMALIZE_POINTER(hwnd, x, y);
            const int previousEditHover = GroupEditHover();
            bool pointerChanged=!g_glassPointer||g_glassPointer->x!=x||g_glassPointer->y!=y;
            g_glassPointer=D2D1::Point2F((float)x,(float)y);
            auto bar=ScrollThumb();
            bool scrollHover=SelectionHasArea(bar)&&InButton(bar,(float)x,(float)y);
            if(std::exchange(g_scrollHover,scrollHover)!=scrollHover)Render();
            if (GroupPointerMove(x,y)||MoveAppSelection(x,y)) return 0;
            UpdateGroupResizeHover(x,y);
            if (pointerChanged && (previousEditHover >= 0 || GroupEditHover() >= 0)) StartFrames();
            int batchHover=BatchActionAt(x,y);
            if(batchHover!=g_batchHover){g_batchHover=batchHover;StartFrames();}
            if (g_searchDrag) {
                SetCaret(CaretAtPoint((float)x), true);
                Render();
                return 0;
            }
            // A drag is real past the system drag threshold only.
            if (g_drag.pending && !g_drag.active &&
                (std::abs(x - g_drag.start.x) >
                     GetSystemMetrics(SM_CXDRAG) ||
                 std::abs(y - g_drag.start.y) >
                     GetSystemMetrics(SM_CYDRAG))) {
                StartDrag();
            }
            if (g_drag.active) {
                DragMove(x, y);
                return 0;
            }
            bool titleHover = g_openFolder && InRect(FolderTitlePill(), (float)x, (float)y);
            if (titleHover != g_folderTitleHover) {
                g_folderTitleHover = titleHover;
                Render();
            }
            int hover = ItemAt(x, y);
            bool clearHover = !g_openFolder && !g_query.empty() &&
                              InButton(g_layout.clearButton, (float)x, (float)y);
            bool searchHover = !g_openFolder && !clearHover &&
                               g_searchExpand > 0.02f &&
                               InRect(g_layout.search, (float)x, (float)y);
            const bool insidePower = !g_openFolder &&
                                     InRect(g_layout.powerBar, (float)x, (float)y);
            g_powerPointerInside = insidePower;
            if (insidePower) g_powerPointerX = (float)x;
            int powerHover = g_openFolder ? -1 : PowerAt(x, y);
            if (hover != g_hover ||
                clearHover != g_clearHover ||
                searchHover != g_searchHover ||
                powerHover != g_powerHover) {
                g_hover = hover;
                g_clearHover = clearHover;
                g_searchHover = searchHover;
                g_powerHover = powerHover;
                StartFrames();
            }
            if (pointerChanged && (powerHover >= 0 || insidePower || (!g_groupEditMode &&
                (g_glassClaimed || g_openFolder || (y >= g_layout.viewTop && y < g_layout.viewBottom)))))
                StartFrames();
            return 0;
        }

        case WM_MOUSELEAVE:
            g_glassPointer.reset();
            g_powerPointerInside = false;
            if(std::exchange(g_scrollHover,false))Render();
            g_folderTitleHover = false;
            g_batchHover=-1;UpdateGroupResizeHover(-1,-1);StartFrames();
            g_trackingMouse = false;
            if (g_hover >= 0 || g_clearHover || g_searchHover ||
                g_powerHover >= 0) {
                g_hover = -1;
                g_clearHover = false;
                g_searchHover = false;
                g_powerHover = -1;
                StartFrames();
                Render();
                StartFrames();
            }
            return 0;

        case WM_LBUTTONDOWN: {
            CancelImeComposition(hwnd);
            if (g_modal||g_batchLaunching) { g_pressed=-1; g_pressedEmpty=false; return 0; }
            int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
            NORMALIZE_POINTER(hwnd, x, y);
            if (g_closing) {
                return 0;  // A click during the closing fade does nothing.
            }
            if(int action=GroupEditButtonAt(x,y);action>=0) {
                g_glassPointer = D2D1::Point2F((float)x, (float)y);
                g_groupEditButton=action;SetCapture(hwnd);Render();return 0;
            }
            if(ToolBarConsumes(x,y)) {
                g_batchPressed=BatchActionAt(x,y);
                if(g_batchPressed>=0)SetCapture(hwnd);
                Render();return 0;
            }
            if (GroupPointerDown(hwnd,x,y,(wParam & MK_CONTROL) != 0)) return 0;
            if (!g_openFolder) {
                int power = PowerAt(x, y);
                if (power >= 0) {
                    g_powerHover = power;
                    g_powerPressed = g_layout.powerItems[power];
                    g_glassPointer = D2D1::Point2F((float)x, (float)y);
                    SetCapture(hwnd);
                    Render();
                    return 0;
                }
                if (g_powerArmed >= 0) {
                    DisarmPower();  // A click elsewhere cancels the prompt.
                }
                if (!g_layout.powerItems.empty() && InRect(g_layout.powerBar, (float)x, (float)y)) {
                    Render();
                    return 0;
                }
                if (!g_query.empty() &&
                    InButton(g_layout.clearButton, (float)x, (float)y)) {
                    EditPush(0);  // Ctrl+Z brings the cleared search back.
                    g_query.clear();
                    g_searchActivated = true;
                    g_caret = g_anchor = 0;
                    g_clearHover = false;
                    QueryChanged();
                    return 0;
                }
                int dot = PageDotAt(x, y);
                if (dot >= 0) {
                    float target=dot*(float)g_layout.width;
                    if(g_scrollTarget!=target)g_groupPagePan=0;
                    g_scrollTarget=target;
                    StartFrames();
                    return 0;
                }
            }
            if(g_groupEditMode)return 0;
            // The resting field is invisible, so it also stops taking clicks:
            // only a field that has opened (typing or a kept-activated clear)
            // takes pointer input. Folder renaming keeps its own pill.
            bool editHit = (g_openFolder || g_searchExpand > 0.02f) &&
                           InRect(g_openFolder ? FolderTitlePill() : g_layout.search,
                                  (float)x, (float)y);
            if (g_openFolder && g_folderRenaming && !editHit) {
                g_folderRenaming = false;
                SaveFolderName();
            }
            if (editHit) {
                ClearAppSelection();
                if (g_openFolder) {
                    g_folderRenaming = true;
                } else {
                    g_searchActivated = true;
                    if (g_query.empty()) {
                        g_selected = -1;
                        LayoutSearch();
                        StartFrames();
                    }
                }
                // A click in the search field, or in an open folder's name,
                // places the caret; holding the button and moving selects
                // text. A second click on the same spot selects the word.
                UINT64 tick = GetTickCount64();
                bool dbl = tick - g_lastSearchClick <=
                               GetDoubleClickTime() &&
                           std::abs(x - g_lastSearchX) <=
                               4 * g_layout.scale;
                g_lastSearchClick = tick;
                g_lastSearchX = x;
                if (dbl) {
                    size_t pos = CaretAtPoint((float)x);
                    g_anchor = WordStart(pos);
                    g_caret = WordEnd(pos);
                } else {
                    SetCaret(CaretAtPoint((float)x), false);
                }
                g_searchDrag = true;
                SetCapture(hwnd);
                Render();
                return 0;
            }
            if (g_anchor != g_caret) {
                g_anchor = g_caret;  // A click elsewhere drops the selection.
            }
            SetCapture(hwnd);
            g_pressed = ItemAt(x, y);
            g_pressedEmpty = g_pressed < 0 && !ClickKeepsOpen(x, y);
            if(g_pressed>=0&&Items()[g_pressed].app&&((wParam&MK_CONTROL)||!g_appSelection.empty()))
                g_selectionPointer.app=Items()[g_pressed].app;
            else if(g_pressed<0&&(!g_openFolder||InRect(GetFolderLayout().panel,(float)x,(float)y))) {
                g_selectionPointer.base=g_appSelection;
                g_selectionPointer.baseKeys=g_selectedAppKeys;
                g_selectionPointer.start=g_selectionPointer.end={x,y};
                g_selectionPointer.pending=true;
            }
            g_drag.pending = g_pressed >= 0 && !(wParam & MK_CONTROL) && g_query.empty();
            g_drag.pressTime = Now();
            g_drag.start = {x, y};
            if (g_drag.pending) {
                StartFrames();  // Ticks the hold time toward the edit mode.
            }
            Render();
            return 0;
        }

        case WM_LBUTTONUP: {
            int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
            NORMALIZE_POINTER(hwnd, x, y);
            if(g_groupEditButton>=0) {
                int action=std::exchange(g_groupEditButton,-1);
                if(GetCapture()==hwnd)ReleaseCapture();
                if(action==GroupEditButtonAt(x,y)) {
                    if(action==0)CancelGroupEdit();
                    else if(action==1)EqualizeGroupSpacing();
                    else FinishGroupEdit();
                } else Render();
                return 0;
            }
            if(g_groupGesture.group || g_groupMarquee.pending)FreeGroupPointerMove(x,y);
            if (GroupPointerUp(hwnd)) return 0;
            if(g_batchPressed>=0) {
                int action=std::exchange(g_batchPressed,-1);
                if(GetCapture()==hwnd)ReleaseCapture();
                if(action==BatchActionAt(x,y)) {
                    if(action==0)ClearAppSelection();
                    else if(action==2)DeleteSelectedApps();
                    else LaunchSelectedApps();
                }
                Render();return 0;
            }
            if(g_selectionPointer.pending)MoveAppSelection(x,y);
            auto selection=std::exchange(g_selectionPointer,{});
            if(selection.active||selection.app) {
                g_pressed=-1;g_pressedEmpty=false;
                if(GetCapture()==hwnd)ReleaseCapture();
                int index=ItemAt(x,y);
                if(selection.app&&index>=0&&Items()[index].app==selection.app)ToggleAppSelection(selection.app);
                Render();return 0;
            }
            g_searchDrag = false;
            bool dragged = g_drag.active;
            // ReleaseCapture fires WM_CAPTURECHANGED synchronously, and that
            // handler clears the press; snapshot it before the capture goes.
            int powerPressed = g_powerPressed;
            int pressed = std::exchange(g_pressed,-1);
            bool pressedEmpty = std::exchange(g_pressedEmpty,false);
            if (dragged) DragMove(x, y);
            EndDrag(true);  // Before the capture goes, which would undo it.
            if (GetCapture() == hwnd) {
                ReleaseCapture();
            }
            g_powerPressed = -1;
            if (powerPressed >= 0) {
                const int slot = PowerAt(x, y);
                if (slot >= 0 && g_layout.powerItems[slot] == powerPressed) {
                    PowerClick(slot);  // May hide the grid.
                }
                Render();
                return 0;
            }
            if (dragged) {
                return 0;
            }
            int item = ItemAt(x, y);
            if (pressed >= 0 && item == pressed) {
                ActivateItem(item);
            } else if (pressedEmpty && item < 0 && g_appSelection.empty() && !ClickKeepsOpen(x, y)) {
                if (g_openFolder) {
                    CloseFolder();
                } else if (SearchExpanded()) {
                    DismissSearch();
                } else {
                    Close();
                }
            } else {
                Render();
            }
            return 0;
        }

        case WM_CAPTURECHANGED: {
            if(g_selectionPointer.active) {
                g_appSelection=std::move(g_selectionPointer.base);SelectionChanged();
            }
            g_selectionPointer={};
            const bool redrawButtons = g_groupEditButton >= 0 || g_powerPressed >= 0;
            g_pressed=-1;g_pressedEmpty=false;g_batchPressed=-1;g_groupEditButton=-1;
            CancelGroupPointer();
            EndDrag(false);  // Another window took the mouse.
            g_searchDrag = false;
            g_powerPressed = -1;
            if (redrawButtons) StartFrames();
            return 0;
        }

        case WM_RBUTTONUP: {
            if (g_drag.active||g_groupGesture.group||g_groupMarquee.pending||g_selectionPointer.pending||g_batchLaunching) {
                return 0;
            }
            int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
            NORMALIZE_POINTER(hwnd, x, y);
            if(ToolBarConsumes(x,y))return 0;
            POINT pt{x, y};
            ClientToScreen(hwnd, &pt);
            if (!g_openFolder && !g_groupEditMode) {
                int slot = PowerAt(x, y);
                if (slot >= 0 && IsUpdatePowerAction(static_cast<PowerAction>(g_layout.powerItems[slot]))) {
                    ShowPowerUpdateMenu(hwnd, pt);
                    return 0;
                }
            }
            int item = ItemAt(x, y);
            int group = GroupAtPoint(x,y);
            if (group >= 0 && (item < 0 || g_groupEditMode)) {
                ShowGroupMenu(hwnd,g_groupGeometry[group].group,pt);
            } else if (item >= 0) {
                ShowTileMenu(hwnd, item, pt);
            } else if (!g_openFolder) {
                ShowOptionsMenu(hwnd, pt);
            }
            return 0;
        }

        case WM_MOUSEWHEEL:
        case WM_MOUSEHWHEEL: {
            const bool vertical = msg == WM_MOUSEWHEEL;
            const bool shift = (GET_KEYSTATE_WPARAM(wParam) & MK_SHIFT) != 0;
            if (vertical && !shift && !g_openFolder &&
                ScrollFreeGroup(hwnd, lParam, GET_WHEEL_DELTA_WPARAM(wParam))) return 0;
            g_groupWheelTarget.reset();
            g_groupWheelRemainder = 0;
            if (vertical && shift && !g_openFolder && g_settings.pages) {
                int page=std::max(0,(int)std::lround(g_scroll/std::max(1,g_layout.width)));
                float limit=MaxPagePan(page);
                float next=std::clamp(g_groupPagePan-GET_WHEEL_DELTA_WPARAM(wParam)*
                    g_layout.cellHeight/WHEEL_DELTA,0.f,limit);
                if(next!=g_groupPagePan) {
                    g_groupPagePan=next;
                    if(g_groupGesture.group)FreeGroupPointerMove(g_groupGesture.at.x,g_groupGesture.at.y);
                    Render();
                    return 0;
                }
            }
            if (g_openFolder) {
                if (msg == WM_MOUSEWHEEL) {
                    g_wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
                    if (std::abs(g_wheelDelta) >= WHEEL_DELTA) {
                        FolderLayout F = GetFolderLayout();
                        g_folderRow =
                            std::clamp(g_folderRow - g_wheelDelta / WHEEL_DELTA,
                                       0, F.rows - F.visibleRows);
                        g_wheelDelta %= WHEEL_DELTA;
                        Render();
                    }
                }
                return 0;
            }
            // Sideways (tilt wheel, touchpad swipe) only flips pages.
            if (msg == WM_MOUSEHWHEEL && !g_settings.pages) {
                return 0;
            }
            g_wheelDelta += msg == WM_MOUSEWHEEL
                                ? GET_WHEEL_DELTA_WPARAM(wParam)
                                : -GET_WHEEL_DELTA_WPARAM(wParam);
            if (std::abs(g_wheelDelta) >= WHEEL_DELTA) {
                ScrollRows(-g_wheelDelta / WHEEL_DELTA);
                g_wheelDelta %= WHEEL_DELTA;
            }
            return 0;
        }

        case WM_IME_SETCONTEXT:
            return DefWindowProcW(hwnd, msg, wParam, lParam & ~static_cast<LPARAM>(ISC_SHOWUICOMPOSITIONWINDOW));

        case WM_IME_STARTCOMPOSITION:
            if (!CanEditWithIme()) break;
            g_imeComposition = {true, {}, 0};
            RevealImeSearch();
            TouchCaret();
            PositionIme(hwnd);
            Render();
            return 0;

        case WM_IME_ENDCOMPOSITION:
            if (g_imeComposition.active) {
                g_imeComposition = {};
                TouchCaret();
                if (g_shown && !g_closing) Render();
            }
            return 0;

        case WM_IME_COMPOSITION: {
            if (!CanEditWithIme()) break;
            const bool hasResult = (lParam & GCS_RESULTSTR) != 0;
            const bool hasComposition = (lParam & (GCS_COMPSTR | GCS_CURSORPOS | GCS_COMPATTR | GCS_COMPCLAUSE)) != 0;
            if (!hasResult && !hasComposition) {
                g_imeComposition = {};
                Render();
                return 0;
            }
            RevealImeSearch();
            HIMC ime = ImmGetContext(hwnd);
            auto committed = hasResult ? ReadImeString(ime, GCS_RESULTSTR) : std::wstring{};
            if (hasResult) g_imeComposition = {};
            if (hasComposition) {
                g_imeComposition.active = true;
                if (lParam & GCS_COMPSTR) g_imeComposition.text = ReadImeString(ime, GCS_COMPSTR);
                const LONG cursor = ime ? ImmGetCompositionStringW(ime, GCS_CURSORPOS, nullptr, 0) : -1;
                g_imeComposition.cursor = cursor < 0 ? g_imeComposition.text.size()
                    : std::min((size_t)cursor, g_imeComposition.text.size());
            }
            if (ime) ImmReleaseContext(hwnd, ime);
            std::erase_if(committed, [](WCHAR c) { return c < 32; });
            if (!committed.empty()) {
                EditPush(0);
                InsertAtCaret(committed);
                EditDone();
                EditChanged();
            } else {
                TouchCaret();
                Render();
            }
            PositionIme(hwnd);
            return 0;
        }

        case WM_IME_REQUEST:
            if (wParam == IMR_QUERYCHARPOSITION &&
                QueryImePosition(hwnd, reinterpret_cast<IMECHARPOSITION*>(lParam))) return TRUE;
            if (wParam == IMR_COMPOSITIONWINDOW && lParam && CanEditWithIme() &&
                (g_openFolder || SearchExpanded())) {
                const auto view = GetEditView(g_openFolder != nullptr);
                auto& form = *reinterpret_cast<COMPOSITIONFORM*>(lParam);
                form = {};
                form.dwStyle = CFS_POINT;
                form.ptCurrentPos = ImePoint(view, view.caret);
                return TRUE;
            }
            break;

        case WM_IME_NOTIFY:
            if ((wParam == IMN_OPENCANDIDATE || wParam == IMN_CHANGECANDIDATE) && CanEditWithIme()) {
                if (!g_openFolder && !SearchExpanded()) {
                    RevealImeSearch();
                    Render();
                }
                PositionIme(hwnd, (DWORD)lParam);
            }
            break;

        case WM_KEYDOWN:
            if (wParam == VK_PROCESSKEY) {
                const UINT key = ImmGetVirtualKey(hwnd);
                if (key >= 'A' && key <= 'Z' && CanEditWithIme() && !g_openFolder && !SearchExpanded()) {
                    RevealImeSearch();
                    PositionIme(hwnd);
                    Render();
                }
                break;
            }
            if (g_imeComposition.active) break;
            if (wParam == VK_RETURN && (lParam & (1L << 30))) return 0;
            OnKeyDown(wParam);
            return 0;

        case WM_CHAR:
            OnChar((WCHAR)wParam);
            return 0;

        case WM_CLOSE:  // Alt+F4
            Close();
            return 0;

        case WM_DPICHANGED:
            if (g_shown && std::abs(HIWORD(wParam)/96.0f-g_layout.scale)>0.001f) PostMessageW(hwnd,WM_APP+62,0,0);
            return 0;

        case WM_DISPLAYCHANGE:
        case WM_SETTINGCHANGE:
            RefreshSystemAnimations();
            if(msg==WM_SETTINGCHANGE&&g_settings.theme==L"system")PostMessageW(hwnd,WM_APP_RELOAD,0,0);
            if (g_shown && (msg == WM_DISPLAYCHANGE || wParam == SPI_SETWORKAREA)) {
                if (!g_closing && !g_modal) PostMessageW(hwnd, WM_APP + 62, 0, 0);
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK BackdropWndProc(HWND hwnd,
                                 UINT msg,
                                 WPARAM wParam,
                                 LPARAM lParam) {
    switch (msg) {
        case WM_MOUSEACTIVATE:
            return MA_NOACTIVATE;
        case WM_NCHITTEST:
            return HTTRANSPARENT;
        case WM_ERASEBKGND: {
            // Black with zero alpha: only the DWM effect shows.
            RECT rc;
            GetClientRect(hwnd, &rc);
            FillRect((HDC)wParam, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
            return 1;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(hwnd, &ps);
            FillRect(dc, &ps.rcPaint, (HBRUSH)GetStockObject(BLACK_BRUSH));
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

HANDLE g_thread;
HANDLE g_readyEvent;
HANDLE g_integrationLease;

// Releases everything the launcher thread created. The order is the point:
// hooks and windows first, then the holders that keep GPU objects alive,
// then the device objects themselves. It is one function so the test
// harness can run it repeatedly on a populated state, which is what a
// disable/enable or an update does inside a live Explorer.
void ReleaseLauncherResources() {
    g_wnd = nullptr;
    StopFullBackupJob();
#ifndef SEAMLESS_PREVIEW
    StopPowerUpdateProbe();
    g_lastPowerUpdateProbe = 0;
    StartButtonHooksStop();
#endif
    g_powerUpdateStatus = PowerUpdateStatus::Unknown;

    g_workerStop = true;
    StopWebIcons();
    if (g_worker) {
        WaitForSingleObject(g_worker, INFINITE);
        CloseHandle(g_worker);
        g_worker = nullptr;
    }
    // A list delivered after the window stopped handling messages.
    MSG msg;
    while (PeekMessageW(&msg, nullptr, WM_APP_APPS, WM_APP_APPS, PM_REMOVE)) {
        delete (AppList*)msg.lParam;
    }

    if (g_backdrop) {
        DestroyWindow(g_backdrop);
        g_backdrop = nullptr;
    }
    AppList().swap(g_apps);  // Also frees the memory (no_destroy).
    Tiles().swap(g_tiles);
    Tiles().swap(g_visible);
    CancelGroupPointer();
    std::vector<ExpandedGroupGeometry>().swap(g_groupGeometry);
    std::vector<Cell>().swap(g_groupCells);
    g_groupTransitions.clear();
    g_groupResizeHover.reset();
    g_selectionPointer={};
    AppSelection().swap(g_appSelection);
    std::unordered_set<std::wstring>().swap(g_selectedAppKeys);
    g_groupEntryOrder.clear();
    g_openFolder = nullptr;
    // A Folder owns its apps and an App owns its icon bitmap, which owns the
    // D2D device: every holder has to go before ResetTarget(), or the device
    // tree survives the teardown and leaks on each disable or update.
    // CancelGroupPointer() above already cleared the header/drop/drag owners;
    // these are the remaining ones.
    g_closingFolder = nullptr;
    ClearGroupPositions();  // Positions, rows, gesture, transitions, edit snapshot.
    std::vector<ArrangementGeometry>().swap(g_arrangementGeometry);
    g_drag = {};
    ReleaseMaterial();
    g_panelBlur = {};
    FreeSurface(g_surface);
    g_labelFormat.Reset();
    g_labelCache.clear();
    g_groupTitleRenders.clear();
    // The menu's frame, backdrop, staging texture, surface and the layouts in
    // its nodes go with Reset(); the three formats live as long as the module.
    LaunchpadMenu::Reset();
    LaunchpadMenu::g_menu.textFormat.Reset();
    LaunchpadMenu::g_menu.boldFormat.Reset();
    LaunchpadMenu::g_menu.hintFormat.Reset();
    g_searchFormat.Reset();
    g_messageFormat.Reset();
    g_iconFormat.Reset();
    g_tipFormat.Reset();
    g_groupFormat.Reset();
    // These containers are no_destroy so their destructors never run under the
    // loader lock at exit. That also means nothing frees them unless the
    // teardown does, so swap them out here instead of leaking them per unload.
    std::set<std::wstring>().swap(g_hidden);
    std::map<std::wstring, std::wstring>().swap(g_customIcons);
    std::set<std::wstring>().swap(g_pins);
    std::map<std::wstring, EntryRecord>().swap(g_entries);
    std::map<std::wstring, DisplayNameRecord>().swap(g_displayNames);
    std::wstring().swap(g_folderOriginalName);
    g_imeComposition = {};
    g_groupWheelTarget.reset();
    ResetTarget();
    if (g_frameWake) { CloseHandle(g_frameWake); g_frameWake = nullptr; }
    g_dwrite.Reset();
    g_d2d.Reset();
    UnregisterClassW(kWindowClass, (HINSTANCE)&__ImageBase);
    UnregisterClassW(kBackdropClass, (HINSTANCE)&__ImageBase);
    UnregisterClassW(kGroupNameClass, (HINSTANCE)&__ImageBase);
    UnregisterClassW(kEntryEditorClass, (HINSTANCE)&__ImageBase);
    LaunchpadMenu::UnregisterMenuClass();
}

DWORD WINAPI StartThread(void*) {
    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    OleInitialize(nullptr);
    g_frameWake = CreateWaitableTimerExW(nullptr, nullptr, 0x00000002 /* high resolution */, TIMER_ALL_ACCESS);
    if (!g_frameWake) g_frameWake = CreateWaitableTimerW(nullptr, FALSE, nullptr);
    LoadSettings();
#ifndef SEAMLESS_PREVIEW
    // Storage was validated before installing shell hooks in Wh_ModInit.
#endif

    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, g_d2d.GetAddressOf());
    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
                        (IUnknown**)g_dwrite.GetAddressOf());

    HINSTANCE instance = (HINSTANCE)&__ImageBase;
    WNDCLASSEXW wc{sizeof(wc)};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = instance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.lpszClassName = kWindowClass;
    RegisterClassExW(&wc);

    WNDCLASSEXW bc{sizeof(bc)};
    bc.lpfnWndProc = BackdropWndProc;
    bc.hInstance = instance;
    bc.lpszClassName = kBackdropClass;
    RegisterClassExW(&bc);

    g_backdrop = CreateWindowExW(
        WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE |
            WS_EX_TRANSPARENT,
        kBackdropClass, L"", WS_POPUP, 0, 0, 0, 0, nullptr, nullptr, instance,
        nullptr);
    HWND wnd = CreateWindowExW(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED,
                               kWindowClass, L"Start", WS_POPUP, 0, 0, 0, 0,
                               nullptr, nullptr, instance, nullptr);
#ifdef SEAMLESS_PREVIEW
    // Isolated UI QA only: let automation enumerate the otherwise tool-only canvas.
    if(Preview::uiCheck&&wnd) {
        auto style=GetWindowLongPtrW(wnd,GWL_EXSTYLE);
        SetWindowLongPtrW(wnd,GWL_EXSTYLE,(style&~WS_EX_TOOLWINDOW)|WS_EX_APPWINDOW);
        SetWindowTextW(wnd,L"Lumiflow UI validation");
    }
#endif
    bool ok = g_d2d && g_dwrite && g_backdrop && wnd;
    if (ok) {
        g_wnd = wnd;
#ifndef SEAMLESS_PREVIEW
        StartButtonHooksStart();
#endif
        g_taskbarCreated = RegisterWindowMessageW(L"TaskbarCreated");
        ChangeWindowMessageFilterEx(wnd, g_taskbarCreated, MSGFLT_ALLOW, nullptr);
        UpdateTrayIcon(wnd);
        // The app list is read a bit later, so it doesn't weigh on explorer's
        // startup.
        SetTimer(wnd, kTimerFirstLoad, 3000, nullptr);
    }
    SetEvent(g_readyEvent);
#ifdef SEAMLESS_PREVIEW
    if (ok) {
        if (!Preview::smoke) SetTimer(wnd, kTimerFirstLoad, 1, nullptr);
        PostMessageW(wnd, Preview::smoke ? WM_APP + 60 : (Preview::settingsOnStart ? WM_APP + 20 : WM_APP_TOGGLE), 0, 0);
    }
#endif

    if (ok) {
        MSG msg;
        bool quit = false;
        while (!quit) {
            DWORD count = g_frameWake && g_frameTimer ? 1 : 0;
            DWORD ready = MsgWaitForMultipleObjectsEx(count, count ? &g_frameWake : nullptr, INFINITE, QS_ALLINPUT, MWMO_INPUTAVAILABLE);
            if (count && ready == WAIT_OBJECT_0 && g_frameTimer) OnFrame();
            // Bound message draining so continuous input cannot starve frames.
            for (int budget = 0; budget < 64 && PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE); budget++) {
                if (msg.message == WM_QUIT) { quit = true; break; }
                TranslateMessage(&msg); DispatchMessageW(&msg);
            }
            if (ready == WAIT_FAILED) { Wh_Log(L"Frame message wait failed: %u", GetLastError()); break; }
        }
    } else {
        Wh_Log(L"Failed to create Start");
        if (wnd) {
            DestroyWindow(wnd);
        }
    }
    g_wnd = nullptr;
    ReleaseLauncherResources();
    OleUninitialize();
    return 0;
}

// With "Launch folder windows in a separate process", folder windows run in
// their own explorer.exe, which never shows Start.
bool IsFolderWindowsProcess() {
    std::wstring commandLine = ToLower(GetCommandLineW());
    return commandLine.find(L"/factory") != std::wstring::npos ||
           commandLine.find(L"-embedding") != std::wstring::npos ||
           commandLine.find(L"/separate") != std::wstring::npos;
}

#ifndef SEAMLESS_PREVIEW
bool HookShowStartView() {
    auto started = GetTickCount64();
    Wh_Log(L"Loading twinui.pcshell.dll");
    HMODULE module = LoadLibraryExW(L"twinui.pcshell.dll", nullptr,
                                    LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (!module) {
        const DWORD error = GetLastError();
        Wh_Log(L"Couldn't load twinui.pcshell.dll: error=%lu elapsed=%llu ms",
               error, GetTickCount64() - started);
        return false;
    }
    Wh_Log(L"Loaded twinui.pcshell.dll in %llu ms", GetTickCount64() - started);
    // twinui.pcshell.dll
    WindhawkUtils::SYMBOL_HOOK hooks[] = {
        {
            {LR"(public: virtual long __cdecl XamlLauncher::ShowStartView(enum IMMERSIVELAUNCHERSHOWMETHOD,enum IMMERSIVELAUNCHERSHOWFLAGS))"},
            &XamlLauncher_ShowStartView_Original,
            XamlLauncher_ShowStartView_Hook,
        },
    };
    started = GetTickCount64();
    Wh_Log(L"Resolving twinui.pcshell.dll ShowStartView");
    const bool resolved = WindhawkUtils::HookSymbols(module, hooks, ARRAYSIZE(hooks));
    Wh_Log(L"twinui.pcshell.dll ShowStartView resolved=%d elapsed=%llu ms",
           resolved, GetTickCount64() - started);
    return resolved;
}

#endif
}  // namespace

#ifndef SEAMLESS_PREVIEW
BOOL Wh_ModInit() {
    Wh_Log(L">");

    if (IsFolderWindowsProcess()) {
        Wh_Log(L"Folder windows process; Start stays in the taskbar's one");
        return FALSE;
    }
    // A differently named DLL must not stack a second Start interceptor on a
    // known running predecessor. Never disable or unload that mod for the user.
    if (FindWindowW(L"WindhawkSeamlessLaunchpad", nullptr) ||
        FindWindowW(L"WindhawkFullscreenStartPlus", nullptr) ||
        FindWindowW(L"WindhawkFullscreenStart", nullptr)) {
        Wh_Log(L"Another launcher is running. Disable it before enabling Lumiflow; original data was not changed.");
        return FALSE;
    }
    g_integrationLease = CreateMutexW(nullptr, FALSE, L"Local\\RYJSLaunchpad.Integration");
    if (!g_integrationLease || GetLastError() == ERROR_ALREADY_EXISTS) {
        if (g_integrationLease) CloseHandle(g_integrationLease);
        g_integrationLease = nullptr;
        return FALSE;
    }
    bool storageReady = false;
    try { storageReady = PrepareStorage(); } catch (...) { }
    if (!storageReady) {
        if (DataRoot().empty()) {
            Wh_Log(L"Lumiflow storage path is unavailable; no hooks installed.");
            CloseHandle(g_integrationLease); g_integrationLease = nullptr;
            return FALSE;
        }
        // A damaged or unsupported data folder must not take Start away for
        // good: open what can be read and keep every write disabled, so the
        // user can still restore a backup from the settings window.
        g_dataReadOnly = true;
        g_layoutReadOnly = true;
        Wh_Log(L"Lumiflow storage is damaged or unsupported; opening read-only.");
    }
    LoadSettings();
    if (!EnsureStartLatchSubclass()) {
        Wh_Log(L"The taskbar reset subclass is required; Start was not replaced");
        CloseHandle(g_integrationLease); g_integrationLease = nullptr;
        return FALSE;
    }
    if (!HookShowStartView()) {
        RemoveStartLatchSubclass();
        CloseHandle(g_integrationLease); g_integrationLease = nullptr;
        return FALSE;
    }
    HookXamlStartVisibility();
    g_readyEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
    g_thread = CreateThread(nullptr, 0, StartThread, nullptr, 0, nullptr);
    if (!g_thread) {
        CloseHandle(g_readyEvent);
        g_readyEvent = nullptr;
        RemoveStartLatchSubclass();
        CloseHandle(g_integrationLease); g_integrationLease = nullptr;
        return FALSE;
    }
    g_keyThread =
        CreateThread(nullptr, 0, KeyThread, nullptr, 0, &g_keyThreadId);
    return TRUE;
}

void Wh_ModBeforeUninit() {
    Wh_Log(L">");

    // Hooks still exist here. Closing the gate and synchronously removing the
    // subclass prevents any new reset from entering module code. Removal joins
    // any reset that is already executing on the taskbar thread.
    RemoveStartLatchSubclass();
}

void Wh_ModUninit() {
    Wh_Log(L">");

    // Defensive: Wh_ModBeforeUninit normally removed this already. This call is
    // synchronous and leaves neither a queued module callback nor a module pin.
    RemoveStartLatchSubclass();
    if (!g_thread) {
        CloseSettingsWindow();
        if (g_integrationLease) { CloseHandle(g_integrationLease); g_integrationLease = nullptr; }
        return;
    }
    if (g_keyThread) {
        // Repeated: the thread may not have its message queue yet.
        do {
            PostThreadMessageW(g_keyThreadId, WM_QUIT, 0, 0);
        } while (WaitForSingleObject(g_keyThread, 100) == WAIT_TIMEOUT);
        CloseHandle(g_keyThread);
        g_keyThread = nullptr;
    }
    WaitForSingleObject(g_readyEvent, INFINITE);
    if (HWND wnd = g_wnd) {
        PostMessageW(wnd, WM_APP_QUIT, 0, 0);
    }
    WaitForSingleObject(g_thread, INFINITE);
    CloseHandle(g_thread);
    g_thread = nullptr;
    CloseHandle(g_readyEvent);
    g_readyEvent = nullptr;
    // All module callbacks are joined: hooks were removed by Windhawk, the
    // taskbar subclass was synchronously removed, and the launcher is joined.
    // Sleep and hibernate return only after the PC wakes; every other action
    // returns at once, so this join cannot hang a normal unload.
    if (g_powerThread) {
        WaitForSingleObject(g_powerThread, INFINITE);
        CloseHandle(g_powerThread);
        g_powerThread = nullptr;
    }
    // Last: until the grid's thread ends, its "..." menu can open the window.
    CloseSettingsWindow();
    if (g_integrationLease) { CloseHandle(g_integrationLease); g_integrationLease = nullptr; }
}

void Wh_ModSettingsChanged() {
    Wh_Log(L">");

    if (HWND wnd = g_wnd) {
        PostMessageW(wnd, WM_APP_RELOAD, 0, 0);
    }
    if (HWND wnd = g_settingsWnd) {
        PostMessageW(wnd, WM_APP_SETTINGS_RELOAD, 0, 0);
    }
}

#endif // !SEAMLESS_PREVIEW
