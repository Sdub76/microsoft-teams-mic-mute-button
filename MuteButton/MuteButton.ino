// Teams Physical Mute Button
//
// Double-Tap to send Mute/Unmute command (Ctl+Shift+M)
// Long-press while muted to send hold-to-unmute command (Ctl+Space)
//
 
#define DEBUG 1


// Use ButtonEvents Library (Depends on Bounce2 Library)
// https://github.com/fasteddy516/ButtonEvents
#include <ButtonEvents.h>
#include <HID.h>
#include <Keyboard.h>


const byte buttonPin = 8;  // Button is connected to pin 8
const byte ledPin    = 17; // LED is connected to pin 17 (RXLED)
#define LED_OFF  1
#define LED_ON   0

ButtonEvents myButton; // create an instance of the ButtonEvents class to attach to our button

#define MUTE_OFF      0  // Mute off
#define MUTE_ON       1  // Mute on
#define MUTE_OVERRIDE 2  // Mute off temporarily
int mute_state = MUTE_ON;

void setup() {
  
  // attach our ButtonEvents instance to the button pin
  pinMode(buttonPin, INPUT_PULLUP);  
  myButton.attach(buttonPin);

  // Setup the output LED.
  pinMode(ledPin, OUTPUT);

  Keyboard.begin();

}

void mute_toggle () {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press('m');
}

void mute_override () {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(' ');
}

// this is the main loop, which will repeat forever
void loop() {
  // the update() method is where all the magic happens - it needs to be called regularly in order
  // to function correctly, so it should be part of the main loop.  
  myButton.update();
  
  switch (mute_state)
  {
    case MUTE_OFF:
      digitalWrite(ledPin, LED_OFF);
      TXLED0;
      if (myButton.doubleTapped()) {
        // Double Tapped - Mute On
        mute_state = MUTE_ON;
        mute_toggle();
        delay(100);
        Keyboard.releaseAll();
      }
      break;
    case MUTE_ON:
      digitalWrite(ledPin, LED_ON);
      TXLED1;
      if (myButton.doubleTapped()) {
        // Double Tapped - Mute Off
        mute_state = MUTE_OFF;
        mute_toggle();
        delay(100);
        Keyboard.releaseAll();
      }
      else if (myButton.held()) {
        // Hold - Mute Temporarily Off
        mute_state = MUTE_OVERRIDE;
        mute_override();
      }
      break;
    case MUTE_OVERRIDE:
      digitalWrite(ledPin, LED_OFF);
      TXLED0;
      if (myButton.rose()) {
        // Hold Released - Mute On
        mute_state = MUTE_ON;
        Keyboard.releaseAll();
      }
      break;
  }
  

}
