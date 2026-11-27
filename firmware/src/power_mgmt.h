#ifndef POWER_MGMT_H
#define POWER_MGMT_H

#include <Arduino.h>

#define PIN_BATTERY_ADC 1  // ADC1 Channel for battery voltage divider
#define PIN_HAPTIC 4       // ERM Motor Gate
#define PIN_OPTICAL_LEDS 7 // Pulsed 650nm LED Array Gate
#define PIN_BUTTON_MODE 0  // Boot/Mode switch button (active LOW)

class PowerManager {
public:
    PowerManager();
    void init();
    float readBatteryVoltage();
    void triggerHapticAlert(uint16_t pulseDurationMs, uint8_t pulseCount = 1);
    void setOpticalLeds(bool active);
    void pulseOpticalLeds(uint16_t flashHz = 4);
    void enterDeepSleep(uint32_t sleepSeconds);

private:
    float m_voltageDividerFactor;
};

#endif // POWER_MGMT_H
