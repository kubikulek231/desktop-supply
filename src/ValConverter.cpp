#include "ValConverter.h"
#include <Arduino.h>
#include <vector>

#include <stdexcept>

class ValueTypeException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Incorrect value type!\nValid value types are:\n 'voltage', 'current', 'power'!";
    }
};

ValConverter::ValConverter(String valueType, int8_t digitNum, double hysScaleThresh, double hysValueThresh) {
  // Check for correct value type
  if (!isValidValueType(valueType)) {throw ValueTypeException(); }

  valueType.toLowerCase();

  this->valueType = valueType;
  this->digitNum = digitNum;
  this->hysScaleThresh = hysScaleThresh;
  this->hysValueThresh = hysValueThresh;
  this->storedValue = 0;
  this->storedScale = 2;
}

bool ValConverter::isValidValueType(String valueType) {
  valueType.toLowerCase();
  std::vector<String> allowedTypes = {"voltage", "current", "power", "v", "c", "p"};
  for (int i = 0; i < allowedTypes.size(); i++) {
    if (valueType == allowedTypes[i]) {return true; }
  }
  return false;
}

bool ValConverter::doesCarry(double value, int8_t roundingDecimal) {
  // Split double value into integer and fractional parts
  double integerPart;
  double fractionalPart;
  fractionalPart = modf(value, &integerPart);

  // Round the fractional part to the desired decimal places
  double roundedFractionalPart = roundToDecimals(fractionalPart, roundingDecimal);

  // Check if the rounded fractional part causes a carry
  if (roundedFractionalPart >= 1.0) {
    return true;  // Carry occurs
  }
  return false;  // No carry
}

int ValConverter::getNumberOfDigits(int number) {
    // Handle the special case where the number is 0
    if (number == 0) return 1;
    
    // For negative numbers, consider the absolute value
    number = std::abs(number);

    // Calculate the number of digits
    return static_cast<int>(std::log10(number)) + 1;
}

double ValConverter::roundToDecimals(double value, int8_t decimalNum) {
  double factor = std::pow(10, decimalNum);
  return std::round(value * factor) / factor;
}

int ValConverter::roundToPlace(int value, int8_t place) {
    int numDigits = getNumberOfDigits(value);

    // If the place is greater than the number of digits, return the original value
    if (place >= numDigits) {
        return value;
    }

    int factor = std::pow(10, place);
    return std::round(static_cast<double>(value) / factor) * factor;
}

String ValConverter::getDigitsString(String value, int8_t digitNum) {
  String returnString = "";
  for (int i = 0; i < value.length(); i++) {
    if (i >= digitNum) {continue; }
    returnString = returnString + value[i];
  }
  return returnString;
}

String ValConverter::getStuffString(String stuffSym, int8_t stuffNum) {
  String returnString = "";
  for (int i = 0; i < stuffNum; i++) {
    returnString = returnString + stuffSym;
  }
  return returnString;
}

// Returns string of int part of double
String ValConverter::getDoubleIntString(double value, int8_t resultLen, bool roundLastPlaceIfGreater, bool stuffToMatchLen) {
  // Split double value into integer and fractional parts
  double doubleInt;
  double doubleFrac;
  doubleFrac = modf(value, &doubleInt);

  int intPart = int(doubleInt);

  // If digit number is greater than resultLen and rounding is required
  if (roundLastPlaceIfGreater) {
    intPart = roundToPlace(intPart, resultLen);
  }

  String stringInt = String(intPart);
  
  // Get desired len by stripping trailing chars
  String strippedString = getDigitsString(stringInt, resultLen);
  // If stuffing is not required, return the strippedString as is
  if (!stuffToMatchLen) {return strippedString; }

  // Stuff the string
  String resultString = getStuffString("0", resultLen - strippedString.length()) + strippedString;
  return resultString;
}

// Returns string of fract part of double
String ValConverter::getDoubleFracString(double value, int8_t resultLen, bool roundLastPlaceIfGreater, bool stuffToMatchLen) {
  // Split double value into integer and fractional parts
  double doubleInt;
  double doubleFrac;
  doubleFrac = modf(value, &doubleInt);

  // Round to desired decimal
  if (roundLastPlaceIfGreater) {
    doubleFrac = roundToDecimals(doubleFrac, resultLen);
  }

  String stringFrac = String(doubleFrac, 10);

  // // Strip first two chars (0.) // Do this dynamically
  // String strippedString = "";
  // for (int i = 2; i < stringFrac.length(); i++) {
  //     strippedString += stringFrac[i];
  // }

  String strippedString = "";

  bool decimalHit = false;
  uint8_t decimalsBeforeHit = 0;

  for (int i = 0; i < stringFrac.length(); i++) {
      char selectedChar = stringFrac[i];
      
      if (decimalHit) {
          strippedString += selectedChar;
      } else if (selectedChar == '.') {
          decimalHit = true;
      } else {
        decimalsBeforeHit++;
      }
  }

  String finalString = getDigitsString(strippedString, resultLen);

  // If stuffing is not required, return the strippedString as is
  if (!stuffToMatchLen) {return finalString; }

  // Stuff the string
  String resultString = finalString + getStuffString("0", resultLen - strippedString.length() + 1 - decimalsBeforeHit);
  return resultString;
}


// Gets resulting display string in base scale
String ValConverter::getValueStringBase(double value) {
  bool negative = false;
  value = std::abs(value);

  String decimalPoint = ".";

  String stringInt = getDoubleIntString(value, digitNum, true, false);

  int8_t remainingDigitNum = digitNum - stringInt.length();

  // If 1 was carried, redo the int part
  // retarded but who cares
  if (doesCarry(value, remainingDigitNum)) {
    value = value++;
    stringInt = getDoubleIntString(value, digitNum, true, false);
    // Serial.println("carries!");
    // Calculate the remaining digits again (in case was 9,999 and not is 10,000)
    remainingDigitNum = digitNum - stringInt.length();
  }

  String stringFrac = getDoubleFracString(value, remainingDigitNum, true, true);

  if (remainingDigitNum < 1) {decimalPoint = "";}

  // Serial.println("value");
  // Serial.println(value);
  // Serial.println("stringInt.length()");
  // Serial.println(stringInt.length());
  // Serial.println("stringInt");
  // Serial.println(stringInt);
  // Serial.println("stringFrac");
  // Serial.println(stringFrac);
  // Serial.println("remainingDigitNum");
  // Serial.println(remainingDigitNum);
  // Serial.println("stringInt + decimalPoint + stringFrac");
  // Serial.println(stringInt + decimalPoint + stringFrac);

  // delay(1000);

  return stringInt + decimalPoint + stringFrac;
}

// Gets resulting display string in millis scale
String ValConverter::getValueStringMilis(double value) {
  return getValueStringBase(value * 1000);
}

// Gets resulting display string in micros scale
String ValConverter::getValueStringMicros(double value) {
  return getValueStringBase(value * 1000000);
}

// Gets string in appropriate scale
String ValConverter::getValueString(double value) {
  this->storedScale = getNewScale(value);
  String valueString = "ERR";

  if (storedScale == 0) {
    valueString = getValueStringBase(value);
  }
  if (storedScale == 1) {
    valueString = getValueStringMilis(value);
  }
  if (storedScale == 2) {
    valueString = getValueStringMicros(value);
  }
  
  getNewValue(value);

  return valueString + getUnit();
}

int8_t ValConverter::getNewScale(double value) {
  // base -> millis
  if (storedScale == 0 && getDirection(value) == -1 && value <= 1) {
    return 1;
  }

  // millis -> micros
  if (storedScale == 1 && getDirection(value) == -1 && value <= 0.001) {
    return 2;
  }

  // possible bug
  // micros -> millis
  if (storedScale == 2 && getDirection(value) == 1 && value >= 0.001 
  && checkHysteresis(value, 0.001, hysScaleThresh, true)) {
    return 1;
  }

  // millis -> base
  if (storedScale == 1 && getDirection(value) == 1 && value >= 1 
  && checkHysteresis(value, 1, hysScaleThresh, true)) {
    return 0;
  }

  return storedScale;
}

bool ValConverter::getNewValue(double value) {
  if (checkHysteresis(value, storedValue, hysValueThresh, true) 
  || checkHysteresis(value, storedValue, hysValueThresh, false)) {
    this->storedValue = value;
    return true;
  }
  return false;
}

// Gets appropriate unit regarding the scale and type
String ValConverter::getUnit(){
  String scaleString = "";
  String typeString = "";
  // Get scale
  if (storedScale == 0) { scaleString = ""; } 
  if (storedScale == 1) { scaleString = "m"; } 
  if (storedScale == 2) { scaleString = "u"; } 

  if (valueType == "v" || valueType == "voltage") { typeString = "V"; }
  if (valueType == "c" || valueType == "current") { typeString = "A"; }
  if (valueType == "p" || valueType == "power") { typeString = "W"; }

  if (scaleString == "") {return " " + typeString; }

  return " " + scaleString + typeString;
}

int ValConverter::getDirection(double value) {
  if (value > storedValue) {return 1; }
  if (value < storedValue) {return -1; }
  return 0;
}

bool ValConverter::checkHysteresis(double value, double refValue, double hysMult, bool direction) {
  // Apply in forward direction
  if (direction) {
    if (value > refValue * (1 + hysMult)) { return true; }
    return false;
  }
  // Apply in backward direction
  if (value > refValue * (1 - hysMult)) { return true; }
  return false;
}

// Implement the overloaded String operator
ValConverter::operator String()  {
    return "ValConverter(" 
        + valueType 
        + ", digitNum: " + String(digitNum) 
        + ", hysScaleThresh: " + String(static_cast<double>(hysScaleThresh)) 
        + ", hysValueThresh: " + String(static_cast<double>(hysValueThresh)) 
        + ", storedScale: " + String(storedScale) 
        + ", storedValue: " + String(storedValue, 6)
        + ") \n";
}