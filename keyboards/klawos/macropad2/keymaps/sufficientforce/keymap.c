// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "quantum.h"


#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┐
     * │ A │ B │ C │ D │
     * ├───┼───┼───┼───┤
     * │ E │ F │ G │ H │
     * ├───┼───┼───┼───┤
     * │ I │ J │ K │ L │
     * ├───┼───┼───┼───┤
     * │ M │ N │ O │ P │
     * └───┴───┴───┴───┘
     */
    [0] = LAYOUT_ortho_4x4(
        KC_A,   KC_B,   KC_C,   KC_D,
        KC_E,   KC_F,   KC_G,   KC_H,
        KC_I,   KC_J,   KC_K,   KC_L,
        KC_M,   KC_N,   KC_O,   KC_P
    )
};


// Brightness ranges from 0-255
uint8_t BRIGHTNESS_DEFAULT = 128;
uint8_t BRIGHTNESS_MAX = 255;
uint8_t BRIGHTNESS_MIN = 0;
uint8_t BRIGHTNESS_STEP = 16;


static deferred_token delayed_write_token = INVALID_DEFERRED_TOKEN;
static const uint16_t WRITE_DELAY = 2000;


typedef enum {
    COLOR_ME = 0,
    COLOR_LOVE = 1,
    COLOR_HER = 2,
    COLOR_DEFAULT = 3
} ColorSetting;


typedef struct {
    uint8_t h;
    uint8_t s;
    uint8_t v;
} CustomHSV;

const CustomHSV hsv_red = {0, 255, 255};
const CustomHSV hsv_blue = {170, 255, 255};
const CustomHSV hsv_green = {85, 255, 255};
const CustomHSV hsv_default = {19, 255, 255};

typedef union {
    uint32_t raw;
    struct {
        bool rgb_0 :1;
        bool rgb_1 :1;
        bool rgb_2 :1;
        bool rgb_3 :1;
        bool rgb_4 :1;
        bool rgb_5 :1;
        bool rgb_6 :1;
        bool rgb_7 :1;
        bool rgb_8 :1;
        bool rgb_9 :1;
        bool rgb_10 :1;
        bool rgb_11 :1;
        bool rgb_12 :1;
        bool rgb_13 :1;
        bool rgb_14 :1;
        unsigned int color_setting :2;
        uint8_t brightness :8;
    };
} rgb_state_t;

rgb_state_t rgb_state;


void eeconfig_init_user(void) {
    eeconfig_init();
    rgb_state.raw = 0;
    rgb_state.color_setting = COLOR_DEFAULT;
    rgb_state.brightness = BRIGHTNESS_DEFAULT;
}


void illuminate_led_by_state(uint8_t led_index, CustomHSV hsv) {
    if (led_index < 0) {
        return;
    }

    if (led_index >= RGBLED_NUM) {
        return;
    }

    bool state = (
                    (led_index == 0) ? false : (
                    (led_index == 1) ? false : (
                    (led_index == 2) ? false : (
                    (led_index == 3) ? rgb_state.rgb_3 : (
                    (led_index == 4) ? rgb_state.rgb_4 : (
                    (led_index == 5) ? rgb_state.rgb_5 : (
                    (led_index == 6) ? rgb_state.rgb_6 : (
                    (led_index == 7) ? rgb_state.rgb_7 : (
                    (led_index == 8) ? rgb_state.rgb_8 : (
                    (led_index == 9) ? rgb_state.rgb_9 : (
                    (led_index == 10) ? rgb_state.rgb_10 : (
                    (led_index == 11) ? rgb_state.rgb_11 : (
                    (led_index == 12) ? rgb_state.rgb_12 : (
                    (led_index == 13) ? rgb_state.rgb_13 : (
                    (led_index == 14) ? rgb_state.rgb_14 : false)))))))))))))));

    if (state) {
        rgblight_sethsv_at(hsv.h, hsv.s, rgb_state.brightness, led_index);
    } else {
        rgblight_setrgb_at(0, 0, 0, led_index);
    }
}


void illuminate_kb_by_state(void) {
    CustomHSV hsv;

    if (rgb_state.color_setting == COLOR_ME) {
        hsv = hsv_blue;
    } else if (rgb_state.color_setting == COLOR_LOVE) {
        hsv = hsv_red;
    } else if (rgb_state.color_setting == COLOR_HER) {
        hsv = hsv_green;
    } else {
        hsv = hsv_default;
    }

    for (uint8_t i = 0; i < RGBLED_NUM; i++) {
        illuminate_led_by_state(i, hsv);
    }
}


void keyboard_post_init_user(void) {
    rgb_state.raw = eeconfig_read_user();
    illuminate_kb_by_state();
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return false;
    }

    switch (keycode) {
        case KC_A:
            rgb_state.color_setting = COLOR_HER;
            break;
        case KC_B:
            rgb_state.color_setting = COLOR_LOVE;
            break;
        case KC_C:
            rgb_state.color_setting = COLOR_ME;
            break;
        case KC_D:
            rgb_state.color_setting = COLOR_DEFAULT;
            break;
        case KC_E:
            rgb_state.rgb_3 = !rgb_state.rgb_3;
            break;
        case KC_F:
            rgb_state.rgb_4 = !rgb_state.rgb_4;
            break;
        case KC_G:
            rgb_state.rgb_5 = !rgb_state.rgb_5;
            break;
        case KC_H:
            rgb_state.rgb_6 = !rgb_state.rgb_6;
            break;
        case KC_I:
            rgb_state.rgb_7 = !rgb_state.rgb_7;
            break;
        case KC_J:
            rgb_state.rgb_8 = !rgb_state.rgb_8;
            break;
        case KC_K:
            rgb_state.rgb_9 = !rgb_state.rgb_9;
            break;
        case KC_L:
            rgb_state.rgb_10 = !rgb_state.rgb_10;
            break;
        case KC_M:
            rgb_state.rgb_11 = !rgb_state.rgb_11;
            break;
        case KC_N:
            rgb_state.rgb_12 = !rgb_state.rgb_12;
            break;
        case KC_O:
            rgb_state.rgb_13 = !rgb_state.rgb_13;
            break;
        case KC_P:
            rgb_state.rgb_14 = !rgb_state.rgb_14;
            break;
    }

    illuminate_kb_by_state();
    eeconfig_update_user(rgb_state.raw);

    return false;
}


uint32_t delayed_write_callback(uint32_t trigger_time, void *cb_arg) {
    if (delayed_write_token == INVALID_DEFERRED_TOKEN) {
        return 0;
    }

    delayed_write_token = INVALID_DEFERRED_TOKEN;
    eeconfig_update_user(rgb_state.raw);

    return 0;
}


bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            rgb_state.brightness = min(rgb_state.brightness + BRIGHTNESS_STEP, BRIGHTNESS_MAX);
        } else {
            rgb_state.brightness = max(rgb_state.brightness - BRIGHTNESS_STEP, BRIGHTNESS_MIN);
        }

        illuminate_kb_by_state();

        if (delayed_write_token == INVALID_DEFERRED_TOKEN) {
            delayed_write_token = defer_exec(WRITE_DELAY, delayed_write_callback, NULL);
        } else {
            extend_deferred_exec(delayed_write_token, WRITE_DELAY);
        }
    }

    // Do not do any other handling (like volume control, etc)
    return false;
};
