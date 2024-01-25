/*
 2024/01/02 by ideasHatch
 */

#include <inttypes.h>
#include <Arduino.h>
#include "CameraLED.h"

#ifdef __cplusplus
extern "C" {

#include "pwmout_api.h"

extern void *gpio_pin_struct[];

}
#endif

CameraLED::CameraLED() {
    cameraledPin = 0xFFFFFFFF;
}

uint8_t CameraLED::attach(int pin) {
    return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t CameraLED::attach(int pin, int min, int max) {
    amb_ard_pin_check_fun(pin, PIO_PWM);

    pinRemoveMode(pin);
    analogWrite(pin, 0);
    pwmout_period_us(((pwmout_t *)gpio_pin_struct[pin]), 1000);

    cameraledPin = pin;
    this->min = min;
    this->max = max;

    return 0;
}

void CameraLED::detach() {
    pinRemoveMode(cameraledPin);
    cameraledPin = 0xFFFFFFFF;
}

void CameraLED::writeMicroseconds(int value) {
    if (value < (int)min) value = min;
    if (value > (int)max) value = max;

    currentWidth = value;
    pwmout_write(((pwmout_t *)gpio_pin_struct[cameraledPin]), (value * 1.0 / 20000));
}

int CameraLED::readMicroseconds() {
    return currentWidth;
}

bool CameraLED::attached() {
    return (cameraledPin != 0xFFFFFFFF);
}
