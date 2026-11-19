# RYJS Launchpad 四项整改交付记录

日期：2026-11-18，更新至本机开始按钮实测确认。修改范围：本仓库启动台及其开发验证代码。经用户授权 UAC，已更新正在运行的同一个 `local@ryjs-launchpad` 模块；未提交、推送、发布，未修改其他插件或系统快捷方式。源码与作者、插件 ID、动画速度保留。

**结论：四项均已有源码实现；分组、文字与名称完成隔离验证。本机任务栏胶囊已取得修复前后真实 XAML 点击状态对比，用户确认“已正常，未再残留，启动台正常开关”。其他功能尚未完成的人工验收仍列于末尾。**

## 1. 审计与数据保护

- 本次工作目录为 `ryjs-launchpad-public`，开始时工作区干净。另一个旧仓库 `../ryjs-launchpad` 原有 `tools/library-check/check.py` 未提交修改保持不动。
- 已完整复制正式模块 `local@ryjs-launchpad/data-v1` 至 `build/remediation-20261118/original-data`，再恢复至隔离目录 `restore-check`。15 个布局、几何、固定项、设置及历史备份文件逐文件 SHA-256 一致；运行时持续增长的 `performance.csv` 不纳入静态校验。
- 最终重新核对正式存储的这 15 个持久化文件，变化数为 **0**。证据：`build/remediation-20261118/original-data-hashes.json`、`final-data-integrity.json`。
- 原数据副本的存储回归读到：固定应用布局 12 组、40 个成员；全部应用分组布局 4 组、267 个成员；旧全部应用布局 1 组、267 个成员。不同文件是不同布局视图，不能将这些数相加作为去重应用总数。
- 原始源码归档：`build/remediation-20261118/source-before.zip`。用户数据和构建产物均留在被忽略的 `build/` 中。

## 2. 四项改动与原因

| 项目 | 原因与修复 | 主要文件 |
|---|---|---|
| 分组二维布局 | 原尺寸按 16 DIP 取整，组内列宽还会被拉伸，无法代表完整图标单元；每次松手立刻写盘，无法取消整次编辑。改为连续 DIP 坐标移动、完整图标节距的整数行列拉伸、实时内容预览与尺寸反馈；只通过右下角手柄调整尺寸。碰撞显示红色预览并拒绝落位，其他组不动。Enter/菜单统一保存，Esc 先取消手势、再取消整个会话，关闭也放弃未保存编辑。 | `launchpad_group_edit.inc`、`launchpad_groups_layout.inc`、`launchpad_group_positions.inc`、`launchpad_groups_actions.inc` |
| 无底板黑白文字 | 原绘制函数显式填充标签胶囊和标题条，固定主题色与显示背景无关。删除标签底板、阴影和文件夹标题胶囊；按实际文字布局范围采样无本层文字的背景，线性化 sRGB 后计算相对亮度，选择纯黑/纯白，加入约 0.155–0.205 的亮度滞回。 | 新增 `launchpad_contrast.inc`，修改 `seamless-launchpad.wh.cpp`、`launchpad_groups_layout.inc`；移除相应无效设置控件 |
| Windows 开始按钮 | 本机原生 XAML `ExperienceToggleButton::OnToggle` 在菜单打开前把 `IsChecked` 置为 1；RYJS 拦截 `ShowStartView` 后没有原生菜单关闭生命周期将它复位。旧修复只处理 Win32 状态和 `IsExperienceVisible`，漏掉独立的选中状态。现在仅对 AutomationId=`StartButton` 且配置由 RYJS 接管的点击拦截原生选中切换，并清除已遗留的 `IsChecked`；指针释放、悬停及后续 Click/ShowStartView 保留。其他 XAML 按钮和原生开始菜单模式透传。真实日志和用户确认见第 4 节。 | `launchpad_start_xaml.h`、`seamless-launchpad.wh.cpp`（补充 Primitives 头文件）、重新生成的 `ryjs-launchpad.wh.cpp`；既有兼容路径 `launchpad_start_button.h` 保留 |
| 应用显示名称 | 原有“编辑入口”包含名称覆盖，但需要同时处理启动目标等字段，没有轻量独立重命名/恢复操作。新增所有应用右键“重命名”和“恢复原始名称”，复用轻量输入框；保存原名与自定义名，显示名按稳定 `AppKey` 关联。恢复原名保留空覆盖标记，避免旧入口别名在重扫后复活。搜索匹配原名与自定义名；原生工具提示提供完整名称。 | 新增 `launchpad_display_names.inc`，修改 `launchpad_entry_editor.inc`、`launchpad_groups_actions.inc`、`seamless-launchpad.wh.cpp` |

所有源码集成到重新生成的 `ryjs-launchpad.wh.cpp`。相关资源/验证文件包括 `dev/generate_resources.py`、`dev/settings-extension.json`、`dev/settings-defaults.json`、`dev/strings.zh-CN.json`、`dev/Build.ps1`、`dev/test_features.cpp`、`dev/test_start_button.cpp`、`dev/preview/*` 及完整备份测试与内嵌脚本。生成的 `launchpad_strings.inc`、`launchpad_backup_script.inc` 随之更新。

### 图标单元与兼容规则

- 横向节距考虑图标、两侧单元内边距、用户横向间距及名称的最低分配宽度；纵向节距包括图标、标签高度及纵向间距。均由当前 DPI 下的真实布局参数计算。
- 分组宽为 `24 DIP + 列数 × 横向节距`；高为 `60 DIP + 行数 × 纵向节距`。其中标题 36 DIP，内容区上下各 12 DIP，独立于图标单元。
- 拖动超过半个完整节距才切换一行/一列；旧任意尺寸在不足阈值时保持不变。最小为 1 列 × 1 行。旧分组未主动拉伸时保留旧尺寸，不批量转换其他组。
- 内容溢出采用**组内滚动**；普通模式滚轮滚动组内，Shift+滚轮滚动整体。键盘导航可将最后一个溢出项滚入视口。仅可见行参与绘制和命中，所有成员和顺序保留。
- `group-geometry-layout-*.txt` schema 2 增加整数行列。仍读 schema 1；首次写含整数行列的数据前保留 `.before-v2.bak`，常规原子写入继续使用既有 `.bak`。未改变的文件不重复写盘。成员文件格式不变。
- 名称独立保存到 `display-names-v1.txt`，不改变入口文件的启动信息。完整备份导入/导出同时支持新几何和名称文件，并拒绝损坏/未知格式。

### 背景检测的准确边界

- 默认自定义模糊：读取**同一张 GPU 模糊位图**，再按渲染方式组合 tint、背景调暗和已有分组面板。并非原始壁纸平均色，也不依赖 Windows 深浅主题。图标标签和标题按实际文字矩形分别判定。
- 文件夹内使用现有面板模糊管线的位图；其底层网格属于背景，本层尚未绘制的文字不会参与自己的采样。网格与文件夹标签区分采样阶段，避免反馈。
- 背景在打开时捕获一次，常规帧只访问缓存像素；面板在其现有失效时读回，不额外高频截屏。无模糊模式使用未模糊的打开前画面。
- **DWM 亚克力/系统模糊最终材质不在本模块 GPU 渲染目标中。** 当前采用打开前屏幕的模糊估计，再组合本模块叠色；不能读取后续桌面变化、系统噪声或最终合成材质，也不能称为“最终合成像素检测”。该模式仍有背景变化导致文字判断滞后的限制。
- 相关 API 依据：[Direct2D 位图 Map](https://learn.microsoft.com/en-us/windows/desktop/api/d2d1_1/nf-d2d1_1-id2d1bitmap1-map)、[CPU_READ 位图选项](https://learn.microsoft.com/en-us/windows/win32/api/d2d1_1/ne-d2d1_1-d2d1_bitmap_options)、[系统绘制背景材质](https://learn.microsoft.com/en-us/windows/win32/api/dwmapi/ne-dwmapi-dwm_systembackdrop_type)。

## 3. 已完成的验证

| 验证 | 结果与证据 |
|---|---|
| `dev/Build.ps1 -Verify -Smoke` | 通过：正式 x64 Windhawk DLL、独立预览链接；中英文、真实应用枚举、GPU/兼容呈现、原有动画调度与隔离布局回归。日志 `build/compile-*.log`、`build/preview-data/preview.log` 及其 `validation-state/preview.log`。 |
| `dev/Test-Features.ps1` | **99 项，0 失败**。100/125/150/200% DPI、整数阈值、固定边距、独立组位置、二维连续移动、分页移动、碰撞拒绝、整次取消、31 应用溢出、末行访问、几何重载、不同目标类型名称、双名称搜索、恢复旧别名兼容、未知版本拒写。 |
| `dev/Test-Phase1.ps1 -SkipBuild -SnapshotData build/remediation-20261118/original-data` | 存储 **43 项通过**；Win32 开始按钮模拟 **29 项通过**；Python 布局备份 **5 项通过**。仅向隔离测试目录恢复，未修改正式数据。 |
| 旧开始按钮代码对比 | 从修改前归档取原头文件，运行相同新增测试：**29 项中 1 项失败**；`build/remediation-20261118/start-before.log`。这证明取消标记缺陷，不证明真实 XAML 胶囊的责任归属。 |
| `dev/Test-FullBackup.ps1` | 原生 GUI 到内嵌 PowerShell 桥接 **25 项通过**；Python 完整备份 **7 项通过**。包括新名称、新整数行列、Unicode、图标、旧备份兼容、损坏拒绝、原数据保留。 |
| 背景与动画离屏绘制 | 白、暗、灰、明暗混合图均已生成并观察；混合背景中不同文字分别为黑/白；GPU 位图读回成功。打开 0/.25/.5/.75/1 和关闭 .75/.5/.25 的帧路径执行。图位于 `build/preview-data/screenshots/readability-*.png`。这不等于所有桌面背景的人眼验收。 |
| 官方 Windhawk 检查 | 元数据检查和官方隔离编译均 `ok: true`；`build/library-validation.log`、`build/library-compile.log`。构建未安装进 Windhawk。 |
| 本机符号/插件只读核对 | 本机 PDB 存在 `ExperienceToggleButton` 的可见状态 ABI setter 及原生 getter/setter。相关证据 `taskbar-symbols.txt`、`related-mods-readonly.json`。图标尺寸插件确有开始按钮尺寸/内边距路径；未做插件开关对比，不能认定它导致残留。 |
| 真实独立预览 | 用 `build/remediation-20261118/ui-check` 的合成应用/分组启动，在真实屏幕快照模糊上观察到黑色标题和名称、无文字底板；应用右键菜单显示“重命名”。尚未完成输入框 Enter/Esc、鼠标拖动和重启 UI 验收。 |

早期电脑控制被物理 Esc 停止后，没有继续发送界面输入。后续经用户明确授权 UAC，通过 Windhawk CLI 更新同一模块，由用户实际点击复现并验收；未修改其他任务栏插件。

## 4. 开始按钮的真实根因、部署与验证

用户确认复现时，诊断日志记录如下（字段来自真实 XAML 属性 getter）：

```text
1529078 toggle-before checked=0 pressed=0 hover=1 replacement=0 native=0
1529078 toggle-after  checked=1 pressed=0 hover=1 replacement=0 native=0
1529078 release-after checked=1 pressed=0 hover=1 replacement=1 native=0
```

因此并非按下状态未释放；是按钮选中状态已锁存，随后菜单被接管。第二次点击会反向切换选中状态，与用户症状一致。这个事件链在没有变更其他插件的情况下被定位并修正；不将责任归于 StartAllBack 或图标尺寸插件。

修复版首次点击也能清除前版留下的状态，后续点击保持未选中：

```text
1797750 toggle-before   checked=1 pressed=0 hover=1 replacement=0 native=0
1797750 toggle-replaced checked=0 pressed=0 hover=1 replacement=0 native=0
1797750 release-after   checked=0 pressed=0 hover=1 replacement=1 native=0
1799640 exit-after      checked=0 pressed=0 hover=0 replacement=1 native=0
```

- 实际部署：Windhawk CLI 成功编译并热更新 `local@ryjs-launchpad_0.2.1_857961.dll`，模块启用。没有重启 Explorer，没有新增模块，没有更换 ID。两阶段管理员脚本已以 `phase=fix,state=ready` 正常完成，不再等待或超时回退。
- 已安装源码与仓库生成源码 SHA-256 相同：`E1A6A623F0341F41A6DCCF4258BF112C21FBF72A3210C4902DE0ADEF0859159F`。
- 用户收到“单击停留、移出、再次/快速点击、启动应用、Esc 关闭、Win 键、切换窗口返回”的验收清单后，整体回复：**“已正常，未再残留，启动台正常开关”**。未声称对每个键盘/窗口操作分别取得独立日志。
- 真实点击日志多次记录释放后 `checked=0,pressed=0`，移出后 `hover=0`；没有固定延时隐藏，也没有禁用所有悬停。
- 本轮重跑 `Build.ps1 -Verify -Smoke` 通过；功能 99 项、当前原数据副本存储 44 项、Win32 兼容路径 29 项、Python 备份 5 项均通过。Win32 模拟测试并不作为 XAML 修复的证据。
- 本轮备份的 16 个持久化数据文件（含用户自定义名称）最终逐项 SHA-256 相同，丢失或变化数为 0；运行日志不计入持久化布局文件。

证据位于忽略目录 `build/start-debug-20261118/`：`baseline-click.log`、`fixed-clicks.log`、`update-status.json`、`diagnostic-compile.log`、`fix-compile.log`、`installed-before.wh.cpp`、`data-before/`、`data-integrity-final.json`。日志仅记录事件和状态，单文件上限 1 MiB。

## 5. 其他功能仍需本机验收

本机开始按钮残留现已获得上述真实事件对比和用户确认；不同 Windows 版本或原生开始菜单配置未另做覆盖验证。

分组须鼠标实测拖标题、双轴拉伸、碰撞拒绝、Enter 保存、Esc 取消、缩小后滚动及重新打开；应用须实测重命名 Enter/Esc、恢复原名、完整名悬停提示和重扫/重启。DWM 模式还需检查实际明暗背景变化；其采样限制如上。

尚存限制为：上述分组、名称的完整人工验收及 DWM 最终合成背景采样限制。当前本机开始按钮异常已经实测确认消失；已执行的隔离测试无失败，持久化数据校验无变化。
