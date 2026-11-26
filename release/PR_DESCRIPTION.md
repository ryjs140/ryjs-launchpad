# Add Lumiflow (流光启动台), formerly RYJS Launchpad

## What this mod is

Lumiflow replaces the Start menu with a fullscreen, Launchpad-style grid of
apps. It is a derivative work of two MIT-licensed mods, and this is stated in
the mod readme, the repository `README.md`, `LICENSE` and
`release/SOURCE_PROVENANCE.md`:

| Project | Author | Role |
| --- | --- | --- |
| Fullscreen Start (Launchpad style) | Rafael Mendes | Original launcher, its rendering and the Start hook |
| Seamless Launchpad | DavidHiFi | Direct base: grid, search, folders, power dock, settings |
| Lumiflow (流光启动台), formerly RYJS Launchpad | Fangyu Guo (ryjs) — this submission | Expanded groups, free group layout, manual and web entries, themes, background materials, backup, Start-button state handling |

- Mod id: `ryjs-launchpad` (unchanged)
- Author / maintainer: Fangyu Guo (ryjs), https://github.com/ryjs140
- Source: https://github.com/ryjs140/ryjs-launchpad
- Version: 1.5.8
- License: MIT, with the upstream notices retained
- Inject into: `explorer.exe` only

I am not claiming an independent implementation. A substantial part of the
file is still upstream code, and the upstream settings are kept so existing
users keep their configuration. The provenance report in the source
repository has the numbers and a reproduction script.

## Name and upgrade path

The first submission was titled “RYJS Launchpad”. The product display name is
now **Lumiflow** in English and **流光启动台** in Chinese. The Windhawk mod id
remains `ryjs-launchpad`, and the storage directory
(`local@ryjs-launchpad`, `data-v1`), `ryjs-storage.ini`, `.ryjs.zip` backups,
`.ryjsm` material presets, integration mutex and window class names are
unchanged. Existing layouts, groups, icons, pinned entries and settings
therefore load without migration. This is a rename of the same submission, not
a separate mod.

## What changed since the 1.0.0 submission

- Expanded groups with free per-group geometry, resizing and overlap rejection;
  group editing, batch selection and deletion.
- Manual entries (`.exe`, `.lnk`, `.rdp`, Store apps, http/https) with aliases,
  arguments and working directories; website icon fetching with a local cache.
- Light, dark, system and custom themes with contrast-aware label colour;
  adjustable background material (solid, transparent, Windows 11 system
  acrylic, Mica, Mica Alt, legacy blur) with tint, dim, blur radius, material
  presets and import/export; liquid-glass tiles, hover glow and power warning
  animation.
- Built-in settings window in addition to the Windhawk settings page; bilingual
  UI, local font picker, tray menu and backup dialogs.
- GPU composition, background caching, animation scheduling and performance
  diagnostics; versioned storage with `.bak` files and read-only protection on
  unknown schemas; full `.ryjs.zip` backup/import; Start-button state handling
  for StartAllBack and the native XAML taskbar.

## Review fixes carried into this revision

The previous AI reviews raised the power-thread lifetime, settings-thread
lifetime, context-menu window-class unregistration, monitor-combo dangling
pointer, group-layout resource ownership, static teardown and Start-latch
lifetime issues. Those fixes are present in this revision. The 1.5.8 release
audit additionally fixed:

- Settings-window GDI/DIB release on `WM_NCDESTROY`, failed-creation cleanup
  and an unbounded join for the settings frame thread; a failed `PostMessage`
  no longer leaves the frame tick pending.
- Direct2D device-loss teardown now releases the material, panel-blur and
  group-title bitmaps with the old render target; material capture failures
  back off instead of retrying every frame.
- Bounds checks for the material blur setting, JSON nesting depth limits and an
  8 MiB cap for imported material files.
- The generated single-file submission now stays within the official
  4096-character line limit, and the four `Wh_Mod*` callbacks are outside the
  anonymous namespace in the official scope checker.

## Testing

Passed locally on the current source and the generated single-file submission:

- Official validator from the current upstream `main` (`7628a65`): 0 problems.
- Official compile script: 0 problems.
- Local x64 module and preview compilation with the Windhawk compiler: PASS,
  0 warnings.
- Feature checks: 612 checks, 0 failures; keyboard batch launch and website
  icon cache checks passed.
- Storage/phase-one checks: 25 and 35 checks, 0 failures; 5 Python backup tests
  OK.
- Full-backup checks: 28 checks, 0 failures; 12 Python full-backup tests OK.
- Offscreen smoke test: PASS (real app enumeration and bilingual rendering);
  three repeated `--smoke` runs exited 0.

Not verified on real hardware, and stated as such in the readme:

- Start-button visual reset on a real StartAllBack taskbar and on the native
  Windows 11 taskbar. The code paths exist and are isolated-tested; the
  on-screen result needs a live click.
- Mixed-refresh multi-monitor setups.
- Windows 10 (the mod targets Windows 11).
- `.rdp` entries and launch arguments end to end.
- Real Explorer restart, sign-out, shutdown and sleep/wake behavior. The
  corresponding shutdown/restart/restore paths were reviewed and isolated
  tested, but no destructive system test was run.

## Checklist

- [x] Single file, `mods/ryjs-launchpad.wh.cpp`, compiles standalone
- [x] `@id` lowercase, unique, matches the file name and remains unchanged
- [x] `@github` matches the PR author
- [x] `@license` MIT, upstream notices retained verbatim
- [x] `@include` limited to `explorer.exe`
- [x] No telemetry, no undocumented network access, no hardcoded paths
- [x] Settings compatible with Windhawk 1.7.3 and 2.0
- [x] Upstream authors credited; no claim of independent implementation

## Changelog

<!-- changelog:start -->

- Rename the display name to Lumiflow (English) / 流光启动台 (Chinese); keep
  the `ryjs-launchpad` mod id and all storage paths unchanged.
- Add expanded groups, free group layout, group editing, batch selection,
  manual entries and website icon caching.
- Add the background-material engine, presets, liquid-glass tiles, hover glow
  and power warning animation.
- Add the built-in settings window, full backup/import, bilingual UI and
  Start-button state handling.
- Fix settings-window GDI/DIB and thread-lifetime leaks, Direct2D device-loss
  resource retention, material capture retry pressure, unbounded material
  parsing and file imports.
- Update the generated submission for the official line-length and callback
  namespace checks.

<!-- changelog:end -->

## Mod authorship

This mod was created by:

- - [ ] The submitter, without AI assistance
- - [x] The submitter, with AI assistance
- - [ ] Claude
- - [ ] ChatGPT
- - [ ] Gemini
- - [ ] Another AI (please specify):
- - [ ] Other (please specify):
