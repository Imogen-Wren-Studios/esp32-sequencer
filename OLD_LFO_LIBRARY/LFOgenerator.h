/*---------------------------------------------

   These scripts generate accuratly timed LFO outputs


   for use in optical controlled effects


   Input to each routine is (TIMER, MINIMUM, MAXIMUM)  timer is in uS



   Declan Heard
   30/08/2019


  This Page has specific scripts for each waveform. These are written to perform well as high resolution
  LFOs, their running speed may be too low to use at audiable frequencies.

  This library relies on the autoDelay Library for timing calculations

*/

#ifndef LFOgenerator_h
#define LFOgenerator_h



#if (ARDUINO >=100)
#include <Arduino.h>
#else
#include <wProgram.h>
#endif


class LFOgenerator
{
  public:

    // constructor
    LFOgenerator();

    // Methods:

    //---- Functions;------------------------------------------------------------------------------------------------
    void begin(int baudRate = 500000);// begin LFO generator. baudRate defaults to 500000

    // This function calculates uSDelay from a bpm input
    void bpmCalc(int tempo, int bars, float beat ); // int BPM, int Number of bars LFO should repeat over, float beat divisions.
    // example figures (tempo = 120, bars = 4, beat = 1.0)



    // Waveform Generating Scripts;
    void sineWave(int timer, int minimum, int maximum);   // Sine Wave LFO Generator. Timer Counts time for 1 phase

    void triangleWave(int timer, int minimum, int maximum);   // Triangle/ Linear Sawtooth Wave

    void rampupWave(int timer, int minimum, int maximum);   // Ramping Up Sawtooth Wave

    void rampdownWave(int timer, int minimum, int maximum);  // Ramping Down Sawtooth Wave

    void squareWave(int timer, int minimum, int maximum);    // Square wave


    //----- Variables;------------------------------------------------------------------------------------------------------
    // Output Variables;
    //These are Final output values which can be used raw or further maths could be carried out to make more complex waveforms.

    int sineOutput;  // variable to output sine wave as integer values

    int triangleOutput;  // variable to output sawtooth as integer

    int rampupOutput;  // variable to output rampup as integer

    int rampdownOutput;  // variable to output rampup as integer

    int squareOutput; // variable to output Square wave as integer values


    //----global;---------------------------------------------------------------------------------------------------------------
    int timerOne;  //  Timer used after calculating steps required between increments used for every wave

    long msDelay;    // BPM value calculated as a MS delay between beats*bars
    long uSDelay;     // BPM value calculates as a uS delay between beats*bars

    // Variables for timing out functions after set amount of loops - used in BPM calcs for early printout of expected settings
    int disableAcc;  // this disables serial printouts after a set number of loops
    int timeoutLoops = 4;  // number of loops to print serial messages for on start




    ///
    ///-------------------------Autodelay Header Copied-------------

    // Methods:
    // Functions;
    void delayScript(int delayTime);   // delayTime passed in mS
    void uSdelayScript(long uSdelayTime);  // delayTime passed in uS

    void timeTest();  // times gap between events and prints to serial monitor in mS
    void averageTime(long actualDelay); // calculates average time taken between events and prints to the serial monitor

    void uStimeTest();// times gap between events and prints to serial monitor in uS
    void uSaverageTime(long actualDelay); // calculates average time taken between events and prints to the serial monitor



    //--------------------------Autodelay Variables--------------


    boolean delayTimeUp;   // returns true for a single loop if delay time has passed

    long actualDelay;      // value to print out actual delay times to serial monitor

    long timeAverage; // this displays results from averaging actual delay times to print to the serial monitor


    // Autodelay Testing Variables;

    boolean timeTestPrint;  // When set to true, serial printouts for uS delay testing is active

    


    //----------------------------
    //-----------------------------Hidden / Private Variables -----------------------------------------------------------------
    //---------
  private:

    //global;
    int steps;  // Counts the number of descrete steps in a function. Genrally the difference between min and maximum values
    // or could describe one full phase of a time based function 2pi or approx 628 centiRadians (6.28 Radians per 360 deg)



    //--------------------------- sine wave specific Variables:------------------------------
    //const int crFullPhase = 628; // Total number of centiRadian steps that make up one phase, 2 pi or 360 deg
    const int crFullPhase = 314; // Total number of centiRadian steps that make up one phase, 2 pi or 360 deg
    // Standard value = 628
    // this value could be halved with zero drop in resolution when used with an 8 bit output,
    //but increment value must be +2 each time to fix the timing while still running an entire phase.
    //or value fed to sine calculations could be multiplied by two.

    int offsetValue = -1;  // uS timing offset value for sine scripts
    // this could be checked and adjusted automatically if ya think hard enough about it.

    int centiRadian = 0;  // hundredths of a radian used to avoid float maths

    float radian = 0.000;  // radians used in sin(x) calculations // didnt avoid it for long, could this be helped?

    float sineInitial; // initial sine wave generation working variable.

    int range;

    float midPoint = range / 2.0; // range/2 gives midpoint.
    //--------------------------- sine wave specific Variables:------------------------------

    //--------------------------- Other wave specific Variables:------------------------------
    int waveADDR = 1;   // generic adder value to be used to modify wave generators set directions etc.
    int bugFix = 0;    // used in linear sawtooth calculations

    int squareState;  // boolean to store the state of a square wave output


    // -----------------------Autodelay Private Variables:--------------------------------------
    long currentMillis;      // mS timing variables
    long previousMillis;

    long currentMicros;       //uS timing variables
    long previousMicros;


    long prevMillis;          // values for timeTest scripts
    long currMillis;


    long prevMicros;            // uS values for uStimeTest script
    long currMicros;


    unsigned int LFOacc = 0;   // accumilator to count lfo cycles for testing average times
    unsigned long timeAcc = 0;  // timer in mS counts all actual delay times to be used for averaging



};

#endif
