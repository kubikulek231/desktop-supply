#include "DisplayHandler.h"

lv_style_t DisplayHandler::rectStyle0;
lv_style_t DisplayHandler::valueVStyle0;
lv_style_t DisplayHandler::valueAStyle0;
lv_style_t DisplayHandler::valueWStyle0;
lv_style_t DisplayHandler::labelStyle0;
lv_style_t DisplayHandler::valueVStyle1;
lv_style_t DisplayHandler::valueAStyle1;
lv_style_t DisplayHandler::valueWStyle1;
lv_style_t DisplayHandler::labelStyle1;
lv_style_t DisplayHandler::labelStyleAlert;
lv_style_t DisplayHandler::bootTextStyle;
lv_style_t DisplayHandler::bootSuccessStyle;
lv_style_t DisplayHandler::bootFailStyle;

void DisplayHandler::setup() {

    // Set up PWM channel
    ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    
    // Attach the PWM channel to the specified GPIO pin
    ledcAttachPin(BACKLIGHT_PIN, PWM_CHANNEL);
    
    // Set initial duty cycle (brightness level)
    ledcWrite(PWM_CHANNEL, 0);

    lv_init();

    this->draw_buf = heap_caps_malloc(DRAW_BUF_SIZE, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);

    // Create display with modified rotation
    this->display = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, DRAW_BUF_SIZE, 1);

    // Define labelStyle0 - font color: WHITE, font size: 48
    lv_style_init(&DisplayHandler::labelStyle0);
    lv_style_set_text_font(&DisplayHandler::labelStyle0, &lv_font_montserrat_48);
    lv_style_set_text_color(&DisplayHandler::labelStyle0, lv_color_hex(0xffffff));

    // Define valueVStyle0 - font color: RED, font size: 48
    lv_style_init(&DisplayHandler::valueVStyle0);
    lv_style_set_text_font(&DisplayHandler::valueVStyle0, &lv_font_montserrat_48);
    lv_style_set_text_color(&DisplayHandler::valueVStyle0, lv_color_hex(0xff0000));

    // Define valueAStyle0 - font color: YELLOW, font size: 48
    lv_style_init(&DisplayHandler::valueAStyle0);
    lv_style_set_text_font(&DisplayHandler::valueAStyle0, &lv_font_montserrat_48);
    lv_style_set_text_color(&DisplayHandler::valueAStyle0, lv_color_hex(0xffff00));

    // Define valueWStyle0 - font color: GREEN, font size: 48
    lv_style_init(&DisplayHandler::valueWStyle0);
    lv_style_set_text_font(&DisplayHandler::valueWStyle0, &lv_font_montserrat_48);
    lv_style_set_text_color(&DisplayHandler::valueWStyle0, lv_color_hex(0x00ff00));

    // Define labelStyle1 - font color: WHITE, font size: 24
    lv_style_init(&DisplayHandler::labelStyle1);
    lv_style_set_text_font(&DisplayHandler::labelStyle1, &lv_font_montserrat_24);
    lv_style_set_text_color(&DisplayHandler::labelStyle1, lv_color_hex(0xffffff));

    // Define valueVStyle1 - font color: RED, font size: 24
    lv_style_init(&DisplayHandler::valueVStyle1);
    lv_style_set_text_font(&DisplayHandler::valueVStyle1, &lv_font_montserrat_24);
    lv_style_set_text_color(&DisplayHandler::valueVStyle1, lv_color_hex(0xff0000));

    // Define valueAStyle1 - font color: YELLOW, font size: 24
    lv_style_init(&DisplayHandler::valueAStyle1);
    lv_style_set_text_font(&DisplayHandler::valueAStyle1, &lv_font_montserrat_24);
    lv_style_set_text_color(&DisplayHandler::valueAStyle1, lv_color_hex(0xffff00));

    // Define valueWStyle1 - font color: GREEN, font size: 24
    lv_style_init(&DisplayHandler::valueWStyle1);
    lv_style_set_text_font(&DisplayHandler::valueWStyle1, &lv_font_montserrat_24);
    lv_style_set_text_color(&DisplayHandler::valueWStyle1, lv_color_hex(0x00ff00));

    // Define labelStyleAlert - font color: RED, font size: 24
    lv_style_init(&DisplayHandler::labelStyleAlert);
    lv_style_set_text_font(&DisplayHandler::labelStyleAlert, &lv_font_montserrat_24);
    lv_style_set_text_color(&DisplayHandler::labelStyleAlert, lv_color_hex(0xff0000));

    // Define rectStyle0 - background color: #101010, no border
    lv_style_init(&DisplayHandler::rectStyle0);
    lv_style_set_radius(&DisplayHandler::rectStyle0, 0);
    lv_style_set_bg_opa(&DisplayHandler::rectStyle0, LV_OPA_COVER);
    lv_style_set_bg_color(&DisplayHandler::rectStyle0, lv_color_hex(0x101010));
    lv_style_set_border_width(&DisplayHandler::rectStyle0, 0);

    // Define bootTextStyle - font color: WHITE, font size: 14
    lv_style_init(&DisplayHandler::bootTextStyle);
    lv_style_set_text_font(&DisplayHandler::bootTextStyle, &lv_font_montserrat_14);
    lv_style_set_text_color(&DisplayHandler::bootTextStyle, lv_color_hex(0xffffff));

    // Define bootSuccessStyle - font color: GREEN, font size: 14
    lv_style_init(&DisplayHandler::bootSuccessStyle);
    lv_style_set_text_font(&DisplayHandler::bootSuccessStyle, &lv_font_montserrat_14);
    lv_style_set_text_color(&DisplayHandler::bootSuccessStyle, lv_color_hex(0x00ff00));

    // Define bootFailStyle - font color: RED, font size: 14
    lv_style_init(&DisplayHandler::bootFailStyle);
    lv_style_set_text_font(&DisplayHandler::bootFailStyle, &lv_font_montserrat_14);
    lv_style_set_text_color(&DisplayHandler::bootFailStyle, lv_color_hex(0xff0000));

    // Set the background color of the active screen to black
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_black(), LV_PART_MAIN);
}


void DisplayHandler::drawMainScreen(String valueV0,
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
                  String valueW3)
{
    unsigned int tickPeriod = millis() - this->lastTickMilis;
    lv_tick_inc(tickPeriod);

    lv_obj_clean(lv_scr_act());

    // Draw black bg
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_black(), LV_PART_MAIN); 
    // Disable scrollbar
    lv_obj_set_scrollbar_mode(lv_screen_active(), LV_SCROLLBAR_MODE_OFF);

    lv_obj_t * rect0 = lv_obj_create(lv_scr_act());
    lv_obj_t * rect1 = lv_obj_create(lv_scr_act());

    lv_obj_set_size(rect0, 480, 90);
    lv_obj_set_pos(rect0, 0, 0);

    lv_obj_set_size(rect1, 480, 75);
    lv_obj_set_pos(rect1, 0, 168);

    lv_obj_add_style(rect0, &DisplayHandler::rectStyle0, 0);
    lv_obj_add_style(rect1, &DisplayHandler::rectStyle0, 0);

    // ------ LABELS
    lv_obj_t * label0 = lv_label_create(lv_screen_active());
    lv_obj_t * label1 = lv_label_create(lv_screen_active());
    lv_obj_t * label2 = lv_label_create(lv_screen_active());
    lv_obj_t * label3 = lv_label_create(lv_screen_active());

    lv_obj_align(label0, LV_ALIGN_OUT_LEFT_MID, 0, 2);
    lv_obj_align(label1, LV_ALIGN_OUT_LEFT_MID, 0, 90);
    lv_obj_align(label2, LV_ALIGN_OUT_LEFT_MID, 0, 165);
    lv_obj_align(label3, LV_ALIGN_OUT_LEFT_MID, 0, 240);

    lv_label_set_text(label0, String("Custom out").c_str());
    lv_label_set_text(label1, String("12 V out").c_str());
    lv_label_set_text(label2, String("5 V out").c_str());
    lv_label_set_text(label3, String("3.3 V out").c_str());

    lv_obj_add_style(label0, &DisplayHandler::labelStyle1, 0);
    lv_obj_add_style(label1, &DisplayHandler::labelStyle1, 0);
    lv_obj_add_style(label2, &DisplayHandler::labelStyle1, 0);
    lv_obj_add_style(label3, &DisplayHandler::labelStyle1, 0);

    // ------ VOLTAGE VALUES
    lv_obj_t * labelVVal0 = lv_label_create(lv_screen_active());
    lv_obj_t * labelVVal1 = lv_label_create(lv_screen_active());
    lv_obj_t * labelVVal2 = lv_label_create(lv_screen_active());
    lv_obj_t * labelVVal3 = lv_label_create(lv_screen_active());

    lv_obj_align(labelVVal0, LV_ALIGN_OUT_LEFT_MID, 0, 20);
    lv_obj_align(labelVVal1, LV_ALIGN_OUT_LEFT_MID, 0, 115);
    lv_obj_align(labelVVal2, LV_ALIGN_OUT_LEFT_MID, 0, 190);
    lv_obj_align(labelVVal3, LV_ALIGN_OUT_LEFT_MID, 0, 265);

    lv_label_set_text(labelVVal0, valueV0.c_str());
    lv_label_set_text(labelVVal1, valueV1.c_str());
    lv_label_set_text(labelVVal2, valueV2.c_str());
    lv_label_set_text(labelVVal3, valueV3.c_str());

    lv_obj_add_style(labelVVal0, &DisplayHandler::valueVStyle0, 0);
    lv_obj_add_style(labelVVal1, &DisplayHandler::valueVStyle1, 0);
    lv_obj_add_style(labelVVal2, &DisplayHandler::valueVStyle1, 0);
    lv_obj_add_style(labelVVal3, &DisplayHandler::valueVStyle1, 0);

    // ------ CURRENT VALUES
    lv_obj_t * labelAVal0 = lv_label_create(lv_screen_active());
    lv_obj_t * labelAVal1 = lv_label_create(lv_screen_active());
    lv_obj_t * labelAVal2 = lv_label_create(lv_screen_active());
    lv_obj_t * labelAVal3 = lv_label_create(lv_screen_active());

    lv_obj_align(labelAVal0, LV_ALIGN_OUT_LEFT_MID, 240, 20); 
    lv_obj_align(labelAVal1, LV_ALIGN_OUT_LEFT_MID, 240, 120); // 5 px offset //
    lv_obj_align(labelAVal2, LV_ALIGN_OUT_LEFT_MID, 240, 195); //             //
    lv_obj_align(labelAVal3, LV_ALIGN_OUT_LEFT_MID, 240, 270); //             //

    lv_label_set_text(labelAVal0, valueA0.c_str());
    lv_label_set_text(labelAVal1, valueA1.c_str());
    lv_label_set_text(labelAVal2, valueA2.c_str());
    lv_label_set_text(labelAVal3, valueA3.c_str());

    lv_obj_add_style(labelAVal0, &DisplayHandler::valueAStyle0, 0);
    lv_obj_add_style(labelAVal1, &DisplayHandler::valueAStyle0, 0);
    lv_obj_add_style(labelAVal2, &DisplayHandler::valueAStyle0, 0);
    lv_obj_add_style(labelAVal3, &DisplayHandler::valueAStyle0, 0);

    // ------ POWER VALUES
    lv_obj_t * labelWVal0 = lv_label_create(lv_screen_active());
    lv_obj_t * labelWVal1 = lv_label_create(lv_screen_active());
    lv_obj_t * labelWVal2 = lv_label_create(lv_screen_active());
    lv_obj_t * labelWVal3 = lv_label_create(lv_screen_active());

    lv_obj_align(labelWVal0, LV_ALIGN_OUT_LEFT_MID, 0, 65);
    lv_obj_align(labelWVal1, LV_ALIGN_OUT_LEFT_MID, 0, 140);
    lv_obj_align(labelWVal2, LV_ALIGN_OUT_LEFT_MID, 0, 215);
    lv_obj_align(labelWVal3, LV_ALIGN_OUT_LEFT_MID, 0, 290);

    lv_label_set_text(labelWVal0, valueW0.c_str());
    lv_label_set_text(labelWVal1, valueW1.c_str());
    lv_label_set_text(labelWVal2, valueW2.c_str());
    lv_label_set_text(labelWVal3, valueW3.c_str());

    lv_obj_add_style(labelWVal0, &DisplayHandler::valueWStyle1, 0);
    lv_obj_add_style(labelWVal1, &DisplayHandler::valueWStyle1, 0);
    lv_obj_add_style(labelWVal2, &DisplayHandler::valueWStyle1, 0);
    lv_obj_add_style(labelWVal3, &DisplayHandler::valueWStyle1, 0);

    // ------ ALERTS
    lv_obj_t * labelAlert = lv_label_create(lv_screen_active());
    lv_obj_add_style(labelAlert, &DisplayHandler::labelStyleAlert, 0);
    lv_obj_align(labelAlert, LV_ALIGN_OUT_RIGHT_MID, 240, 0);
    lv_obj_set_style_text_align(labelAlert, LV_TEXT_ALIGN_LEFT, 0);
    lv_label_set_text(labelAlert, stringFlasherAlert.getString(true, false, true, false).c_str());
    lv_obj_set_width(labelAlert, 240);

    this->lastTickMilis = millis();
    lv_task_handler(); 
}

String DisplayHandler::convertLinesVectorToString(std::vector<String> stringVector) {
    String returnString = "";
    for (const String &str : stringVector) {
        returnString = returnString + str + "\n";
    }
    return returnString;
}

void DisplayHandler::drawBootScreen(bool condition, String textLine, String conditionSuccess, String conditionFail, uint8_t delayAfterDraw) {
    
    this->bootLinesText.push_back(textLine);
    if (condition) {
        this->bootLinesSuccess.push_back(conditionSuccess);
        this->bootLinesFail.push_back("");
    } else {
        this->bootLinesSuccess.push_back("");
        this->bootLinesFail.push_back(conditionFail);
    }

    unsigned int tickPeriod = millis() - this->lastTickMilis;
    lv_tick_inc(tickPeriod);

    lv_obj_clean(lv_scr_act());

    // Draw bootLinesText
    lv_obj_t * bootLabelText = lv_label_create(lv_screen_active());
    lv_obj_align(bootLabelText, LV_ALIGN_TOP_LEFT, 0, 0); 
    lv_obj_add_style(bootLabelText, &DisplayHandler::bootTextStyle, 0);
    lv_label_set_text(bootLabelText, DisplayHandler::convertLinesVectorToString(bootLinesText).c_str());

    // Draw bootLinesSuccess
    lv_obj_t * bootLabelSuccess = lv_label_create(lv_screen_active());
    lv_obj_align(bootLabelSuccess, LV_ALIGN_TOP_LEFT, 250, 0); 
    lv_obj_add_style(bootLabelSuccess, &DisplayHandler::bootSuccessStyle, 0);
    lv_label_set_text(bootLabelSuccess, DisplayHandler::convertLinesVectorToString(bootLinesSuccess).c_str());

    // Draw bootLinesSuccess
    lv_obj_t * bootLabelFail = lv_label_create(lv_screen_active());
    lv_obj_align(bootLabelFail, LV_ALIGN_TOP_LEFT, 250, 0); 
    lv_obj_add_style(bootLabelFail, &DisplayHandler::bootFailStyle, 0);
    lv_label_set_text(bootLabelFail, DisplayHandler::convertLinesVectorToString(bootLinesFail).c_str());

    this->lastTickMilis = millis();
    lv_task_handler(); 
    delay(delayAfterDraw);
}

void DisplayHandler::drawStandbyScreen() {
    
    unsigned int tickPeriod = millis() - this->lastTickMilis;
    lv_tick_inc(tickPeriod);

    lv_obj_clean(lv_scr_act());

    // Draw bootLinesText
    lv_obj_t * standbyLabel = lv_label_create(lv_screen_active());
    lv_obj_align(standbyLabel, LV_ALIGN_CENTER, 0, 0); 
    lv_obj_add_style(standbyLabel, &DisplayHandler::labelStyle1, 0);
    lv_label_set_text(standbyLabel, String("Standby ...").c_str());

    this->lastTickMilis = millis();
    lv_task_handler(); 
}

void DisplayHandler::setBacklightPWM(int value) {
    ledcWrite(PWM_CHANNEL, value);
}