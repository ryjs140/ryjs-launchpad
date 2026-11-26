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
- Version: 1.5.10
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

Version 1.5.10 addresses the blocking lifecycle issue and the import note from
the latest review:

- `Wh_ModInit` no longer requires `Shell_TrayWnd` to exist. Windhawk calls it
  before Explorer creates the taskbar on sign-in and on every Explorer restart,
  so the previous check refused to load in the normal startup path. The
  Start-latch subclass is installed on demand by `ClearStartButtonLatch`, with
  its existing synchronous fallback when it cannot be installed.
- `Wh_ModUninit` removes the taskbar subclass a second time, after the launcher
  thread is joined and with a fresh `Shell_TrayWnd` lookup. A `HideNow` that
  passed the stopping gate just before it closed can no longer reinstall the
  subclass and leave Explorer calling into an unloaded image.
- Import keeps this PC's private `ui-settings.ini` overrides of `program`,
  `winKeyAction`, `shiftWinKeyAction` and `startButtonAction` when the Windhawk
  value they were saved against is still current, so the readme's promise holds
  for values saved in the built-in settings window too.
- **Background website icon download** moved from the Power page to the Grid
  page's Website icons group; the readme now names that location and Windhawk's
  settings page.

Version 1.5.9 fixed the previous round: synchronous taskbar-subclass Start
reset, optional background website downloads, backup-import disclosure, local
shortcut preservation, per-window D2D ownership, and material file-filter
termination. Device-loss teardown now also releases every app bitmap; web-icon
completion no longer waits on a worker found by reusable thread ID; group grid
coordinates are capped; and legacy tint migration no longer performs per-frame
settings reads or writes in read-only mode. The earlier GDI/DIB,
settings-thread, material, parser and official line-length/callback-scope fixes
remain in this revision.

## Testing

Passed locally on the current source and the generated single-file submission:

- Official validator: 0 problems.
- Official compile script: 0 problems.
- Local x64 module and preview compilation with the Windhawk compiler: PASS,
  0 warnings.
- Feature checks: 602 checks, 0 failures; keyboard batch launch and website
  icon cache checks passed.
- Storage/phase-one checks: 25 and 37 checks, 0 failures; 5 Python backup tests
  OK.
- Full-backup checks: 28 checks, 0 failures; 14 Python full-backup tests OK.
- Offscreen smoke test: PASS (real app enumeration and bilingual rendering).

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

- Address the latest review by letting `Wh_ModInit` load before the taskbar
  exists, removing the taskbar subclass once more after the launcher thread is
  joined, keeping local private shortcut overrides across an import, and
  relocating the background website icon switch to the Grid page.

- Add synchronous taskbar-subclass Start reset, optional background website
  downloads, safer backup-import disclosure and local shortcut-setting
  preservation, per-window D2D ownership, and correct material file-filter
  termination.

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
