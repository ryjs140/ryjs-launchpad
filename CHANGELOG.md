# Changes

## 0.1.1 — Phase 2

- Opening, closing and folder durations have sliders and exact millisecond input in the existing settings window (60–1500 ms, including the requested 150–800 ms range). Existing defaults remain 280/180/180 ms; saved settings are preserved.
- All three transitions use elapsed time. A completed opening transition no longer cancels a still-running folder transition's frame scheduling.
- Fallback frame timer follows the selected monitor's queried refresh period. The existing fractional refresh query, frame cap, high-resolution deadline scheduler and idle cancellation remain in use.
- Isolated verification: preview and Windhawk x64 compilation, bilingual offscreen smoke, 11 timing/control assertions. Actual StartAllBack input and mixed-refresh display behavior still require live verification.

## 0.1.0 — Independent source baseline

- Independent identity and transactional, versioned data inheritance; no changes to old storage.
- Menu dismissal click filtering and taskbar Start-button visual reset, pending live verification.
- Source-only public repository, retaining upstream MIT notices and author credits.
