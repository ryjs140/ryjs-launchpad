# Changes

## 1.0.0 — first public release

First submission to the Windhawk mod library. The code is the 0.2.1 feature
set; no behaviour changed.

- `@version` is `1.0.0`. `@author` is `Fangyu Guo (ryjs)`, the developer and
  maintainer; upstream authors are credited by role in the readme, README,
  LICENSE and provenance report rather than in `@author`.
- Copyright notices corrected: `Copyright (c) 2026 DavidHiFi` is kept verbatim
  from the upstream LICENSE; Rafael Mendes is credited as the author of
  Fullscreen Start under MIT with no year asserted, because the upstream
  project declares only `@license MIT` and has no copyright line.
- The mod readme gained a three-way provenance table, a `## Copyright notices`
  section, `## Compatibility` and `## Data and network`; the upstream
  screenshot was removed.
- Development source moved to `mods/local/ryjs-launchpad/ryjs-launchpad.wh.cpp`;
  upstream snapshots used by the audit moved to `reference/upstream/`.
- One 192-character line that packed five statements was split; it was the only
  finding of the official validator.
- Removed `@donateUrl`, which pointed at the upstream author's Ko-fi page.

Verification: official validator 0 errors / 0 review items; official compile
x86, x64 and arm64 clean; local x64 build 0 warnings.

## 0.2.1 — GUI complete backup and import

Functional changes since 0.2.0:

- Native menu actions and ZIP file dialogs export complete
  layout/entry/geometry/settings/icon data, plus installed source/module and
  registry settings for reference. The embedded worker uses Windows PowerShell
  and .NET; Python is not required for users.
- Import validates paths, checksums, schemas, assets, entry identities and
  geometry bindings before confirmation. The confirmed archive hash is
  rechecked; only data/settings/assets are restored. Imported source, binaries
  and registry exports are never installed or executed.
- App/icon workers and settings are paused before replacement. Staged import
  preserves previous data in a sibling directory, rolls back a failed commit
  and reloads on success. Existing schema-1 legacy ZIP backups remain
  compatible.
- Preview and Windhawk x64 builds, bilingual offscreen smoke, 21 native bridge
  assertions, 7 worker tests (including 10 invalid-archive cases), 45 feature
  assertions, 26 storage assertions, 28 Start-button assertions and 5
  legacy-backup tests passed.
- Official metadata and x64 compile checks passed for the final source.

## 0.2.0 — Phases 3–5

- Manual `.exe`, `.lnk`, `.rdp`, Store app and HTTP/HTTPS entries; native
  add/edit dialog, aliases, arguments, working directory, custom icons and
  explicit destination group. Missing targets stay repairable. File launching
  uses the original file and its Windows association.
- Stable entry identities and separate versioned overrides preserve raw layout
  references when a target changes. Automatic discovery never inserts new apps
  into an existing arrangement. Aliases and original application names are
  searchable.
- Website icon declarations and favicon fallback are fetched asynchronously
  only on add/update/explicit refresh. Direct requests have size/time limits,
  no third-party icon proxy and a local cache. WIC selects suitable raster/ICO
  frames; supported SVG is rasterized once through Direct2D.
- Explicit group edit mode, separate geometry, snapped two-dimensional
  movement and resizing, overlap rejection, per-group scrolling, traditional
  folders alongside resident groups.
- Light/dark/system/custom appearance, text/group/hover/selection colours,
  group opacity, localized name backplates, shadow controls and optional
  background dimming.
- Local checks: 45 feature assertions, 26 synthetic storage assertions, 28
  Start-button callback assertions and 5 backup-tool cases passed.

## 0.1.1 — Phase 2

- Opening, closing and folder durations have sliders and exact millisecond
  input in the settings window (60–1500 ms). Defaults remain 280/180/180 ms;
  saved settings are preserved.
- All three transitions use elapsed time. A completed opening transition no
  longer cancels a still-running folder transition's frame scheduling.
- Fallback frame timer follows the selected monitor's queried refresh period.

## 0.1.0 — Independent source baseline

- Independent mod id `ryjs-launchpad` and transactional, versioned data
  inheritance; the old mod's storage is read once and never modified.
- Menu dismissal click filtering and taskbar Start-button visual reset.
- Source-only public repository, retaining upstream MIT notices and author
  credits.
