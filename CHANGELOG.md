# Changes

## 1.5.10

- The Start-latch reset no longer makes `Wh_ModInit` fail when `Shell_TrayWnd` does not exist yet. Windhawk calls `Wh_ModInit` before Explorer creates its windows on sign-in and on every Explorer restart, so the mod refused to load in the normal startup path; the subclass is now installed only when `ClearStartButtonLatch` first needs it, with its existing synchronous fallback when it cannot be installed.
- `Wh_ModUninit` removes the taskbar subclass a second time, after the launcher thread is joined and with a fresh `Shell_TrayWnd` lookup. This closes the window where a `HideNow` that passed the stopping gate just before it closed could reinstall the subclass and leave Explorer calling into an unloaded image.
- Full-backup import keeps this PC's private `ui-settings.ini` overrides of `program`, `winKeyAction`, `shiftWinKeyAction` and `startButtonAction` when the Windhawk value they were saved against is still current. Values saved in the built-in settings window therefore remain in force after an import, as the readme already stated.
- **Background website icon download** moved from the Power page to the Grid page's Website icons group in the built-in settings window; the readme now points at that location and at Windhawk's settings page.

## 1.5.9

- Replace the queued XAML Start-button reset with a registered message handled by `WindhawkUtils::SetWindowSubclassFromAnyThread` on `Shell_TrayWnd`. Unload synchronously removes the subclass and joins any active reset; there is no queued module lambda, five-second timeout or permanent module pin.
- Add **Background website icon download** (on by default) to stop automatic app-list icon requests while retaining cached icons and explicit save/manual refresh downloads. Document first-load and Desktop/Public Desktop shortcut requests, third-party/intranet icon hosts, and the absence of cookies or automatic credentials. A missing/corrupt cache no longer erases an already loaded shell icon.
- Full-backup import now preserves this PC's Start-button and Win-key actions and program command. Its confirmation lists every imported entry target and arguments; archive code and binaries are still never installed or executed.
- Move the settings title-mark bitmap into `SetWindow` and release it with the window render target, removing function-static COM destruction and stale D2D resource-domain reuse.
- Build the material import/export file filter with explicit embedded NULs so both `.ryjsm` and `.json` filters reach the Windows dialog safely.
- Release every app D2D bitmap on device loss; stop resolving completed web-icon workers by reusable thread IDs; cap persisted group grid geometry; run legacy tint migration once, without per-frame settings reads or read-only writes.

## 1.5.8

- Release audit fixes. The settings window now releases its memory DC and DIB on `WM_NCDESTROY`, so repeated open/close cycles no longer accumulate GDI objects; a failed creation also cleans the brush and owned window.
- The settings animation thread is joined without a timeout before the module can unload. A failed `PostMessage` no longer leaves the animation tick permanently pending.
- A lost or recreated Direct2D device now drops the material, panel-blur and group-title bitmaps together with the render target, so none of them keeps the old device alive or gets sampled by the new one.
- A material capture that fails (screen copy, upload or blur allocation) is not retried on every frame; the next time the launcher opens tries again. The blur radius read from settings is clamped to the documented 0–120 DIP range.
- Material preset JSON parsing has a nesting limit, and importing a material file refuses files above 8 MiB instead of reading them into one string.
- Corrected the settings-window glass description (custom rounded glass over a blurred desktop snapshot, not Windows acrylic), updated the material/feature sections of the readme, and removed unused helper functions and constants.
- Kept the generated single-file submission within the official 4096-character line limit by emitting the embedded app icon as adjacent string literals, and moved the test-only mouse-tracking branch out of a preprocessor `#if`/`#else` pair so the official scope checker sees the four `Wh_Mod*` callbacks in the global namespace.

## 1.5.7

- Build app-label shadows when drawing the grid, after the graphics device is ready. Temporary shadow failures retry in the same open session, including with animations disabled.

## 1.5.6

- Follow the pointer through the gap between warned power buttons. The connection stretches and reverses with the pointer while the circular lobes stay centred in Follow button mode.

## 1.5.5

- Give app tiles a wider, inward-fading edge glow beneath the glass bevel. The highlight stays local to the edge facing the pointer.

## 1.5.4

- The power warning transfers between two anchored lobes through a thin, closing connection. Crossing the empty gap holds the outgoing lobe on its button.
- Preserve private setting overrides when Windhawk first writes the declared defaults. Strip header-only pragmas from the standalone module.

## 1.5.3

- Remove duplicate Windhawk power warning declarations and keep the six action switches nested under their settings group. Stored setting names and defaults stay compatible.

## 1.5.2

- The app icon glow distance also controls how quickly the square rim catches the light. At 80, moving slightly away from the icon centre lights the facing edge, even while the pointer is still over the icon.

## 1.5.1

- The power warning stretches and coalesces when moving between buttons, and redirects smoothly when the pointer reverses. Each label reflects the warning light directly above its own icon.

## 1.5.0

- The product is now 流光启动台 in Chinese and Lumiflow in English. The Windhawk metadata, settings title, About page, tray tooltip/menu, message boxes, backup file names and file-dialog filters follow the current interface language; neither language mixes the two names.
- The internal Windhawk id remains `ryjs-launchpad`, and the storage path, `ryjs-storage.ini`, `.ryjs.zip` backups, `.ryjsm` material files, integration mutex and window class names are unchanged. Existing layouts, groups, icons, pinned entries and settings therefore load without migration.
- The development preview executable is built as `Lumiflow.exe`; the product itself remains a Windhawk module and has no standalone installer.
- The settings window has its own palette now, and it is no longer permanently dark. `Theme mode` decides it: `Light` and `Dark` force it, `System` follows the Windows app theme, and `Custom` (the default) follows the Windows app theme too. The launcher's own tint is untouched — this only affects the settings window's page, rail, cards, text, controls, title bar and the sample wallpaper behind the material preview. The dark palette is unchanged.
- The icon glow distance is now measured from the icon's own rectangle instead of the whole grid cell. Cells tile the grid, so the old cell-based reach never had any distance left to spend and the setting did nothing once the pointer was inside the grid. The nearest icon within the distance lights, and the hover no longer has to hold the pointer for the glow to appear, so a pointer in empty space still lights the closest icon: at 0 only the icon under the pointer reacts, at 80 the nearest icon lights from up to 80 DIP away.
- Power dock glyphs are reduced from 22 to 20 DIP while keeping the bold stroke. The warning is one continuous #A91223 environment light inside the dock's existing glass capsule, never a separate button background. Its X follows the pointer across the capsule in the default mode (Y stays the hovered button's click-rectangle centre), with 35 ms exponential position smoothing and 120 ms fade-in / 180 ms fade-out. The radial lobe is a 23×23 DIP circle at 78% centre concentration, with the fixed 0 / 35% = 88% / 65% = 48% / 100% = 0 ramp; pressing compresses it to 0.95 without brightening it. The 65% bottom-edge response reuses the same field and is multiplied by 0.45 so it cannot pull the perceived centre down. The label pill above receives a 10–20% position-dependent reflection on its lower edge without changing the text colour. No extra outline, relief, inner shadow or specular layer is added. `Follow button` remains available and uses the same smoothing.
- The Power page has a `Warning light` section: enable/disable, per-action toggles for all six power actions, follow mode, colour, strength, spread, edge tint and label bleed. It uses the existing switches, sliders, colour picker and save/cancel/suggested paths; missing fields in old `ui-settings.ini` files fall back to the defaults, and the sample capsule in the page previews unsaved changes immediately.
- The settings window now uses a 20 DIP custom outer corner instead of DWM's 8 DIP preset. The custom title bar disables acrylic, blur-behind and frame extension, renders into a 32bpp premultiplied DIB through `DWMWA_REDIRECTIONBITMAP_ALPHA` (Windows 11 26100+), and masks the whole surface with a D2D rounded-layer geometry; the transparent corners expose the real desktop. The glass background is one blurred virtual-desktop snapshot taken while the window is hidden and cropped as the window moves, not a per-frame desktop capture. The full-window client area plus manual `WM_NCHITTEST` resize edges also remove the old left/right black bars. Native title bar mode and older Windows keep an opaque DWM-rounded fallback rather than showing black wedges.
- Upstream credit for Fullscreen Start and Seamless Launchpad is kept; the generic “Launchpad” wording that described this product has been removed from current UI text.
- The settings sidebar keeps its two long page titles as captions — 启动方式 and 应用布局 in Chinese, `Keys` and `Grid` in English — but the rows themselves now read 启动 and 布局 (`Startup` and `Grid layout` in English), so they fit the 36 px row instead of crowding it.
- The settings sidebar no longer repeats the product mark and name at its top. The title bar already carries both, so the rail now holds only the panel toggle above the page rows; the collapsed icon rail is unchanged.

## 1.4.1

- Material presets describe texture only (blur, transparency, grain, dim). Applying one leaves the tint colour untouched, and built-in presets no longer carry a hue; `Stained Glass` became `Vivid Glass`. The settings preview uses a neutral grey scene, so any colour on it comes from the user's own tint.
- Text shadow and power-dock icon shadow are separate settings (`textShadow*` / `powerShadow*`); changing label typography no longer changes the power glyphs.
- Group headers and menu labels now use the typography page's shadow and outline values; they no longer fall back to a built-in 1 px outline when the user has turned outlines off.
- Power-dock glyphs are larger (22 DIP instead of 16), and hovering a power button replaces the white glass with a restrained crimson glow. Sign out, restart and shut down get the full warning strength; lock, sleep and hibernate use a softer tint.
- The settings window draws its labels once per frame instead of replaying each text layout for a fake shadow; its animation tick now uses a 120 Hz high-resolution waitable timer, and wheel scrolling settles in a few frames instead of the previous 52 ms tail.
- The supplied 3×3 ice-glass sheet is now the application mark itself: its PNG is embedded in the module and decoded for the title-bar mark, the settings window HICONs and the tray icon, rather than replaced with a redrawn single tile or glyph.
- The pointer-following glass glow has an adjustable trigger distance (`glowDistance`, default 24 DIP) for grid icons and power buttons; a shadow bitmap that fails to render is retried on the next frames instead of staying missing.
- Prevent concurrent Start button symbol lookups for the same taskbar module, and apply a deferred hook when only one of the optional symbols resolves.
- Log elapsed time for loading `twinui.pcshell.dll` and resolving the Start hooks when debug logging is enabled.

## 1.4.0

- Overflowing groups show a compact page indicator. One standard wheel detent moves one group page, and the normal wheel action outside a group flips the full grid in page mode.
- Chinese IME input opens the search field as soon as composition starts. The launcher renders the composition inline and reports the caret position for the input method's own candidate window.

## 1.3.1

- Native Start keeps control of the taskbar button throughout dismissal, including when a click outside closes the menu. Removed the private XAML checked-state callback and the diagnostic pointer hooks.

## 1.3.0

- Shift + Windows hands off to the original Start menu without clearing the native button state from the launcher thread.
- The power dock refreshes its pending-update indicator while open and checks Windows Update Orchestrator and servicing signals alongside the Windows Update Agent.
- Typography settings independently adjust text shadow opacity and blur, and outline opacity and width. Defaults use a stronger dark shadow and a thin outline.

## 1.2.0

- Power buttons now use the same pointer-driven glass highlight as app icons, shaped to the circular buttons. The icons use a larger, bolder symbol font with the existing soft shadow.
- The Power settings page can reorder buttons using the row arrows or Ctrl+Up/Down. The saved left-to-right order retains disabled and unavailable actions so they return to their chosen positions.

## 1.1.0

- Layout editing actions now use the group-title type and shadow, the context-menu tint, and distinct hover and press states.
- Power dock lighting tracks only the hovered button. Glyphs have a subtle shadow, and a separate inner ring marks an action waiting for confirmation.
- Settings now use glass cards, page icons, and the same pointer-driven highlights as the launcher. Save is visually distinct from the secondary actions.

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
