/*---------------------------------------------

   These scripts generate LFO outputs
   of an ESP32 board to Drive LEDs via an LFO

   for use in optical controlled effects


   Input to each routine is (TIMER, MINIMUM, MAXIMUM)  timer is in uS



   Declan Heard
   30/08/2019


  This Page has specific scripts for each waveform. These are written to perform well as high resolution
  LFOs, their running speed may be too low to use at audiable frequencies.


  This library contains functions from the autoDelay Library for timing calculations

*/

//#include "autoDelay.h"  // legacy

#include "LFOgenerator.h"


//---------------------------------------------------------------------------LFOgenerator setup-------------------
LFOgenerator::LFOgenerator() {   // Anything needed to setup instance goes here


}
//---------------------------------------------------------------------------LFOgenerator setup------------------




//--------------------------------------------LFOgenerator begin--------------------
// Function calls  serial communication. Baudrate defaults to 500000
void LFOgenerator::begin(int baudrate) {


  Serial.begin(baudrate);
  Serial.print("LFOgenerator Setup Instance");
  Serial.println(baudrate);

}
//--------------------------------------------LFOgenerator begin--------------------



//--------------------------
//--------------------------------------------------------------------BPM Calculators-----------------
//--------------
// This function calculates uSDelay from a bpm input

void LFOgenerator::bpmCalc(int tempo, int bars, float beat) { // int BPM, int Number of bars LFO should repeat over, float beat divisions.
  // example figures (tempo = 120, bars = 4, beat = 1.0)

  //msDelay = (60000 / tempo) * bars * beat;
  uSDelay = (60000000 / tempo) * bars * beat;   // if function is used in uS 60,000*1000 = 60000000

  //uS delay has max resolution of 4

  // ---------------------------all this code does is print out uSDelay for a few loops then not again to help making testing easier
  if (disableAcc < 4) {
    Serial.print("Expected Delay = ");
    Serial.println(uSDelay);
    disableAcc++;
  }
  // ---------------------------all this code does is print out uSDelay for a few loops then not again to help making testing easier
  // As soon as disableAcc is above 4 this script wont run


  /*  // This isnt needed at all I think
    if (disableAcc > 4000) {
    disableAcc = 10;
    }
    disableAcc++;
  */

  // uSDelay = msDelay * 1000;  // this line not needed as it is working directly in uS already
  //msDelay = msDelay*bars;
  //Serial.print("  BPM:");
  //Serial.print(beat);
  //Serial.print("  Bars:");
  //Serial.print(bars);
  //Serial.print("   delay in ms =");
  //Serial.print(msDelay);
  //Serial.print("   delay in uS =");
  //Serial.println(uSDelay);
}
// ms in a second = 1000

// ms in one minuite = 60000

// (ms per min) / BPM = 60000/BPM

// example 60,000/120 = 500 ms delay between beats

// This is multiplied by the number of bars LFO repeats over.

//--------------------------------------------------------------------BPM Calculators-----------------





//
//------------------------------------------------------------------------------------LFO GENERATING SCRIPTS----------------------------------------------------------------------------
//


//------------0--------LFO---NUMBER----ZERO
//-------------------------------------------------------------------------------------------------LFO Sine Wave---------
//----------
// This function generates a sineWave from a timer input

void LFOgenerator::sineWave(int timer, int minimum, int maximum) {  // timer == time in uS for one phase, min & max value for ramp


  steps = crFullPhase;             // = 628 Total number of centiRadian steps that make up one phase, 2 pi or 360 deg
  // this has been halved to 314 to save processing. As max resolution is 255 there is nothing lost.

  timerOne = (timer / steps) + offsetValue; // Timer one is timer value passed to sub routine divided by the number of steps in one phase of LFO

  // Serial.print("timerOne    ");   // print the delay time speced in uS   needs to be 20 ms actual value is 119 currently
  // Serial.println(timerOne);

  if (delayTimeUp == true) {   // if delay time has passed

    // radian = centiRadian / 100.000;
    radian = centiRadian / 50.000;     // doubled here as resoluton of crFullPhase was halved

    sineInitial = sin(radian); // this generates a sine wave from 1 to -1.

    sineInitial = sineInitial + 1.0; // this moves the sine wave to  from 0 to +2

    // sineInitial = sineInitial * 127.5; // // this line multiplys the sine up to 0 to 255 as a float value
    // This line has been superceided

    // more maths needed here to constrain waveform to max and min values.
    // other maths may be needed to adjust sine output to account of LED brightness at lower drive
    //voltages.

    // alternatives to single line above to take min and max values into account;

    // find the mid point between max and min values:

    range = (maximum - minimum);  // Calculates the range between min and max output values


    midPoint = range / 2.0; // range/2 gives midpoint.

    sineInitial = (sineInitial * midPoint) + minimum; // this line has now set the waveform to the correct range,
    //and minimum value is added to offset it to the middle point between min and max


    sineOutput = round(sineInitial);    // Round the sine value from float to integer

    // Serial.println (sineOutput);

    centiRadian++;  // increment centiRadian Value (do at end of this if statement)
  }


  if (centiRadian >= crFullPhase) { // reset centiRadian value if it reaches 2pi...360 deg

    if (timeTestPrint == true) {        // if timeTestPrint is true, prints out values tothe serial monitor

      // ----------------Timing test scripts--------------------
      // These can be run when the waveform resets to time each phase

      uStimeTest();  // Time test for LFO reset/loop // If TESTOUTPUT is true, also prints
      //actual delay to serial

      uSaverageTime(actualDelay); // tests averagetime to ensure it is stable

      // feeds the difference back into offsetValue to calibrate output timing
      // If TESTOUTPUT is true, also prints to serial

      // ----------------Timing test scripts--------------------
    }

    //--- Self Calibration---
    // NO POINT HAVING SELF CALIBRATION, OFFSET WAS ALWAYS -1

    //  float calibration = timer - timeAverage ;  // subtract the required time from the actual average time



    // calibration = calibration / steps;

    // Serial.println(calibration);

    // offsetValue = round(calibration);

    //  Serial.println(offsetValue);




    centiRadian = 0;  // reset the centiRadian value
  }

}
//-------------------------------------------------------------------------------------------------------------------LFO Sine Wave---------
//-^^^---LFO---NUMBER----0--^^^





///------------1-----LFO---NUMBER----ONE
//---------------------------------------------------------------------------------------------------LFO Triangle Wave---------
//----------
// This function generates a Linear Sawtooth or Triangle Wave
void LFOgenerator::triangleWave(int timer, int minimum, int maximum) {  // timer == to ms between peaks, min & max value for ramp


  steps = ((maximum - minimum) * 2); // This returns the number of steps *2 as it will ramp up & down

  // Serial.println(steps);  // Print the number of steps  for testing

  timerOne = timer / steps;   // Timer one is timer value passed to sub routine divided by the number of steps in one phase of LFO


  //Serial.print("timerOne    ");   // print the delay time speced in ms   needs to be 20 ms actual value is 119 currently
  // Serial.println(timerOne);

  // int timingTest = timerOne * steps;   // just to test values

  // Serial.print("timingTest  ");   // print the delay time speced in ms
  // Serial.println(timingTest);       // timing test for testing obviously. this works correct valyue 2000


  if (delayTimeUp == true) {   // if delay time has passed add the waveADDR to the sawtooth output

    triangleOutput = triangleOutput + waveADDR;


    // timeTest();  // Time test for value incrementing

  }



  //--------------------------------------------------------------------------LFO Reset-----------------------------------
  if (triangleOutput >= maximum) {            // if LFO reaches end, reset

    waveADDR = -1;
    bugFix = 0;

  } else if (triangleOutput <= minimum && bugFix < 1) {

    waveADDR = 1;
    if (timeTestPrint == true) {        // if timeTestPrint is true, prints out values tothe serial monitor
      ///-------------------------------------------------all this for testing timings
        uStimeTest();  // Time test for LFO reset/loop // If TESTOUTPUT is true, also prints
      //actual delay to serial

      uSaverageTime(actualDelay); // tests averagetime to ensure it is stable
    }

    bugFix++;   // this value is required.
  }

  //   Serial.println(bugFix);
  //------------------------------------------------------all this for testing timing
}
//-------------------------------------------------------------------------------------------------------------------LFO Triangle Wave---------
//-^^^---LFO---NUMBER----1--^^^






///------------2-----LFO---NUMBER----TWO
//-------------------------------------------------------------------------------------------------LFO Ramp Up---------
//----------
// This function produces a ramping up sawtooth wave
void LFOgenerator::rampupWave(int timer, int minimum, int maximum) {  // timer == to ms between peaks, min & max value for ramp


  steps = (maximum - minimum);   // This returns the number of steps
  //per phase of the LFO
  // Serial.println(steps);  // Print the number of steps  for testing

  timerOne = (timer / steps) * 2; // Timer one is timer value passed to sub routine divided by the number of steps in one phase of LFO
  // The *2 modifer makes the entire wave ramp up over
  //the same time as taken for the Triangle wave to ramp up & down,
  //Making them seem to work to the same beat.


  // Serial.print("timerOne    ");   // print the delay time speced in ms   needs to be 20 ms actual value is 119 currently
  // Serial.println(timerOne);

  // int timingTest = timerOne * steps;   // just to test values

  // Serial.print("timingTest  ");   // print the delay time speced in ms
  // Serial.println(timingTest);       // timing test for testing obviously. this works correct valyue 2000


  if (delayTimeUp == true) {  // if delay time has passed increase dac level

    rampupOutput++;

    // timeTest();  // Time test for value incrementing

  }



  //--------------------------------------------------------------------------LFO Reset-----------------------------------
  if (rampupOutput >= maximum) {            // if LFO reaches end, reset
    rampupOutput = minimum;


    ///-------------------------------------------------all this shit for testing shit
    if (timeTestPrint == true) {        // if timeTestPrint is true, prints out values tothe serial monitor
       uStimeTest();  // Time test for LFO reset/loop // If TESTOUTPUT is true, also prints
      //actual delay to serial

      uSaverageTime(actualDelay); // tests averagetime to ensure it is stable
    }
    //------------------------------------------------------all this for testing timing

  }

  // Serial.println(rampupOutput);  // print the DAC level
}



//-------------------------------------------------------------------------------------------------------------------LFO Ramp Up---------
//-^^^---LFO---NUMBER----2--^^^





///------------3-----LFO---NUMBER----THREE
//-------------------------------------------------------------------------------------------------LFO Ramp Down---------
//----------
void LFOgenerator::rampdownWave(int timer, int minimum, int maximum) {  // timer == to ms between peaks, min & max value for ramp


  steps = (maximum - minimum);   // This returns the number of steps
  //per phase of the LFO
  // Serial.println(steps);  // Print the number of steps  for testing

  timerOne = (timer / steps) * 2; // Timer one is timer value passed to sub routine divided by the number of steps in one phase of LFO
  // *2 modifyer makes wave happen at same rate as other waves


  // Serial.print("timerOne    ");   // print the delay time speced in ms   needs to be 20 ms actual value is 119 currently
  // Serial.println(timerOne);

  // Serial.print("timingTest  ");   // print the delay time speced in ms
  // Serial.println(timingTest);       // timing test for testing obviously. this works correct valyue 2000


  if (delayTimeUp == true) {  // if delay time has passed increase dac level

    rampdownOutput--;

    // timeTest();  // Time test for value incrementing

  }



  //--------------------------------------------------------------------------LFO Reset-----------------------------------
  if (rampdownOutput <= minimum) {            // if LFO reaches end, reset
    rampdownOutput = maximum;

    ///-------------------------------------------------all this for testing timings
    if (timeTestPrint == true) {        // if timeTestPrint is true, prints out values tothe serial monitor
       uStimeTest();  // Time test for LFO reset/loop // If TESTOUTPUT is true, also prints
      //actual delay to serial

      uSaverageTime(actualDelay); // tests averagetime to ensure it is stable
    }
    //------------------------------------------------------all this for testing timing

  }
}
//-------------------------------------------------------------------------------------------------------------------LFO Ramp Down---------
//-^^^---LFO---NUMBER----3--^^^


///------------4-----LFO---NUMBER----FOUR
//-------------------------------------------------------------------------------------------------LFO Square Wave---------
//----------
void LFOgenerator::squareWave(int timer, int minimum, int maximum) {  // timer == to ms between peaks, min & max value for ramp

  // timerOne = timer / 2; // Timer is halved as this function only contains 2 values. "HIGH" and "LOW"  // Timer one is timer value passed to sub routine divided by the number of steps in one phase of LFO
  timerOne = timer;  //  got rid of /2 to make rhythems match other funtions. Could change back.

  // Serial.print("timerOne    ");   // print the delay time speced in uS   needs to be 20 ms actual value is 119 currently
  // Serial.println(timerOne);


  if (delayTimeUp == true) {   // if delay time has passed

    if (squareState == HIGH) {

      squareState = LOW;

    } else if (squareState == LOW) {

      squareState = HIGH;
      ///-------------------------------------------------all this for testing timings
      if (timeTestPrint == true) {     // if timeTestPrint is true, prints out values tothe serial monitor
  uStimeTest();  // Time test for LFO reset/loop // If TESTOUTPUT is true, also prints
      //actual delay to serial

      uSaverageTime(actualDelay); // tests averagetime to ensure it is stable

      }
      ///-------------------------------------------------all this for testing timings
    }
  }

  
  if (squareState == HIGH) {

    squareOutput = maximum;     // SquareOutput value used so maths can be carried out later

  } else if (squareState == LOW) {

    squareOutput = minimum;

  }

  // dac1Level = squareOutput;  // dont need this line as output is fixed elsewhere

}
//-------------------------------------------------------------------------------------------------------------------LFO Square Wave---------
//-^^^---LFO---NUMBER----4--^^^














/*-------------------------------------------------------Auto Delay Library----------------------------------------
   autoDelay.h - A library for creating delays in software without using the delay function,
   leaving the processer avaliable to do other things, while ensuring critical timing
   functions happen as required.



   Created by Declan Heard. 03/08/2019
   Pan Galactic FX
   Released into Public Domain.

*/




//-----------------------------------------------------------------------------------autoDelay delayScript------------------
void LFOgenerator::delayScript(int delayTime) {


  currentMillis = millis();                  // sets a variable for the current milliseconds passed


  if (currentMillis - previousMillis >= delayTime) {  // if the delay time has been passed

    delayTimeUp = true;

    previousMillis = currentMillis;

  } else {

    delayTimeUp = false;

  }


}  // end of delayScript routine
//-----------------------------------------------------------------------------------autoDelay delayScript------------------






//-----------------------------------------------------------------------------------autoDelay uSdelayScript------------------
// Passed DelayTime value in uS
// returns delayTimeUP True if this time has passed since the last time it ran
// remains true for a single loop

void LFOgenerator::uSdelayScript(long uSdelayTime) {




  currentMicros = micros();                  // sets a variable for the current milliseconds passed





  if (currentMicros - previousMicros >= uSdelayTime) {  // if the delay time has been passed

    delayTimeUp = true;

    previousMicros = currentMicros;

  } else {

    delayTimeUp = false;

  }


}  // end of uSdelayScript routine
//-----------------------------------------------------------------------------------autoDelay uSdelayScript------------------




//-----------------------------------------------------------------------------------autoDelay timeTest------------------
// This function Saves when an event happens,
// then when the event happens again records the time taken between events and prints this to serial monitor

void LFOgenerator::timeTest() {

  // these functions check and print timing to serial
  prevMillis = currMillis;
  currMillis = millis();

  actualDelay = currMillis - prevMillis;

  if (timeTestPrint == true) {        // if timeTestPrint is true, prints out values tothe serial monitor
    Serial.print("                       Actual Delay Time mS = ");
    Serial.println(actualDelay);
  }

}
//-----------------------------------------------------------------------------------autoDelay timeTest------------------



//-----------------------------------------------------------------------------------autoDelay uStimeTest------------------
// This function Saves when an event happens,
// then when the event happens again records the time taken between events and prints this to serial monitor
void LFOgenerator::uStimeTest() {

  // these functions check and print timing to serial

  prevMicros = currMicros;
  currMicros = micros();

  actualDelay = currMicros - prevMicros;

  if (timeTestPrint == true) {        // if timeTestPrint is true, prints out values tothe serial monitor
    Serial.print("                       Actual Delay Time uS = ");
    Serial.println(actualDelay);
  }

}




//-----------------------------------------------------------------------------------autoDelay averageTest------------------
// this function uses measured actual delay times and calculates an average to ensure the delay time is stable. Useful for when
// multiple shorter times need to be added and checked against some much longer interval.

//Uses accumilator to ignore first few loops as the system settles and ensure true average is taken

void LFOgenerator::averageTime(long actualDelay) {

  if (LFOacc > 0) {                     // This if statement insures the very first reading is not counted into the average.
    timeAcc = timeAcc + actualDelay;  // timeAcc adds up all the measured delay values   // so the system has time to settle
    timeAverage = timeAcc / LFOacc;

    if (timeTestPrint == true) {   // if timeTestPrint is true, prints out values tothe serial monitor
      Serial.print("Average Time = ");
      Serial.println(timeAverage);
    }

  }

  LFOacc++;

  if (timeAcc >= 60000) {   // Fairly arbitrary number, just ensures rollover happens at an expected time
    timeAcc = 0;            // could make this lower if we want to reset averages at other times.
    LFOacc = 0;
    timeAverage = 0;

  }
}
//-----------------------------------------------------------------------------------autoDelay averageTest------------------







//-----------------------------------------------------------------------------------autoDelay uSaverageTest------------------
// this function uses measured actual delay times and calculates an average to ensure the delay time is stable. Useful for when
// multiple shorter times need to be added and checked against some much longer interval.

//Uses accumilator to ignore first few loops as the system settles and ensure true average is taken

// Does this actually need to be any different from the mS average test?? -- Delete this whole routine if unneeded to save space.

void LFOgenerator::uSaverageTime(long actualDelay) {

  if (LFOacc > 0) {                     // This if statement insures the very first reading is not counted into the average.
    timeAcc = timeAcc + actualDelay;  // timeAcc adds up all the measured delay values   // so the system has time to settle


    // timeAverage = timeAcc / LFOacc;
    float betterAverage = timeAcc / LFOacc;

    timeAverage = round(betterAverage);



    Serial.print("Average Time = ");
    Serial.println(timeAverage);


  }

  LFOacc++;

  if (timeAcc >= 4000000000) {   // Fairly arbitrary number 4,294,967,295 is max for an unsigned long, just ensures rollover happens at an expected time
    timeAcc = timeAverage;            //6000000 could make this lower if we want to reset averages at other times.
    LFOacc = 1;
    // timeAverage = 0;

  }
}
//-----------------------------------------------------------------------------------autoDelay uSaverageTest------------------
