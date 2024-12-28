#include <lvgl.h>
#include <TFT_eSPI.h>
#include "ValConverter.h"
#include "Beeper.h"
#include "StringFlasher.h"
#include "StringFlasherAlert.h"
#include "INA226.h"
#include "DisplayHandler.h"
#include "Adafruit_AHTX0.h"
#include "ButtonPressTimer.h"

// great resource on lvgl esp32s3
// https://www.youtube.com/watch?v=s4_fUwhw-dI

// #define TFT_CS   10       10     CS
// #define TFT_SCK  12       12     SC
// #define TFT_MISO 13       13     SDA-O
// #define TFT_MOSI 11       11     SD
// #define TFT_DC   7        1      DC
// #define TFT_RST  15       42     RST
// #define TFT_FREQ 2700000
// #define TFT_BL            2      BL

#define BEEPER_PIN    4
#define ON_BUTTON_PIN 48

INA226 INA0(0x40);
INA226 INA1(0x41);
INA226 INA2(0x44);
INA226 INA3(0x45);

Adafruit_AHTX0 aht;

// Voltage
ValConverter valConverterV0 = ValConverter("V", 4, 0.1, 0.1);
ValConverter valConverterV1 = ValConverter("V", 4, 0.1, 0.1);
ValConverter valConverterV2 = ValConverter("V", 4, 0.1, 0.1);
ValConverter valConverterV3 = ValConverter("V", 4, 0.1, 0.1);
// Current
ValConverter valConverterA0 = ValConverter("C", 4, 0.1, 0.1);
ValConverter valConverterA1 = ValConverter("C", 4, 0.1, 0.1);
ValConverter valConverterA2 = ValConverter("C", 4, 0.1, 0.1);
ValConverter valConverterA3 = ValConverter("C", 4, 0.1, 0.1);
// Power
ValConverter valConverterP0 = ValConverter("P", 4, 0.1, 0.1);
ValConverter valConverterP1 = ValConverter("P", 4, 0.1, 0.1);
ValConverter valConverterP2 = ValConverter("P", 4, 0.1, 0.1);
ValConverter valConverterP3 = ValConverter("P", 4, 0.1, 0.1);

double measuredValue = 0.0;
bool direction = true;
Beeper beeperMulti = Beeper("multi", 1000, 1000, BEEPER_PIN, true, true);
Beeper beeperSingle = Beeper("single", 0, 500, BEEPER_PIN, true, true);
DisplayHandler displayHandler = DisplayHandler();


void setup()
{
    displayHandler.setup();
    displayHandler.drawBootScreen(true, "Connect display:", "SUCCESS", "");
    displayHandler.setBacklightPWM(255);
    displayHandler.drawBootScreen(true, "Set full brightness:", "SUCCESS", "");
    Serial.begin(115200);
    displayHandler.drawBootScreen(true, "Open serial:", "SUCCESS", "");
    displayHandler.drawBootScreen(Wire.setPins(8, 18) && Wire.begin(), "Open I2C:", "SUCCESS", "");
    displayHandler.drawBootScreen(aht.begin(), "Connect to AHT30:", "SUCCESS", "FAIL");
    displayHandler.drawBootScreen(true, String("INA226 (voltage and current sensors) lib version: ") + String(INA226_LIB_VERSION), "", "");
    displayHandler.drawBootScreen(INA0.begin() && INA0.setMaxCurrentShunt(1, 0.002) == 0, "Connect to INA0:", "SUCCESS", "FAIL");
    displayHandler.drawBootScreen(INA1.begin() && INA1.setMaxCurrentShunt(1, 0.002) == 0, "Connect to INA1:", "SUCCESS", "FAIL");
    displayHandler.drawBootScreen(INA2.begin() && INA2.setMaxCurrentShunt(1, 0.002) == 0, "Connect to INA2:", "SUCCESS", "FAIL");
    displayHandler.drawBootScreen(INA3.begin() && INA3.setMaxCurrentShunt(1, 0.002) == 0, "Connect to INA3:", "SUCCESS", "FAIL");
    pinMode(ON_BUTTON_PIN, INPUT_PULLUP);
    displayHandler.drawBootScreen(digitalRead(ON_BUTTON_PIN) == HIGH, "On button check:", "SUCCESS", "FAIL");
}

void loop()
{
    ButtonPressTimer onButtonTimer = ButtonPressTimer(true, true, 3000);
    while (true) {
        while (true) {
            displayHandler.drawStandbyScreen();

            uint8_t newState = onButtonTimer.getState(digitalRead(ON_BUTTON_PIN) == LOW);
            // newState 1 means transition after given delay transition
            if (newState > 1) {onButtonTimer.deactivateTillReset(); break; }
        }

        Serial.println("breaking the loop...");

        while (true) {
            sensors_event_t humidity, temp;
            aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
            Serial.print("Temperature: ");Serial.print(temp.temperature);Serial.println(" degrees C");

            displayHandler.drawMainScreen(
                valConverterV0.getValueString(INA0.getBusVoltage()),
                valConverterV1.getValueString(INA1.getBusVoltage()),
                valConverterV2.getValueString(INA2.getBusVoltage()),
                valConverterV3.getValueString(INA3.getBusVoltage()),
                valConverterA0.getValueString(INA0.getCurrent()),
                valConverterA1.getValueString(INA1.getCurrent()),
                valConverterA2.getValueString(INA2.getCurrent()),
                valConverterA3.getValueString(INA3.getCurrent()),
                valConverterP0.getValueString(INA0.getPower()),
                valConverterP1.getValueString(INA1.getPower()),
                valConverterP2.getValueString(INA2.getPower()),
                valConverterP3.getValueString(INA3.getPower()));

            beeperMulti.setTimingMode(3);
            if (direction) {measuredValue = measuredValue + 0.000033; }
            if (!direction) {measuredValue = measuredValue - 0.000033; }

            if (measuredValue <= 0.00) {direction = true; } 
            if (measuredValue >= 0.02) {direction = false; } 

            uint8_t newState = onButtonTimer.getState(digitalRead(ON_BUTTON_PIN) == LOW);
            // newState 0 means immidiate transition
            if (newState > 0) {onButtonTimer.deactivateTillReset(); break; }
        }
    }
}
