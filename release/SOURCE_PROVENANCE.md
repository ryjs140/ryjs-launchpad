# 源码来源与继承关系审计

对象：RYJS Launchpad 1.0.0
文件：`ryjs-launchpad.wh.cpp`（16,326 行，739,135 字节，SHA256 `af72dda026efabc021cc92ddff1a20905a32bcea8366e9d63da425bc558a8b89`）
审计日期：2026-11-19

---

## 0. 结论摘要

1. **RYJS Launchpad 是衍生作品，不是独立重写。** 代码血统为：Rafael Mendes 的 Fullscreen Start 1.0.0 → DavidHiFi 的 Seamless Launchpad 2.4.9 → 本项目前身 Seamless Launchpad Custom 3.0.x → RYJS Launchpad 0.1.0–1.0.0。
2. **行级证据**：发布文件中 50.9% 的代码行与 Seamless Launchpad 2.4.9 逐字相同；其中 30.1% 同时与 Fullscreen Start 1.0.0 逐字相同（相当于整个文件的 15.3%）。Rafael 原始代码行仍有 77.0% 存活，93 个函数中 91 个仍在。
3. **本项目实际改动**：49.1% 的代码行相对 2.4.9 是新增或修改；新增 370 个函数级符号；30 项上游设置全部保留，另新增 25 项设置；新增 12 个功能子系统（手动入口、网页图标、组自由几何、主题、完整备份等）。
4. **许可证不构成障碍。** 两个上游均为 MIT（Rafael 通过 mod 元数据 `@license MIT` 声明，其仓库无独立 LICENSE 文件、也没有版权行；David 仓库有 MIT LICENSE，写明 `Copyright (c) 2026 DavidHiFi`）。MIT 允许修改与再发布，条件是在所有副本中保留版权声明与许可文本。mod readme、仓库 `LICENSE`、`release/LICENSE` 三处均保留 DavidHiFi 的原始声明与 Rafael Mendes 的署名（后者不写年份，因为上游没有声明，本项目不代为推测）。
5. **可以独立发布的含义**：可以拥有独立的 mod id、独立存储、独立维护者与独立的官方库条目；**不能**声称代码为独立实现。任何对外说明都必须表述为"基于 Seamless Launchpad 的衍生作品"。当前开发者与维护者为 Fangyu Guo (ryjs)。

---

## 1. 审计方法

| 项目 | 做法 |
| --- | --- |
| 对照文件 | Fullscreen Start 1.0.0（官方库 `mods/fullscreen-start.wh.cpp`，67,438 字节）<br>Seamless Launchpad 2.4.9（`reference/upstream/seamless-launchpad-2.4.9-upstream.wh.cpp`，9,011 行）<br>RYJS 发布文件 |
| 行级比对 | 去除空行、`//` 注释行、单独的 `{`/`}`，按 strip 后整行集合匹配，统计逐字命中比例 |
| 结构比对 | 正则提取"标识符 + 参数表 + 函数体左花括号"的函数签名集合，比较交集 |
| 设置比对 | 解析 `==WindhawkModSettings==` YAML 块顶层键 |
| 历史比对 | Git 提交、`ryjs-project.json` 记录的基线提交、前身分支提交 |

方法学局限，须一并阅读：

- 逐字行匹配**低估**改写量：重命名变量、重排代码、改缩进的等价代码会被判定为"不同"。
- 逐字行匹配也可能**高估**继承：常见样板行（`return false;`、`#include <windows.h>` 之类）会天然重合，故同时给出函数结构与设置项的独立证据。
- 函数名相似本身不构成复制证据；本报告不单凭函数名判断，结论以逐字行与文件血统为准。

复现命令见第 8 节。

---

## 2. 继承链与证据

| 环节 | 项目 / 版本 | 作者 | 证据 | 许可证 |
| --- | --- | --- | --- | --- |
| 源头 | Fullscreen Start (Launchpad style) 1.0.0 | Rafael Mendes | 官方库 `mods/fullscreen-start.wh.cpp`，`@license MIT`，`@include explorer.exe` | MIT（元数据声明） |
| 第二环 | Seamless Launchpad 2.4.9 | DavidHiFi, Rafael Mendes | 上游仓库 `mods/local/seamless-launchpad/`，`@author DavidHiFi, Rafael Mendes`，readme 明示 "Based on Fullscreen Start by Rafael Mendes" | MIT（仓库 LICENSE，Copyright 2026 DavidHiFi） |
| 第三环 | Seamless Launchpad Custom 3.0.x | 本项目（前身分支） | 基线提交 `598c8f5`，上游基线 `09a71ba`，均记录于 `ryjs-project.json` | MIT |
| 当前 | RYJS Launchpad 0.1.0 → 1.0.0 | Fangyu Guo (ryjs) | 本仓库提交 `c5622ad` → `8c9a9a2` + 待提交工作区 | MIT |

`ryjs-project.json` 记录：

```
upstream          https://github.com/DavidHiFi/davids-windhawk-mods
upstreamBaseCommit 09a71bacd81b356d61ae388982cc3426dfea6101
customBaseCommit   598c8f565a04765ec415a0f5d05bc04be05aee9f
preservedWorkingTreeTag ryjs-fork-baseline
credits           DavidHiFi / Seamless Launchpad；Rafael Mendes / Fullscreen Start
```

注意：RYJS 仓库的 Git 历史从 `c5622ad` 开始，**不包含**上游 362 条提交。因此血统证据由本文件、`ryjs-project.json` 与源码内的版权块共同承担，不能靠 `git log` 单独还原。

### 2.1 版权归属划分

| 主体 | 身份 | 采用的版权声明 | 依据 |
| --- | --- | --- | --- |
| Rafael Mendes | Fullscreen Start 作者；原始启动器、渲染与 Start 钩子代码的版权人 | `Copyright (c) Rafael Mendes`（不写年份） | 上游 mod 只有 `@license MIT`，仓库无 LICENSE 文件、文件内无版权行；年份不可考，故不推测 |
| DavidHiFi | Seamless Launchpad 作者；直接基础的网格、搜索、文件夹、电源栏、设置代码的版权人 | `Copyright (c) 2026 DavidHiFi` | 上游 `LICENSE` 原文，逐字保留 |
| Fangyu Guo (ryjs) | RYJS Launchpad 的当前开发者与维护者；本项目新增与修改部分的版权人 | `Copyright (c) 2026 Fangyu Guo (ryjs)` | 本仓库 |

三点必须同时成立，缺一即失实：

1. Windhawk 元数据 `@author` 只写当前开发者 `Fangyu Guo (ryjs)`，**不**把上游作者列为共同作者——上游作者并未参与 RYJS 的维护，列进去反而制造"他们为当前版本负责"的误导。
2. 上游作者的身份与版权声明写在 mod readme、仓库 `README.md`、`LICENSE`、本文件四处，**不因 `@author` 简化而消失**。
3. 本项目只对"新增与修改"部分主张版权，不对继承代码主张版权。

---

## 3. 量化结果

### 3.1 行级

| 指标 | 数值 |
| --- | --- |
| 发布文件有效代码行（去空行/注释/括号） | 12,964 |
| 与 Seamless Launchpad 2.4.9 逐字相同 | 6,593（**50.9%**） |
| 上述行中同时与 Fullscreen Start 1.0.0 逐字相同 | 1,984（占继承行 30.1%，占全文件 **15.3%**） |
| 相对 2.4.9 新增或修改 | 6,359（**49.1%**） |
| Rafael 1.0.0 原始行存活率 | 1,120 / 1,455（**77.0%**） |

### 3.2 结构级

| 指标 | 数值 |
| --- | --- |
| Rafael 1.0.0 函数数 | 93 |
| Seamless Launchpad 2.4.9 函数数 | 398 |
| RYJS 函数数 | 753 |
| Rafael 函数在 RYJS 中仍存在 | **91 / 93** |
| RYJS 相对 2.4.9 新增函数级符号 | 370 |

### 3.3 设置级

| 指标 | 数值 |
| --- | --- |
| 上游 2.4.9 设置项 | 30 |
| RYJS 设置项 | 55 |
| 上游设置被删除 | **0**（30 项全部保留，键名未变，向后兼容） |
| 新增设置 | 25 |

新增键：`animationEasing` `backgroundDim` `closeDuration` `folderDuration` `frameRateLimit` `globalColumns` `gpuPresenter` `groupColor` `groupFlow` `groupNavigation` `groupOpacity` `groupSpacing` `groupWidth` `horizontalSpacing` `hoverColor` `interfaceLanguage` `labelFontSize` `launchpadAnimation` `openDuration` `performanceLogging` `reducedMotion` `selectionColor` `themeMode` `uiFont` `verticalSpacing`

设置项 100% 向后兼容这一点，是"同一血脉"最直接的行为层证据：新版本读取旧配置不会丢失任何上游选项。

---

## 4. 代码分类

### A. 直接继承（逐字保留）

启动器的核心骨架，来自 Rafael 经 David 传下来的部分：

| 模块 | 代表符号 | 出处 |
| --- | --- | --- |
| 启动视图接管 | `HookShowStartView`、`XamlLauncher_ShowStartView_Hook` | Rafael |
| 布局计算 | `ComputeLayout`、`CellAt`、`ItemAt`、`MaxScroll`、`EnsureVisible` | Rafael |
| 网格绘制 | `DrawGrid`、`DrawSearchBox`、`DrawMessage`、`Fill`、`EndDraw` | Rafael |
| 应用枚举 | `EnumerateApps`、`LoadAppsThread`、`LoadAppIcon`、`IconPixels`、`FinishIcon` | Rafael |
| 搜索 | `MatchRank`、`NameLess`、`OnChar`、`OnKeyDown`、`MoveSelection` | Rafael |
| 背景与模糊 | `CaptureBackdrop`、`GetBackdropBitmap`、`ApplyBackdropEffect`、`BackdropWndProc` | Rafael |
| 电源栏 | `kPower[]`、`PowerButton`、`PowerThread` | David |
| 文件夹 | 文件夹玻璃面板、拖放建夹、文件夹内重命名 | David |
| 设置读取 | `LoadSettings`、`IntSetting`、`GetStringSetting` | Rafael/David |
| 多显示器 | `ChosenMonitor`、`AvailableArea`、`PlaceWindows` | Rafael/David |

### B. 继承后修改

| 模块 | 上游做法 | RYJS 做法 |
| --- | --- | --- |
| 布局持久化 | 单一 `layout-*.txt` | 保留旧文件，另加 `groups-v1`、`entries-v1`、`display-names-v1`、`group-geometry-layout-*` 多层文件 + 版本化 schema + 只读保护 |
| 分组 | 展开分组 / 传统文件夹二选一，顺序固定 | 增加组级自由几何（位置、宽高）、编辑会话、重叠拒绝、组内滚动 |
| 右键菜单 | 原生 `#32768` 菜单 | 自绘圆角亚克力面板，保留原生子菜单与键盘语义 |
| 动画 | 固定时长 | 时长滑块 + 毫秒输入 + 四种缓动 + 减少动画 + 飞入 |
| 标签绘制 | 底衬 + 阴影 | 无底衬，按渲染后亮度取纯黑/纯白并滞回稳定 |
| 设置窗口 | 固定字段表 | 55 项、分页、主题、动态显示器标签、私有 INI 覆盖 + 宿主优先规则 |
| 开始按钮 | 仅 `ShowStartView` 钩子 | 追加 StartAllBack/Win32 按钮子类化与 XAML `ExperienceToggleButton` 状态复位 |

### C. 参考思想、自行实现

| 功能 | 参考对象 | 实现说明 |
| --- | --- | --- |
| 组自由布局与缩放 | 通用桌面图标网格交互 | 未复制任何上游代码；吸附到整图标节距、红色重叠预览、Enter 提交 / Esc 逐级取消 |
| 圆角亚克力菜单 | 本 mod 自己的搜索栏玻璃配方 | 复用 `DrawGlassSurface`/`BlurToBitmap` 配方，菜单命令仍走 `TPM_RETURNCMD` |
| 标签对比度 | 通用对比度算法思路 | 对无文字模糊图采样，sRGB 线性化后比较亮度，滞回避免闪烁 |
| 完整备份格式 | 本项目此前的 Python 工具 | 改为内嵌 PowerShell worker，`.ryjs.zip` + 清单/哈希/版本校验 |

### D. 独立新增

| 子系统 | 文件 | 行数 |
| --- | --- | --- |
| 分组布局引擎 | `launchpad_groups_layout.inc` | 685 |
| 分组编辑（拖动/缩放/会话） | `launchpad_group_edit.inc` | 545 |
| 备份 worker（内嵌 PowerShell） | `launchpad_backup_script.inc` | 425（生成） |
| 中文资源 | `launchpad_strings.inc` | 352（生成） |
| 分组动作（建/删/移动） | `launchpad_groups_actions.inc` | 294 |
| XAML 开始按钮状态 | `launchpad_start_xaml.h` | 211 |
| 完整备份/导入 | `launchpad_full_backup.inc` | 203 |
| 组几何持久化 | `launchpad_group_positions.inc` | 199 |
| Win32 开始按钮子类化 | `launchpad_start_button.h` | 195 |
| 入口编辑对话框 | `launchpad_entry_editor.inc` | 178 |
| 存储迁移与校验 | `ryjs_data.h` | 129 |
| 网页图标抓取 | `launchpad_web_icons.inc` | 128 |
| 入口模型 | `launchpad_entries.inc` | 101 |
| GPU 呈现（flip-model） | `launchpad_presenter.h` | 90 |
| 性能记录 | `launchpad_metrics.h` | 88 |
| 私有设置存储 | `launchpad_settings_store.h` | 86 |
| 飞入动画 | `launchpad_fly_in.h` | 77 |
| 显示名覆盖 | `launchpad_display_names.inc` | 50 |
| 标签对比度 | `launchpad_contrast.inc` | 42 |
| 国际化 | `launchpad_i18n.h` | 40 |
| SVG 光栅化 | `launchpad_svg.inc` | 29 |

其中 `launchpad_presenter.h`、`launchpad_metrics.h`、`launchpad_settings_store.h`、`launchpad_i18n.h`、`launchpad_start_button.h` 及中文资源来自本项目前身分支（Custom 3.0.x），属于本项目自有代码，不是 David 或 Rafael 的代码。

### E. 暂时无法确认来源

无。逐行核对后没有出现既不在两个上游、也无法归入前身分支的孤立代码块。唯一需要说明的边界：本项目前身分支由 AI 编程助手参与编写（提交作者 `Codex`），但该分支是本项目自己的仓库历史，归属清晰。

---

## 5. 功能来源对照表

| 功能 | 上游是否存在 | 当前实现差异 | 来源类型 | 代码证据 |
| --- | --- | --- | --- | --- |
| 全屏启动器界面 | 是（Rafael） | 基本一致，增加主题与 GPU 呈现 | 直接继承 | `DrawGrid`、`ComputeLayout`、`CaptureBackdrop` |
| 应用扫描与启动 | 是（Rafael） | 增加手动入口、别名、原名双匹配 | 继承 + 扩展 | `EnumerateApps`、`MatchRank`、`LaunchTarget` |
| 文件夹 | 是（David） | 保留，可与常驻分组并存 | 直接继承 | 文件夹玻璃面板与拖放建夹路径 |
| 分组管理 | 部分（David 的展开分组） | 自由几何、缩放、编辑会话、重叠拒绝 | 继承 + 新增 | `launchpad_group_edit.inc`、`group_positions.inc` |
| 全局网格布局 | 是（Rafael） | 增加 `globalColumns` | 继承 + 扩展 | `ComputeLayout` |
| 自定义图标列数 | 是（`gridColumns`） | 追加全局列数控件 | 继承 + 扩展 | 设置键比对 |
| 布局编辑 / 拖动 / 缩放 | 部分（仅图标拖动、组排序） | 新增组级拖动与整格缩放 | 新增 | `GroupDragMove`、`GroupEntryProgress`、`Finish` |
| 图标名称显示 | 是 | 去底衬、对比度自适应纯黑白 | 修改 | `launchpad_contrast.inc` |
| 搜索 | 是（Rafael） | 别名与原名同时命中、IME 候选、结果菜单 | 继承 + 修改 | `MatchRank`、`OnChar`、`OnKeyDown` |
| 毛玻璃 / 液态玻璃 | 是（Rafael 的模糊） | 自绘圆角亚克力菜单、主题化文字 | 修改 + 重实现 | `LaunchpadMenu`、`DrawGlassSurface` |
| 鼠标悬停交互 | 是 | 保留，增加悬停色与选中色设置 | 继承 + 扩展 | `hoverColor`、`selectionColor` |
| 展开 / 收起动画 | 部分 | 时长滑块、毫秒输入、四种缓动、减少动画、飞入 | 扩展 | `openDuration`、`closeDuration`、`launchpad_fly_in.h` |
| 多显示器适配 | 是（写死 1–4 号） | 枚举已连接显示器并显示友好名与分辨率 | 扩展 | `Set_MonitorLabels`、`EnumDisplayDevicesW` |
| DPI 与分辨率适配 | 是 | 保留，组几何按 DIP 参与换算 | 继承 | `AvailableArea`、DPI 缩放路径 |
| 设置管理与持久化 | 是（David 私有 INI） | 55 项、分页、schema 版本、宿主优先覆盖 | 继承 + 新增 | `launchpad_settings_store.h` |
| 主题（浅/深/跟随/自定义） | 否 | 新增 | 新增 | `themeMode`、`groupColor`、`backgroundDim` |
| 手动入口（exe/lnk/rdp/商店/网址） | 否 | 新增 | 新增 | `launchpad_entries.inc`、`entry_editor.inc` |
| 网页图标抓取与缓存 | 否 | 新增（WinHttp，仅显式添加/刷新） | 新增 | `launchpad_web_icons.inc`、`launchpad_svg.inc` |
| 完整备份 / 导入（`.ryjs.zip`） | 否 | 新增（内嵌 PowerShell worker + 校验） | 新增 | `launchpad_full_backup.inc`、`launchpad_backup_script.inc` |
| 中英双语 + 本机字体 | 否（前身分支新增） | 351 条中文资源、字体选择 | 本项目自有 | `launchpad_i18n.h`、`launchpad_strings.inc` |
| GPU 合成呈现 | 否（前身分支新增） | DirectComposition + flip-model，失败回退分层窗口 | 本项目自有 | `launchpad_presenter.h` |
| 性能日志 CSV | 否（前身分支新增） | 阶段延迟、渲染耗时、帧间隔、呈现统计 | 本项目自有 | `launchpad_metrics.h` |
| 开始按钮状态复位 | 否 | Win32 子类化 + XAML `ExperienceToggleButton` 拦截 | 新增 | `launchpad_start_button.h`、`launchpad_start_xaml.h` |

---

## 6. 主要改进与独立开发内容

以下为本项目相对直接上游 Seamless Launchpad 2.4.9 的实质改动。每项都标注了它是"新增"还是"改写"，避免把改写说成从零实现。

### 6.1 布局与分组（改写 + 新增）

- **分组自由几何**：组可自由移动，拖动右下角按整图标节距缩放，重叠时红色预览并拒绝提交；位置与尺寸独立持久化（`group-geometry-layout-*`，schema 2）。上游只有固定顺序的展开分组。
- **编辑会话**：Enter/保存统一写盘，Esc 先取消当前手势再取消整次编辑，关闭启动台即放弃未保存改动。
- **分组与传统文件夹并存**：同一布局里可以既有常驻分组又有可展开文件夹，可逐组切换。
- **组内滚动与画布滚动分离**：缩小后组内滚轮看剩余应用，Shift+滚轮移动整个画布。

### 6.2 入口模型（新增）

- 支持 `.exe`、`.lnk`、`.rdp`、商店应用、http/https 五类入口，可设置显示名、参数、工作目录、图标与目标分组。
- 稳定入口标识 + 独立版本化覆盖层，路径变化不会破坏原有排列引用；目标丢失时入口保留并可修复。
- 显示名覆盖层（`display-names-v1`）与原名并行，搜索同时命中两者。

### 6.3 视觉与主题（改写 + 新增）

- 浅色 / 深色 / 跟随系统 / 自定义四种主题，文字、分组、悬停、选中四组颜色独立可调。
- 标签去掉底衬与阴影，改为对"无文字模糊图"采样、sRGB 线性化后取纯黑/纯白，并加滞回避免闪烁。
- 右键菜单改为自绘圆角亚克力面板，命令仍走 `TPM_RETURNCMD`，保留原生子菜单与键盘语义。

### 6.4 系统集成（新增）

- Win32 开始按钮子类化（StartAllBack/经典任务栏），接管完整左键手势，避免原生选中状态与启动器脱节。
- 原生 XAML `ExperienceToggleButton` 的可见性与选中状态修正，仅在 `AutomationId == StartButton` 且当前由本 mod 接管时生效，原生菜单操作继续透传。
- 与上游 mod 的共存保护：检测到 `WindhawkSeamlessLaunchpad` / `WindhawkFullscreenStartPlus` 窗口时不加载，且不禁用对方。

### 6.5 数据与运维（新增）

- 版本化存储（`data-v1`）、写盘前 `.bak`、未知 schema 转只读、失败不覆盖。
- 首次运行从旧 mod 目录**只读**迁移，注册表只读取一次。
- `.ryjs.zip` 完整备份与导入：清单/哈希/路径/版本四重校验，导入只恢复数据，暂存目录 + 失败回退。
- 内嵌 PowerShell worker，实现完整备份不需要用户安装 Python。

### 6.6 渲染与性能（本项目前身分支引入）

- DirectComposition + DXGI flip-model 的 GPU 提交路径，失败自动回退分层窗口。
- 按所选显示器刷新率的帧调度，空闲停表；`performance.csv` 分别记录绘制耗时、帧间隔与提交频率，并明确"提交频率不等于呈现帧率"。

### 6.7 国际化（本项目前身分支引入）

- 351 条中文资源、跟随系统语言、本机字体枚举（图标使用独立符号字体，不随文字字体变化）。

**不属于本项目原创的部分**：网格绘制、模糊、应用枚举、搜索排序、文件夹、电源栏、`ShowStartView` 钩子及其骨架逻辑，均来自 Rafael Mendes 与 DavidHiFi。

## 7. 为什么可以作为独立插件发布

可以，但边界必须说清：

1. **法律上可行。** 两个上游都是 MIT，明确允许修改、再发布与再许可，只要求保留版权声明与许可文本。本项目已在 mod readme 的 License 块、仓库 `LICENSE`、`release/LICENSE` 中同时保留 DavidHiFi 与 Rafael Mendes 的版权。
2. **工程上是独立条目。** 独立 mod id `ryjs-launchpad`、独立存储目录 `local@ryjs-launchpad`、独立设置 schema、独立版本号、独立维护者与仓库，不覆盖也不修改上游 mod 的任何数据（迁移只读）。
3. **功能上有实质差异。** 相对直接上游 2.4.9，49.1% 的代码行是新增或修改，新增 12 个子系统与 25 项设置，且 30 项上游设置全部保留，用户从上游迁移不会丢配置。
4. **不能声称的部分。** 不能说"独立开发""从零实现""与上游无代码关系"，也不能把 DavidHiFi 或 Rafael Mendes 写成当前开发者，或反过来把本项目的改动归给上游。任何简介、readme、PR 说明都必须以"分支/衍生"表述。

---

## 8. 复现方式

上游快照已放在 `reference/upstream/`，无需联网即可复跑：

```bash
py -3.12 release/tools/audit-provenance.py \
  --candidate ryjs-launchpad.wh.cpp \
  --upstream-seamless reference/upstream/seamless-launchpad-2.4.9-upstream.wh.cpp \
  --upstream-original reference/upstream/fullscreen-start-1.0.0.wh.cpp
```

三个输入文件的出处、抓取方式与各自许可证见 `reference/README.md`。若要从头获取：

```bash
curl -o fullscreen-start-1.0.0.wh.cpp \
  https://raw.githubusercontent.com/ramensoftware/windhawk-mods/main/mods/fullscreen-start.wh.cpp
git -C <davids-windhawk-mods> show 09a71ba:mods/local/seamless-launchpad/seamless-launchpad.wh.cpp \
  > seamless-launchpad-2.4.9-upstream.wh.cpp
```

脚本输出第 3 节的全部数字。
