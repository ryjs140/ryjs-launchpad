#!/usr/bin/env python3
"""Measure how much of a Windhawk mod file comes from its upstream files.

The comparison is intentionally simple and reproducible:

* lines  - blank lines, ``//`` comments and lone braces are dropped, then
           stripped lines are compared as a set. Identical lines may sit at a
           different offset, which is what we want: the question is "does this
           line exist upstream", not "is it in the same place".
* funcs  - ``name(args) {`` shapes are extracted with a regex. This over-counts
           a little (lambdas, macros) but treats both sides the same way.
* settings - top level keys of the ==WindhawkModSettings== YAML block.

Usage:
    python audit-provenance.py --candidate ryjs-launchpad.wh.cpp \
        --upstream-seamless seamless-launchpad-2.4.9.wh.cpp \
        --upstream-original fullscreen-start-1.0.0.wh.cpp
"""
import argparse
import re
import sys
from pathlib import Path


def code_lines(path):
    out = []
    for raw in Path(path).read_text(encoding='utf-8', errors='replace').splitlines():
        text = raw.strip()
        if not text or text in ('{', '}'):
            continue
        if text.startswith('//') or text.startswith('/*') or text.startswith('*'):
            continue
        out.append(text)
    return out


def functions(path):
    text = Path(path).read_text(encoding='utf-8', errors='replace')
    text = re.sub(r'//[^\n]*', '', text)
    return set(re.findall(r'\b([A-Za-z_]\w{2,})\s*\([^;{}]{0,400}\)\s*(?:const\s*)?\{', text))


def settings(path):
    text = Path(path).read_text(encoding='utf-8', errors='replace')
    match = re.search(r'==WindhawkModSettings==\n/\*\n(.*?)\n\*/', text, re.S)
    if not match:
        return []
    return [m.group(1) for m in re.finditer(r'^- ([A-Za-z0-9_]+):', match.group(1), re.M)]


def pct(part, whole):
    return 0.0 if not whole else 100.0 * part / whole


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--candidate', required=True)
    ap.add_argument('--upstream-seamless', required=True, help='Seamless Launchpad, the direct base')
    ap.add_argument('--upstream-original', required=True, help='Fullscreen Start, the origin')
    args = ap.parse_args()

    candidate = code_lines(args.candidate)
    seamless = code_lines(args.upstream_seamless)
    original = code_lines(args.upstream_original)

    seamless_set = set(seamless)
    original_set = set(original)
    candidate_set = set(candidate)

    inherited = sum(1 for line in candidate if line in seamless_set)
    inherited_from_origin = sum(1 for line in candidate if line in seamless_set and line in original_set)
    origin_surviving = sum(1 for line in original if line in candidate_set)

    cand_funcs = functions(args.candidate)
    seamless_funcs = functions(args.upstream_seamless)
    original_funcs = functions(args.upstream_original)

    cand_keys = settings(args.candidate)
    seamless_keys = settings(args.upstream_seamless)

    print(f'candidate code lines            : {len(candidate)}')
    print(f'verbatim from seamless          : {inherited} ({pct(inherited, len(candidate)):.1f}%)')
    print(f'  ...also verbatim from origin  : {inherited_from_origin}'
          f' ({pct(inherited_from_origin, inherited):.1f}% of inherited,'
          f' {pct(inherited_from_origin, len(candidate)):.1f}% of file)')
    print(f'new or changed vs seamless      : {len(candidate) - inherited}'
          f' ({pct(len(candidate) - inherited, len(candidate)):.1f}%)')
    print(f'origin lines surviving          : {origin_surviving}/{len(original)}'
          f' ({pct(origin_surviving, len(original)):.1f}%)')
    print()
    print(f'functions candidate/seamless/origin : {len(cand_funcs)}/{len(seamless_funcs)}/{len(original_funcs)}')
    print(f'origin functions still present      : {len(original_funcs & cand_funcs)}/{len(original_funcs)}')
    print(f'function symbols new vs seamless    : {len(cand_funcs - seamless_funcs)}')
    print()
    print(f'settings candidate/seamless         : {len(cand_keys)}/{len(seamless_keys)}')
    print(f'upstream settings dropped           : {len(set(seamless_keys) - set(cand_keys))}')
    print(f'settings added                      : {len(set(cand_keys) - set(seamless_keys))}')
    return 0


if __name__ == '__main__':
    sys.exit(main())
