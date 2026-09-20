// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U

#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_TX_PIN GP0
#define SERIAL_USART_RX_PIN GP1
#define SERIAL_USART_PIN_SWAP

#define USB_VBUS_PIN GP19

#define SPLIT_HAND_PIN GP17
#define SPLIT_HAND_PIN_LOW_IS_LEFT

#define MASTER_LEFT

// #define SPLIT_WATCHDOG_ENABLE
// #define SPLIT_WATCHDOG_TIMEOUT 3000

#define UNICODE_SELECTED_MODES UNICODE_MODE_WINCOMPOSE

#define TAPPING_TERM 150  // Zeit (in ms), um zwischen Tap und Hold zu unterscheiden
// #define PERMISSIVE_HOLD  // Erlaubt das Halten von Mod-Taps auch nach Ablauf des Tapping-Terms
// #define HOLD_ON_OTHER_KEY_PRESS
#define COMBO_TERM 500
#define COMBO_MUST_HOLD_MODS
#define COMBO_HOLD_TERM 150
#define COMBO_MUST_TAP_PER_COMBO

#define DUMMY_MOD_NEUTRALIZER_KEYCODE KC_F18
#define MODS_TO_NEUTRALIZE { \
    MOD_BIT(KC_LALT), \
    MOD_BIT(KC_RALT), \
    MOD_BIT(KC_LALT) | MOD_BIT(KC_LSFT), \
    MOD_BIT(KC_LALT) | MOD_BIT(KC_RSFT), \
    MOD_BIT(KC_RALT) | MOD_BIT(KC_LSFT), \
    MOD_BIT(KC_RALT) | MOD_BIT(KC_RSFT), \
}

// Mouse wheel scrolling
#define MK_KINETIC_SPEED // quadratische Scrollgeschwindigkeit, am Anfang langsamer dann schneller

#define MOUSEKEY_WHEEL_DELAY 10  
#define MOUSEKEY_WHEEL_INTERVAL 20
#define MOUSEKEY_WHEEL_INITIAL_MOVEMENTS 8
#define MOUSEKEY_WHEEL_BASE_MOVEMENTS 120
#define MOUSEKEY_WHEEL_ACCELERATED_MOVEMENTS 100
#define MOUSEKEY_WHEEL_DECELERATED_MOVEMENTS 8


// ohne Kinetic
// #define MOUSEKEY_WHEEL_DELAY 150
// #define MOUSEKEY_WHEEL_INTERVAL 80
// #define MOUSEKEY_WHEEL_DELTA 1 // wie viel pro einem scroll gescrollt wird
// #define MOUSEKEY_WHEEL_TIME_TO_MAX 40
// #define MOUSEKEY_WHEEL_MAX_SPEED 3
