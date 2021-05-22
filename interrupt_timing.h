/*  Timing with Interrupts

      - Advantages, faster more accurate timings than autoDelay()


    Method Found @ https://techtutorialsx.com/2017/10/07/esp32-arduino-timer-interrupts/

*/



volatile int16_t interruptCounter;    // Used by ISR & Main loop to track number of ISRs triggered (more accurate than using bool, as multiple triggers can be accounted for)

int16_t totalInterruptCounter;        // Total number of interrupts triggered in run time

hw_timer_t * timer = NULL;     // variable of type hw_timer_t. Used to configure timer

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;   // Used for syncronisation of shared variable between main loop & ISR



// ISR Routine

/*
   ISR Function will be as simple as incrementing the interrupts counter that will signal the main loop that an interrupt as occurred.

   This will be done inside a critical section, declared with the portENTER_CRITICAL_ISR and portEXIT_CRITICAL_ISR macros,
   which both receive as input parameters the address of the portMUX_TYPE global variable declared earlier.

   The interrupt handling routine should have the IRAM_ATTR attribute,
   in order for the compiler to place the code in IRAM.

   !! Interrupt handling routines should only call functions also placed in IRAM !! see IDF documentation


*/


void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}


/*  Notes on Using Internal Timers


   Initialize our timer with a call to the timerBegin function.
   Returns a pointer to a structure of type hw_timer_t,
   one of the timer global variable declared in the previous section.

   As input, this function receives:
   -    Number of timer, from 0 to 3, since we have 4 hardware timers
   -    value of the prescaler
   -    flag indicating if the counter should count up (true) or down (false).

   Prescaler Notes:
   Typically the frequency of the base signal used by the ESP32 counters is 80 MHz (this is true for the FireBeetle board).
   This value is equal to 80 000 000 Hz, which means means the signal would make the timer counter increment 80 000 000 times per second.

   Thus, if we divide this value by 80 (using 80 as the prescaler value),
   we will get a signal with a 1 MHz frequency that will increment the timer counter 1 000 000 times per second.

   From the previous value, if we invert it, we know that the counter will be incremented at each microsecond.
   And thus, using a prescaler of 80, when we call the function to set the counter value for generating the interrupt,

*/


void timerSetup(uint32_t timer_delay = 250000) {

  // Begin the timer

  timer = timerBegin(0 , 80, true);                    // timerBegin(timerNumber, prescaler, count_up)

  // Attach interrupt

  timerAttachInterrupt(timer, &onTimer, true);        // timerAttachInterrupt(pointer to initialised_timer, address of function to handle interrupt, edgeTriggered)   if edgeTriggered == flase, levelTriggered = true

  // Specify the counter value in which the timer interrupt will be generated

  timerAlarmWrite(timer, timer_delay, true);           // timerAlarmWrite (pointer to timer, value of counter at which to generate interupt*, reset interrupt to repeat)
  // * if prescaler of 80 is specified then each counter = 1 uS therefore, * 1000 = 1 mS or * 1 000 000 = 1 s 
  // Therefor 500,000 = 120 ticks per min, as each tick = half beat -> 250,000 = 120bpm for the stepsequencer

  // Enable the timer

  timerAlarmEnable(timer);

}


 
void updateTimer(uint32_t delayTime){      // Delaytime needs to be passed in microseconds

  timerAlarmWrite(timer, delayTime, true);           // timerAlarmWrite (pointer to timer, value of counter at which to generate interupt*, reset interrupt to repeat)

  
}


// Interrupt Detection

/*
    To detect if the ISR has fired, we just check to see if interruptCounter has been incremented, i.e greater than zero.

    Once we have performed our interrupt function, we must decrement the interruptCounter, however this
    must happen within a critical section, as this variable is shared between main loop & ISR

    A much more efficient approach would be using a semaphore to lock the main loop,
    which would be then unlocked by the ISR. This is the approach used on the original example. (Available in ESP32 examples in Arduino IDE)
*/

/* Note on our use of interrupts:
 *  
 *  In order to make adjustable duty cycles easier, each clock pulse will = 1/2 a beat
 *  
 *  Therefore our interupt counter is going to count in 16's
 * 
 * 
 * 
 */

bool interruptCheck() {

 
  if (interruptCounter > 0) {
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);

    totalInterruptCounter++;

  //  Serial.print("Interrupt. Total number: ");
  //  Serial.println(totalInterruptCounter);
    
    return true;
  } else {
    return false;
  }


}
