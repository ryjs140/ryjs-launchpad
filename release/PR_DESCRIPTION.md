# Add RYJS Launchpad

## What this mod is

RYJS Launchpad replaces the Start menu with a fullscreen, Launchpad-style grid
of apps. It is a derivative work of two MIT-licensed mods, and this is stated
in the mod readme, the repository `README.md`, `LICENSE` and
`release/SOURCE_PROVENANCE.md`:

| Project | Author | Role |
| --- | --- | --- |
| Fullscreen Start (Launchpad style) | Rafael Mendes | Original launcher, its rendering and the Start hook |
| Seamless Launchpad | DavidHiFi | Direct base: grid, search, folders, power dock, settings |
| RYJS Launchpad | Fangyu Guo (ryjs) — this submission | Expanded groups, free group layout, manual and web entries, themes, backup, Start-button state handling |

- Mod id: `ryjs-launchpad`
- Author / maintainer: Fangyu Guo (ryjs), https://github.com/ryjs140
- Source: https://github.com/ryjs140/ryjs-launchpad
- Version: 1.0.0
- License: MIT, with the upstream notices retained
- Inject into: `explorer.exe` only

I am not claiming an independent implementation. Measured on code lines, about
half of the file is still verbatim upstream code, and all 30 upstream settings
are kept unchanged so existing users keep their configuration. The provenance
report in the source repository has the numbers and a reproduction script.

## What is new

Roughly half the code is new or rewritten: expanded groups with free per-group
geometry, resizing and overlap rejection; manual entries (`.exe`, `.lnk`,
`.rdp`, Store apps, http/https) with aliases, arguments and working
directories; website icon fetching with a local cache; light/dark/system/custom
themes with contrast-aware label colour; bilingual UI with a local font picker;
GPU composition; performance logging; versioned storage with `.bak` and
read-only protection on unknown schemas; `.ryjs.zip` backup/import; and
Start-button state handling for StartAllBack and the native XAML taskbar.

## Why a separate entry rather than an update

`fullscreen-start` belongs to Rafael Mendes, and `seamless-launchpad` is not in
this library; the submission rules only allow the original author to update an
existing mod. RYJS has its own id, storage directory (`local@ryjs-launchpad`),
settings schema, versioning and maintainer, and never writes to another mod's
storage. On first run it can *read* an existing Seamless Launchpad / Fullscreen
Start Plus layout and copy it into its own storage, which is why the readme asks
the user to disable the older mod first.

## Testing

Passed locally:

- Official validator (`.github/pr_validation.py`): 0 errors, 0 review items.
- Official compile script: x86, x64 and arm64 built with no problems.
- Local x64 build with the Windhawk compiler (Clang 20.1.3) and the 2.0.0
  import library: 0 warnings, 0 errors.
- Settings use only `$name`, `$description` and `$options`, so the block also
  parses on Windhawk 1.7.3.
- Source repository tests: bilingual offscreen render at four screen sizes,
  group editing, storage migration and rollback, Start-button callbacks,
  backup/import validation (including invalid archives), feature and storage
  assertions.
- No telemetry. The only network access fetches an icon for a web entry the
  user adds: 3 s timeouts, 15 s total, 2 MB cap, cookies disabled, no
  autologon, no HTTPS to HTTP redirects. Documented in the mod readme.

Not verified on real hardware, and stated as such in the readme:

- Start-button visual reset on a real StartAllBack taskbar and on the native
  Windows 11 taskbar. The code paths exist and are isolated-tested; the
  on-screen result needs a live click.
- Mixed-refresh multi-monitor setups.
- Windows 10 (the mod targets Windows 11).
- `.rdp` entries and launch arguments end to end.

## Checklist

- [x] Single file, `mods/ryjs-launchpad.wh.cpp`, compiles standalone
- [x] `@id` lowercase, unique, matches the file name
- [x] `@github` matches the PR author
- [x] `@license` MIT, upstream notices retained verbatim
- [x] `@include` limited to `explorer.exe`
- [x] No telemetry, no undocumented network access, no hardcoded paths
- [x] Settings compatible with Windhawk 1.7.3 and 2.0
- [x] Upstream authors credited; no claim of independent implementation
