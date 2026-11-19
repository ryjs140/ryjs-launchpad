# 推送与投稿操作手册

状态：**本地已就绪，外部提交被两个环境问题阻塞**（见第 1 节）。本文件给出修复后可直接执行的命令。

---

## 1. 两个阻塞项

### 1.1 GitHub 凭据不是 ryjs140

本机 Windows 凭据管理器中存的 GitHub 凭据属于 **`purewaterg`**：

```
Target: LegacyGeneric:target=git:https://github.com
User:   purewaterg
```

官方规则要求 `@github` 必须与 PR 作者一致，而本插件声明的是
`https://github.com/ryjs140`。用 `purewaterg` 的凭据推送会以错误身份提交，
或在没有权限时被拒绝。**在你把凭据换成 ryjs140 之前，不要推送。**

### 1.2 `github.com` 走本地代理不通

当前环境变量：

```
HTTP_PROXY=http://127.0.0.1:17766
HTTPS_PROXY=http://127.0.0.1:17766
```

实测：

| 地址 | 走代理 | 直连 |
| --- | --- | --- |
| `api.github.com` | 200 | — |
| `github.com` | **000（代理返回 502 CONNECT tunnel failed）** | **200** |
| `raw.githubusercontent.com` | 301 | — |

也就是说 `git push` 会先撞上代理再失败。推送时绕过代理即可。

---

## 2. 修复步骤

### 2.1 换成 ryjs140 的凭据

```bash
# 1) 清掉现有 github.com 凭据
printf "protocol=https\nhost=github.com\n\n" | git credential reject

# 2) 确认已清空（应输出空）
printf "protocol=https\nhost=github.com\n\n" | git credential fill 2>&1 | head -3

# 3) 下一次推送时重新登录（浏览器弹窗），必须是 ryjs140
```

也可以在「Windows 凭据管理器 → 普通凭据」里删除 `git:https://github.com`，再执行一次推送让 Git Credential Manager 重新登录。

验证身份：

```bash
printf "protocol=https\nhost=github.com\n\n" | git credential fill 2>&1 | grep '^username='
# 期望：username=ryjs140
```

若你的 GitHub 账户开启了 2FA，GCM 会走浏览器 OAuth，登录页显示的账号必须是 **ryjs140**。

### 2.2 绕过代理

每次需要联网到 `github.com` 的命令前执行：

```bash
unset HTTP_PROXY HTTPS_PROXY http_proxy https_proxy
```

或对单条命令：

```bash
git -c http.proxy= -c https.proxy= push origin main
```

---

## 3. 推送本仓库

本地已有两个新提交，位于 `8c9a9a2` 之上：

```
1447aac Release 1.0.0 for the Windhawk mod library
c9f261f Move development source to mods/local/ryjs-launchpad
```

```bash
cd D:/Project/desktop-enhanced/ryjs-launchpad-public
git log --oneline -3
git push origin main
```

推送后确认：<https://github.com/ryjs140/ryjs-launchpad> 的 HEAD 是 `1447aac`。

---

## 4. 投稿到官方仓库

### 4.1 Fork

在浏览器打开 <https://github.com/ramensoftware/windhawk-mods> → Fork → 所有者选 `ryjs140`。
（本机没有 `gh` CLI，用网页最省事；如已安装可 `gh repo fork ramensoftware/windhawk-mods --clone=false`。）

### 4.2 分支与唯一文件

```bash
unset HTTP_PROXY HTTPS_PROXY http_proxy https_proxy
git clone https://github.com/ryjs140/windhawk-mods.git
cd windhawk-mods
git checkout -b add-ryjs-launchpad

cp D:/Project/desktop-enhanced/ryjs-launchpad-public/release/mods/ryjs-launchpad.wh.cpp \
   mods/ryjs-launchpad.wh.cpp

git add mods/ryjs-launchpad.wh.cpp
git status --short          # 必须只有一个新增文件
git commit -m "Add RYJS Launchpad"
git push origin add-ryjs-launchpad
```

### 4.3 创建 PR

- base：`ramensoftware/windhawk-mods:main`
- head：`ryjs140:add-ryjs-launchpad`
- 标题：`Add RYJS Launchpad`
- 正文：整篇粘贴 `release/PR_DESCRIPTION.md` 的内容

正文里已经写明：本插件是基于 Fullscreen Start 与 Seamless Launchpad 持续开发的衍生作品，由 Fangyu Guo (ryjs) 维护，未声称整库原创。

### 4.4 审核流程

```
/ai-review            → 等自动审查（可能 20–30 分钟）
（逐条判断 → 修复成立项 → 再 /ai-review）
/ready-for-reviewer   → 进入人工审核队列，标签变为 waiting-for-reviewer
```

- 对不成立的意见，在该评论下回复技术依据，不要为通过审核而改正常代码。
- 每次 `push` 后都要重新 `/ai-review`，否则人工审核会退回。
- 合并前对外只能说"已提交，等待审核"。

---

## 5. 投稿文件校验值

| 项 | 值 |
| --- | --- |
| 文件 | `release/mods/ryjs-launchpad.wh.cpp` |
| 行数 | 16,326 |
| 字节 | 755,461 |
| SHA256 | `9f238d3a296afb3e3b3ecbd881303f436f68102196a7dfa73a7a6c20a7a5b933` |

放入 fork 后可复核：

```bash
sha256sum mods/ryjs-launchpad.wh.cpp
```

---

## 6. 已经不需要再做的事

- 版本、作者、版权、readme、目录结构：已完成并提交。
- 官方校验器 0 errors、三架构编译通过、本地 x64 零警告：已通过。
- 发布文件与仓库源码一致：已用 `git archive HEAD` 重新生成验证，哈希相同。

## 7. 仍属"待人工验收"的部分

见 `release/MANUAL_ACCEPTANCE.md`。这些不影响投稿，但 PR 说明里已如实写明未在真实桌面验证。
