#include <Arduino.h>

#ifndef BEEPER_H
#define BEEPER_H

/*
beeperType ("single", "multi"),
beepDelayMillis,
beepDurationMillis,
beepPin,
beepActiveHigh,
beepActive
*/
class Beeper {
public:
    Beeper(
        String beeperType,
        int beepDelayMillis,
        int beepDurationMillis,
        int beepPin,
        bool beepActiveHigh,
        bool beepActive
        );
    void run();
    void reset();
    void setTiming(int beepDelayMillis, int beepDurationMillis);
    void setTimingMode(int8_t mode);


    operator String();
private:
    bool isValidBeeperType(String type);
    void beepOff();
    void beepOn();


    String beeperType;
    int beepDelayMillis;
    int beepDurationMillis;
    bool beepBeeped;
    unsigned int beepNextBeep;
    bool beepBeepin;
    int beepPin;
    bool beepActiveHigh;
    bool beepActive;
};
#endif