#!/usr/bin/env bash
# gen-app-icon.sh — Generates app-icon.png (512x512) with SeamOS logo on black background
# Requirements: ImageMagick 6.x
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
UI_DIR="$(cd "$SCRIPT_DIR/../.." && pwd)"
LOGO="$UI_DIR/public/title-card/logo.png"
OUT="$UI_DIR/marketplace-assets/app-icon.png"

mkdir -p "$UI_DIR/marketplace-assets"

# Create 512x512 black canvas with centered SeamOS logo (scaled to 400px width)
convert -size 512x512 xc:black \
  \( "$LOGO" -resize 400x \) \
  -gravity center -composite \
  "$OUT"

echo "Done → $OUT"
identify "$OUT"
