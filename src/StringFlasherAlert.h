#include <Arduino.h>
#include "StringFlasher.h"

#ifndef STRINGFLASHERALERT_H
#define STRINGFLASHERALERT_H

/*
StringFlasherAlert class extends StringFlasher to manage alert strings for voltage, current, power, and temperature conditions.
*/
class StringFlasherAlert : public StringFlasher {
public:
    // Constructor: Initializes the StringFlasher base class with activeHigh parameter.
    StringFlasherAlert(bool activeHigh);

    // Returns the appropriate string based on alert conditions.
    String getString(bool overVoltage, bool overCurrent, bool overPower, bool overHeat);

private:
    // Alert strings for different conditions.
    String overVoltageString = "OV";
    String overCurrentString = "OC";
    String overPowerString = "OP";
    String overHeatString = "OH";
};

#endif // STRINGFLASHERALERT_H
