# Lumiflow development

Work on this launcher only. Preserve upstream author and MIT notices.

- Product display name is 流光启动台 in Chinese and Lumiflow in English. Never show a mixed bilingual brand in one interface.
- Edit `mods/local/ryjs-launchpad/` and `dev/` sources; regenerate the root single-file mod with `dev/generate_resources.py` or `dev/Build.ps1`.
- Keep the compatibility identifiers `ryjs-launchpad`, `local@ryjs-launchpad/data-v1`, `ryjs-storage.ini`, `.ryjs.zip`, and the integration mutex unless a migration is explicitly designed and tested.
- For each user-facing update, advance the SemVer version in `ryjs-project.json`, add a matching `CHANGELOG.md` entry, and regenerate the standalone mod and preview version metadata before delivery.
- Do not commit user layouts, settings, snapshots, credentials or build output.
- Do not back up or modify the user's desktop layout during development. Use isolated fixtures under `build/` for tests.
- Preserve existing group membership, names, icons and order unless the user actively edits them. Reading failures must never overwrite layouts.
- Run `dev/Build.ps1 -Verify -Smoke` for relevant rendering/runtime changes and official checks for releases. Report build, isolated and live verification separately.
- Do not install other Windhawk mods or use the inherited full-setup deployment workflow.
- The actual GitHub remote is still `https://github.com/ryjs140/ryjs-launchpad`; the target display/repository name is Lumiflow. Do not rename the remote without explicit confirmation.
