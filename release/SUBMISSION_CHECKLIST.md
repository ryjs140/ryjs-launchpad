# 官方上架检查清单

对象：RYJS Launchpad 1.0.0 · 目标仓库 `ramensoftware/windhawk-mods`
状态日期：2026-11-19

图例：✅ 已完成 · ⚠️ 已完成但有限制 · ⬜ 待办（需要你确认或执行）

---

## 一、提交前必须成立的条件

| # | 条件 | 状态 | 证据 |
| --- | --- | --- | --- |
| 1 | 单文件、可独立编译，无本地 include | ✅ | `ryjs-launchpad.wh.cpp` 自包含；官方编译脚本三架构通过 |
| 2 | `@id` 合法且未被占用 | ✅ | `ryjs-launchpad`；官方库 946 个 mod 中无同名，raw 地址 404 |
| 3 | `@version` 为本版本实际状态 | ✅ | `1.0.0`，与 `ryjs-project.json`、`CHANGELOG.md` 一致；属首次提交，无历史版本可比 |
| 4 | `@author` 长度 3–28 且不冒认上游 | ✅ | `Fangyu Guo (ryjs)`（17 字符）；两位上游作者按角色署名于 readme/README/LICENSE/来源报告 |
| 5 | `@github` 与 PR 作者一致 | ✅ | `ryjs140`，仓库 `ryjs140/ryjs-launchpad` 归其所有 |
| 6 | `@license` 正确 | ✅ | `MIT`，与两个上游一致，无 copyleft 冲突 |
| 7 | 保留上游版权声明与许可文本 | ✅ | mod readme、仓库 `LICENSE`、`release/LICENSE` 三处；David 年份取自上游 LICENSE，Rafael 不推测年份 |
| 8 | `@include` 范围最小 | ✅ | 仅 `explorer.exe` |
| 9 | 设置块兼容 Windhawk 1.7.3 | ✅ | 仅用 `$name`/`$description`/`$options`；无 2.0 专属注解 |
| 10 | readme 图片来自允许域名 | ✅ | 当前无图片；补图时须用 `raw.githubusercontent.com` 或 `i.imgur.com` |
| 11 | 官方校验器 0 error | ✅ | `pr_validation.py` 无输出、退出码 0 |
| 12 | 官方编译脚本 0 problem | ✅ | `RESULT {..., "ok": true, "problems": []}` |
| 13 | 无遥测、无未披露网络请求 | ✅ | 仅网页图标抓取，已在 readme 披露 |
| 14 | 无硬编码私有路径/令牌 | ✅ | 全文件检索无命中 |
| 15 | PR 只含一个文件 | ✅ | `mods/ryjs-launchpad.wh.cpp` |

---

## 二、本地准备工作（本目录已完成）

- ✅ 发布源码：`release/mods/ryjs-launchpad.wh.cpp`（与仓库根 `ryjs-launchpad.wh.cpp` 逐字节一致，SHA256 `9f238d3a296afb3e3b3ecbd881303f436f68102196a7dfa73a7a6c20a7a5b933`）
- ✅ 开发源码已纳入版本管理：`mods/local/ryjs-launchpad/`（含 `launchpad_contrast.inc`、`launchpad_display_names.inc`、`launchpad_fly_in.h`、`launchpad_start_xaml.h`）
- ✅ 上游参考材料：`reference/upstream/`（不参与构建）
- ✅ 来源审计：`release/SOURCE_PROVENANCE.md`
- ✅ 审计报告：`release/RELEASE_AUDIT.md`
- ✅ 许可证：`release/LICENSE`、仓库根 `LICENSE`
- ✅ 变更记录：`release/CHANGELOG.md`
- ✅ 说明文档：`release/README.md`
- ✅ PR 说明：`release/PR_DESCRIPTION.md`
- ✅ 人工验收清单：`release/MANUAL_ACCEPTANCE.md`
- ✅ 构建记录（命令、校验方法、SHA256、输入哈希）：`release/BUILD_RECORD.md`
- ✅ 复现脚本：`release/tools/audit-provenance.py`

---

## 三、需要你决定或执行的事

| # | 事项 | 说明 |
| --- | --- | --- |
| ✅ 1 | 署名 | 已按你的决定改为 `@author Fangyu Guo (ryjs)`，`@github ryjs140`；两位上游作者在 readme/README/LICENSE/来源报告中按角色署名 |
| ✅ 2 | 四个未跟踪源码 | 已确认无敏感信息并 `git add`；发布文件可由仓库源码完整复现 |
| ⬜ 3 | 提交工作区改动 | 本轮只暂存发布相关文件，未 commit；建议按下面信息单独提交 |
| ⬜ 4 | 推送本仓库 | `git push origin main`；本轮**未**推送任何内容 |
| ⬜ 5 | 补 mod 截图（不阻塞提交） | 见 `release/MANUAL_ACCEPTANCE.md` 第 3 节；`build/preview-data/screenshots/` 里的离屏渲染含本机已装软件清单，不宜直接公开 |
| ⬜ 6 | 实机验收 | `release/MANUAL_ACCEPTANCE.md` 全部条目；其中开始按钮复位、网页图标获取各至少 5 次 |

建议提交信息：

```
Release 1.0.0 for the Windhawk mod library

- Version 1.0.0, @author "Fangyu Guo (ryjs)", upstream authors credited by role
- Correct copyright notices: keep DavidHiFi's year, do not assert one for Rafael Mendes
- Move development source to mods/local/ryjs-launchpad, upstream snapshots to reference/
- Add release/ with provenance, audit, checklist, PR text and acceptance list
- Split one over-long line rejected by the official validator
- No functional changes
```

---

## 四、提交步骤

### 1. Fork 并建分支

```bash
# 在 GitHub 上 fork ramensoftware/windhawk-mods 到 ryjs140/windhawk-mods
git clone https://github.com/ryjs140/windhawk-mods.git
cd windhawk-mods
git checkout -b add-ryjs-launchpad
```

### 2. 放入唯一文件

```bash
cp <本仓库>/release/mods/ryjs-launchpad.wh.cpp mods/ryjs-launchpad.wh.cpp
git add mods/ryjs-launchpad.wh.cpp
git status            # 确认只有一个新增文件
git commit -m "Add RYJS Launchpad"
git push origin add-ryjs-launchpad
```

### 3. 建 PR

- 标题：`Add RYJS Launchpad`
- 正文：直接粘贴 `release/PR_DESCRIPTION.md`
- 不要附带 `dev/`、`release/`、截图说明等额外文件

### 4. PR 流程（官方要求，不可跳步）

```
/ai-review          → 等待自动审查（可能 20–30 分钟）
（按意见修复 → 再 /ai-review）
/ready-for-reviewer → 进入人工审核
```

- 自动审查的每条意见要么修复，要么在评论里说明为何不采纳（官方明确允许"AI 说错了/超出范围/留待后续版本"）。
- 人工审核通过并合并后，才可对外说"已上架"。合并前对外只能表述为"已提交，等待审核"。

---

## 五、提交时不要做的事

- 不要把 `release/` 下的文档放进 PR；它们属于本仓库。
- 不要把根目录的 `ryjs-launchpad.wh.cpp` 和 `release/mods/` 版本同时放进 PR。
- 不要为了"看起来更独立"删除 DavidHiFi / Rafael Mendes 的版权行。
- 不要在 PR 中声称"独立开发"或"从零实现"。
- 不要 push 到 `DavidHiFi/davids-windhawk-mods`，也不要动上游仓库。

---

## 六、合并后的收尾

| # | 事项 |
| --- | --- |
| ⬜ | 确认 `https://windhawk.net/mods/ryjs-launchpad` 可访问，版本为 1.0.0 |
| ⬜ | 在 `CHANGELOG.md` 记录"已合并到官方库"的日期与 PR 号 |
| ⬜ | 后续版本更新走同一路径：只改 `mods/ryjs-launchpad.wh.cpp`，`version` 必须递增 |
