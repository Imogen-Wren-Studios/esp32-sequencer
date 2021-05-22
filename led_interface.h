/*  LED Interface

     This page contains everything for interfacing with the LEDs used for user interaction



*/


#include <ledObject.h>


// Specify the Channel Marker LEDs. These are set HIGH when CH number is active
ledObject leds[8] = {ledObject(13), ledObject(12), ledObject(14), ledObject(27), ledObject(5), ledObject(18), ledObject(19), ledObject(21)};

fadeLED clockLED(23);
ledObject pulseLED(2);

#define INITIAL_STATE 0




void ledStartup() {

  for (int i = 0; i < 8; i++) {
    leds[i].begin(INITIAL_STATE);
  }
  for (int i = 0; i < 8; i++) {
    leds[i].turnOn();                                     //
    delay(110);
  }
  delay(200);

  clockLED.setup(0);
  pulseLED.begin(0);

  while (clockLED.ledBrightness < 255) {
    clockLED.modifyPWM();
    delay(1);
  }

  pulseLED.turnOn();
  
  delay(1000);

  for (int i = 0; i < 8; i++) {
    leds[i].turnOff();                                     //
    delay(40);
  }



  for (int i = 0; i < 3; i ++) {
    Serial.printf("LED State: [%i] \n", clockLED.ledState);
    clockLED.hardToggle();
    delay(100);
  }
  pulseLED.turnOff();
  
}


void sequencerLED(byte beat) {
  if (beat == 0) {
    leds[7].turnOff();
  } else {
    leds[beat - 1].turnOff();
  }
  leds[beat].turnOn();

}
