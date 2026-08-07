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
    _HYPR,
};

enum custom_keycodes {
    CTL_ENT = SAFE_RANGE,
    CTL_SPC,
    WIN_NEQ,
};

// https://docs.qmk.fm/features/combo
// Definiere die Combo-Events

enum combo_events {
  EM_EMAIL,
  BSPC_LSFT_CLEAR,
  XC_LEADER, // damit xc den leader aktiviert
};

bool get_combo_must_tap(uint16_t combo_index, combo_t *combo) {
    // Prüfe, ob die Combo die Tap-Aktion benötigt
    switch (combo_index) {
        case BSPC_LSFT_CLEAR:  // Dein Combo-Name
            return true;       // Nur die Tap-Aktion erlauben
        default:
            return false;      // Andere Combos können normal funktionieren
    }
}
const uint16_t PROGMEM email_combo[] = {KC_Q, KC_W, COMBO_END};
#define HYPR_TAB LT(_HYPR, KC_TAB)
#define LOWER_DEL LT(_LOWER, KC_DEL)
#define UPPER_BSPC LT(_UPPER, KC_BSPC)

const uint16_t PROGMEM clear_line_combo[] = {CTL_ENT, UPPER_BSPC, COMBO_END};
const uint16_t PROGMEM xc_leader_combo[] = { KC_X, KC_C, COMBO_END };

combo_t key_combos[] = {
  [EM_EMAIL] = COMBO_ACTION(email_combo),
  [BSPC_LSFT_CLEAR] = COMBO_ACTION(clear_line_combo),
    [XC_LEADER] = COMBO(xc_leader_combo, QK_LEAD),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
  switch(combo_index) {
    case EM_EMAIL:
      if (pressed) {
        SEND_STRING("john.doe@example.com");
      }
      break;
    case BSPC_LSFT_CLEAR:
      if (pressed) {
        tap_code16(S(KC_HOME));
        tap_code16(KC_BSPC);
      }
      break;
  }
}
void leader_end_user(void) {
    if (leader_sequence_one_key(KC_S)) {
    }
    // } else if (leader_sequence_three_keys(KC_A, KC_R, KC_B)) {
    //     SEND_STRING("arbeit@example.com");
    // }
}

// #define LT_REP LT(_LOWER, KC_0)
#define ALT_REP MT(MOD_LALT, KC_0)
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

// Key Override für Morph Umlaute
const key_override_t alt_a_to_ae = KO_SUPPRESS(MOD_MASK_ALT, KC_A, DE_ADIA, MOD_MASK_ALT);
const key_override_t alt_o_to_oe = KO_SUPPRESS(MOD_MASK_ALT, KC_O, DE_ODIA, MOD_MASK_ALT);
const key_override_t alt_u_to_ue = KO_SUPPRESS(MOD_MASK_ALT, KC_U, DE_UDIA, MOD_MASK_ALT);
const key_override_t alt_s_to_ss = KO_SUPPRESS(MOD_MASK_ALT, KC_S, DE_SS, MOD_MASK_ALT);

// // Key Override für Windows allgemein
// const key_override_t hyper_del_to_ctrl_shift_enter = {
//     .trigger_mods      = MOD_MASK_CTRL | MOD_MASK_SHIFT | MOD_MASK_ALT | MOD_MASK_GUI,
//     .layers            = ~0,
//     .negative_mod_mask = 0,
//     .suppressed_mods   = MOD_MASK_CTRL | MOD_MASK_SHIFT | MOD_MASK_ALT | MOD_MASK_GUI,
//     .options           = ko_options_default,
//     .trigger           = KC_DEL,
//     .replacement       = C(S(KC_ENT)),
//     .custom_action     = NULL,
//     .context           = NULL,
//     .enabled           = NULL,
// };

bool delete_line_to_start(bool activated, void *context) {
    if (activated) {
        tap_code16(S(KC_HOME));
        tap_code(KC_BSPC);
    }
    return false;
}

static bool alt_tab_active = false;
static bool ctl_ent_pressed = false;
static bool ctl_ent_registered = false;
static bool ctl_ent_search_sent = false;
static bool ctl_ent_ctrl_enter_sent = false;
static uint16_t ctl_ent_timer = 0;
static bool ctl_spc_pressed = false;
static bool ctl_spc_registered = false;
static bool ctl_spc_search_sent = false;
static uint16_t ctl_spc_timer = 0;

static void tap_windows_search(void) {
    tap_code16(G(KC_S));
}

static void tap_ctrl_backspace_without_alt(void) {
    uint8_t mods = get_mods();
    uint8_t oneshot_mods = get_oneshot_mods();

    del_mods(MOD_MASK_ALT);
    del_oneshot_mods(MOD_MASK_ALT);
    tap_code16(C(KC_BSPC));
    set_mods(mods);
    set_oneshot_mods(oneshot_mods);
}

static void tap_backspace_without_shift(void) {
    uint8_t mods = get_mods();
    uint8_t oneshot_mods = get_oneshot_mods();

    del_mods(MOD_MASK_SHIFT);
    del_oneshot_mods(MOD_MASK_SHIFT);
    tap_code(KC_BSPC);
    set_mods(mods);
    set_oneshot_mods(oneshot_mods);
}

static void release_alt_tab(void) {
    if (alt_tab_active) {
        unregister_code(KC_LALT);
        alt_tab_active = false;
    }
}

static void tap_windows_not_equal(void) {
    register_code(KC_LALT);
    tap_code(KC_KP_8);
    tap_code(KC_KP_8);
    tap_code(KC_KP_0);
    tap_code(KC_KP_0);
    unregister_code(KC_LALT);
}

// Key Override für Windows Navigation
const key_override_t lalt_shift_f_to_lgui_up = {
    .trigger_mods    = MOD_BIT(KC_LALT) | MOD_BIT(KC_LSFT),
    .layers          = ~0,
    .negative_mod_mask = 0,
    .suppressed_mods = MOD_BIT(KC_LALT) | MOD_BIT(KC_LSFT),
    .options         = ko_options_default,
    .trigger         = KC_F,
    .replacement     = LGUI(KC_UP),
    .custom_action   = NULL,
    .context         = NULL,
    .enabled         = NULL,
};
const key_override_t lalt_shift_p_to_lgui_right = {
    .trigger_mods    = MOD_BIT(KC_LALT) | MOD_BIT(KC_LSFT),
    .layers          = ~0,
    .negative_mod_mask = 0,
    .suppressed_mods = MOD_BIT(KC_LALT) | MOD_BIT(KC_LSFT),
    .options         = ko_options_default,
    .trigger         = KC_P,
    .replacement     = LGUI(KC_RGHT),
    .custom_action   = NULL,
    .context         = NULL,
    .enabled         = NULL,
};
const key_override_t lalt_shift_w_to_lgui_left = {
    .trigger_mods    = MOD_BIT(KC_LALT) | MOD_BIT(KC_LSFT),
    .layers          = ~0,
    .negative_mod_mask = 0,
    .suppressed_mods = MOD_BIT(KC_LALT) | MOD_BIT(KC_LSFT),
    .options         = ko_options_default,
    .trigger         = KC_W,
    .replacement     = LGUI(KC_LEFT),
    .custom_action   = NULL,
    .context         = NULL,
    .enabled         = NULL,
};


const key_override_t lalt_q_to_lgui_1 = {
    .trigger_mods    = MOD_BIT(KC_LALT),
    .layers          = ~0,
    .negative_mod_mask = 0,
    .suppressed_mods = MOD_BIT(KC_LALT),
    .options         = ko_options_default,
    .trigger         = KC_Q,
    .replacement     = LGUI(KC_1),
    .custom_action   = NULL,
    .context         = NULL,
    .enabled         = NULL,
};
const key_override_t lalt_w_to_lgui_2 = {
    .trigger_mods    = MOD_BIT(KC_LALT),
    .layers          = ~0,
    .negative_mod_mask = 0,
    .suppressed_mods = MOD_BIT(KC_LALT),
    .options         = ko_options_default,
    .trigger         = KC_W,
    .replacement     = LGUI(KC_2),
    .custom_action   = NULL,
    .context         = NULL,
    .enabled         = NULL,
};
const key_override_t lalt_f_to_lgui_3 = {
    .trigger_mods    = MOD_BIT(KC_LALT),
    .layers          = ~0,
    .negative_mod_mask = 0,
    .suppressed_mods = MOD_BIT(KC_LALT),
    .options         = ko_options_default,
    .trigger         = KC_F,
    .replacement     = LGUI(KC_3),
    .custom_action   = NULL,
    .context         = NULL,
    .enabled         = NULL,
};
const key_override_t lalt_p_to_lgui_4 = {
    .trigger_mods    = MOD_BIT(KC_LALT),
    .layers          = ~0,
    .negative_mod_mask = 0,
    .suppressed_mods = MOD_BIT(KC_LALT),
    .options         = ko_options_default,
    .trigger         = KC_P,
    .replacement     = LGUI(KC_4),
    .custom_action   = NULL,
    .context         = NULL,
    .enabled         = NULL,
};
const key_override_t lalt_b_to_lgui_5 = {
    .trigger_mods    = MOD_BIT(KC_LALT),
    .layers          = ~0,
    .negative_mod_mask = 0,
    .suppressed_mods = MOD_BIT(KC_LALT),
    .options         = ko_options_default,
    .trigger         = KC_B,
    .replacement     = LGUI(KC_5),
    .custom_action   = NULL,
    .context         = NULL,
    .enabled         = NULL,
};

const key_override_t lalt_d_to_lgui_d = KO_SUPPRESS(MOD_BIT(KC_LALT), KC_D, LGUI(KC_D), MOD_BIT(KC_LALT));

const key_override_t lalt_backspace_to_lctl_backspace = KO_SUPPRESS(MOD_BIT(KC_LALT), KC_BSPC, C(KC_BSPC), MOD_BIT(KC_LALT));
const key_override_t lalt_del_to_lctl_del = KO_SUPPRESS(MOD_BIT(KC_LALT), KC_DEL, C(KC_DEL), MOD_BIT(KC_LALT));

const key_override_t lshift_lgui_space_to_lctl_enter = ko_make_basic(MOD_MASK_SHIFT | MOD_MASK_GUI, KC_SPACE, C(KC_ENT));

const key_override_t lgui_w_to_lalt_f4 = ko_make_basic(MOD_MASK_GUI, KC_W, A(KC_F4));

const key_override_t lgui_backspace_to_delete_line_to_start = {
    .trigger_mods      = MOD_BIT(KC_LGUI),
    .layers            = ~0,
    .negative_mod_mask = MOD_MASK_ALT,
    .suppressed_mods   = MOD_BIT(KC_LGUI),
    .options           = ko_options_default,
    .trigger           = KC_BSPC,
    .replacement       = KC_NO,
    .custom_action     = delete_line_to_start,
    .context           = NULL,
    .enabled           = NULL,
};


// Array von Key Overrides
const key_override_t *key_overrides[] = {
    // Key Override für Morph Umlaute
    &alt_a_to_ae,
    &alt_o_to_oe,
    &alt_u_to_ue,
    &alt_s_to_ss,

    // Key Override für Windows
    // &hyper_del_to_ctrl_shift_enter,

    // Key Override für Windows Navigation
    &lalt_shift_f_to_lgui_up,
    &lalt_shift_p_to_lgui_right,
    &lalt_shift_w_to_lgui_left,
    &lalt_q_to_lgui_1,
    &lalt_w_to_lgui_2,
    &lalt_f_to_lgui_3,
    &lalt_p_to_lgui_4,
    &lalt_b_to_lgui_5,
    &lalt_d_to_lgui_d,

    &lalt_backspace_to_lctl_backspace,
    &lalt_del_to_lctl_del,

    &lshift_lgui_space_to_lctl_enter,

    &lgui_w_to_lalt_f4,  // Neuer Key Override
    &lgui_backspace_to_delete_line_to_start,
    NULL // Array muss mit NULL enden
};


bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (record->event.pressed) {
    if (ctl_ent_pressed && keycode != CTL_ENT && keycode != CTL_SPC && keycode != HYPR_TAB && !ctl_ent_registered) {
      register_code(KC_LCTL);
      ctl_ent_registered = true;
    } else if (ctl_spc_pressed && keycode != CTL_ENT && keycode != CTL_SPC && !ctl_spc_registered) {
      register_code(KC_LCTL);
      ctl_spc_registered = true;
    }
  }

  switch (keycode) {
    case HYPR_TAB:
      if (record->event.pressed && (ctl_ent_pressed || alt_tab_active || (get_mods() & MOD_MASK_GUI))) {
        del_mods(MOD_MASK_GUI);
        ctl_ent_search_sent = true;
        register_code(KC_LALT);
        tap_code(KC_TAB);
        alt_tab_active = true;
        return false;
      }
      if (!record->event.pressed && alt_tab_active) {
        return false;
      }
      break;

    case CTL_ENT:
      if (record->event.pressed) {
        if (ctl_spc_pressed) {
          tap_code16(C(KC_ENT));
          ctl_ent_ctrl_enter_sent = true;
          return false;
        }
        ctl_ent_pressed = true;
        ctl_ent_registered = false;
        ctl_ent_search_sent = false;
        ctl_ent_timer = timer_read();
      } else {
        if (ctl_ent_ctrl_enter_sent) {
          ctl_ent_ctrl_enter_sent = false;
          return false;
        }
        if (ctl_ent_registered) {
          unregister_code(KC_LCTL);
        } else if (!ctl_ent_search_sent && timer_elapsed(ctl_ent_timer) < TAPPING_TERM) {
          tap_code(KC_ENT);
        }
        ctl_ent_pressed = false;
        ctl_ent_registered = false;
        ctl_ent_search_sent = false;
        release_alt_tab();
      }
      return false;

    case CTL_SPC:
      if (record->event.pressed) {
        if (ctl_ent_pressed) {
          tap_windows_search();
          ctl_ent_search_sent = true;
          ctl_spc_search_sent = true;
          return false;
        }
        ctl_spc_pressed = true;
        ctl_spc_registered = false;
        ctl_spc_timer = timer_read();
      } else {
        if (ctl_spc_search_sent) {
          ctl_spc_search_sent = false;
          return false;
        }
        if (ctl_spc_registered) {
          unregister_code(KC_LCTL);
        } else if (timer_elapsed(ctl_spc_timer) < TAPPING_TERM) {
          tap_code(KC_SPACE);
        }
        ctl_spc_pressed = false;
        ctl_spc_registered = false;
      }
      return false;

    case UPPER_BSPC:
      if (record->tap.count && record->event.pressed && ((get_mods() | get_oneshot_mods()) & MOD_MASK_ALT)) {
        tap_ctrl_backspace_without_alt();
        return false;
      }
      break;

    case LOWER_DEL:
      if (record->tap.count && record->event.pressed && ((get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT)) {
        tap_backspace_without_shift();
        return false;
      }
      break;

    case WIN_NEQ:
      if (record->event.pressed) {
        tap_windows_not_equal();
      }
      return false;

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
//
/* bool process_record_user(uint16_t keycode, keyrecord_t *record) { */
/*     if (record->event.pressed) { */
/*         bool alt_held = get_mods() & MOD_MASK_ALT;    // Prüfen, ob Alt gehalten wird */
/*         bool shift_held = get_mods() & MOD_BIT(KC_LSFT); // Prüfen, ob Shift gehalten wird */
        /* bool right_shift_held = get_mods() & MOD_BIT(KC_RSFT); // Prüfen, ob Shift gehalten wird */

/*         switch (keycode) { */
/*             case MORPH_AE: */
/*                 if (alt_held && shift_held) { */
/*                     uint8_t mods = get_mods(); // Aktuelle Modifikatoren speichern */
/*                     del_mods(MOD_MASK_ALT | MOD_BIT(KC_LSFT)); // Alt und Shift entfernen */
/*                     tap_code16(S(DE_ADIA)); // Shift DE_ADIA -> Ä */
/*                     set_mods(mods); // Modifikatoren wiederherstellen */
/*                 } else if (alt_held) { */
/*                     uint8_t mods = get_mods(); // Aktuelle Modifikatoren speichern */
/*                     del_mods(MOD_MASK_ALT); // Alt entfernen */
/*                     tap_code(DE_ADIA); // DE_ADIA -> ä */
/*                     set_mods(mods); // Modifikatoren wiederherstellen */
/*                 } else { */
/*                     tap_code(KC_A); // Standard: A */
/*                 } */
/*                 return false; */
/**/
/*         } */
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


/* void matrix_scan_user(void) { */
/*     if (backspace_active && timer_elapsed(backspace_timer) > 150) { */
/*         tap_code(KC_BSPC); // Wiederhole Backspace */
/*         backspace_timer = timer_read(); // Timer zurücksetzen */
/*     } */
/* } */

// nicht mehr notwendig da Layer über HYPR gelöst
// void matrix_scan_user(void) {
//     if (layer_state_is(_LOWER) && layer_state_is(_UPPER)) {
//         layer_on(_FUNCTION); // Aktiviere das Function-Layer
//     } else {
//         layer_off(_FUNCTION); // Deaktiviere das Function-Layer
//     }
// }


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x6_3(
        HYPR_TAB,                KC_Q,       KC_W,       KC_F,       KC_P,       KC_B,                                  KC_J,           KC_L,        KC_U,        KC_Z,        KC_RBRC,     KC_NUHS,
        MT(MOD_LSFT, KC_ESC),    KC_A,       KC_R,       KC_S,       KC_T,       KC_G,                                  KC_M,           KC_N,        KC_E,        KC_I,        KC_O,        KC_RSFT,
        KC_LCTL,                 KC_Y,       KC_X,       KC_C,       KC_D,       KC_V,                                  KC_K,           KC_H,        KC_COMM,     KC_DOT,      KC_SLSH,     KC_NUBS,
                                         ALT_REP,    LOWER_DEL,  CTL_ENT,                   CTL_SPC,  UPPER_BSPC,     KC_RALT
    ),

    [_LOWER] = LAYOUT_split_3x6_3(
        KC_GRV,       LSFT(KC_1), LSFT(KC_2), LSFT(KC_3),  LSFT(KC_4),  LSFT(KC_5),                                       LSFT(KC_6),         LSFT(KC_7),         LSFT(KC_8),     LSFT(KC_9), LSFT(KC_0), LSFT(KC_MINS),
        _______,      KC_1,       KC_2,       KC_3,        KC_4,        KC_5,                                             DE_LABK,        DE_BSLS,        DE_LCBR,    DE_RCBR, WIN_NEQ,    DE_ACUT,
        _______,      KC_6,       KC_7,       KC_8,        KC_9,        KC_0,                                             DE_RABK,        DE_PIPE,        DE_LBRC,    DE_RBRC, DE_QUOT,    DE_QUOT,
                                                        _______,     _______,    _______,                    _______, _______, _______
    ),
    [_UPPER] = LAYOUT_split_3x6_3(
        KC_NO,      KC_NO,       KC_NO,       KC_NO,        LGUI(LSFT(KC_S)),       QK_REP,                    KC_HOME,             LCTL(KC_HOME),       LCTL(KC_END),           KC_END,    KC_NO,      QK_BOOT,
         _______,   KC_NO,       KC_NO,       KC_NO,        KC_NO,       KC_NO,                                KC_LEFT,             KC_DOWN,             KC_UP,             KC_RGHT,   KC_NO,      KC_NO,
         _______,   KC_NO,       KC_NO,       KC_NO,        KC_NO,       KC_NO,                                LCTL(KC_LEFT),       KC_PAGE_DOWN,        KC_PAGE_UP,        LCTL(KC_RGHT),    KC_MNXT,    KC_MPLY,

                                                        _______,    _______,    _______,                    _______,  _______,    _______
    ),
    [_HYPR] = LAYOUT_split_3x6_3(
        KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      QK_REP,                                            KC_NO,      KC_NO,      KC_NO,      KC_NO,     KC_NO,      KC_NO,
        _______,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,                                            KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_SCLN,    QK_BOOT,
        KC_NO,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,                                           KC_NO,      KC_MPRV,    KC_VOLD,    KC_VOLU,    KC_MNXT,    KC_MPLY,
        _______,    LCTL(LALT(KC_DEL)),    KC_SPACE,                    _______,  _______,    _______


    )
};
