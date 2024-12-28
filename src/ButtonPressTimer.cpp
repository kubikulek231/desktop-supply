#include "ButtonPressTimer.h"

ButtonPressTimer::ButtonPressTimer(bool buttonActiveHigh, bool buttonActive, unsigned int longPressThreshMillis) 
{
  this->buttonActiveHigh = buttonActiveHigh;
  this->buttonActive = buttonActive;
  this->buttonPressedMillis = -1;
  this->longPressThreshMillis = longPressThreshMillis;
  this->buttonDeactivatedTillReset = false;
}

unsigned int ButtonPressTimer::getState(bool pressed) {

  if (buttonDeactivatedTillReset && pressed == getInactiveState()) {
    buttonDeactivatedTillReset = false;
  }

  if (!buttonActive || (pressed == getInactiveState()) || buttonDeactivatedTillReset) {
    this->buttonPressedMillis = -1;
    return (int8_t)getInactiveState();
  }

  unsigned int millisNow = millis();

  if ((pressed == getActiveState()) && (buttonPressedMillis == -1)) {
      this->buttonPressedMillis = millisNow; 
    }
  // Return how many times longPressThreshMillis in the overall pressed time
  if (millisNow - buttonPressedMillis > longPressThreshMillis) {
    unsigned int returnNum = (millisNow - buttonPressedMillis) / longPressThreshMillis;
    return returnNum + 1; }
  return 1;
}

bool ButtonPressTimer::getInactiveState() {
  return !getActiveState();
}

bool ButtonPressTimer::getActiveState() {
  if (buttonActiveHigh) {
    return true;
  }
  return false;
}

void ButtonPressTimer::setPressThreshMillis(unsigned int longPressThreshMillis) {
  this->buttonPressedMillis = millis() + longPressThreshMillis;
  this->longPressThreshMillis = longPressThreshMillis;
}

void ButtonPressTimer::deactivateTillReset() {
  this->buttonDeactivatedTillReset = true;
}
