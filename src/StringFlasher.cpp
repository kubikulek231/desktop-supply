#include "StringFlasher.h"

StringFlasher::StringFlasher(
                            String flasherStringActive,
                            String flasherStringInactive,
                            int flasherDelayMillis,
                            int flasherDurationMillis,
                            bool flasherActiveHigh,
                            bool flasherActive
                            ) 
{
    this->flasherStringActive = flasherStringActive;
    this->flasherStringInactive = flasherStringInactive;
    this->flasherDelayMillis = flasherDelayMillis;
    this->flasherDurationMillis = flasherDurationMillis;
    this->flasherActiveHigh = flasherActiveHigh;
    this->flasherActive = flasherActive;
    this->flasherNextFlash = millis();
}

String StringFlasher::getString() {

    if (!flasherActive) {
        return getInactiveString();
    }

    unsigned int currentMillis = millis();
    // Flash time has not come yet
    if (flasherNextFlash > currentMillis) {
        return getInactiveString();
    }
    // Flashed
    if (flasherNextFlash + flasherDurationMillis < currentMillis) {
        this->flasherNextFlash = currentMillis + flasherDelayMillis;
        return getInactiveString();
    }
    // Flashing
    return getActiveString();
}

String StringFlasher::getActiveString() {
    return flasherActiveHigh ? flasherStringActive : flasherStringInactive;
}

String StringFlasher::getInactiveString() {
    return flasherActiveHigh ? flasherStringInactive : flasherStringActive;
}

void StringFlasher::setTimingMode(int8_t mode) {
    if (mode == 0) {flasherActive = false; }
    if (mode == 1) {setTiming(1500, 1500); }
    if (mode == 2) {setTiming(1000, 1000); }
    if (mode == 3) {setTiming(500, 500); }
}

// Override operator String
StringFlasher::operator String() {
    String result = "StringFlasher(";
    result += "flasherStringActive: " + flasherStringActive + ", ";
    result += "flasherStringInactive: " + flasherStringInactive + ", ";
    result += "flasherDelayMillis: " + String(flasherDelayMillis) + ", ";
    result += "flasherDurationMillis: " + String(flasherDurationMillis) + ", ";
    result += "flasherActiveHigh: " + String(flasherActiveHigh ? "true" : "false") + ", ";
    result += "flasherActive: " + String(flasherActive ? "true" : "false") + ", ";
    result += "flasherNextFlash: " + String(flasherNextFlash);
    result += ") millis: " + String(millis()) + "\n";
    return result;
}