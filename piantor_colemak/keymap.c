// Copyright 2022 beekeeb
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include  "keymap_german.h"

// Globale Variablen für die Backspace-Wiederholung
/* static bool backspace_active = false;  // Status der Backspace-Taste */
/* static uint16_t backspace_timer = 0;   // Timer für die Wiederholung */
/* static bool first_repeat = true;       // Status der ersten Wiederholung */


enum layers {
    _BASE,
    _LOWER,
    _UPPER,
    _FUNCTION
};

#define HYPR_TAB MT(MOD_HYPR, KC_TAB)
#define HYPR_Y MT(MOD_HYPR, KC_Y)
#define HYPR_SLSH MT(MOD_HYPR, KC_SLSH)
#define ALT_REP MT(MOD_LALT, KC_0)
#define LOWER_DEL LT(_LOWER, KC_DEL)
#define UPPER_BSPC LT(_UPPER, KC_BSPC)
#define GUI_ENT MT(MOD_LGUI, KC_ENT)
#define GUI_SPC MT(MOD_LGUI, KC_SPC)

// Use `ALT_REP` in your layout...
// https://getreuer.info/posts/keyboards/faqs/index.html#layer-tap-repeat-key
bool remember_last_key_user(uint16_t keycode, keyrecord_t* record,
                            uint8_t* remembered_mods) {
  if (keycode == ALT_REP) { return false; }
  return true;
}

#define KO_SUPPRESS(trigger_mods_, trigger_, replacement_, suppressed_mods_) \
    { \
        .trigger_mods      = (trigger_mods_), \
        .layers            = ~0, \
        .negative_mod_mask = 0, \
        .suppressed_mods   = (suppressed_mods_), \
        .options           = ko_options_default, \
        .trigger           = (trigger_), \
        .replacement       = (replacement_), \
        .custom_action     = NULL, \
        .context           = NULL, \
        .enabled           = NULL, \
    }

// Key Override
const key_override_t lshift_lgui_space_to_lctl_enter = ko_make_basic(MOD_MASK_SHIFT | MOD_MASK_GUI, KC_SPACE, G(KC_ENT));

// Key Override für Morph Umlaute
const key_override_t alt_a_to_ae = KO_SUPPRESS(MOD_BIT(KC_LALT), KC_A, DE_ADIA, MOD_BIT(KC_LALT));
const key_override_t alt_o_to_oe = KO_SUPPRESS(MOD_BIT(KC_LALT), KC_O, DE_ODIA, MOD_BIT(KC_LALT));
const key_override_t alt_u_to_ue = KO_SUPPRESS(MOD_BIT(KC_LALT), KC_U, DE_UDIA, MOD_BIT(KC_LALT));
const key_override_t alt_s_to_ss = KO_SUPPRESS(MOD_BIT(KC_LALT), KC_S, DE_SS, MOD_BIT(KC_LALT));

/* const key_override_t hyper_nuhs_to_shift_minus = ko_make_with_layers_and_negmods( */
const key_override_t hyper_nuhs_to_shift_minus = ko_make_basic(
    MOD_HYPR,               // Erwarte Hyper (Ctrl + Alt + Shift + GUI)
    KC_NUHS,                // Erwarte NUHS
    LSFT(KC_MINS)          // Ersetze mit Shift + Minus
);
// Array von Key Overrides
const key_override_t *key_overrides[] = {
    // Key Override für Morph Umlaute
    &alt_a_to_ae,
    &alt_o_to_oe,
    &alt_u_to_ue,
    &alt_s_to_ss,

    &lshift_lgui_space_to_lctl_enter,
    &hyper_nuhs_to_shift_minus,
    NULL // Array muss mit NULL enden
};

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    case ALT_REP:  // LALT on hold, Repeat Key on tap.
      if (record->tap.count) {  // On tap.
        repeat_key_invoke(&record->event);  // Repeat the last key.
        return false;  // Skip default handling.
      }
      break;

    // Other macros...
  }
  return true;  // Continue default handling.
}


// Custom keycodes
/* enum custom_keycodes { */
/*     MORPH_AE = SAFE_RANGE, */
/*     MORPH_OE, */
/*     MORPH_UE, */
/*     MORPH_SS, */
/*     MORPH_SPACE, */
/*     MORPH_BSPC */
/* }; */

// Custom behavior for morph keys
/* bool process_record_user(uint16_t keycode, keyrecord_t *record) { */
/*     if (record->event.pressed) { */
/*         bool alt_held = get_mods() & MOD_MASK_ALT;    // Prüfen, ob Alt gehalten wird */
/*         bool shift_held = get_mods() & MOD_BIT(KC_LSFT); // Prüfen, ob Shift gehalten wird */
        /* bool right_shift_held = get_mods() & MOD_BIT(KC_RSFT); // Prüfen, ob Shift gehalten wird */

        /* switch (keycode) { */
        /*     case MORPH_AE: */
        /*         if (alt_held && shift_held) { */
        /*             uint8_t mods = get_mods(); // Aktuelle Modifikatoren speichern */
        /*             del_mods(MOD_MASK_ALT | MOD_BIT(KC_LSFT)); // Alt und Shift entfernen */
        /*             tap_code16(S(DE_ADIA)); // Shift DE_ADIA -> Ä */
        /*             set_mods(mods); // Modifikatoren wiederherstellen */
        /*         } else if (alt_held) { */
        /*             uint8_t mods = get_mods(); // Aktuelle Modifikatoren speichern */
        /*             del_mods(MOD_MASK_ALT); // Alt entfernen */
        /*             tap_code(DE_ADIA); // DE_ADIA -> ä */
        /*             set_mods(mods); // Modifikatoren wiederherstellen */
        /*         } else { */
        /*             tap_code(KC_A); // Standard: A */
        /*         } */
        /*         return false; */
        /**/
        /*     case MORPH_OE: */
        /*         if (alt_held && shift_held) { */
        /*             uint8_t mods = get_mods(); // Aktuelle Modifikatoren speichern */
        /*             del_mods(MOD_MASK_ALT | MOD_BIT(KC_LSFT)); // Alt und Shift entfernen */
        /*             tap_code16(S(DE_ODIA)); // Shift DE_ADIA -> Ä */
        /*             set_mods(mods); // Modifikatoren wiederherstellen */
        /*         } else if (alt_held) { */
        /*             uint8_t mods = get_mods(); // Aktuelle Modifikatoren speichern */
        /*             del_mods(MOD_MASK_ALT); // Alt entfernen */
        /*             tap_code(DE_ODIA); // DE_ADIA -> ä */
        /*             set_mods(mods); // Modifikatoren wiederherstellen */
        /*         } else { */
        /*             tap_code(KC_O); // Standard: A */
        /*         } */
        /*         return false; */
        /**/
        /*     case MORPH_UE: */
        /*         if (alt_held && shift_held) { */
        /*             uint8_t mods = get_mods(); // Aktuelle Modifikatoren speichern */
        /*             del_mods(MOD_MASK_ALT | MOD_BIT(KC_LSFT)); // Alt und Shift entfernen */
        /*             tap_code16(S(DE_UDIA)); // Shift DE_ADIA -> Ä */
        /*             set_mods(mods); // Modifikatoren wiederherstellen */
        /*         } else if (alt_held) { */
        /*             uint8_t mods = get_mods(); // Aktuelle Modifikatoren speichern */
        /*             del_mods(MOD_MASK_ALT); // Alt entfernen */
        /*             tap_code(DE_UDIA); // DE_ADIA -> ä */
        /*             set_mods(mods); // Modifikatoren wiederherstellen */
        /*         } else { */
        /*             tap_code(KC_U); // Standard: A */
        /*         } */
        /*         return false; */
        /**/
        /*     case MORPH_SS: */
        /*         if (alt_held) { */
        /*             uint8_t mods = get_mods(); // Aktuelle Modifikatoren speichern */
        /*             del_mods(MOD_MASK_ALT| MOD_BIT(KC_LSFT)); // Alt und Shift entfernen */
        /*             tap_code(DE_SS); // DE_ADIA -> ä */
        /*             set_mods(mods); // Modifikatoren wiederherstellen */
        /*         } else { */
        /*             tap_code(KC_S); // Standard: A */
        /*         } */
        /*         return false; */

            /* case MORPH_SPACE: */
            /*     if (right_shift_held) { */
            /*         uint8_t mods = get_mods(); // Aktuelle Modifikatoren speichern */
            /*         del_mods(MOD_BIT(KC_RSFT)); // Alt und Shift entfernen */
            /*         tap_code(KC_RGUI); // DE_ADIA -> ä */
            /*         set_mods(mods); // Modifikatoren wiederherstellen */
            /*     } else { */
            /*         tap_code(KC_SPACE); // Standard: A */
            /*     } */
            /*     return false; */

            /* case MORPH_BSPC: */
            /*     if (shift_held) { */
            /*         uint8_t mods = get_mods(); // Aktuelle Modifikatoren speichern */
            /*         del_mods(MOD_BIT(KC_LSFT)); // Alt und Shift entfernen */
            /*         tap_code(KC_DEL); // DE_ADIA -> ä */
            /*         set_mods(mods); // Modifikatoren wiederherstellen */
            /*     } else { */
            /*         tap_code(KC_BSPC); // Standard: A */
            /*     } */
            /*     return false; */

            /* case MORPH_BSPC: */
            /*     if (shift_held) { */
            /*         uint8_t mods = get_mods(); // Aktuelle Modifikatoren speichern */
            /*         del_mods(MOD_BIT(KC_LSFT)); // Shift entfernen */
            /*         tap_code(KC_DEL); // Shift + Backspace -> Delete */
            /*         set_mods(mods); // Modifikatoren wiederherstellen */
            /*     } else { */
            /*         // Aktivieren der Wiederholung für Backspace */
            /*         backspace_active = true; // Markiere, dass Backspace gehalten wird */
            /*         backspace_timer = timer_read(); // Timer für Wiederholung starten */
            /*         first_repeat = true;            // Erste Wiederholung aktivieren */
            /*         tap_code(KC_BSPC); // Backspace initial senden */
            /*     } */
            /*     return false; */

        /*     default: */
        /*         break; */
        /* } */
    /* } else { */
    /*     switch (keycode) { */
    /*         case MORPH_BSPC: */
    /*             // Deaktivieren, wenn Taste losgelassen wird */
    /*             backspace_active = false; */
    /*             first_repeat = true; // Reset für die nächste Nutzung */
    /*             break; */
    /**/
    /*         default: */
    /*             break; */
    /**/
    /**/
    /*     } */
/*     } */
/*     return true; */
/* } */

// ZMK-Vorlage hat keinen Tri-Layer; LOWER und UPPER bleiben getrennt.


/* void matrix_scan_user(void) { */
/*     if (backspace_active && timer_elapsed(backspace_timer) > 150) { */
/*         tap_code(KC_BSPC); // Wiederhole Backspace */
/*         backspace_timer = timer_read(); // Timer zurücksetzen */
/*     } */
/* } */


/* void matrix_scan_user(void) { */
/*     static bool first_repeat = true; // Markiere, ob es die erste Wiederholung ist */
/**/
/*     if (backspace_active) { */
/*         if (first_repeat && timer_elapsed(backspace_timer) > 150) { // Initiale Verzögerung: 150 ms */
/*             tap_code(KC_BSPC);          // Sende erste Wiederholung */
/*             backspace_timer = timer_read(); */
/*             first_repeat = false;       // Wechsel zur schnelleren Wiederholung */
/*         } else if (!first_repeat && timer_elapsed(backspace_timer) > 85) { // Wiederholrate: 50 ms */
/*             tap_code(KC_BSPC); */
/*             backspace_timer = timer_read(); */
/*         } */
/*     } */
/* } */


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x6_3(
        HYPR_TAB,               KC_Q,       KC_W,       KC_F,       KC_P,       KC_B,                                                               KC_J,           KC_L,           KC_U,     KC_Z,    KC_RBRC,     HYPR_TAB,
        MT(MOD_LSFT, KC_ESC),   KC_A,       KC_R,       KC_S,       KC_T,       KC_G,                                                               KC_M,           KC_N,           KC_E,     KC_I,    KC_O,        KC_RSFT,
        KC_LCTL,                HYPR_Y,     KC_X,       KC_C,       KC_D,       KC_V,                                                               KC_K,           KC_H,           KC_COMM,  KC_DOT,  HYPR_SLSH,    KC_LCTL,
                                                                    ALT_REP,    LOWER_DEL, GUI_ENT,                                      GUI_SPC, UPPER_BSPC,    KC_LALT
    ),

    [_LOWER] = LAYOUT_split_3x6_3(
        KC_GRV,     LSFT(KC_1), LSFT(KC_2), LSFT(KC_3),  LSFT(KC_4),  LSFT(KC_5),                                      LSFT(KC_6),         LSFT(KC_7),         LSFT(KC_8),           LSFT(KC_9),         LSFT(KC_0),     DE_QUES,
        _______,    KC_1,       KC_2,       KC_3,        KC_4,        KC_5,                                            KC_NUBS,            LSFT(LALT(KC_7)),   LALT(KC_8),           LALT(KC_9),         RALT(KC_0),     KC_EQL,
        _______,    KC_6,       KC_7,       KC_8,        KC_9,        KC_0,                                            LSFT(KC_NUBS),      LALT(KC_7),         LSFT(LGUI(KC_8)),     LSFT(LGUI(KC_9)),   KC_PIPE,        KC_PIPE,
                                                        _______,     _______,    _______,                    _______, _______, _______
    ),
    [_UPPER] = LAYOUT_split_3x6_3(
        QK_BOOT,   KC_NO,      KC_NO,      KC_NO,      LGUI(LSFT(KC_4)), KC_NO,                                       LGUI(KC_LEFT),      LGUI(KC_UP),        LGUI(KC_DOWN),       LGUI(KC_RGHT),      KC_NO,          QK_BOOT,
        _______,   KC_NO,      KC_BRIU,    KC_MPRV,    KC_MNXT,    KC_MPLY,                                           KC_LEFT,            KC_DOWN,            KC_UP,              KC_RGHT,            LALT(KC_HOME),  _______,
        KC_NO,     KC_NO,      KC_BRID,    KC_VOLD,    KC_VOLU,    KC_MUTE,                                           LALT(KC_LEFT),      KC_PGDN,            KC_PGUP,            LALT(KC_RGHT),      KC_NO,          KC_NO,
                                                        _______,    _______,    _______,                    _______,  _______,    _______

    ),
    [_FUNCTION] = LAYOUT_split_3x6_3(
        KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      QK_REP,                                           KC_NO,      KC_NO,      KC_NO,      KC_NO,     KC_NO,      KC_NO,
        _______,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,                                            KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_SCLN,    QK_BOOT,
        KC_NO,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,                                           KC_NO,      KC_MPRV,    KC_VOLD,    KC_VOLU,    KC_MNXT,    KC_MPLY,
        _______,    _______,    _______,                    _______,  _______,    _______


    )

};

