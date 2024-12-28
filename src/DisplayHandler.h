#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <vector>
#include "StringFlasherAlert.h"

#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

/*Set to your screen resolution*/
#define TFT_HOR_RES    480
#define TFT_VER_RES    320

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))

#define PWM_CHANNEL    0
#define PWM_FREQUENCY  25000 
#define PWM_RESOLUTION 8   // 8-bit resolution, i.e., values from 0 to 255
#define BACKLIGHT_PIN  35

class DisplayHandler {
public:
    DisplayHandler() : stringFlasherAlert(true) { 
        this->stringFlasherAlert.setTimingMode(3);
    }
    void setup();
    void drawMainScreen(String valueV0,
                  String valueV1,
                  String valueV2,
                  String valueV3,
                  String valueA0,
                  String valueA1,
                  String valueA2,
                  String valueA3,
                  String valueW0,
                  String valueW1,
                  String valueW2,
                  String valueW3);
    void setBacklightPWM(int value);
    void drawBootScreen(bool condition, String lineToAppend, String conditionSuccess, String conditionFail, uint8_t delayAfterDraw = 200);
    void drawStandbyScreen();
private:
    String convertLinesVectorToString(std::vector<String> stringVector);
    std::vector<String> bootLinesText;
    std::vector<String> bootLinesSuccess;
    std::vector<String> bootLinesFail;
    void *draw_buf;
    unsigned long lastTickMilis = 0;
    lv_display_t* display;
    static lv_style_t rectStyle0;
    static lv_style_t valueVStyle0;
    static lv_style_t valueAStyle0;
    static lv_style_t valueWStyle0;
    static lv_style_t labelStyle0;
    static lv_style_t valueVStyle1;
    static lv_style_t valueAStyle1;
    static lv_style_t valueWStyle1;
    static lv_style_t labelStyle1;
    static lv_style_t labelStyleAlert;
    static lv_style_t bootTextStyle;
    static lv_style_t bootFailStyle;
    static lv_style_t bootSuccessStyle;
    StringFlasherAlert stringFlasherAlert;
};

#endif