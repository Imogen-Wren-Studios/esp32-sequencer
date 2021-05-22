/* Step Sequencer
 * 
 * - Functions & Methods that directly relate to the values saved and used by the step sequencer
 * 
 * 
 */



byte stepSequenceA[8] = {0, 255, 10, 90, 40, 150 , 30, 255};      // Test values for the step sequencer






void randomise_sequence(int16_t modMin = -255, int16_t modMax = 255) {

  for (int i = 0; i < 8; i++) {
    int modifier = random(modMin, modMax);
    Serial.printf("Step[%i] A: [%i], Mod: [%i], New: [", i, stepSequenceA[i], modifier);
    stepSequenceA[i] = stepSequenceA[i] + modifier;
    Serial.printf("%i]\n", stepSequenceA[i]);


  }

}





void step_sequencer(byte beat) {

  dac_output_voltage(DAC_CHANNEL_1, stepSequenceA[beat]);  // Write analogue value to output

}
