/*   Globals.h
 * 
 *    Global Variables
 * 
 * 
 */



 uint16_t bpm = 140;    // BPM variable is modified by rotary_encoder.h




 
uint32_t duty_high;   // variable to hold the HIGH time for the variable duty cycle output
bool pulse_triggered = false;
uint32_t pulse_start_t;           // time pulse was triggered in micros

float DUTY_CYCLE = 0.1;

bool PEDAL_TONE = true;     // Pedal tone effectivily doubles the number of steps in the sequence, by treating the offbeat as a pedal tone,
byte PEDAL_VALUE = 0;       // pedal tone value





byte current_step = 0;

byte tick = 0;            // Counts each half beat (each interrupt tick)
byte beat = 0;              // Counts each beat (1 to 8)
