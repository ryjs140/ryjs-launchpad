# Changes

## 1.0.0 — first public release

Version 1.0.0 marks the first submission to the Windhawk mod library.

### Unload lifecycle hardening (fifth push)

The fourth AI review found that three resources outlive `Wh_ModUninit`; this
push fixes them and folds in the defects found while checking the fix.

- **Folders and their GPU resources.** `ReleaseLauncherResources()` releases
  every holder of a `Folder` (group tables, gesture, transitions, edit
  snapshot, open and closing folder) before `ResetTarget()`, and clears the
  label caches first. A group held by a dozen globals no longer keeps the
  `ID2D1Device` tree alive after the device is gone. `LaunchpadMenu::Reset()`
  runs before the three DirectWrite formats are reset, so an open menu cannot
  leak its surfaces and node layout.
- **Settings window statics.** `g_setWin`, `g_setD2d`, `g_setDw` and
  `g_setWic` are `[[clang::no_destroy]]` with explicit release in
  `WM_NCDESTROY` and `SettingsThreadTeardown`. Explorer exiting with the
  settings window open no longer destroys COM objects on the exiting thread
  under the loader lock.
- **Queued start-button reset.** Each unit that runs code in this image holds
  one count: the call itself and the lambda it queues. `Wh_ModBeforeUninit`
  closes the gate while the hooks are still installed, `Wh_ModUninit` drains
  after they are removed, and the wait is bounded at 5 s. On timeout the image
  is pinned with `GET_MODULE_HANDLE_EX_FLAG_PIN` and the event is logged,
  instead of letting a queued callback run unmapped code. No fixed sleeps are
  used to mask the race.
- **Self-audit fixes.** `EnsureTarget()` returns failure when no D2D factory
  exists instead of dereferencing a null `ComPtr`; `WriteIconPng()` checks the
  buffer against `size * size`; `FreeSurface()` releases each handle
  independently when `CreateDIBSection` fails; the `EnumWindows` and
  `EnumDisplayMonitors` callbacks are `stdcall`, so i686 builds; dead functions
  and locals removed. A migration directory that fails validation now starts
  with an empty layout plus `migration-note.txt` instead of staying read-only,
  and read-only mode explains itself once.
- **Functional corrections.** Group titles no longer append an "unavailable"
  count, multi-select offers delete, and the inbox (ungrouped) group can be
  removed instead of being recreated.

Verification: `dev/Test-Features.ps1` 516 checks, storage 25, start button 29,
layout/backup 5, full backup 25 native and 12 worker, all 0 failures; official
`pr_validation` 0 problems and the official compile job clean; the submitted
file also compiles for i686, aarch64 and x86_64 with 0 warnings. The submitted
single file was regenerated from these sources (17,018 lines, SHA256
`b844b076…4fe760`).

### Rejected by the library check (fourth push)

The submitted file failed the repository's "Manual review" job with
`Failed to extract symbols, manual inspection required`. The cause was the
preview harness: the `ExperienceToggleButton` symbol block and the
`ShowStartView` symbol block were wrapped in `#ifndef SEAMLESS_PREVIEW`, and
the library's symbol extractor only resolves architecture macros, so it refused
the file.

- `dev/generate_resources.py` now resolves the preview conditionals when it
  packs the single file, exactly as a Windhawk build does, and fails if any
  `SEAMLESS_PREVIEW` mention survives. `dev/preview` still compiles the full
  source with the macro, so the offscreen harness is unchanged.
- Checked with the library's own `pr_validation.validate_mod_file` and
  `extract_mod_symbols.get_mod_symbols`: 0 errors, symbols extracted for
  `amd64` and `arm64`, 0 review items.

### Review hardening (third push)

- **Migration no longer reads Windhawk's registry.** Only the layout, group,
  pins and icon files are inherited; the predecessor's settings file is copied
  into the new folder as `migration-original-ui-settings.ini` for reference
  instead of being applied, so nothing is inherited silently and no private
  registry layout is touched.
- **No no-op hook apply.** The queued `kernelbase` hook is applied by Windhawk
  when `Wh_ModInit` returns; the call that was refused during init is gone.
- **Cancelling UAC is not an error.** `LaunchTarget` treats `ERROR_CANCELLED`
  as the user saying no instead of "this entry is broken".
- `labelRadius` was removed from `Settings` as well, so no undeclared setting
  is read at all.
- The launch-probe assertion in the feature tests now waits long enough for a
  process start on a loaded machine, so it no longer reports a false failure.

### Review hardening (second push)

Changes made in response to the repository's AI review of the submitted file:

- **Unload is clean.** The power thread is stored in a global handle and joined
  in `Wh_ModUninit`; the `GetModuleHandleExW` / `FreeLibraryAndExitThread` pair
  is gone, so Windhawk's single `FreeLibrary` can no longer be defeated by a
  running thread.
- **The settings thread cannot outlive the DLL.** `CloseSettingsWindow` sets the
  stop flag, posts `WM_QUIT` to the thread, and joins it; the thread checks the
  flag right after building its window, so a close that races the build still
  leaves it. Teardown lives in one place (`SettingsThreadTeardown`).
- **The popup-menu window class is per image.** `MenuClassName()` appends the
  image base and process tick to the class name, so an in-place update never
  reuses a window procedure from the previous image; the class is unregistered
  in the UI thread's teardown.
- **The Monitor dropdown owns its labels.** `SetField::options` holds
  `std::wstring`, and the per-call monitor list is moved into the field instead
  of pointing into a growing function-local static.
- **COM/GPU lifetimes.** `MakeLayout` returns a `ComPtr`; the label shadow hands
  over its bitmap with `std::move` instead of `Detach`; the shadow device
  context is a global reset by `ResetTarget()`; the menu's text formats and the
  group-title render cache are released at teardown; both label caches are
  dropped when the device is recreated.
- **Dead settings removed**, so the Windhawk settings page only lists settings
  that do something: `accentColor`, `glowSize`, `groupColor`, `groupOpacity`,
  `hoverColor` and `selectionColor`. The undeclared `labelRadius` read is gone
  with them. `performanceLogging` now defaults to off.
- **Settings precedence is documented** at the top of the readme: the built-in
  settings window overrides the Windhawk page until the same setting is changed
  in Windhawk again.
- **XAML taskbar hooks.** `LoadLibraryExW` is hooked on `kernelbase.dll` through
  `WindhawkUtils::SetFunctionHook`, so WinRT activation of `Taskbar.View.dll` is
  seen too; the 1 s polling timer is removed. The symbol table is declared for
  both `Taskbar.View.dll` and `ExplorerExtensions.dll`.
- **No files outside the mod's storage.** "Run as administrator" for a Store app
  uses a uniquely named shortcut in the mod's `tmp` folder and deletes it after
  the launch; `Export-StartLayout` output and the backup worker also live there.
- **No Windhawk internals.** Migration treats a missing legacy settings key as
  non-fatal (layout is still copied, settings keep their defaults); the backup
  worker takes settings from the mod instead of `reg.exe export`, and no longer
  packs the installed source or DLL, so portable Windhawk works.
- **A bad data file no longer disables the mod.** An empty legacy layout is a
  valid empty list, a stale `data-v1.pending` is removed and retried, and a data
  folder that fails validation opens read-only instead of refusing to load.
- **Smaller fixes.** `StripIconFrame` cannot walk a side past its opposite; the
  readme has a screenshot, the network section states that icons may come from
  another host, and the start-state trace is compiled out.

### First submission

- `@version` is now `1.0.0`.
- `@author` is `Fangyu Guo (ryjs)`, the developer and maintainer. The two
  upstream authors are credited by role in the mod readme, the repository
  `README.md`, `LICENSE` and `release/SOURCE_PROVENANCE.md`, not in `@author`.
- Copyright notices were corrected: `Copyright (c) 2026 DavidHiFi` is taken
  from the upstream LICENSE; Rafael Mendes is named as the author of Fullscreen
  Start under MIT, with no year asserted, because the upstream project declares
  only `@license MIT` and has no copyright line or LICENSE file.
- Development source moved to `mods/local/ryjs-launchpad/ryjs-launchpad.wh.cpp`;
  the root single file keeps its name. Upstream snapshots used by the audit
  moved to `reference/upstream/`.
- `release/` holds the provenance report, release audit, submission checklist,
  PR text and a manual acceptance checklist.

## 0.2.1 — GUI complete backup and import

- Native menu actions and ZIP file dialogs export complete layout/entry/geometry/settings/icon data, plus installed source/module and registry settings for reference. The embedded worker uses Windows PowerShell and .NET; Python is not required for users.
- Import validates paths, checksums, schemas, assets, entry identities and geometry bindings before confirmation. The confirmed archive hash is rechecked; only data/settings/assets are restored. Imported source, binaries and registry exports are never installed or executed.
- App/icon workers and settings are paused before replacement. Staged import preserves previous data in a sibling directory, rolls back a failed commit and reloads on success. Existing schema-1 legacy ZIP backups remain compatible.
- Preview and Windhawk x64 builds, bilingual offscreen smoke, 21 native bridge assertions, 7 worker tests (including 10 invalid-archive cases), 45 feature assertions, 26 storage assertions, 28 Start-button assertions and 5 legacy-backup tests passed. The previously created desktop ZIP passed read-only validation. No live installation or layout changes were performed.
- Local official checks explicitly use the upstream compile script, keeping outputs in the workspace rather than invoking the installed Windhawk CLI's install command.
- Official metadata and x64 compile checks passed. A small adapter accepts the installed Windhawk alpha version label; the upstream compiler implementation is unchanged.

See `release/` for the provenance audit, release audit, checklist and PR text.

## 0.2.0 — Phases 3–5

- Manual `.exe`, `.lnk`, `.rdp`, Store app and HTTP/HTTPS entries; native add/edit dialog, aliases, arguments, working directory, custom icons and explicit destination group. Missing targets stay repairable. File launching uses the original file and its Windows association.
- Stable entry identities and separate versioned overrides preserve raw layout references when a target changes. Automatic discovery never inserts new apps into an existing arrangement. Aliases and original application names are searchable.
- Website icon declarations and favicon fallback are fetched asynchronously only on add/update/explicit refresh. Direct requests have size/time limits, no third-party icon proxy and a local cache. WIC selects suitable raster/ICO frames; supported SVG is rasterized once through Direct2D.
- Explicit group edit mode, separate geometry, snapped two-dimensional movement and resizing, overlap rejection, per-group scrolling, traditional folders alongside resident groups. Drag cancellation and restart retain dimensions and presentation. Layouts with no geometry retain the old arrangement.
- Light/dark/system/custom appearance, text/group/hover/selection colors, group opacity, localized name backplates, shadow controls and optional background dimming. Existing icon/font/spacing controls remain available. Appearance never changes membership/order.
- Local checks: 45 feature assertions, 26 synthetic storage assertions, 28 Start-button callback assertions and 5 backup-tool cases passed. Preview and Windhawk x64 builds, bilingual offscreen rendering, four screen sizes and white/dark readability renders passed. Official metadata and x64 compile checks passed for the final source.
- Actual StartAllBack border, menu behavior, web networking and launch behavior require loading the new mod. No production layout backup or writes were performed during feature development.

## 0.1.1 — Phase 2

- Opening, closing and folder durations have sliders and exact millisecond input in the existing settings window (60–1500 ms, including the requested 150–800 ms range). Existing defaults remain 280/180/180 ms; saved settings are preserved.
- All three transitions use elapsed time. A completed opening transition no longer cancels a still-running folder transition's frame scheduling.
- Fallback frame timer follows the selected monitor's queried refresh period. The existing fractional refresh query, frame cap, high-resolution deadline scheduler and idle cancellation remain in use.
- Isolated verification: preview and Windhawk x64 compilation, bilingual offscreen smoke, 11 timing/control assertions. Actual StartAllBack input and mixed-refresh display behavior still require live verification.

## 0.1.0 — Independent source baseline

- Independent identity and transactional, versioned data inheritance; no changes to old storage.
- Menu dismissal click filtering and taskbar Start-button visual reset, pending live verification.
- Source-only public repository, retaining upstream MIT notices and author credits.
