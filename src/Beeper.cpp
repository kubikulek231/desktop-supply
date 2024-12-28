#include <Arduino.h>
#include "Beeper.h"

class ValueTypeException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Incorrect beeper type!\nValid beeper types are:\n 'single', 'multi'!";
    }
};

void Beeper::run() {
    if (!beepActive) {
        return;
    }

    unsigned long currentMillis = millis();

    if (beeperType == "single") {
        // Already beeped
        if (beepBeeped) {
            return;
        }
        // Beeping time has not come yet
        if (beepNextBeep > currentMillis) {
            beepOff();
            return;
        }
        // Beepin finished
        if (beepNextBeep + beepDurationMillis < currentMillis) {
            beepOff();
            this->beepBeeped = true;
            this->beepNextBeep = currentMillis + beepDelayMillis;
            return;
        }
        // Beepin
        beepOn();
        return;
    }

    if (beeperType == "multi") {
        
        // Beeping time has not come yet
        if (beepNextBeep > currentMillis) {
            beepOff();
            return;
        }
        // Beepin finished
        if (beepNextBeep + beepDurationMillis < currentMillis) {
            beepOff();
            this->beepBeeped = true;
            this->beepNextBeep = currentMillis + beepDelayMillis;
            return;
        }
        // Beepin
        beepOn();
        return;
    }
}

void Beeper::reset() {
    this->beepBeeped = false;
    this->beepNextBeep = millis() + beepDelayMillis; 
}

Beeper::Beeper(String beeperType, 
               int beepDelayMillis,
               int beepDurationMillis,
               int beepPin,
               bool beepActiveHigh,
               bool beepActive
               ) {
    beeperType.toLowerCase();

    if (!isValidBeeperType(beeperType)) {throw ValueTypeException(); }

    this->beepPin = beepPin;
    this->beepActiveHigh = beepActiveHigh;
    this->beeperType = beeperType;
    this->beepDelayMillis = beepDelayMillis;
    this->beepDurationMillis = beepDurationMillis;
    this->beepBeeped = false;
    this->beepNextBeep = millis() + beepDelayMillis;
    this->beepBeepin = false;
    this->beepActive = true;
}

bool Beeper::isValidBeeperType(String type) {
    if (type == "single") {return true; }
    if (type == "multi") {return true; }
    return false;
}

void Beeper::beepOn() {
    if (beepBeepin) {return; }
    digitalWrite(beepPin, beepActiveHigh ? HIGH : LOW);
    this->beepBeepin = true;
}

void Beeper::beepOff() {
    if (!beepBeepin) {return; }
    digitalWrite(beepPin, beepActiveHigh ? LOW : HIGH);
    this->beepBeepin = false;
}

Beeper::operator String() {
    String result = "Beeper(";
    result += "beepPin: " + String(beepPin) + ", ";
    result += "beepActiveHigh: " + String(beepActiveHigh ? "true" : "false") + ", ";
    result += "beeperType: " + beeperType + ", ";
    result += "beepDelayMillis: " + String(beepDelayMillis) + ", ";
    result += "beepDurationMillis: " + String(beepDurationMillis) + ", ";
    result += "beepBeeped: " + String(beepBeeped ? "true" : "false") + ", ";
    result += "beepNextBeep: " + String(beepNextBeep) + ", ";
    result += "beepBeepin: " + String(beepBeepin ? "true" : "false") + ", ";
    result += "beepActive: " + String(beepActive ? "true" : "false");
    result += ") millis: " + String(millis()) + "\n";
    return result;
}

void Beeper::setTiming(int beepDelayMillis, int beepDurationMillis) {
    this->beepDelayMillis = beepDelayMillis;
    this->beepDurationMillis = beepDurationMillis;
}

void Beeper::setTimingMode(int8_t mode) {
    if (mode == 0) {this->beepActive = false; }
    if (mode == 1) {setTiming(1500, 1500); }
    if (mode == 2) {setTiming(1000, 1000); }
    if (mode == 3) {setTiming(500, 500); }
}