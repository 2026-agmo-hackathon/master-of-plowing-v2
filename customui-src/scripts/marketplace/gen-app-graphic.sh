#!/usr/bin/env bash
# gen-app-graphic.sh — Generates ui/marketplace-assets/app-graphic.png (1024x1024)
# Requirements: ImageMagick 6.x
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
UI_DIR="$(cd "$SCRIPT_DIR/../.." && pwd)"
PUBLIC="$UI_DIR/public"
OUT="$UI_DIR/marketplace-assets/app-graphic.png"
TMP="$SCRIPT_DIR/tmp-graphic"

mkdir -p "$TMP" "$UI_DIR/marketplace-assets"

BG="$PUBLIC/title-card/bg-field.jpg"
TRACTOR="$PUBLIC/tractors/200hp.png"
LOGO="$PUBLIC/title-card/logo.png"

FONT_BOLD="/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"

# ── Step 1: Scale bg-field.jpg to cover 1024x1024 (center-crop) ──────────────
# Source is 600x354. We need to cover 1024x1024.
# Scale up to at least 1024 in both dimensions, then crop center.
convert "$BG" \
  -resize "1024x1024^" \
  -gravity Center \
  -extent 1024x1024 \
  "$TMP/bg_1024.png"

# ── Step 2: Create left-to-right dark gradient overlay ────────────────────────
# Left side darker (0.70 alpha black) → right side lighter (0.20 alpha black)
# ImageMagick gradient: use a PNG with gradient, then composite as multiply/screen
# We create a 1024x1024 gradient from rgba(0,0,0,180) → rgba(0,0,0,51)
convert \
  -size 1024x1024 \
  gradient:"rgba(0,0,0,0.80)-rgba(0,0,0,0.15)" \
  "$TMP/overlay_gradient.png"

# Composite overlay onto background
convert "$TMP/bg_1024.png" \
  "$TMP/overlay_gradient.png" \
  -composite \
  "$TMP/bg_with_overlay.png"

# ── Step 3: Prepare tractor (resize to 720px wide, keep aspect) ──────────────
# 200hp.png is 767x508. At 720 wide → 480 tall.
convert "$TRACTOR" \
  -resize 720x \
  "$TMP/tractor_720.png"

# Add soft drop shadow (generate shadow layer, then composite)
# Shadow: clone, colorize black, blur, offset, put under tractor
convert "$TMP/tractor_720.png" \
  \( +clone -background "rgba(0,0,0,0.55)" -shadow 60x18+0+16 \) \
  +swap \
  -background none \
  -layers merge \
  "$TMP/tractor_shadow.png"

# ── Step 4: Composite tractor onto background ─────────────────────────────────
# Position: lower-right. Canvas 1024x1024.
# Tractor at 720x~480 → right edge at 1024-40=984, bottom at 1024-60=964
# offset from NorthWest: x = 1024 - 720 - 20 = 284, y = 1024 - 480 - 60 = 484
TRACTOR_W=$(identify -format "%w" "$TMP/tractor_shadow.png")
TRACTOR_H=$(identify -format "%h" "$TMP/tractor_shadow.png")
TRACTOR_X=$(( 1024 - TRACTOR_W - 10 ))
TRACTOR_Y=$(( 1024 - TRACTOR_H - 40 ))

convert "$TMP/bg_with_overlay.png" \
  "$TMP/tractor_shadow.png" \
  -gravity NorthWest \
  -geometry "+${TRACTOR_X}+${TRACTOR_Y}" \
  -composite \
  "$TMP/with_tractor.png"

# ── Step 5: Prepare logo (SeamOS wordmark, top-right, ~150px wide) ────────────
convert "$LOGO" \
  -resize 150x \
  "$TMP/logo_150.png"

LOGO_W=$(identify -format "%w" "$TMP/logo_150.png")
LOGO_H=$(identify -format "%h" "$TMP/logo_150.png")
LOGO_X=$(( 1024 - LOGO_W - 40 ))
LOGO_Y=40

convert "$TMP/with_tractor.png" \
  "$TMP/logo_150.png" \
  -gravity NorthWest \
  -geometry "+${LOGO_X}+${LOGO_Y}" \
  -composite \
  "$TMP/with_logo.png"

# ── Step 6: Add subtitle tag ("MASTER OF PLOWING SIMULATOR") ─────────────────
# Small green, letter-spaced appearance via space-padded text, ~24pt
convert "$TMP/with_logo.png" \
  -font "$FONT_BOLD" \
  -pointsize 26 \
  -fill "#16C172" \
  -gravity NorthWest \
  -annotate +80+80 "SEAMOS HACKATHON 2026" \
  "$TMP/with_subtitle.png"

# ── Step 7: Add main title "Master of\nPlowing." ─────────────────────────────
# Line 1: "Master of"
convert "$TMP/with_subtitle.png" \
  -font "$FONT_BOLD" \
  -pointsize 128 \
  -fill "#FFFFFF" \
  -gravity NorthWest \
  -annotate +80+118 "Master of" \
  "$TMP/with_title1.png"

# Line 2: "Plowing" in white, then "." in green
# We do "Plowing." in white first, then overlay a green period
# Measure approximate width of "Plowing" at 128pt:
# Rough estimate: ~70px per char at 128pt → "Plowing" ~7chars → ~490px
# We annotate "Plowing." in white, then overdraw just "." in green at offset
convert "$TMP/with_title1.png" \
  -font "$FONT_BOLD" \
  -pointsize 128 \
  -fill "#FFFFFF" \
  -gravity NorthWest \
  -annotate +80+258 "Plowing." \
  "$TMP/with_title2.png"

# Overdraw the period character in green
# At 128pt DejaVu Sans Bold, "Plowing" width is roughly 530px.
# Position period at x=80+530=610, y=258 (same baseline)
convert "$TMP/with_title2.png" \
  -font "$FONT_BOLD" \
  -pointsize 128 \
  -fill "#16C172" \
  -gravity NorthWest \
  -annotate +614+258 "." \
  "$OUT"

# ── Step 8: Cleanup temp ──────────────────────────────────────────────────────
rm -rf "$TMP"

echo "Done → $OUT"
identify "$OUT"
