# 发布审计报告（安全、质量、兼容性）

对象：RYJS Launchpad 1.0.0 · `ryjs-launchpad.wh.cpp`（16,326 行，755,461 字节，SHA256 `9f238d3a296afb3e3b3ecbd881303f436f68102196a7dfa73a7a6c20a7a5b933`）
审计日期：2026-11-19
结论：**可以提交**，前提是接受第 8 节的"未实机验证"清单并在提交前决定是否补截图。

---

## 1. 范围

| 项目 | 内容 |
| --- | --- |
| 审计对象 | 打包后的单文件 mod（16,308 行，754,417 字节） |
| 注入目标 | `explorer.exe`（`@include explorer.exe`，唯一目标） |
| 架构 | `@architecture x86-64` |
| 依赖 | 无第三方库、无外部进程依赖（可选 helper 除外，默认不安装） |
| 未审计 | `dev/` 下的预览程序与测试夹具（不进入提交文件；仅作为验证手段） |

---

## 2. 注入与系统安全

### 2.1 注入范围

`@include explorer.exe` 一条，没有 `@exclude`，也不会注入 `StartMenuExperienceHost.exe`、`SearchHost.exe`、`dwm.exe` 等其它进程。进程内主动退出条件：

- `IsFolderWindowsProcess()`：命令行含 `/factory`、`-embedding`、`/separate` 时立即返回 `FALSE`，避免在"文件夹窗口独立进程"里重复接管开始菜单。
- 另有 `WindhawkSeamlessLaunchpad` / `WindhawkFullscreenStartPlus` 窗口检测，避免与上游 mod 同时接管；检测到即退出，**不会**去禁用或卸载对方的 mod。

### 2.2 钩子与系统回调清单

| 机制 | 目标 | 是否可缺失 | 行为 |
| --- | --- | --- | --- |
| 符号钩子 `XamlLauncher::ShowStartView` | `twinui.pcshell.dll` | 否（缺失则 mod 返回 `FALSE` 不加载） | 接管开始菜单显示请求；按设置决定打开网格、原生开始菜单或外部程序 |
| 6 个符号钩子 `ExperienceToggleButton::*` | `Taskbar.View.dll` 或 `ExplorerExtensions.dll` | 是（`SYMBOL_HOOK` 标记 optional） | 仅修正开始按钮的可见/选中状态；符号缺失时静默跳过，其余功能不受影响 |
| `Wh_SetFunctionHook(LoadLibraryExW)` | 进程内 | 是 | 仅在任务栏模块尚未加载时安装；先调用原函数并**原样返回其结果**，只在检测到目标模块后再启动钩子安装 |
| `SetWindowsHookExW(WH_KEYBOARD_LL)` | 系统级键盘 | 是 | 只观察 Win 键单击；**每条路径都 `CallNextHookEx`**，从不吞键、不修改输入；独立线程带消息循环，退出时 `UnhookWindowsHookEx` |
| `SetWinEventHook(EVENT_OBJECT_CREATE/SHOW)` | **仅当前进程**（`GetCurrentProcessId()`） | 是 | 发现任务栏开始按钮窗口并子类化；不是全局事件钩子 |
| `SetWindowSubclass` | 开始按钮窗口 | 是 | 仅当按钮动作设为"启动器/程序"时接管完整左键手势；右键与原生菜单路径保持透传 |

补充说明：

- `Wh_ApplyHookOperations()` 只在"任务栏模块晚于 mod 加载"的场景调用（`LoadLibraryExW` 观察路径或 1 s 惰性定时器路径各一次），正常启动路径不调用，符合官方对性能的告诫。
- 所有 XAML 相关回调内部用 `try/catch(...)` 包裹再调用原函数，异常不会穿出到任务栏 UI 线程。
- 键盘钩子的判定只读 `vkCode` 与 `GetAsyncKeyState(VK_SHIFT)`，不记录按键内容、不落盘。

### 2.3 初始化失败与卸载

| 场景 | 行为 |
| --- | --- |
| 另一个 Launchpad 在运行 | 返回 `FALSE`，不安装任何钩子 |
| 存储迁移/校验失败 | 返回 `FALSE`，保留既有数据，不写盘 |
| `ShowStartView` 符号解析失败 | 返回 `FALSE` |
| 单实例互斥体已存在 | 返回 `FALSE`，释放句柄 |
| 工作线程创建失败 | 返回 `FALSE`，关闭事件与互斥体 |
| 卸载 | `Wh_ModUninit` 先停键盘钩子线程，再让窗口线程退出并 `WaitForSingleObject`，最后关闭设置窗口、释放互斥体；钩子由 Windhawk 引擎统一摘除 |

**风险项（低）**：`Wh_ModUninit` 中对键盘线程的等待是 `do { PostThreadMessage } while (WaitForSingleObject(..., 100) == WAIT_TIMEOUT)`，没有上限；窗口线程用 `INFINITE` 等待。若线程卡死，禁用/卸载会挂住。实际线程只做消息泵，触发条件苛刻，未观察到发生。**未改**，因为改成超时可能在半卸载状态留下窗口；记录为已知风险。

---

## 3. 数据与隐私

| 检查项 | 结果 | 证据 |
| --- | --- | --- |
| 遥测 / 上报 | 无 | 全文件无 HTTP 上报路径；唯一网络代码在 `DownloadWeb` |
| 网络请求 | 仅"网页入口图标"，用户显式添加/刷新时触发 | `launchpad_web_icons.inc`；代码注释明确 "Fetch only for an explicit add/update. No periodic networking or third-party proxy." |
| 请求约束 | 3 s 连接/发送/接收超时；单次总时长上限 15 s；响应体上限 2 MB；禁用 Cookie；禁止自动登录；禁止 HTTPS→HTTP 降级；最多 5 次重定向；URL 含用户名/密码直接拒绝 | `DownloadWeb` |
| 上传 | 无 | 只有 `WinHttpSendRequest(..., WINHTTP_NO_REQUEST_DATA, ...)` |
| 硬编码私有路径 / 用户名 / 令牌 | 未发现 | 全文件正则检索 `X:\Users\...`、`/Users/`、`api_key`、`token=`、`password` 均无命中 |
| 数据落盘位置 | 仅 mod 私有存储目录（`Wh_GetModStoragePath`） | `DataRoot()`、`StoragePath()` |
| 注册表 | 只在首次迁移时**读取** `HKLM/HKCU\SOFTWARE\Windhawk\Engine\Mods\<旧 id>\Settings` | `PrepareStorage` |
| 旧数据 | 只读；不修改、不删除旧 mod 的目录 | 同上，注释与实现一致 |
| 日志内容 | `Wh_Log` 只输出阶段名与计数；`start-state-trace.log` 只记录按钮状态位，上限 1 MB | `StartStateTrace` |
| 备份文件 | 只写到用户选择的路径；导入只恢复数据，**不执行**备份内的源码/DLL/注册表 | `launchpad_full_backup.inc`、内嵌 worker |

结论：无隐私阻碍。建议在 mod readme 的 "Data and network" 一节保持现有披露（已写入）。

---

## 4. 稳定性

| 场景 | 处理方式 | 状态 |
| --- | --- | --- |
| 连续展开/收起 | 帧调度按实际时间推进，关闭时停止帧计时器 | 离屏与逻辑测试通过 |
| 动画中途反转 | 缓动基于 elapsed-time，可从当前进度反向 | 测试覆盖 |
| 搜索与应用启动 | 沿用上游路径；新增入口走 `ShellExecute`/关联打开 | 逻辑测试通过，真实启动未逐一验收 |
| 分组编辑与保存 | 编辑会话内不入库；Enter/保存统一写盘，Esc 逐级取消；重叠拒绝提交 | 隔离测试通过 |
| 布局写盘 | 临时文件替换 + `.bak`；未知 schema 转只读，不覆盖 | 单测覆盖 |
| 多显示器切换 | `WM_DISPLAYCHANGE`、`SPI_SETWORKAREA` 后重算工作区 | 代码路径存在；混合刷新率未实测 |
| 高 DPI | 线程 `DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2`，处理 `WM_DPICHANGED` | 4 种分辨率离屏验证通过 |
| 锁屏 / 睡眠 / 恢复 | 未做专门处理；网格随会话切换关闭 | **未验证** |
| 启用 / 禁用 / 卸载 | 见 2.3 | 逻辑审查通过，未在真实 Windhawk 中反复装卸 |

---

## 5. 性能

| 检查项 | 结果 |
| --- | --- |
| 长时间 CPU 占用 | 空闲时不跑帧计时器；无忙等循环 |
| 持续重绘 | 仅在动画/交互期间按刷新率提交；静止不重绘 |
| GPU 资源释放 | `ComPtr` 管理；尺寸变化或失败时 `Reset()` 并回退分层窗口路径 |
| 高频轮询 | 仅一个 1 s 定时器等待任务栏模块出现，装好钩子或首次发现模块即 `KillTimer` |
| 设置窗口定时器 | 15 ms，仅在设置窗口打开期间存在 |
| 重复截图/模糊 | 打开时抓一次背景；模糊结果缓存为位图复用 |
| 后台线程 | 应用枚举线程、键盘钩子线程、电源操作线程（按需）、网页图标 worker（按需，最多并发受控） |
| 日志体积 | `performance.csv` 与 `start-state-trace.log` 均设上限，前者超 4 MB 轮转 |

---

## 6. Windhawk 规范核对

| 项 | 要求 | 实际 | 结果 |
| --- | --- | --- | --- |
| `@id` | 小写、数字、连字符，全局唯一 | `ryjs-launchpad`；官方库 946 个 mod 中不存在（`mods/ryjs-launchpad.wh.cpp` 返回 404） | 通过 |
| `@name` | 可本地化 | `RYJS Launchpad` | 通过 |
| `@version` | 递增 | `1.0.0`（首次提交，无需与历史版本比较） | 通过 |
| `@author` | 3–28 字符 | `Fangyu Guo (ryjs)`（17 字符），仅当前开发者 | 通过 |
| `@github` | 必须与 PR 作者一致 | `https://github.com/ryjs140`，仓库 `ryjs140/ryjs-launchpad` 为其本人所有 | 通过 |
| `@license` | 声明许可证 | `MIT` | 通过 |
| `@include` | 合理范围 | 仅 `explorer.exe` | 通过 |
| `@architecture` | 与实际一致 | `x86-64` | 通过 |
| `@compilerOptions` | 链接所需库 | 22 个库，含 `-lwinhttp`、`-lruntimeobject`、`-loleaut32` | 通过 |
| 设置语法 | 1.7.3 只接受 `$name`/`$description`/`$options` | 55 项设置，**未使用** `$format`/`$float`/`$min`/`$max`/`$showIf`/`$hideIf`/`$dynamicSelect` | 1.7.3 与 2.0 均可解析 |
| readme 图片域名 | 仅 `i.imgur.com`、`raw.githubusercontent.com` | 已移除上游截图，当前无图片 | 通过（待补自有截图） |
| 提交文件数 | 单文件 `mods/<id>.wh.cpp` | 单文件 | 通过 |

### 官方校验器结果

```
py -3.12 .github/pr_validation.py mods/ryjs-launchpad.wh.cpp ryjs140
Test run: Validating single file...
（无输出 = 0 errors，0 review items）
```

校验器版本取自 `build/library-upstream`（官方仓库 sparse checkout，2026-11-18）。`ryjs140` 作为 PR 作者传入，与 `@github` 一致。

### 编译结果

| 检查 | 命令来源 | 结果 |
| --- | --- | --- |
| 官方三架构编译 | 官方 `scripts/compile_mod.py`（x86 / x64 / arm64） | `RESULT {"check":"ryjs-1.0.0","ok":true,"problems":[]}` |
| 本地 x64 严格编译 | Windhawk Compiler clang 20.1.3 + 2.0.0 x64 导入库 | 退出码 0，**0 warning / 0 error**，产物 2,065,920 字节 |

---

## 7. 本次为发布所做的修改

未改动任何功能逻辑；全部为元数据、署名、文档与一处校验器要求的排版修改。

| # | 文件 | 修改 | 原因 |
| --- | --- | --- | --- |
| 1 | `mods/local/ryjs-launchpad/launchpad_group_positions.inc` | 把一行 192 字符、含 5 条语句的 `if(valid){...}` 拆成多行 | 官方校验器报错 "Line packs 5 statements into 192 characters"，属**阻断项** |
| 2 | 开发源 `.wh.cpp` 元数据 | `@description` / `@description:zh-CN` 重写为覆盖实际功能 | 原描述停留在 3.0 时代，未提手动入口、主题、自由布局 |
| 3 | 同上 | `@author` 改为 `Fangyu Guo (ryjs)` | 按项目所有者确认：该字段只写当前开发者；上游作者在 readme/README/LICENSE/来源报告中按角色署名 |
| 4 | 同上 | 删除 `@donateUrl https://ko-fi.com/DavidHiFi` | 打赏页属于上游作者，fork 不应代为收款 |
| 5 | 同上 + `ryjs-project.json` | `@version` 0.2.1 → `1.0.0` | 首次公开提交使用稳定版本号；`ryjs-project.json` 是版本唯一来源，`generate_resources.py` 会写入元数据 |
| 6 | 开发源 readme | 顶部改为三方角色表（Rafael → David → ryjs），新增 `## Copyright notices` | 明确区分原始代码版权、后续修改版权与当前维护者身份 |
| 7 | 开发源 readme | 新增 `## Compatibility`、`## Data and network` | 版本兼容、网络行为与隐私必须写明 |
| 8 | 开发源 readme | 移除指向 David 仓库的上游界面截图；把"曾叫 Fullscreen Start Plus"改写为"从上游存储一次性只读迁移" | 原截图不代表本 mod；原文案与 RYJS 的实际历史不符 |
| 9 | 开发源 readme + `LICENSE` / `release/LICENSE` | Rafael 的声明改为不带年份的 `Copyright (c) Rafael Mendes`，并注明"上游未声明年份，不代为推测"；David 的 `Copyright (c) 2026 DavidHiFi` 逐字保留 | 原先写的 2026 是推测值，违反"不得推测原作者版权年份" |
| 10 | 目录结构 | 开发源移至 `mods/local/ryjs-launchpad/ryjs-launchpad.wh.cpp`；上游快照移至 `reference/upstream/` | 自有代码与上游参考材料分开，避免把别人的文件混在发布树里 |
| 11 | `release/` | 新增来源报告、审计、检查清单、PR 说明、README、CHANGELOG、人工验收清单、复现脚本 | 发布材料与源码分离，不进入官方 PR |

修改后重新生成单文件、重跑官方校验与三架构编译，均通过。

---

## 7.1 源码与发布文件的一致性

| 检查 | 方法 | 结果 |
| --- | --- | --- |
| 单文件可复现 | 删除根文件后由 `py -3.12 dev/generate_resources.py` 重新生成 | 与提交前逐字节相同（`cmp` 无差异） |
| 根文件 = 提交文件 | `sha256sum` 比对 | 两者均为 `9f238d3a296afb3e3b3ecbd881303f436f68102196a7dfa73a7a6c20a7a5b933` |
| 打包无残留 include | 生成脚本内置断言：仍存在本地 `#include "..."` 即抛错 | 通过 |
| 未跟踪源码 | `git status` + 依赖检查 | 四个原未跟踪文件已 `git add`，生成不再依赖工作区私有文件 |

重新生成的命令：

```powershell
py -3.12 dev/generate_resources.py
# 或
powershell -NoProfile -ExecutionPolicy Bypass -File dev/Build.ps1 -Verify
```

---

## 8. 未验证事项（不得声称为已通过）

可执行的验收步骤见 [`MANUAL_ACCEPTANCE.md`](MANUAL_ACCEPTANCE.md)，其中所有条目当前状态均为"待人工验收"。

| # | 事项 | 影响 | 建议 |
| --- | --- | --- | --- |
| 1 | 开始按钮视觉状态复位（StartAllBack Win32 按钮子类化、原生 XAML `ExperienceToggleButton`） | 若失效，表现为按钮残留选中外圈，不影响启动 | 在装有 StartAllBack 与原生任务栏的两台机器上各点 5 次开/关 |
| 2 | 混合刷新率多显示器（如 60 Hz + 144 Hz） | 帧调度可能偏向主屏周期 | 双屏实机观察动画是否均匀 |
| 3 | 网页图标抓取的真实站点行为 | 图标缺失或失败时回退地球占位 | 添加 2–3 个真实网址验证 |
| 4 | 带参数启动、`.rdp` 连接 | 参数传递或凭据提示可能不符预期 | 各测一次 |
| 5 | Windows 10 | 任务栏 XAML 模块不存在，仅 Win32 路径可用 | 明确标注"未在 Windows 10 测试" |
| 6 | 锁屏/睡眠恢复 | 未见异常但未验证 | 锁屏再解锁，检查网格与托盘 |
| 7 | 反复启用/禁用/卸载 mod | 见 2.3 的无限等待风险 | 连续 5 次禁用/启用 |
| 8 | mod 页面截图 | 官方列表无图 | 补 1 张真实界面图并推送到本仓库 `media/` 后，再在 readme 引用 |

---

## 9. 结论

- 代码层面无发布阻断项；唯一的官方校验错误已修复。
- 许可证与署名已补齐两处上游版权，符合 MIT 要求。
- 元数据符合官方规范，1.7.3 与 2.0 的设置语法均可用。
- 三架构编译通过，x64 零警告。
- 剩余风险集中在"实机交互"而非"代码正确性"，已在第 8 节逐条列出，可在 PR 说明中如实标注。
