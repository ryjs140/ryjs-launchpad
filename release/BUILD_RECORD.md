# Build record (1.5.8)

Date: 2026-11-26

Artifact: `ryjs-launchpad.wh.cpp` (repository root) and
`release/mods/ryjs-launchpad.wh.cpp` (byte-identical submission copy).

| Item | Value |
| --- | --- |
| Version | 1.5.8 |
| Lines | 21,392 |
| Bytes | 1,119,813 |
| SHA256 | `6915160ece5672b1f6723b49010dfc914036652efde3bfeb7f5258b2642ae2a3` |
| Declared architecture | x86-64 |
| Official validator | 0 problems |
| Official compile | 0 problems |

## Inputs

The generator reads the development source and its fragments, plus the settings
schema, Chinese strings, the full-backup worker and the icon asset.

| Input | Path |
| --- | --- |
| Version source | `ryjs-project.json` |
| Settings schema | `dev/settings-extension.json` |
| Chinese resources | `dev/strings.zh-CN.json` |
| Embedded backup worker | `dev/full_backup_worker.ps1` |
| Development source | `mods/local/ryjs-launchpad/ryjs-launchpad.wh.cpp` |
| App icon | `assets/app-icon-256.png`, embedded as `launchpad_app_icon.inc` |

The development source includes the `launchpad_*.h` / `launchpad_*.inc`
fragments and `ryjs_data.h` from `mods/local/ryjs-launchpad/`. The only
developer-only include, `preview_api.h`, is replaced by a comment in the
submission, so the artifact contains no local `#include`.

## Generation

```powershell
py -3.12 dev/generate_resources.py
```

The generator writes the artifact with LF (`newline='\n'`). On a checkout with
`core.autocrlf=true`, the working copy may contain CRLF; the committed blob and
the GitHub raw file remain LF. Verify the GitHub bytes with:

```bash
curl -sL https://raw.githubusercontent.com/ryjs140/ryjs-launchpad/main/ryjs-launchpad.wh.cpp | sha256sum
```

Equivalent local build and verification entry point:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File dev/Build.ps1 -Verify
```

Requires PyYAML 6.0.3. This record used `py -3.12`.

## Checks performed on the final artifact

| Check | Result |
| --- | --- |
| Official metadata validator, current upstream `main` (`7628a65`) | `ok: true`, 0 problems |
| Official compile script, current upstream `main` | `ok: true`, 0 problems |
| Local x64 module compile | PASS |
| Local preview compile | PASS |
| Longest physical line | 2,282 characters (limit 4,096) |
| `Wh_ModInit`, `Wh_ModBeforeUninit`, `Wh_ModUninit`, `Wh_ModSettingsChanged` scope | global namespace, 0 validator errors |
| Root file and `release/mods/` copy | SHA256 identical |
| Local `#include` directives in the artifact | 0 |
| Feature checks | 612 checks, 0 failures |
| Phase-one checks | 25 + 35 checks, 0 failures; 5 Python backup tests OK |
| Full-backup checks | 28 checks, 0 failures; 12 Python full-backup tests OK |
| Offscreen smoke test | PASS; three repeated `--smoke` runs exited 0 |

## Notes

The app icon is emitted as adjacent C++ string literals, one 1,024-character
chunk per line, rather than one very long raw-string line. This keeps the
submission inside the official line-length rule without changing the decoded
PNG. The `LP_TEST_HOVER` test branch is expressed as a boolean
`startTracking` value before a single `if`, so the official callback-scope
checker sees balanced braces even though it does not evaluate preprocessor
branches.
