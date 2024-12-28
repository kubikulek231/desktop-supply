#include <Arduino.h>
#include "StringFlasherAlert.h"

#ifndef ALERTMANAGER_H
#define ALERTMANAGER_H


class AlertManager{
public:
    AlertManager(
                float voltageAlert2,
                float voltageAlert1,
                float voltageAlert0,
                float currentAlert2,
                float currentAlert1,
                float currentAlert0,
                float powerAlert2,
                float powerAlert1,
                float powerAlert0,
                float tempAlert2,
                float tempAlert1,
                float tempAlert0,
                int patienceCritical,
                int patienceAboveCritical
                );
    int8_t check(
                float voltage,
                float current,
                float power,
                float temp
                );
    StringFlasherAlert& getStringFlasher();
private:
    StringFlasherAlert stringFlasherAlert;
    float voltageAlert2;
    float voltageAlert1;
    float voltageAlert0;
    float currentAlert2;
    float currentAlert1;
    float currentAlert0;
    float powerAlert2;
    float powerAlert1;
    float powerAlert0;
    float tempAlert2;
    float tempAlert1;
    float tempAlert0;
    int patienceCritical;
    int patienceAboveCritical;
    bool triggered;
    unsigned int triggerTime;
};
#endif