#ifndef BUTTONPRESSTIMER_H
#define BUTTONPRESSTIMER_H

#include <Arduino.h>

class ButtonPressTimer {
public:
    ButtonPressTimer(bool buttonActiveHigh = true, 
                  bool buttonActive = true,
                  unsigned int longPressThreshMillis = 3000);
    unsigned int getState(bool pressed);
    void setPressThreshMillis(unsigned int longPressThreshMillis);
    void deactivateTillReset();
private:
    bool getInactiveState();
    bool getActiveState();

    long longPressThreshMillis;
    bool buttonActiveHigh;
    bool buttonActive;
    long buttonPressedMillis;
    bool buttonDeactivatedTillReset;
};

#endif