#include "StringFlasherAlert.h"
#include "StringFlasher.h"

// Constructor: initializing the base class StringFlasher
StringFlasherAlert::StringFlasherAlert(bool activeHigh) 
    : StringFlasher("", "", 1000, 1000, activeHigh, false) {
}

// getString method: returns the appropriate string based on the overVoltage, overCurrent, and overPower conditions
String StringFlasherAlert::getString(bool overVoltage, bool overCurrent, bool overPower, bool overHeat) {
    
    // If no over values, deactivate and return the base class string
    if (!overVoltage && !overCurrent && !overPower && !overHeat) {
        setActive(false);
        return StringFlasher::getString();
    }

    // Activate if any over value is true
    setActive(true);

    // Constructing the result string
    String result;
    if (overVoltage) {
        if (!result.isEmpty()) { result += " "; }
        result += overVoltageString;
    }
    if (overCurrent) {
        if (!result.isEmpty()) { result += " "; }
        result += overCurrentString;
    }
    if (overPower) {
        if (!result.isEmpty()) { result += " "; }
        result += overPowerString;
    }
    if (overHeat) {
        if (!result.isEmpty()) { result += " "; }
        result += overHeatString;
    }

    // Setting the active and inactive strings in the base class
    setFlasherStringInactive("");
    setFlasherStringActive(result);

    // Returning the base class string
    return StringFlasher::getString();
}
