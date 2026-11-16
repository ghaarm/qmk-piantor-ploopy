# Ploppy A+

### Ploppy umgedreht
- https://github.com/bleys43/ploopy_aplus_qmk_firmware/tree/ploopyco/a%2B/keyboards/ploopyco/aplus/keymaps/bleys43-top-knobs
- https://www.reddit.com/r/ploopy/comments/1w4s42l/%C9%90_my_upsidedown_a/

Für spätere Builds kannst du von jedem Verzeichnis aus Folgendes ins Terminal kopieren:

SOURCE_DIR='/Users/g/Library/Mobile Documents/com~apple~CloudDocs/!Docs iCloud/qmk-icloud'
QMK_DIR='/private/tmp/ploopy-qmk-build'

rsync -a "$SOURCE_DIR/ploopy-mac-90/" "$QMK_DIR/keyboards/ploopyco/aplus/keymaps/ploopy-mac-90/" &&
PATH=/private/tmp/arm-gnu-toolchain/Payload/bin:$PATH make -C "$QMK_DIR" ploopyco/aplus/rev2_001:ploopy-mac-90 &&
cp "$QMK_DIR/ploopyco_aplus_rev2_001_ploopy-mac-90.uf2" "$SOURCE_DIR/"

Der Befehl nutzt die derzeit vorhandenen Verzeichnisse unter /private/tmp. Wenn macOS sie später löscht, muss die Build-Umgebung erneut eingerichtet werden.
