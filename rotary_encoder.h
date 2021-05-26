
/*   rotary_encoder.h

      - Functions for the Rotary Encoder as a user input device



*/



#include <rotaryEncoder.h>


//----------------------------------------------
// ~~ ENCODER INPUT ~~


#define ENCODER_PIN_A 35  // 2 & 4 worked
#define ENCODER_PIN_B 34

// Hardware Interrupts
// on Arduino UNO & NANo:
// = Pins 2 & 3
//
// Only 1 interupt required for each encoder
// So max TWO Encoder inputs.

#define ENCODER_BUTTON_PIN 34

// Set up instance of rotaryEncoder. Requires button pin, button active state, and 2 encoder pins.

rotaryEncoder encoderOne(ENCODER_BUTTON_PIN, BUTTON_PULL_HIGH, ENCODER_PIN_A, ENCODER_PIN_B);


//-- This is a fudge, as isr methods cannot be inside our rotaryEncoder library
// This is a workaround - for some reason isr must not be in capitals.


void isr () {
  encoderOne.isr();
}



void rotary_encoder_start() {

  encoderOne.begin();    // encoder Begin function also calls the buttonObject begin function


  //-- This is a fudge, as isr methods cannot be inside our rotaryEncoder library
  // This is a workaround - Each instance of rotaryEncoder will require this line attaching
  // hardware interrupt to one of the encoders pins.
  // & The ISR function below the main loop, referencing the isr function contained in the library

  attachInterrupt (digitalPinToInterrupt (ENCODER_PIN_A), isr , CHANGE);   // interrupt 0 is pin 2

}


int counter;
int previousCount;

uint16_t adjust_tempo(uint16_t initial) {

//  Serial.print("rotaryCount: ");
//  Serial.print(encoderOne.rotaryCount);
 // Serial.println("");

  if (encoderOne.rotaryUp) {
    encoderOne.resetEncoder();
    counter++;
    if (counter > 2) {
      bpm++;
      update_clock_tempo(bpm) ;
      counter = 0;
    }
  } else if (encoderOne.rotaryDown) {
    encoderOne.resetEncoder();
    counter--;
    if (counter < -2) {
      bpm--;
      update_clock_tempo(bpm);
      counter = 0;
    }
  }
  return initial;
}
