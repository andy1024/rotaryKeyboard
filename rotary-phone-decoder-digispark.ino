#include "DigiKeyboard.h"

//based on http://www.instructables.com/id/Interface-a-rotary-phone-dial-to-an-Arduino/
//using digispark keyboard library

int refreshCount = 100;
int needToPrint = 0;
int count;
int in = 0;
int lastState = LOW;
int trueState = LOW;
long lastStateChangeTime = 0;
int cleared = 0;

// constants
int dialHasFinishedRotatingAfterMs = 100;
int debounceDelay = 10;
int loopDelay = 1;

void setup()
{
  pinMode(in, INPUT); //INPUT_PULLUP
}

void loop()
{
  if ((refreshCount++)>=100) {
    DigiKeyboard.sendKeyStroke(0);
    refreshCount = 0;
  }
  int reading = digitalRead(in);
  
  if ((millis() - lastStateChangeTime) > dialHasFinishedRotatingAfterMs) {
    // the dial isn't being dialed, or has just finished being dialed.
    if (needToPrint) {
      // if it's only just finished being dialed, we need to send the number down the serial
      // line and reset the count. We mod the count by 10 because '0' will send 10 pulses.
      int number = count % 10;
      DigiKeyboard.print(number, DEC);
  
      needToPrint = 0;
      count = 0;
      cleared = 0;
    }
  } 
  
  if (reading != lastState) {
    lastStateChangeTime = millis();
  }
  if ((millis() - lastStateChangeTime) > debounceDelay) {
    // debounce - this happens once it's stablized
    if (reading != trueState) {
      // this means that the switch has either just gone from closed->open or vice versa.
      trueState = reading;
      if (trueState == HIGH) {
        // increment the count of pulses if it's gone high.
        count++; 
        needToPrint = 1; // we'll need to print this number (once the dial has finished rotating)
      } 
    }
  }
  lastState = reading;
  //DigiKeyboard.delay(loopDelay);
} 
