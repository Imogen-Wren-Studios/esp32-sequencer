/*---------------------------------------------

   This Sketch Controls the DAC Outputs
   of an ESP32 board to Drive LEDs via an LFO

   for use in optical controlled effects



   Declan Heard
   30/08/2019


*/

#include <driver/dac.h>

#include "LFOgenerator.h"




LFOgenerator LFOone;   /// set up instance of LFO generator, called lfoONE to generate the first LFO
// This also contains all the autoDelay Functions

//int baudRate = 115200;
int baudRate = 500000;    // Highest value that worked = 500000

//int timerOne;

//autoDelay barCounter;    // autodelay instance for counting bars


// DAC Channel 1 (GPIO 25)
// DAC Channel 2 (GPIO 26)


int dac1Level = 255; // Starting voltage for DAC 1 0 - 255 (8bit)
//int dac2Level = 5; // Starting voltage for DAC 2 0 - 255

// None of this works atm
int barCount;  // variable to count length of bars and time LFO changes to coencide with beats
int repeatBars = 128;   // switches LFOs after this count of bars
long barLength;  // mS variable for total length of all bars



//-----------------------------------Configuration----------------

//---Testing variables---
//#define SELFCALIBRATION true  // 

//#define TESTOUTPUT true    // When set to true, serial printouts for uS delay testing is active 
#define TESTOUTPUT false

//boolean LFOserialOutput = false; // when set true LFOs are also printed to serial output
boolean LFOserialOutput = true; // when set true LFOs are also printed to serial output




//---------------------
//-------------------------------------------------------------------------------------These Variables Configure Waves------------------
//----------
int AmixB = 50;  // Percentage of waveform A to mix into Output B
int BmixA = 50;  // Percentage of waveform B to mix into Output A


int lfoMin = 150;  // LFO minimum output level
int lfoMax = 230;  // LFO maximum output Level

int BPM = 240; // BPM value
int BARS = 1; // number of bars LFO should repeat over
float BEAT = 0.125  ; //Float for beats within the bar

// Common Beat Values:
//____________________________________________________________________________________________
// |_Note Length:_______|_____Duration compared to One Bar:____|____Number of Beats_________
// |  Whole Note:       | 1.0                                  |  4
// |   Half Note:       | 0.5                                  |  2
// | Quater Note:       | 0.25                                 |  1
// |    8th Note:       | 0.125                                |  0.5
// | 1/16th Note:       | 0.0625                               |  0.25

// | Dotted Notes       |                                      |
// | Dotted Whole Note: | 1.5                                  |  6
// | Dotted Half Note:  | 0.725                                |  3
// | Dotted Quater:     | 0.375                                |  1.5
// | Dotted 8th Note    | 0.1875                               |  0.75
// | Dotted 16th Note:  | 0.09375                              |  0.375
//___________________________________________________________________________________________

int HZ = 1000;  // Value to calculate timings from HZ input


// SET LFO TYPE HERE
int LFOtype = 0;  // sets LFO type:
//  0 = Sine,
//  1 = Triangle Saw,
//  2 = Ramp Up Saw,
//  3 = Ramp Down Saw,
//  4 = Square Wave,
//  5 = RandomSeed
















void setup() {



  dac_output_enable(DAC_CHANNEL_1);
  //dac_output_enable(DAC_CHANNEL_2);

  dac_output_voltage(DAC_CHANNEL_1, 0);  // Write analogue value to the LED

  LFOone.begin(baudRate); // Highest value that worked = 500000

  LFOone.timeTestPrint = TESTOUTPUT;  // turns test output on or off

  Serial.println(LFOone.timeTestPrint);


  // Flashes the LED on startup
  dac_output_voltage(DAC_CHANNEL_1, 255);  // Write analogue value to the LED
  delay(25);
  dac_output_voltage(DAC_CHANNEL_1, 0);  // Write analogue value to the LED
  delay(25);
  dac_output_voltage(DAC_CHANNEL_1, 255);  // Write analogue value to the LED
  delay(35);
  dac_output_voltage(DAC_CHANNEL_1, 0);  // Write analogue value to the LED
  delay(25);


  // then sets to min level before starting main loop
  dac1Level = lfoMin;   // sets this up with the stock LFO min setting. Could later be set via SD card
  dac_output_voltage(DAC_CHANNEL_1, dac1Level);  // Write analogue value to the LED
  delay(25);

}





void loop() {


  LFOone.bpmCalc(BPM, BARS, BEAT);  // int BPM, int number of bars LFO repeats over

  // hzCalc(HZ);  // Input HZ value, outputs  needs work


  /// THIS SCRIPT CYCLES THROUGH DIFFERENT WAVEFORMS. BROKEN
  // BPM/4 = Bars per Min....... /60 = bars per second OR bars per 1000mS
  //
  // 60s / BPM = seconds per beat, x4 = seconds per bar. x1000 is mS per bar
  // times repeatBars = total time in mS to complete all of the bars

  // barLength = (60000 / BPM * 4) * repeatBars;

  //  barCounter.uSdelayScript(barLength);

  // if (barCounter.delayTimeUp == true) {

  //  LFOtype++;

  //  if (LFOtype >= 3) {

  //    LFOtype = 0;

  //  }

  // }
  // END OF BROKEN SCRIPT




  LFOone.uSdelayScript(LFOone.timerOne);   // call delayScript by feeding it an integer


  // Selects which waveform is being generated:
  if (LFOtype == 0) {

    LFOone.sineWave(LFOone.uSDelay, lfoMin, lfoMax); // Sine Wave Output
    // set the Digital to Analogue Converter to the output of LFOone;
    dac1Level = LFOone.sineOutput;

  } else if (LFOtype == 1) {

    LFOone.triangleWave(LFOone.uSDelay, lfoMin, lfoMax);  // Triangle / Linear Sawtooth
    dac1Level = LFOone.triangleOutput;

  } else if (LFOtype == 2) {

    LFOone.rampupWave(LFOone.uSDelay, lfoMin, lfoMax);   // delay time total between phases of LFO, int min, int max
    dac1Level = LFOone.rampupOutput;

  } else if (LFOtype == 3) {

    LFOone.rampdownWave(LFOone.uSDelay, lfoMin, lfoMax);   // delay time total between phases of LFO, int min, int max
    dac1Level = LFOone.rampdownOutput;

  }  else if (LFOtype == 4) {

    LFOone.squareWave(LFOone.uSDelay, lfoMin, lfoMax); // Squarewave
    dac1Level = LFOone.squareOutput;

  }



  if (LFOserialOutput == true) {
    // Print the Waveform out to Serial
    Serial.println(dac1Level);
  }



  // Output the waveform to DAC channel 1
  dac_output_voltage(DAC_CHANNEL_1, dac1Level);  // Write analogue value to the LED


}
