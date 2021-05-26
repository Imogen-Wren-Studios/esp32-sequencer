/*   Seven Segment Display - Four Character Module

      All functions for the control of a 4 digit Seven Segment Display Module


*/




#include "TM1637.h"
#define CLK 32//pins definitions for TM1637 and can be changed to other ports
#define DIO 33
TM1637 tm1637(CLK, DIO);


/*
    Display Number (4 character 7 Segment Display)

        - Takes in any 4 digit number and prints to 4 digit 7 segment display
        - Any digit unable to be displayed is shown as "FFFF"
*/

void displayNumber(uint16_t number) {

  if (number <= 9999) {
    Serial.printf("Number: [%i]   \n", number);
    byte digits[5];                      // Must be length of largest input integer + 1

    for (int i = 0; i < 5; i++) {
      digits[i] = number % 10;
      Serial.printf("output: [%i]    \n", digits[i]);
      number = number / 10 ;
    }

    tm1637.display(0, digits[3]);
    tm1637.display(1, digits[2]);
    tm1637.display(2, digits[1]);
    tm1637.display(3, digits[0]);
  } else {
    tm1637.display(0, 15);
    tm1637.display(1, 15);
    tm1637.display(2, 15);
    tm1637.display(3, 15);
  }
}






int animateArray[14] = {9, 8 , 7 , 6 , 5 , 4, 3, 2, 1, 0, 0, 0, 0, 0};

void animationA() {
  for (int i = 0; i < 10; i++) {
    tm1637.display(0, animateArray[i + 3]);
    delay(50);
    tm1637.display(1, animateArray[i + 2]);
    delay(50);
    tm1637.display(2, animateArray[i + 1]);
    delay(40);
    tm1637.display(3, animateArray[i]);
    delay(30);
  }
}



void animationB() {        // Only really useful for bombcounters
  uint16_t  number = 9999;
  displayNumber(number);
  delay(200);
  while (number >= 0) {
    displayNumber(number);
    number--;
  }
}

void seven_seg_setup(bool animate = false) {
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  if (animate) {
    animationA();
    delay(400);
    tm1637.clearDisplay();
    delay(500);
  }
}







uint16_t preNum;


// Loop function runs all the time and only calls the print to display loop if the number has changed

void seven_seg_loop(uint16_t newNumber) {
  if (newNumber != preNum) {
    displayNumber(newNumber);
    preNum = newNumber;
  }
}
