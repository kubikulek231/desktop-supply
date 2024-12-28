#include "AlertManager.h"

AlertManager::AlertManager(
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
    ) :
        stringFlasherAlert(true), // Initialize StringFlasherAlert with a parameter
        voltageAlert2(voltageAlert2),
        voltageAlert1(voltageAlert1),
        voltageAlert0(voltageAlert0),
        currentAlert2(currentAlert2),
        currentAlert1(currentAlert1),
        currentAlert0(currentAlert0),
        powerAlert2(powerAlert2),
        powerAlert1(powerAlert1),
        powerAlert0(powerAlert0),
        tempAlert2(tempAlert2),
        tempAlert1(tempAlert1),
        tempAlert0(tempAlert0),
        patienceCritical(patienceCritical),
        patienceAboveCritical(patienceAboveCritical),
        triggered(false),
        triggerTime(0)
    {}

int8_t AlertManager::check(float voltage, float current, float power, float temp)
{
    // Approaching critical values
    int approachingScore = 0;
    if (voltage > voltageAlert2) {approachingScore += 1;}
    if (current > currentAlert2) {approachingScore += 1;}
    if (power > powerAlert2) {approachingScore += 1;}
    if (temp > tempAlert2) {approachingScore += 1;}

    // Critical values
    int criticalScore = 0;
    if (voltage > voltageAlert1) {criticalScore += 1;}
    if (current > currentAlert1) {criticalScore += 1;}
    if (power > powerAlert1) {criticalScore += 1;}
    if (temp > tempAlert1) {criticalScore += 1;}

    // Above critical values
    int aboveCriticalScore = 0;
    if (voltage > voltageAlert0) {aboveCriticalScore += 1;}
    if (current > currentAlert0) {aboveCriticalScore += 1;}
    if (power > powerAlert0) {aboveCriticalScore += 1;}
    if (temp > tempAlert0) {aboveCriticalScore += 1;}

    // If above critical of if more than 2 critical values are reached, trigger patience countdown
    return 0;
}