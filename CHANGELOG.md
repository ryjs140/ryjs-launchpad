# Changes

## 0.2.1 — GUI complete backup and import

- Native menu actions and ZIP file dialogs export complete layout/entry/geometry/settings/icon data, plus installed source/module and registry settings for reference. The embedded worker uses Windows PowerShell and .NET; Python is not required for users.
- Import validates paths, checksums, schemas, assets, entry identities and geometry bindings before confirmation. The confirmed archive hash is rechecked; only data/settings/assets are restored. Imported source, binaries and registry exports are never installed or executed.
- App/icon workers and settings are paused before replacement. Staged import preserves previous data in a sibling directory, rolls back a failed commit and reloads on success. Existing schema-1 legacy ZIP backups remain compatible.
- Preview and Windhawk x64 builds, bilingual offscreen smoke, 21 native bridge assertions, 7 worker tests (including 10 invalid-archive cases), 45 feature assertions, 26 storage assertions, 28 Start-button assertions and 5 legacy-backup tests passed. The previously created desktop ZIP passed read-only validation. No live installation or layout changes were performed.
- Local official checks explicitly use the upstream compile script, keeping outputs in the workspace rather than invoking the installed Windhawk CLI's install command.
- Official metadata and x64 compile checks passed. A small adapter accepts the installed Windhawk alpha version label; the upstream compiler implementation is unchanged.

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
