#include <Arduino.h>
#include <vector>

#ifndef VALCONVERTER_H
#define VALCONVERTER_H

class ValConverter {
public:
    ValConverter(
        String valueType,
        int8_t digitNum = 4,    
        double hysScaleThresh = 0.1,
        double hysValueThresh = 0.0005
        );
    String getValueString(double value);
    String getUnit();

    operator String();
private:
    String getValueStringBase(double value);
    String getValueStringMilis(double value);
    String getValueStringMicros(double value);
    int8_t getNewScale(double value);
    bool getNewValue(double value);
    int getDirection(double value);

    // Static helper funcs
    static bool isValidValueType(String valueType);
    static int getNumberOfDigits(int number);
    static double roundToDecimals(double value, int8_t decimalNum); 
    static int roundToPlace(int value, int8_t place);
    static String getStuffString(String stuffSym, int8_t stuffNum);
    static String getDigitsString(String value, int8_t digitNum);
    static String getDoubleIntString(double value, int8_t resultLen, bool roundLastPlaceIfGreater = true, bool stuffToMatchLen = false);
    static String getDoubleFracString(double value, int8_t resultLen, bool roundLastPlaceIfGreater = true, bool stuffToMatchLen = true);
    static bool doesCarry(double value, int8_t roundingDecimal);
    static bool checkHysteresis(double value, double refValue, double hysMult, bool direction = true);

    String valueType;

    int8_t digitNum;
    double hysScaleThresh;
    double hysValueThresh;
    int8_t storedScale;
    double storedValue;
};
#endif