/* Timing Maths


    Algorithms to convert:

    BPM to:
      - Delay Between Beats (uS)   (Db)
      - Delay Between Clock Ticks (uS)

    Db to:
      - Delay between beats * duty cycle

*/




uint32_t bpm_to_delay(uint16_t bpm) {   // Output is delay value for an entire beat. This needs to be halved to use with our tick system (50% duty), or subdivided to account for other duty cycles

  // BPM / 60 = Beats per second (Hz)
  // Hz * 1000 = lambda (mS)
  // Hz * 1,000,000 = lambda (uS)
  // Hz * 500,000 = 1/2 lambda (uS)  // This one for ticks system < This produced a bpm that felt far too low. Each beat on the sequencer actually needs to be a half beat
  // Hz * 25000 = 1/4 lambda (uS) // <- This felt far more correct

  //


  Serial.printf("\n\nBPM: [%i]\n", bpm);

  float hz = bpm / 60.0;

  char buffer[16];
  dtostrf(hz, 3, 6, buffer);

  Serial.printf("hz: [%s]\n", buffer);

  uint32_t delay_per_tick = 250000 / hz;

  Serial.printf("delay_per_tick: [%i]\n\n", delay_per_tick);

  return delay_per_tick;


}


// Although this data structure and function might work, our implementation only requires the time HIGH,
struct duty_delay {
  uint32_t time_high;
  uint32_t time_low;
};



duty_delay duty_cycle_calc( uint32_t lambda, float duty_cycle) {    //

  uint32_t time_high = lambda * duty_cycle;
  uint32_t time_low = lambda - time_high;

  duty_delay output = {time_high, time_low};

  return output;

}


// Same Function as above but only returns time HIGH

uint32_t duty_cycle_high(uint32_t lambda, float duty_cycle) {    //

  // As lambda is a half beat, we must double it here to make the duty cycle calcs work for a whole beat

  lambda = lambda * 2;

  uint32_t time_high = lambda * duty_cycle ;    
  uint32_t time_low = lambda - time_high;

   char buffer[16];
  dtostrf(duty_cycle, 3, 6, buffer);

  Serial.printf("\nDuty Cycle: [%s], Time High: [%i], Time Low: [%i]   \n", buffer, time_high, time_low);


  return time_high;

}
