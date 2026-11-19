# 构建记录（1.0.0）

日期：2026-11-19
产物：`ryjs-launchpad.wh.cpp`（仓库根）与 `release/mods/ryjs-launchpad.wh.cpp`

| 项 | 值 |
| --- | --- |
| 版本 | 1.0.0 |
| 行数 | 16,525 |
| 字节数 | 746,747 |
| SHA256 | `d6beba2020cfd0de1e9bf262f196288d159df8514ba85bb3f9d6b7dcf746ea7e` |
| 架构 | x86-64（官方编译脚本另测 x86 / arm64） |

## 输入

生成器读取下列文件；任一变化都会改变产物。

| 文件 | Git 对象哈希 |
| --- | --- |
| `ryjs-project.json`（版本唯一来源） | `d37c74ddfd81777ef8e3a2c1a066bee7dc78c088` |
| `dev/settings-extension.json`（设置字段来源） | `c1000bdf0d1fff8c5b805f9af9beccf9ae207da0` |
| `dev/strings.zh-CN.json`（中文资源） | `f36c8bf31c23c22c88e2b1c614c1fe02c906ce11` |
| `dev/full_backup_worker.ps1`（内嵌备份脚本） | `af687151c11ba046b68f647d0bd0ad6f406341da` |
| `mods/local/ryjs-launchpad/ryjs-launchpad.wh.cpp`（开发主文件） | `e4d8796b489ff309d9490167fbf0574505a4607f` |

开发主文件通过 `#include "..."` 引入 18 个分片（`launchpad_*.h`、`launchpad_*.inc`、`ryjs_data.h`），全部位于 `mods/local/ryjs-launchpad/`，均已纳入版本管理。唯一例外是 `preview_api.h`：它只属于 `dev/preview`，生成时会替换为一行注释，因此产物中本地 `#include` 计数为 **0**。

## 生成命令

```powershell
py -3.12 dev/generate_resources.py
```

生成器对交付文件固定使用 LF 写入（`newline='\n'`）。原因：Git 在 Windows 上按 LF 存库，若工作副本是 CRLF，"本地文件哈希"与"GitHub 上的文件哈希"就会不一致，本记录里的 SHA256 无法从 GitHub 复核。现在工作副本、提交 blob 与 GitHub 上的文件是同一串字节。

注意：在 `core.autocrlf=true` 的机器上重新 clone 时，工作副本会被 Git 转成 CRLF，`sha256sum` 会得到另一个值——那是换行规范化，不是内容差异。要核对请以 GitHub 上该文件的原始字节为准：

```bash
curl -sL https://raw.githubusercontent.com/ryjs140/ryjs-launchpad/main/ryjs-launchpad.wh.cpp | sha256sum
```

等价入口（额外编译预览与 x64 模块）：

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File dev/Build.ps1 -Verify
```

需要 PyYAML 6.0.3（`dev/requirements.txt`）。注意：`python` 若指向未安装 PyYAML 的解释器会直接失败，本记录使用 `py -3.12`。

## 校验方法

```bash
# 1. 两条发布路径必须逐字节一致
cmp ryjs-launchpad.wh.cpp release/mods/ryjs-launchpad.wh.cpp && echo identical

# 2. 哈希
sha256sum ryjs-launchpad.wh.cpp release/mods/ryjs-launchpad.wh.cpp

# 3. 从 Git 索引重建，证明只靠已入库文件即可复现
rm -rf /tmp/ryjs-staged && mkdir -p /tmp/ryjs-staged
git checkout-index -a --prefix=/tmp/ryjs-staged/
cd /tmp/ryjs-staged && py -3.12 dev/generate_resources.py
sha256sum ryjs-launchpad.wh.cpp
```

## 结果

| 检查 | 结果 |
| --- | --- |
| 连续两次生成（工作区） | 哈希相同 `9f238d3a…` |
| 从 Git 索引重建（`git checkout-index` + 重新生成） | 哈希相同 `9f238d3a…` |
| 根文件与 `release/mods/` 副本 | `cmp` 无差异 |
| 产物中本地 `#include` | 0 |
| 产物中系统 `#include` | 72 |

三项检查均对最终产物执行，无遗留例外。
