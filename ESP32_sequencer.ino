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
      - Step Trigger (for external trigger)
      - Midi Clock
      - Groove



    Outputs:
      - CH1 Analog Voltage Per Step
      - CH2 Analog Voltage Per Step
      - LED Per Step
      - Step Trigger Output
      - Square Wave Clock with 50% Duty Cycle
      - Pulse Clock Output with adjustable Duty Cycle
      - MDI Clock
      - LED Tempo Heartbeat
      - Clock/Tempo Value



*/


#include <driver/dac.h>

// DAC Channel 1 (GPIO 25)
// DAC Channel 2 (GPIO 26)

// LED Interface (For user interaction & UI)

#include "led_interface.h"



// Using Interrupts & ISR for timing purposes.

#include "interrupt_timing.h"

// Timing Functions & Methods

#include "timing_maths.h"


// Step Sequencer Methods

#include "step_sequencer.h"


uint16_t bpm = 140;

uint32_t duty_high;   // variable to hold the HIGH time for the variable duty cycle output
bool pulse_triggered = false;
uint32_t pulse_start_t;           // time pulse was triggered in micros

float DUTY_CYCLE = 0.1;

bool PEDAL_TONE = true;     // Pedal tone effectivily doubles the number of steps in the sequence, by treating the offbeat as a pedal tone, 
byte PEDAL_VALUE = 0;       // pedal tone value


void setup() {

  Serial.begin(115200);

  randomSeed(34);

  ledStartup();




  dac_output_enable(DAC_CHANNEL_1);
  //dac_output_enable(DAC_CHANNEL_2);
  dac_output_voltage(DAC_CHANNEL_1, 0);  // Write analogue value to the LED

  uint32_t master_clock_delay = bpm_to_delay(bpm);

  timerSetup(master_clock_delay);
  duty_high = duty_cycle_high(master_clock_delay, DUTY_CYCLE);
}


byte current_step = 0;

byte tick = 0;            // Counts each half beat (each interrupt tick)
byte beat = 0;              // Counts each beat (1 to 8)





void loop() {



  if (interruptCheck()) {
   
    tick++;

    if (tick == 1) {
      clockLED.turnOff();      // LED will flash on every half beat, making a 50% clock pulse on every beat
      if (PEDAL_TONE){
        pedalTone(PEDAL_VALUE);
      }
      Serial.printf(" & ");
    }
    if (tick >= 2) {
      step_sequencer(beat);
      sequencerLED(beat);
      pulse_start_t = pulse_trigger();
      clockLED.turnOn(); 
      beat++;
      tick = 0;
      Serial.printf("\nBeat: [%i]\n", beat);
    }

    if (beat >= 8) {
      beat = 0;
      // randomise_sequence(0, 40);
    }

  }

  pulse_end(duty_high, pulse_start_t);
}


// First funtion is triggered by the interrupt to start the HIGH pulse output
uint32_t pulse_trigger() {
  pulse_triggered = true;
  pulseLED.turnOn();
  uint32_t  pulse_start = micros();
  return pulse_start;
}

//2nd function always runs in loop and makes pulse LOW at the time after defined by the duty cycle.
void pulse_end(uint32_t pulse_high_time, uint32_t pulse_start_time) {
  if (pulse_triggered) {
    if (micros() - pulse_start_time >= pulse_high_time) {
      pulseLED.turnOff();
      pulse_triggered = false;
    }
  }
}

void sawtooth(long stepDelay, byte lfoMin, byte lfoMax) {



}
