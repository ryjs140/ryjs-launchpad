# Changes

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
