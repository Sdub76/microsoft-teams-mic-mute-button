// MS Teams Physical Mute Button
//
// Double-Tap to send Mute/Unmute command (Ctl+Shift+M)
// Long-press while muted to send hold-to-unmute command (Ctl+Space)
//
// Print parts in translucent PLA so you can see LEDs
 

// Use ButtonEvents Library (Depends on Bounce2 Library)
// https://github.com/fasteddy516/ButtonEvents
#include <ButtonEvents.h>
#include <HID.h>
#include <Keyboard.h>


const byte buttonPin = 8;  // Button is connected to pin 8
const byte ledPin    = 17; // LED is connected to pin 17 (RXLED)

ButtonEvents myButton; // create an instance of the ButtonEvents class to attach to our button

#define MUTE_OFF      0  // Mute off
#define MUTE_ON       1  // Mute on
#define MUTE_OVERRIDE 2  // Mute off temporarily

int mute_state = MUTE_OFF; // Default to "mute" off state.  Manually sync Teams with button state after starting meeting

void setup() {
  
  pinMode(buttonPin, INPUT_PULLUP);  
  myButton.attach(buttonPin);

  pinMode(ledPin, OUTPUT);

  Keyboard.begin();

}

void keyboard_mute_toggle () {
  //Keyboard.press(KEY_LEFT_GUI); // Mac
  Keyboard.press(KEY_LEFT_CTRL); // Windows
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('m');
}

void keyboard_mute_override () {
  //Keyboard.press(KEY_LEFT_GUI); // Mac
  Keyboard.press(KEY_LEFT_CTRL); // Windows
  Keyboard.press(' ');
}

void leds_off () {
  digitalWrite(ledPin, true);
  TXLED0; // Can't write to pin... need to use built-in macro
}

void leds_on () {
  digitalWrite(ledPin, false);
  TXLED1; // Can't write to pin... need to use built-in macro
}

void loop() {

  myButton.update();

  // Three states... off, on, and temp override
  switch (mute_state)
  {
    case MUTE_OFF:
      leds_off();
      if (myButton.doubleTapped()) {
        // Double Tapped - Mute On
        mute_state = MUTE_ON;
        keyboard_mute_toggle();
        delay(100);
        Keyboard.releaseAll();
      }
      break;
      
    case MUTE_ON:
      leds_on();
      if (myButton.doubleTapped()) {
        // Double Tapped - Mute Off
        mute_state = MUTE_OFF;
        keyboard_mute_toggle();
        delay(100);
        Keyboard.releaseAll();
      }
      else if (myButton.held()) {
        // Hold - Mute Temporarily Off
        mute_state = MUTE_OVERRIDE;
        keyboard_mute_override();
      }
      break;
      
    case MUTE_OVERRIDE:
      leds_off();
      if (myButton.rose()) {
        // Hold Released - Mute On
        mute_state = MUTE_ON;
        Keyboard.releaseAll();
      }
      break;
  }
  

}
