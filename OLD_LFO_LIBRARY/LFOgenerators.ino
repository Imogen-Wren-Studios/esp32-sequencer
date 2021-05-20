/*---------------------------------------------

   These scripts generate LFO outputs
   of an ESP32 board to Drive LEDs via an LFO

   for use in optical controlled effects


   Input to each routine is (TIMER, MINIMUM, MAXIMUM)  timer is in uS



   Declan Heard
   30/08/2019


 This Page has specific scripts for each waveform. These are written to perform well as high resolution
 LFOs, their running speed may be too low to use at audiable frequencies. 

*/


//--------------------------------------------------------------------- LFO Variables-------
//
//______________________________________LFO Variables___________________________________
//
//
//-------------- LFO Variables---------------------------------------------------------------
//



//--------------------------- Variables used within LFO calculations-------------------

//int squareState;  // boolean to store the state of a square wave output


//--------------------------- Variables used within LFO calculations-------------------






//--------------------------------------------------------------------- LFO FUNCTIONS-------
//
//______________________________________LFO FUNCTIONS___________________________________
//
//
//-------------- LFO FUNCTIONS---------------------------------------------------------------
//


/*

//-------------------------------------------------------------------------------------------------------------------LFO RAMP UP---------
void lfoRampUp(int timer, int minimum, int maximum) {  // timer == to ms between peaks, min & max value for ramp


  int steps = (maximum - minimum);   // This returns the number of steps
  //per phase of the LFO
  // Serial.println(steps);  // Print the number of steps  for testing

  timerOne = timer / steps;   // Timer one is timer value passed to sub routine divided by the number of steps in one phase of LFO



  // Serial.print("timerOne    ");   // print the delay time speced in ms   needs to be 20 ms actual value is 119 currently
  // Serial.println(timerOne);

  // int timingTest = timerOne * steps;   // just to test values

  // Serial.print("timingTest  ");   // print the delay time speced in ms
  // Serial.println(timingTest);       // timing test for testing obviously. this works correct valyue 2000


  if (LFOoneDelay.delayTimeUp == true) {  // if delay time has passed increase dac level

    rampupOutput++;

    // Serial.println(dac1Level);  // print the DAC level

    // timeTest();  // Time test for value incrementing

  }



  //--------------------------------------------------------------------------LFO Reset-----------------------------------
  if (rampupOutput >= maximum) {            // if LFO reaches end, reset
    rampupOutput = minimum;

    ///-------------------------------------------------all this shit for testing shit
    // autodelay.timeTest();  // Time test for LFO reset/loop

    // autodelay.averageTime(autodelay.actualDelay); // Prints out averagetime to ensure it is stable

    //------------------------------------------------------all this for testing timing

  }

  // Serial.println(rampupOutput);  // print the DAC level
}
//-------------------------------------------------------------------------------------------------------------------LFO RAMP UP---------




//-------------------------------------------------------------------------------------------------------------------LFO RAMP UP---------
void lfoRampDown(int timer, int minimum, int maximum) {  // timer == to ms between peaks, min & max value for ramp


  int steps = (maximum - minimum);   // This returns the number of steps
  //per phase of the LFO
  // Serial.println(steps);  // Print the number of steps  for testing

  timerOne = timer / steps;   // Timer one is timer value passed to sub routine divided by the number of steps in one phase of LFO



  // Serial.print("timerOne    ");   // print the delay time speced in ms   needs to be 20 ms actual value is 119 currently
  // Serial.println(timerOne);

  // int timingTest = timerOne * steps;   // just to test values

  // Serial.print("timingTest  ");   // print the delay time speced in ms
  // Serial.println(timingTest);       // timing test for testing obviously. this works correct valyue 2000


  if (LFOoneDelay.delayTimeUp == true) {  // if delay time has passed increase dac level

    rampdownOutput--;

    // Serial.println(dac1Level);  // print the DAC level

    // timeTest();  // Time test for value incrementing

  }



  //--------------------------------------------------------------------------LFO Reset-----------------------------------
  if (rampdownOutput <= minimum) {            // if LFO reaches end, reset
    rampdownOutput = maximum;

    ///-------------------------------------------------all this shit for testing shit
    // autodelay.timeTest();  // Time test for LFO reset/loop

    // autodelay.averageTime(autodelay.actualDelay); // Prints out averagetime to ensure it is stable

    //------------------------------------------------------all this for testing timing

  }

  // Serial.println(rampupOutput);  // print the DAC level
}
//-------------------------------------------------------------------------------------------------------------------LFO RAMP UP---------





//-------------------------------------------------------------------------------------------------------------------LFO Linear Sawtooth---------
void lfoLinearSawtooth(int timer, int minimum, int maximum) {  // timer == to ms between peaks, min & max value for ramp


  int steps = ((maximum - minimum) * 2); // This returns the number of steps *2 as it will ramp up & down

  // Serial.println(steps);  // Print the number of steps  for testing

  timerOne = timer / steps;   // Timer one is timer value passed to sub routine divided by the number of steps in one phase of LFO


  //Serial.print("timerOne    ");   // print the delay time speced in ms   needs to be 20 ms actual value is 119 currently
  // Serial.println(timerOne);

  // int timingTest = timerOne * steps;   // just to test values

  // Serial.print("timingTest  ");   // print the delay time speced in ms
  // Serial.println(timingTest);       // timing test for testing obviously. this works correct valyue 2000


  if (LFOoneDelay.delayTimeUp == true) {   // if delay time has passed increase dac level

    sawtoothOutput = sawtoothOutput + waveADDR;

    // Serial.println(dac1Level);  // print the DAC level

    // timeTest();  // Time test for value incrementing

  }



  //--------------------------------------------------------------------------LFO Reset-----------------------------------
  if (sawtoothOutput >= maximum) {            // if LFO reaches end, reset

    waveADDR = -1;
    bugFix = 0;

  } else if (sawtoothOutput <= minimum && bugFix < 1) {

    waveADDR = 1;
    //dac1Level++; // bug fix
    // dac1Level = minimum;


    ///-------------------------------------------------all this shit for testing shit
    // autodelay.timeTest();  // Time test for LFO reset/loop

    //  autodelay.averageTime(autodelay.actualDelay); // Prints out averagetime to ensure it is stable
    bugFix++;
  }

  //   Serial.println(bugFix);
  //------------------------------------------------------all this for testing timing



  // Serial.println(dac1Level);  // print the DAC level
}
//-------------------------------------------------------------------------------------------------------------------LFO Linear Sawtooth---------









//-------------------------------------------------------------------------------------------------------------------LFO Square Wave---------


void lfoSquareWave(int timer, int minimum, int maximum) {  // timer == to ms between peaks, min & max value for ramp

  timerOne = timer / 2; // Timer is halved as this function only contains 2 values. "on" and "off"  // Timer one is timer value passed to sub routine divided by the number of steps in one phase of LFO

  // Serial.print("timerOne    ");   // print the delay time speced in uS   needs to be 20 ms actual value is 119 currently
  // Serial.println(timerOne);


  if (autodelay.delayTimeUp == true) {   // if delay time has passed

    if (squareState == HIGH) {

      squareState = LOW;

    } else if (squareState == LOW) {

      squareState = HIGH;

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
















// Better way of doing things? Probably if it worked?!/
// yeet this section?
//-------------------------------------------------------------------------------------------------------------------------Doing a Maths Function way of like doing stuff?-----------------------------------
void doingMathsFunction(int timer, int minimum, int maximum) {


  int steps = maximum - minimum;// some amount of steps maximum - minimum; is probably not a bad place to start
  // may need modifications to deal with higher frequencies.


  //-- Algorithm needed here that expresses y as a function of x.
  //
  //


  unsigned int x;
  unsigned int y;


  // y in this case is the step on which we are on



  y = sin(x);


  if (x >= steps) {


    x = 0;
  }



}

*/
//
