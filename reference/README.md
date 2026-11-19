# Reference: upstream sources

These files are **not part of RYJS Launchpad**. They are kept so the provenance
claims in [`release/SOURCE_PROVENANCE.md`](../release/SOURCE_PROVENANCE.md) can
be checked without network access, and so
[`release/tools/audit-provenance.py`](../release/tools/audit-provenance.py) can
be re-run with the exact inputs the numbers were computed from.

| File | Origin | Author | License |
| --- | --- | --- | --- |
| `fullscreen-start-1.0.0.wh.cpp` | https://raw.githubusercontent.com/ramensoftware/windhawk-mods/main/mods/fullscreen-start.wh.cpp (v1.0.0, added 2026-10-22) | Rafael Mendes | MIT, declared as `@license MIT` in the mod metadata |
| `seamless-launchpad-2.4.9-upstream.wh.cpp` | `mods/local/seamless-launchpad/seamless-launchpad.wh.cpp` at commit `09a71ba` of https://github.com/DavidHiFi/davids-windhawk-mods | DavidHiFi, Rafael Mendes | MIT |
| `seamless-launchpad-LICENSE.txt` | `LICENSE` of https://github.com/DavidHiFi/davids-windhawk-mods | DavidHiFi | MIT, `Copyright (c) 2026 DavidHiFi` |

Notes:

- The Fullscreen Start file has no copyright line and its repository has no
  `LICENSE` file; MIT is declared in the mod metadata, which is the standard
  Windhawk mechanism. No year is asserted for it anywhere in this repository.
- These files are reference material under their own MIT terms. They are not
  compiled, not shipped inside the mod, and not covered by the RYJS build.

Reproduce the audit numbers:

```bash
py -3.12 release/tools/audit-provenance.py \
  --candidate ryjs-launchpad.wh.cpp \
  --upstream-seamless reference/upstream/seamless-launchpad-2.4.9-upstream.wh.cpp \
  --upstream-original reference/upstream/fullscreen-start-1.0.0.wh.cpp
```
