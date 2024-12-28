#include <Arduino.h>
#include <vector>

#ifndef STRINGFLASHER_H
#define STRINGFLASHER_H

/*
flasherStringActive,
flasherStringInactive,
flasherDelayMillis,
flasherDurationMillis,
flasherActiveHigh,
flasherActive
*/
class StringFlasher {
public:
    StringFlasher(
        String flasherStringActive,
        String flasherStringInactive,
        int flasherDelayMillis,
        int flasherDurationMillis,
        bool flasherActiveHigh,
        bool flasherActive
        );

    virtual String getString();

    // Getters
    unsigned int getNextFlash() {
        return flasherNextFlash;
    }
    int getDelayMillis() {
        return flasherDelayMillis;
    }
    int getDurationMillis() {
        return flasherDurationMillis;
    }
    bool getActive() {
        return flasherActive;
    }

    // Setters
    void setNextFlash(unsigned int flasherNextFlash) {
        this->flasherNextFlash = flasherNextFlash;
    }
    void setActive(bool flasherActive) {
        this->flasherActive = flasherActive;
    }
    void setDelayMillis(int flasherDelayMillis) {
        this->flasherDelayMillis = flasherDelayMillis;
    }
    void setDurationMillis(int flasherDurationMillis) {
        this->flasherDurationMillis = flasherDurationMillis;
    }
    void setTiming(int flasherDelayMillis, int flasherDurationMillis) {
        this->flasherDelayMillis = flasherDelayMillis;
        this->flasherDurationMillis = flasherDurationMillis;
    }
    void setFlasherStringActive(String flasherStringActive) {
        this->flasherStringActive = flasherStringActive;
    }
    void setFlasherStringInactive(String flasherStringInactive) {
        this->flasherStringInactive = flasherStringInactive;
    }
    
    void setTimingMode(int8_t mode);

    operator String();
private:
    int flasherDelayMillis;
    int flasherDurationMillis;
    unsigned int flasherNextFlash;
    bool flasherActiveHigh;
    bool flasherActive;
    String flasherStringActive;
    String flasherStringInactive;

    String getActiveString();
    String getInactiveString();
};
#endif