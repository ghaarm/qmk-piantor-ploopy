#!/usr/bin/env bash

set -euo pipefail

# Ordner, in dem dieses Skript liegt
SOURCE_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

TARGET_DIR="/Users/g/qmk_firmware/keyboards/beekeeb/piantor_pro/keymaps"

mkdir -p "$TARGET_DIR"

find "$SOURCE_DIR" \
    -mindepth 1 \
    -maxdepth 1 \
    -type d \
    -name '*piantor*' \
    -print0 |
while IFS= read -r -d '' directory; do
    echo "Kopiere: $(basename "$directory")"
    rsync -a "$directory" "$TARGET_DIR/"
done

echo "Kopiervorgang abgeschlossen."
