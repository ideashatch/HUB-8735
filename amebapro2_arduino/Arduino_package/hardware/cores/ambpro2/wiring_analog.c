/*
 Copyright (c) 2011 Arduino.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "analogin_api.h"
#include "analogout_api.h"
#include "pwmout_api.h"
#include "gpio_ex_api.h"

/* ADC */
// analogin_t   adc0;
// analogin_t   adc1;
// analogin_t   adc2;
// analogin_t   adc3;
// analogin_t   adc4;
// analogin_t   adc5;
// analogin_t   adc6;
// analogin_t   adc7;

// static const float ADC_slope1 = (3.3)/(255.0 - 16.0);
// static const float ADC_slope2 = (3.3 - 3.12)/(3454.0-3410.0);

#ifdef FEATURE_DAC
dac_t dac0;

bool g_dac_enabled[] = {
    false,
};
#endif

static int _readResolution = 10;
extern void *gpio_pin_struct[];
static int _writeResolution = 8;
static int _writePeriod = 1000;
static float _offset = 0;
static float _gain = 0;
static int _calibrate_en = 0;

void analogReadResolution(int res)
{
    if (res > 12) {
        printf("\r\n[INFO] Analog read has a maximum resolution of 12 bits. \n");
        _readResolution = 12;
    } else {
        _readResolution = res;
    }
}

void analogWriteResolution(int res)
{
    _writeResolution = res;
}

void analogWritePeriod(int us)
{
    _writePeriod = us;
}

static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to)
{
    if (from == to) {
        return value;
    }
    if (from > to) {
        return value >> (from - to);
    } else {
        return value << (to - from);
    }
}

eAnalogReference analog_reference = AR_DEFAULT;

void analogReference(eAnalogReference ulMode)
{
    analog_reference = ulMode;
}

void analogSet(float gain, float offset)
{
    _offset = offset;
    _gain = gain;
    _calibrate_en = 1;
}

uint32_t analogRead(uint32_t ulPin)
{
    void *pAdc_t;
    analogin_t *adc_obj;

    float ret = 0;
    // float    voltage;
    float mv;

    amb_ard_pin_check_type(ulPin, TYPE_ANALOG);
    amb_ard_pin_check_fun(ulPin, PIO_ADC);

    if (_calibrate_en == 0) {
        _offset = 0x83B;    // copy from AmbD
        _gain = 0x2E25;
    }

#if 0
    if ((_offset == 0) || (_gain == 0)) {
        u8 EfuseBuf[2];
        u32 index;
        u32 addressOffset = 0x1D0;
        u32 addressGain = 0x1D2;

        // Read pre-calibrated values from EFUSE
        for (index = 0; index < 2; index++) {
            EFUSE_PMAP_READ8(0, (addressOffset + index), (EfuseBuf + index), L25EOUTVOLTAGE);
        }
        _offset = EfuseBuf[1]<<8|EfuseBuf[0];

        for (index = 0; index < 2; index++) {
            EFUSE_PMAP_READ8(0, (addressGain + index), (EfuseBuf + index), L25EOUTVOLTAGE);
        }
        _gain = EfuseBuf[1] << 8 | EfuseBuf[0];

        // Use default values if invalid values obtained from EFUSE
        if (_offset == 0xFFFF) {
            _offset = 0x9B0;
        }
        if (_gain == 0xFFFF) {
            _gain = 0x2F12;
        }
    }
#endif

    if ((g_APinDescription[ulPin].ulPinMode & ADC_MODE_ENABLED) != ADC_MODE_ENABLED) {
        pinRemoveMode(ulPin);
        gpio_pin_struct[ulPin] = malloc(sizeof(analogin_t));
        pAdc_t = gpio_pin_struct[ulPin];

        switch (g_APinDescription[ulPin].pinname) {
            case PF_0:
                analogin_init((analogin_t *)pAdc_t, PF_0);
                break;
            case PF_1:
                analogin_init((analogin_t *)pAdc_t, PF_1);
                break;
            case PF_2:
                analogin_init((analogin_t *)pAdc_t, PF_2);
                break;
            case PF_3:
                analogin_init((analogin_t *)pAdc_t, PF_3);
                break;
            case PA_0:
                analogin_init((analogin_t *)pAdc_t, PA_0);
                break;
            case PA_1:
                analogin_init((analogin_t *)pAdc_t, PA_1);
                break;
            case PA_2:
                analogin_init((analogin_t *)pAdc_t, PA_2);
                break;
            case PA_3:
                analogin_init((analogin_t *)pAdc_t, PA_3);
                break;
            default:
                printf("\r\n[ERROR] %s : ulPin %d wrong\n", __FUNCTION__, ((int)ulPin));
                return 0;
        }
        g_APinDescription[ulPin].ulPinMode |= ADC_MODE_ENABLED;
        g_APinDescription[ulPin].ulPinMode &= (~MODE_NOT_INITIAL);
    }
    adc_obj = (analogin_t *)gpio_pin_struct[ulPin];
    ret = analogin_read_u16(adc_obj);

    if (ret < 0xfa) {
        mv = 0;    // Ignore persistent low voltage measurement error
    } else {
        mv = ((10 * ret - _offset) * 1000 / _gain);    // Convert measured ADC value to millivolts
    }
    ret = (mv / 3300.0) * (1 << _readResolution);    // Return user required resolution
    return ret;
}

void analogOutputInit(void)
{
    // nop
}

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.
void analogWrite(uint32_t ulPin, int32_t ulValue)
{
    // amb_ard_pin_check_type(ulPin, TYPE_ANALOG);

#ifdef FEATURE_DAC
    if (ulPin == DAC0) {
        if (g_dac_enabled[0] == false) {
            analogout_init(&dac0, DA_0);
            g_dac_enabled[0] = true;
        }
        ulValue %= (1 << _writeResolution);
        analogout_write(&dac0, ulValue * 1.0 / (1 << _writeResolution));
    } else
#endif    // #ifdef FEATURE_DAC
    {
        amb_ard_pin_check_fun(ulPin, PIO_PWM);
        if ((g_APinDescription[ulPin].ulPinMode & PWM_MODE_ENABLED) != PWM_MODE_ENABLED) {
            pinRemoveMode(ulPin);
            gpio_pin_struct[ulPin] = malloc(sizeof(pwmout_t));
            pwmout_t *obj = (pwmout_t *)gpio_pin_struct[ulPin];
            pwmout_init(obj, g_APinDescription[ulPin].pinname);
            pwmout_period_us(obj, _writePeriod);
            pwmout_write(obj, ulValue * 1.0 / (1 << _writeResolution));
            g_APinDescription[ulPin].ulPinMode |= PWM_MODE_ENABLED;
            g_APinDescription[ulPin].ulPinMode &= (~MODE_NOT_INITIAL);
        } else {
            pwmout_t *obj = (pwmout_t *)gpio_pin_struct[ulPin];
            pwmout_period_us(obj, _writePeriod);
            pwmout_write(obj, ulValue * 1.0 / (1 << _writeResolution));
        }
    }
}

typedef struct _tone_argument {
    uint32_t ulPin;
    uint32_t timer_id;
} tone_argument;

void _tone_timer_handler(const void *argument)
{
    // passed in value is a double pointer to a tone_argument struct
    tone_argument **pptimer = (tone_argument **)argument;
    tone_argument *arg = *pptimer;
    // Stop the currently playing tone and delete the timer
    noTone(arg->ulPin);
    os_timer_delete_arduino(arg->timer_id);
    // Set to NULL, and free the previously assigned memory
    *pptimer = NULL;
    free(arg);
}

void _tone(uint32_t ulPin, unsigned int frequency, unsigned long duration)
{
    static tone_argument *ptimer = NULL;

    // amb_ard_pin_check_type(ulPin, TYPE_ANALOG);
    amb_ard_pin_check_fun(ulPin, PIO_PWM);

    if ((g_APinDescription[ulPin].ulPinMode & PWM_MODE_ENABLED) != PWM_MODE_ENABLED) {
        pinRemoveMode(ulPin);
        gpio_pin_struct[ulPin] = malloc(sizeof(pwmout_t));
        pwmout_t *obj = (pwmout_t *)gpio_pin_struct[ulPin];
        pwmout_init(obj, g_APinDescription[ulPin].pinname);
        if (frequency == 0) {
            pwmout_pulsewidth(obj, 0);
        } else {
            pwmout_period(obj, 1.0 / frequency);
            pwmout_pulsewidth(obj, 1.0 / (frequency * 2));
        }
        g_APinDescription[ulPin].ulPinMode |= PWM_MODE_ENABLED;
        g_APinDescription[ulPin].ulPinMode &= (~MODE_NOT_INITIAL);
    } else {
        // There is already a PWM configured
        pwmout_t *obj = (pwmout_t *)gpio_pin_struct[ulPin];
        if (frequency == 0) {
            pwmout_pulsewidth(obj, 0);
        } else {
            pwmout_period(obj, 1.0 / frequency);
            pwmout_pulsewidth(obj, 1.0 / (frequency * 2));
        }
    }

    if (duration > 0) {
        // If a tone timer already exists, assume new tone command to refresh timer, stop and delete old timer
        if (ptimer != NULL) {
            os_timer_delete_arduino(ptimer->timer_id);
            free(ptimer);
            ptimer = NULL;
        }
        tone_argument *arg = (tone_argument *)(malloc(sizeof(tone_argument)));
        arg->ulPin = ulPin;
        arg->timer_id = os_timer_create_arduino(_tone_timer_handler, 0, &ptimer);
        ptimer = arg;
        os_timer_start_arduino(arg->timer_id, duration);
    }
    delay(5);
}

void noTone(uint32_t ulPin)
{
    pinRemoveMode(ulPin);
}

#ifdef __cplusplus
}
#endif
