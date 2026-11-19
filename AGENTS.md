# RYJS development

Work on this launcher only. Preserve upstream author and MIT notices.

- Edit `mods/local/ryjs-launchpad/` and `dev/` sources; regenerate the root single-file mod with `dev/generate_resources.py` or `dev/Build.ps1`.
- Do not commit user layouts, settings, snapshots, credentials or build output.
- Do not back up or modify the user's desktop layout during development. Use isolated fixtures under `build/` for tests.
- Preserve existing group membership, names, icons and order unless the user actively edits them. Reading failures must never overwrite layouts.
- Run `dev/Build.ps1 -Verify -Smoke` for relevant rendering/runtime changes and official checks for releases. Report build, isolated and live verification separately.
- Do not install other Windhawk mods or use the inherited full-setup deployment workflow.
- The user has authorized pushing this project to `https://github.com/ryjs140/ryjs-launchpad`.
