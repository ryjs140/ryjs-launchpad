# AI 审核整改记录（PR #7661）

审核对象：`e44baa41bb6d3a977d0fe6cbfce9d5a070428869`（1.0.0 初投稿，16,326 行）
本轮产物：`d6beba2020cfd0de1e9bf262f196288d159df8514ba85bb3f9d6b7dcf746ea7e`（16,525 行）

状态标记：**已修复**＝代码已改并有本地验证；**已说明**＝核对后不改，附技术依据；**待人工**＝需要真实桌面/实机才能确认。

## 1. 与既有模组高度重合（维护策略）

**已说明。** 事实与 PR 正文一致：本模组是 Fullscreen Start 与 Seamless Launchpad 的衍生作品，已在模组 readme、仓库 `README.md`、`LICENSE`、`release/SOURCE_PROVENANCE.md` 中逐项披露来源、作者与许可，未声称独立实现。是否接受一个独立条目而非并入 `fullscreen-start`，属于维护者判断。README 仍要求先停用其他 Launchpad 模组。

## 2. 电源线程钉住模组且从不 join

**已修复。** `RunPowerAction` 不再使用 `GetModuleHandleExW`/`FreeLibraryAndExitThread`；句柄保存在 `g_powerThread`，重复触发时先探测是否仍在运行，`Wh_ModUninit` 中 `WaitForSingleObject(..., INFINITE)` 后关闭句柄。睡眠/休眠在唤醒后才返回，锁屏、注销、重启、关机立即返回，因此 join 不会拖住正常卸载。

## 3. 设置线程可能比 `Wh_ModUninit` 活得更久

**已修复。** 记录线程 ID；`CloseSettingsWindow` 置位 `g_settingsStop`、向窗口发 `WM_CLOSE`、循环 `PostThreadMessageW(id, WM_QUIT, ...)` 直到线程退出，再无界等待该线程。线程在 `Settings_Open()` 之后先检查停止标志再进入消息循环，因此“关窗请求早于窗口建成”的窗口期也会退出；清理逻辑集中在 `SettingsThreadTeardown()`。不存在超时后强行卸载的路径。

关于审核建议的 `INFINITE`：这里正是需要的语义。退出由线程自身的返回保证，超时后继续 `FreeLibrary` 才是崩溃来源。为保证不会卡住，线程内没有阻塞等待，也不持有需要主线程参与的锁。

## 4. 右键菜单窗口类从未注销

**已修复，且比建议更进一步。** 仅注销不能解决“更新时旧代码地址被复用”的根因：类在注册它的映像卸载后仍存活。现在类名由 `MenuClassName()` 生成，`RYJSLaunchpadMenu_<映像基址>_<tick>`，每个加载实例独占一个类名；`RegisterClassExW` 失败按失败处理；UI 线程退出时注销该类。

## 5. 显示器下拉框的悬空指针

**已修复。** `SetField::options` 改为 `std::vector<std::pair<std::wstring, std::wstring>>`；`Set_MakeFields()` 每次调用新建 `monitorLabels` 并用 `std::move` 交给字段，取消函数内静态累积。备份导出与设置窗口并发调用不再竞争同一个静态容器。

## 6. COM 引用泄漏与超出设备的设备上下文

**已修复。**

- `MakeLayout` 返回 `ComPtr<IDWriteTextLayout>`，赋值不再多一次引用。
- 阴影位图 `out.bitmap = std::move(bitmap)`，不再 `Detach` 后泄漏一份引用。
- 阴影用的 `ID2D1DeviceContext` 改为全局 `g_shadowContext`，由 `ResetTarget()` 重置。
- `Wh_ModUninit` 释放菜单的 `textFormat`/`boldFormat`/`hintFormat` 与 `g_groupTitleRenders`。
- 设备丢失重建时同时清空 `g_labelCache` 与 `g_groupTitleRenders`（旧设备的位图）。

## 7. 无效设置、未声明设置、数字读取方式

**已修复。**

- 移除 `accentColor`、`glowSize`、`groupColor`、`groupOpacity`、`hoverColor`、`selectionColor`：代码从未读取它们，内置设置窗口也主动隐藏。同时移除设置页、`dev/settings-extension.json` 与 `Set_MakeFields` 中的对应条目和隐藏逻辑。
- 已删除未声明的 `labelRadius` 读取（文字底板功能已移除）。
- `performanceLogging` 默认改为关闭。
- 关于 `Wh_GetIntSetting`：**经核对不改。** Windhawk 引擎 `LoadedMod::GetStringSetting` → `PortableSettings` → `RawItemToString` 会把 `REG_DWORD` 转换为十进制字符串，因此数字/布尔设置以字符串读取在本机（`HKLM\...\Settings` 中确为 `REG_DWORD`）与便携版（INI 存储）都能正确取到值；而 `Wh_GetIntSetting` 对缺失键返回 0，无法与真实 0 区分，才会把“未设置”误判成关闭。当前 `IntSetting` 保留“空字符串＝使用 schema 默认值”的语义。

## 8. 私有设置存储覆盖 Windhawk 设置

**已说明 + 已披露。** 保留私有存储是有意的产品设计（内置设置窗口无需管理员权限），覆盖规则是确定的：内置窗口保存的值优先；之后在 Windhawk 设置页再次修改同一项，该项以 Windhawk 为准（`launchpad_settings_store::Resolve` 用 baseline 比对实现）。按要求在 readme 顶部新增 **Settings precedence** 段落明确说明，并说明 Windhawk 设置页不会刷新显示内置窗口保存的值。

## 9. XAML 任务栏挂钩位置与模块声明

**已修复。**

- `LoadLibraryExW` 改用 `WindhawkUtils::SetFunctionHook` 挂到 `kernelbase.dll` 的导出，覆盖 WinRT 激活路径；同时删除 1 秒轮询定时器（`kTimerXamlHook`）与惰性安装中的定时器参数。
- `taskbarViewHooks` 上方补 `// Taskbar.View.dll, ExplorerExtensions.dll`；解析时先 `Taskbar.View.dll` 再 `ExplorerExtensions.dll`。

## 10. 网页图标联网行为

**已修复（文档）。** readme 的 Data and network 段落补充：图标声明可指向与所填页面不同的主机（CDN 或第三方）；为条目设置自定义图标后不会发起该请求。行为本身未改变。

## 11. 存储之外的文件（含持久化项）

**已修复。**

- 商店应用“以管理员身份运行”改为在存储目录相邻的 `tmp` 子目录创建唯一命名的 `.lnk`（`runas-<pid>-<tick>-<n>.lnk`），检查 `SetPath`/`Save` 结果，失败即删除并记录；成功后以前台等待方式启动并立即删除。不再写 `%LocalAppData%\RYJSLaunchpad`，也不再复用固定文件名。
- `Export-StartLayout` 输出改为 `tmp\pins-<pid>-<tid>.json`。
- 完整备份工作目录改为 `tmp\RYJSBackup-{GUID}\`。
- 新增 `ModTempPath()`，统一位于存储目录相邻的 `tmp`（不放进 `data-v1`，否则备份进程的工作目录会锁住数据目录导致导入失败——本轮测试即暴露该问题）。

## 12. 依赖 Windhawk 内部实现，便携版失效

**已修复。**

- 迁移：旧设置的注册表键缺失不再导致 `PrepareStorage` 失败——照常复制布局与图标，设置保留默认值并记录日志。
- 备份：内嵌 worker 不再 `reg.exe export`，也不再打包已安装源码与 DLL；设置由模组通过 Windhawk API 读好后放进 job 传给 worker，便携版可完整备份与恢复。
- 兼容旧备份：导入仍然只恢复 `data/` 与 `assets/`，旧包中的 `source/`、`binary/`、`metadata/settings.reg` 继续被忽略。

## 13. 一个坏数据文件就让模组彻底不加载

**已修复。**

- 空布局：`layout-*.txt` 为空视为有效空列表（取消固定最后一个应用就是这种状态）；`groups-v1-*.txt` 为空仍拒绝（必须带 schema 头）。
- 残留 `data-v1.pending`：删除后重试，不再永久拒绝。
- 校验失败：`Wh_ModInit` 改为以只读方式加载（`g_dataReadOnly`/`g_layoutReadOnly`），用户仍能打开设置窗口恢复备份；只有存储路径本身不可用才放弃加载。

## 14. `StripIconFrame` 越界读取

**已修复（首轮修复保留）。** `room()` 保证 `top+strip[1] < bottom-strip[3]` 且 `left+strip[0] < right-strip[2]` 才继续内缩，不会把一边推过对边。

## 15. README 缺截图

**已修复。** 新增 `media/preview.png`（隔离数据与合成背景下的离屏渲染，2560×1440），模组 readme 与仓库 `README.md` 均引用 `raw.githubusercontent.com/ryjs140/ryjs-launchpad/main/media/preview.png`。

## 可选建议

- **遗留调试 trace：** 已修复。`StartStateTrace` 由 `kStartStateTrace = false` 编译期关闭，不再在任务栏 UI 线程上写日志。
- **默认开启性能日志：** 已修复（默认关闭）。
- **多余的 `[[clang::no_destroy]]`：** 保留。这些容器析构只释放堆内存，但模组被卸载时 CRT 析构会在加载器锁下运行；`no_destroy` 与本模组其他全局一致，风险低于改动带来的收益。
- **死代码：** `g_settingsStop` 现在被真正读取；`Set_FormatHotkey`/`Hotkey` 字段类型与 `saving` 标志未清理（不影响行为，避免扩大改动面）。
- **预览代码混在交付文件里：** 保留。`SEAMLESS_PREVIEW` 分支在正式编译中不参与，且生成器要求单文件可直接编译。
- **早加载 `twinui.pcshell.dll`：** **核对后不改。** 早加载用于 `ShowStartView` 符号解析，属于现有功能路径；README 已写明与 StartAllBack 的实机验证状态，本轮不做未经实机验证的行为改变。
- **“Fill small icons”改动不立即生效：** 已修复。新增 `g_appsNormalized`，`WM_APP_RELOAD` 在该值变化时重新加载应用列表。
- **设备丢失：** 已修复（关闭文件夹淡出时恢复 `g_openFolder`；重建时清空两个标签缓存）。
- **设置窗口 15 ms 定时器：** 未改。属于绘制节流，未发现对卸载安全性的影响。
- **Windhawk 改动会丢弃内置窗口未保存的编辑：** 未改，行为保持现状。

## 本轮本地验证

| 检查 | 结果 |
| --- | --- |
| `dev/Build.ps1 -Verify -Smoke` | 通过（预览 + 正式 x64 模块，0 warning） |
| `dev/Test-Features.ps1` | 464 项，0 失败 |
| `dev/Test-Phase1.ps1 -SkipBuild`（原数据副本） | 存储 42 项、开始按钮 29 项、Python 备份 5 项，全部通过 |
| `dev/Test-FullBackup.ps1` | 原生桥接 25 项、worker 11 项，全部通过 |
| 官方元数据校验 | 除 `media/preview.png` 404（尚未推送）外无问题 |
| 官方三架构编译（`check.py compile`） | `ok: true` |

## 第二轮审核（针对 `578dd37`）

第二轮的结论是上一轮问题全部确认修复，并明确更正了它对数字设置读取方式的判断
（`RawItemToString` 会把 `REG_DWORD` 转成十进制字符串，因此字符串读取是正确的）。
剩余三条都是“由维护者判断”的事实说明，本轮按其中可执行的部分继续处理：

| 第二轮条目 | 处理 |
| --- | --- |
| 1. 与既有模组功能重合 | **已说明**（政策）。PR 正文已写明来源、作者与差异化范围；是否接受独立条目由维护者判断。已在 PR 下留一条技术说明。 |
| 2. 网页图标联网 | **已说明**（文档已覆盖）。readme 的 Data and network 段已写明第三方图标主机与“自定义图标即不请求”。 |
| 3. 首次迁移读取其他模组的存储与 Windhawk 注册表内部布局 | **已修复**。采纳审核给出的替代方案：迁移只复制布局/分组/固定项/图标，不再读取 `HKLM|HKCU\SOFTWARE\Windhawk\Engine\Mods\<id>\Settings`；旧 `ui-settings.ini` 复制为 `migration-original-ui-settings.ini` 留档而不应用，因此不静默继承任何设置，便携版也不受影响。备份导入的 `source_mod` 校验保留（它只比对备份清单里的 ID 字符串，不读注册表）。 |

可选建议中本轮采纳的：`Wh_ApplyHookOperations()` 在 `Wh_ModInit` 期间的空调用已删除（Windhawk 会在
`Wh_ModInit` 返回后自行应用）；用户取消 UAC 时不再弹“入口损坏”的错误，按 `ERROR_CANCELLED` 静默处理。
未采纳的：`no_destroy` 清理、`Hotkey`/`saving` 死代码清理、`SEAMLESS_PREVIEW` 拆分——都属于扩大改动面而无行为收益，
且第三轮修改后已无 `labelRadius` 读取。

## 仍未验证

以下只能在真实桌面确认，不得视为已验证：电源线程 join 在锁屏/休眠/重启时的实际行为；设置窗口打开期间禁用或更新模组；任务栏 XAML 钩子在 WinRT 激活路径下的实际命中；`tmp` 目录在真实 Windhawk 存储下的创建与清理；设备丢失（切换显卡/驱动重启）时的表现。
