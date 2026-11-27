#include "power_mgmt.h"
#include <esp_sleep.h>

PowerManager::PowerManager() : m_voltageDividerFactor(2.0f) {}

void PowerManager::init() {
    pinMode(PIN_HAPTIC, OUTPUT);
    digitalWrite(PIN_HAPTIC, LOW);

    pinMode(PIN_OPTICAL_LEDS, OUTPUT);
    digitalWrite(PIN_OPTICAL_LEDS, LOW);

    pinMode(PIN_BUTTON_MODE, INPUT_PULLUP);
    analogReadResolution(12);
}

float PowerManager::readBatteryVoltage() {
    // 12-bit ADC reading across 100k/100k divider (factor = 2.0)
    int raw = analogRead(PIN_BATTERY_ADC);
    float pinVolts = (raw / 4095.0f) * 3.3f;
    return pinVolts * m_voltageDividerFactor;
}

void PowerManager::triggerHapticAlert(uint16_t pulseDurationMs, uint8_t pulseCount) {
    for (uint8_t i = 0; i < pulseCount; i++) {
        digitalWrite(PIN_HAPTIC, HIGH);
        delay(pulseDurationMs);
        digitalWrite(PIN_HAPTIC, LOW);
        if (i + 1 < pulseCount) {
            delay(100);
        }
    }
}

void PowerManager::setOpticalLeds(bool active) {
    digitalWrite(PIN_OPTICAL_LEDS, active ? HIGH : LOW);
}

void PowerManager::pulseOpticalLeds(uint16_t flashHz) {
    uint32_t periodMs = 1000 / flashHz;
    uint32_t halfPeriod = periodMs / 2;
    digitalWrite(PIN_OPTICAL_LEDS, HIGH);
    delay(halfPeriod);
    digitalWrite(PIN_OPTICAL_LEDS, LOW);
    delay(halfPeriod);
}

void PowerManager::enterDeepSleep(uint32_t sleepSeconds) {
    esp_sleep_enable_timer_wakeup((uint64_t)sleepSeconds * 1000000ULL);
    esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_BUTTON_MODE, 0); // Wake on button press
    esp_deep_sleep_start();
}
