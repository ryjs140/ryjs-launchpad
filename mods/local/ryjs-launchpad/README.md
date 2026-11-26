# Lumiflow — development source

This directory holds the split development source of Lumiflow. The file
that goes into Windhawk is the generated single file in the repository root,
[`ryjs-launchpad.wh.cpp`](../../../ryjs-launchpad.wh.cpp); it already contains
every fragment below and needs no adjacent headers. The filename and the
Windhawk mod ID `ryjs-launchpad` are retained for build and storage compatibility;
the displayed product name is Lumiflow in English and 流光启动台 in Chinese.

```
ryjs-launchpad.wh.cpp        main source; includes the fragments
launchpad_*.h / *.inc        rendering, groups, entries, backup, i18n
ryjs_data.h                  storage migration and validation
helper/                      optional admin-key helper (not part of the mod)
```

Regenerate the single file after any edit here:

```powershell
py -3.12 dev/generate_resources.py      # or: dev/Build.ps1 -Verify -Smoke
```

`dev/generate_resources.py` rewrites the settings block from
`dev/settings-extension.json`, rebuilds the zh-CN string table, and packs the
fragments into the root file. Never edit the root file by hand: it is
generated.

Release material — provenance, audit, checklist, PR text — is in
[`release/`](../../../release). Upstream snapshots used by the provenance
script are in [`reference/upstream/`](../../../reference/upstream).

## Provenance

Lumiflow is a derivative work, not an independent rewrite:

- [Rafael Mendes](https://github.com/RafaelMendes1407) — Fullscreen Start
  (Launchpad style), MIT. The launcher, its rendering and the Start hook.
- [DavidHiFi](https://github.com/DavidHiFi) — Seamless Launchpad, MIT. The
  direct base: grid, search, folders, power dock, settings.
- Fangyu Guo (ryjs) — Lumiflow, MIT. The changes described in
  `release/CHANGELOG.md`.

Measured on code lines, about half of the current file is still verbatim
upstream code. See [`release/SOURCE_PROVENANCE.md`](../../../release/SOURCE_PROVENANCE.md)
for the numbers and the reproduction script.
