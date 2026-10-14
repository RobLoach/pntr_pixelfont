#!/usr/bin/env python3
"""Regenerate pntr_pixelfont_data.h from uGUI's ugui.c.

pntr_pixelfont_data.h is not a vendored copy of anything: it is a one-time
mechanical conversion of the bitmap tables in uGUI's ugui.c into this repo's
own pntr_pixelfont_font struct and PNTR_PIXELFONT_* naming. There is no
upstream version to bump. This script is the record of that conversion, so the
next person does not have to guess what was done to the 4000-odd lines of data.

Usage:

    git clone https://github.com/achimdoebler/UGUI /tmp/ugui
    tools/convert_ugui_fonts.py /tmp/ugui/ugui.c --check
    tools/convert_ugui_fonts.py /tmp/ugui/ugui.c -o pntr_pixelfont_data.h

--check regenerates in memory and diffs against the committed header, which is
the quickest way to confirm that the data still matches upstream byte for byte.

What the conversion does:

  * Renames each `font_<w>x<h>` table to `PNTR_PIXELFONT_FONT_<w>x<h>` and
    changes `__UG_FONT_DATA unsigned char` to `const uint8_t`. The byte rows,
    including their trailing `// 0xNN` comments, are copied verbatim.
  * Renames each `#ifdef USE_FONT_<WXH>` guard to
    `#ifdef PNTR_PIXELFONT_ENABLE_<WXH>`, and adds PNTR_PIXELFONT_ENABLE_ALL.
  * Replaces uGUI's UG_FONT descriptors with the pntr_pixelfont_font table,
    keeping only the data pointer and the character width and height. uGUI's
    FONT_TYPE_1BPP, start_char, end_char and widths fields are dropped, since
    every one of these fonts is 1bpp fixed-width covering 0x00 to 0xFF.
  * Skips uGUI's USE_FONT_8X12_CYRILLIC table. Upstream declares it as a
    second array also called `font_8x12`, so uGUI itself can only compile one
    of the two at a time; pntr_pixelfont takes the non-Cyrillic one. Nothing
    else in ugui.c is left out: all 16 of uGUI's other fonts are converted.

The byte layout is uGUI's and is left untouched: one glyph is
ceil(char_width / 8) * char_height bytes, laid out as char_height rows of
ceil(char_width / 8) bytes, least significant bit leftmost, and glyph N starts
at offset N * ceil(char_width / 8) * char_height. That stride rule is what
pntr_load_pixelfont() computes as `bytesPerRow`; getting it wrong is what
broke the wide fonts in issue #1.
"""

import argparse
import difflib
import re
import sys

# uGUI arrays that are deliberately not converted, and why.
SKIPPED_GUARDS = {
    "USE_FONT_8X12_CYRILLIC": "duplicate font_8x12 array; uGUI can only build one",
}

ARRAY_RE = re.compile(
    r"#ifdef (USE_FONT_\w+)\n"
    r"__UG_FONT_DATA unsigned char font_(\w+)\[256\]\[(\d+)\]=\{\n"
    r"(.*?)\n\};",
    re.DOTALL,
)

DESCRIPTOR_RE = re.compile(
    r"const UG_FONT FONT_\w+ = \{\(unsigned char\*\)font_(\w+),"
    r"FONT_TYPE_1BPP,(\d+),(\d+),"
)

HEADER = """\
/**********************************************************************************************
*
*   pntr_pixelfont_data.h - Bitmap font tables for pntr_pixelfont.
*
*   GENERATED FILE. Do not hand-edit. Regenerate with:
*
*       git clone https://github.com/achimdoebler/UGUI /tmp/ugui
*       tools/convert_ugui_fonts.py /tmp/ugui/ugui.c -o pntr_pixelfont_data.h
*
*   See tools/convert_ugui_fonts.py for the full description of the conversion.
*
*   ORIGIN
*
*   Every PNTR_PIXELFONT_FONT_* array below is a verbatim copy of one
*   `__UG_FONT_DATA unsigned char font_<w>x<h>[256][...]` table from the single
*   file `ugui.c` in uGUI:
*
*       https://github.com/achimdoebler/UGUI
*
*   uGUI publishes no tags or releases. The tables here are byte-for-byte (in
*   fact character-for-character, trailing `// 0xNN` comments included)
*   identical to `ugui.c` at commit:
*
*       80a4aa755d5836ca576f30d47fb76d9500546818  ("V0.31 beta", 2015-12-20)
*
*   That is the most recent commit in uGUI's history to touch `ugui.c`, so it
*   is also the content of `ugui.c` on uGUI's master today; master's own tip,
*   ce0bccb5b7d4877c42081419fccadf7aa5727303 (2016-01-02), only edits
*   README.md. Verified by fetching uGUI and diffing all 16 tables on
*   2026-10-14.
*
*   Which commit the original conversion was actually run against is not
*   recorded anywhere and cannot be recovered from this repository; any uGUI
*   checkout from 80a4aa7 onwards gives the same bytes. Likewise there was no
*   generator script in the repo before this one, so whether the first
*   conversion was scripted or done by hand is unknown -
*   tools/convert_ugui_fonts.py is a reconstruction, verified by regenerating
*   this file and diffing it against what was committed.
*
*   LAYOUT
*
*   The byte layout is uGUI's, unchanged. For a font of char_width by
*   char_height:
*
*       bytesPerRow = ceil(char_width / 8)
*       glyph stride = bytesPerRow * char_height
*       glyph N starts at N * bytesPerRow * char_height
*
*   Each row is bytesPerRow bytes, least significant bit leftmost. The second
*   array dimension below is the glyph stride, so PNTR_PIXELFONT_FONT_10x16 is
*   [256][32] because ceil(10 / 8) * 16 == 32. pntr_load_pixelfont() recomputes
*   that same stride; mismatching it is what made the fonts wider than 8 pixels
*   render as garbage in issue #1, fixed in adde55a.
*
*   Only glyphs 0x20 through 0x7E are ever read out of these tables. The rest
*   of each table is kept as it came from uGUI, but is not reachable through
*   pntr_load_pixelfont(); see the comment on that function in
*   pntr_pixelfont.h.
*
*   LICENSE
*
*   uGUI is (C) 2015 Achim Döbler, all rights reserved, under uGUI's own terms:
*   free software, no warranty, no restriction on use, and redistributions of
*   source code must retain the copyright notice. The full notice is in
*   LICENSE.md of the uGUI repository linked above. The copyright notice is
*   retained at the top of pntr_pixelfont.h.
*
**********************************************************************************************/
"""


def parse(source):
    """Pull the font tables and their dimensions out of ugui.c."""
    sizes = {name: (int(w), int(h)) for name, w, h in DESCRIPTOR_RE.findall(source)}

    fonts = []
    for guard, name, stride, body in ARRAY_RE.findall(source):
        if guard in SKIPPED_GUARDS:
            continue
        if name not in sizes:
            raise SystemExit("no UG_FONT descriptor for font_%s" % name)

        width, height = sizes[name]
        stride = int(stride)
        expected = -(-width // 8) * height
        if stride != expected:
            raise SystemExit(
                "font_%s has stride %d, expected ceil(%d / 8) * %d == %d"
                % (name, stride, width, height, expected)
            )

        fonts.append(
            {
                "name": name,                 # e.g. "10x16"
                "macro": name.upper(),        # e.g. "10X16"
                "stride": stride,
                "width": width,
                "height": height,
                "body": body,
            }
        )

    if not fonts:
        raise SystemExit("found no __UG_FONT_DATA tables; is this ugui.c?")

    return fonts


def generate(fonts):
    out = [HEADER]
    out.append(
        "\n#ifndef PNTR_PIXELFONT_FONT_H_\n"
        "#define PNTR_PIXELFONT_FONT_H_\n"
        "\n"
        "#include <stdint.h>\n"
        "\n"
        "typedef struct pntr_pixelfont_font {\n"
        "    const uint8_t* data;\n"
        "    const uint8_t char_width;\n"
        "    const uint8_t char_height;\n"
        "} pntr_pixelfont_font;\n"
        "\n"
        "extern const pntr_pixelfont_font PNTR_PIXELFONT_FONTS[];\n"
        "\n"
        "#endif  // PNTR_PIXELFONT_FONT_H_\n"
        "\n"
        "#ifdef PNTR_PIXELFONT_DATA_IMPLEMENTATION\n"
        "#ifndef PNTR_PIXELFONT_DATA_IMPLEMENTATION_ONCE\n"
        "\n"
        "#ifdef PNTR_PIXELFONT_ENABLE_ALL\n"
    )

    for font in fonts:
        out.append(
            "#ifndef PNTR_PIXELFONT_ENABLE_%(macro)s\n"
            "#define PNTR_PIXELFONT_ENABLE_%(macro)s\n"
            "#endif\n" % font
        )

    out.append("#endif  // PNTR_PIXELFONT_ENABLE_ALL\n")

    for font in fonts:
        out.append(
            "\n#ifdef PNTR_PIXELFONT_ENABLE_%(macro)s\n"
            "const uint8_t PNTR_PIXELFONT_FONT_%(name)s[256][%(stride)d]={\n"
            "%(body)s\n"
            "};\n"
            "#endif\n" % font
        )

    out.append("\nconst pntr_pixelfont_font PNTR_PIXELFONT_FONTS[] = {\n")
    for index, font in enumerate(fonts):
        out.append(
            "    {\n"
            "        .data =\n"
            "        #ifdef PNTR_PIXELFONT_ENABLE_%(macro)s\n"
            "            (const uint8_t*)PNTR_PIXELFONT_FONT_%(name)s,\n"
            "        #else\n"
            "            NULL,\n"
            "        #endif\n"
            "        .char_width = %(width)d,\n"
            "        .char_height = %(height)d\n" % font
        )
        out.append("    }%s\n" % ("," if index + 1 < len(fonts) else ""))
    out.append("};\n")

    out.append(
        "\n#endif // PNTR_PIXELFONT_DATA_IMPLEMENTATION_ONCE\n"
        "#endif // PNTR_PIXELFONT_DATA_IMPLEMENTATION\n"
    )

    return "".join(out)


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("ugui_c", help="path to uGUI's ugui.c")
    parser.add_argument("-o", "--output", help="where to write the header")
    parser.add_argument(
        "--check",
        metavar="HEADER",
        nargs="?",
        const="pntr_pixelfont_data.h",
        help="diff against an existing header instead of writing one",
    )
    args = parser.parse_args()

    with open(args.ugui_c, encoding="utf-8", errors="replace") as handle:
        fonts = parse(handle.read())

    generated = generate(fonts)

    for font in fonts:
        print(
            "font_%(name)s -> PNTR_PIXELFONT_FONT_%(name)s  "
            "%(width)dx%(height)d, stride %(stride)d" % font,
            file=sys.stderr,
        )
    for guard, reason in sorted(SKIPPED_GUARDS.items()):
        print("skipped %s: %s" % (guard, reason), file=sys.stderr)

    if args.check:
        with open(args.check, encoding="utf-8", errors="replace") as handle:
            existing = handle.read()
        if existing == generated:
            print("%s is up to date" % args.check, file=sys.stderr)
            return 0
        diff = difflib.unified_diff(
            existing.splitlines(True),
            generated.splitlines(True),
            fromfile=args.check,
            tofile="generated",
            n=1,
        )
        sys.stdout.writelines(diff)
        return 1

    if args.output:
        with open(args.output, "w", encoding="utf-8") as handle:
            handle.write(generated)
    else:
        sys.stdout.write(generated)

    return 0


if __name__ == "__main__":
    sys.exit(main())
