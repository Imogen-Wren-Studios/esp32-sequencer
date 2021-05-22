/*  ESP32 |1| |2| |3| |4| |5| |6| |7| |8| Step Sequencer

    2021 Pan Galactic Tech

    Written by Imogen Wren

        ESP32 based Step sequencer with 8 beats.

        Main function: - Output a step varying analog voltage level, over 8 beat bars.
                       - Output an LFO as an analog voltage level, time synchonised with the step sequencer

        Secondary functions: - Act as a master clock, able to perform as both Master and Slave device.
                             - Manage delay timed events based on a tap tempo & rotary encoder input
                             - Provide visual output for state and timing of step sequencer.


        Analog Outputs can be set to eather step sequencer, or LFO output.

        Available LFOs:
            - Sine Wave
            - Sawtooth
            - Ramp Up Sawtooth
            - Ramp Down Sawtooth
            - Square Wave
            - Random Wave

            ALL LFOs will have 256 steps, as this keeps LFO timings consistant using 8bit values. LFOs that cannot be generated mathmatically will have 256 "Samples"
            that can be played back at any speed

       Step sequencer should be able to trigger LFO output per beat, enabling shaping of the stepped envelope



    Inputs:
      - Tap Tempo
      - Rotary Set Tempo
      - CH1 variable Per Step
      - CH2 Variable Per Step
      - LFO/Sequencer selection
      - Step Trigger
      - Midi Clock
      - Groove



    Outputs:
      - CH1 Analog Voltage Per Step
      - CH2 Analog Voltage Per Step
      - LED Per Step
      - Step Trigger Output
      - MDI Clock
      - LED Tempo Heartbeat
      - Clock/Tempo Value



*/


#include <driver/dac.h>
#include <ledObject.h>

// Specify the Channel Marker LEDs. These are set HIGH when CH number is active
ledObject leds[8] = {ledObject(13), ledObject(12), ledObject(14), ledObject(27), ledObject(5), ledObject(18), ledObject(19), ledObject(21)};

#define INITIAL_STATE 0



// DAC Channel 1 (GPIO 25)
// DAC Channel 2 (GPIO 26)


// Using Interrupts & ISR for timing purposes.

#include "interrupt_timing.h"

 //   - Advantages, faster more accurate timings than autoDelay()

 // Method Found @ https://techtutorialsx.com/2017/10/07/esp32-arduino-timer-interrupts/

volatile int16_t interruptCounter;    // Used by ISR & Main loop to track number of ISRs triggered (more accurate than using bool, as multiple triggers can be accounted for)

int16_t totalInterrptCounter;        // Total number of interrupts triggered in run time

hw_timer_t * timer = NULL;     // variable of type hw_timer_t. Used to configure timer

portMUX_TYPE timerMux = partMUX_INITIALIZER_UNLOCKED;   // Used for syncronisation of shared variable between main loop & ISR




void setup() {

  Serial.begin(115200);

  for (int i = 0; i < 8; i++) {
    leds[i].begin(INITIAL_STATE);
  }
  for (int i = 0; i < 8; i++) {
    leds[i].turnOn();                                     //
    delay(200);
  }
  delay(1000);

  for (int i = 0; i < 8; i++) {
    leds[i].turnOff();                                     //
    delay(40);
  }


  dac_output_enable(DAC_CHANNEL_1);
  //dac_output_enable(DAC_CHANNEL_2);

  dac_output_voltage(DAC_CHANNEL_1, 0);  // Write analogue value to the LED

}


byte current_step = 0;

uint16_t bpm = 120;



void loop() {



}


void sawtooth(long stepDelay, byte lfoMin, byte lfoMax) {



}
