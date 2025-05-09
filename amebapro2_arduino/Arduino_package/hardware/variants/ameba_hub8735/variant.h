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

#ifndef _VARIANT_ARDUINO_AMEBA_
#define _VARIANT_ARDUINO_AMEBA_

#include "Arduino.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C"{
#include "PinNames.h"

// zzw
//#define portOutputRegister(P) ((volatile uint32_t *)(0x48014000 + (P) * 0x400))
//#define portInputRegister(P)  ((volatile uint32_t *)(0x48014050 + (P) * 0x400))
//#define portModeRegister(P)   ((volatile uint32_t *)(0x48014004 + (P) * 0x400))

/*
 * Wait until enter debug mode
 *
 * Check DHCSR(0xE000EDF0) register and hold until bit C_DEBUGEN is set.
 * Use this function along with J-LINK or other debug tool
 **/
extern void wait_for_debug(void);
}
#endif

#define MAIN_THREAD_STACK_SIZE                  (4096 * 4)

#define TOTAL_GPIO_PIN_NUM                      (31)
#define TOTAL_PWM_PIN_NUM                       (10)

/* Digital pin mapping refer to g_APinDescription */
#define AMB_D0                                  0  // NULL
#define AMB_D1                                  1  // SPK
#define AMB_D2                                  2  // PA_1
#define AMB_D3                                  3  // PA_0
#define AMB_D4                                  4  // PA_3
#define AMB_D5                                  5  // PA_2
#define AMB_D6                                  6  // PF_0
#define AMB_D7                                  7  // PF_8
#define AMB_D8                                  8  // PF_7
#define AMB_D9                                  9  // PF_6
#define AMB_D10                                 10 // PF_5
#define AMB_D11                                 11 // PE_2
#define AMB_D12                                 12 // PE_1
#define AMB_D13                                 13 // GND
#define AMB_D14                                 14 // 5V
#define AMB_D15                                 15 // 3.3V
#define AMB_D16                                 16 // PA_5
#define AMB_D17                                 17 // PF_10
#define AMB_D18                                 18 // GND
#define AMB_D19                                 19 // VCC
#define AMB_D20                                 20 // PF_4
#define AMB_D21                                 21 // PF_3
#define AMB_D22                                 22 // GND

#define AMB_D23                                 23 // GND
#define AMB_D24                                 24 // DP
#define AMB_D25                                 25 // DN
#define AMB_D26                                 26 // 5V
#define AMB_D27                                 27 // PF_1
#define AMB_D28                                 28 // PF_2
#define AMB_D29                                 29 // PE_6
#define AMB_D30                                 30 // PF_9
//#define AMB_D29                                 29 // PF_10

/* Analog pin mapping */
#define A0                                      AMB_D6 // PF_0    3.3V    CH0
#define A1                                      AMB_D27 // PF_1    3.3V    CH1
#define A2                                      AMB_D28  // PF_2    3.3V    CH2
//#define A3                                      AMB_D26 // PF_3    3.3V    CH3    LOG_RX not recomanded to be used as ADC
#define A4                                      AMB_D3 // PA_0    3.3V    CH4
#define A5                                      AMB_D2 // PA_1    3.3V    CH5
#define A6                                      AMB_D5 // PA_2    3.3V    CH6
#define A7                                      AMB_D4 // PA_3    3.3V    CH7

/* LED pin mapping */
//HUB8735 no LED
#define LED_B                                   AMB_D30 //  Blue
#define LED_G                                   AMB_D29 //  Green
#define LED_BUILTIN                             LED_B   //  Blue

/* Push button mapping */
//HUB 8735 need jump to button
#define PUSH_BTN                                AMB_D17 //

/* ADC/DAC pin mapping */
#define FEATURE_ADC
//#define FEATURE_DAC

/* SPI pin mapping */ 
// Master and Slave
#define SPI_MOSI                                AMB_D0 // SPI_0_MOSI   AMB_D13
#define SPI_MISO                                AMB_D0 // SPI_0_MISO   AMB_D14
#define SPI_SCLK                                AMB_D0 // SPI_0_SCL    AMB_D15
#define SPI_SS                                  AMB_D0 // SPI_0_CS0    AMB_D12
#define SPI1_MOSI                               AMB_D8  // SPI_1_MOSI   AMB_D2
#define SPI1_MISO                               AMB_D10  // SPI_1_MISO   AMB_D0
#define SPI1_SCLK                               AMB_D9  // SPI_1_SCL    AMB_D1
#define SPI1_SS                                 AMB_D7  // SPI_1_CS0v   AMB_D3

/* TwoWire/I2C pin mapping */
#define I2C_SDA                                 AMB_D2 // I2C0_SDA     AMB_D2
#define I2C_SCL                                 AMB_D3 // I2C0_SCL     AMB_D3
#define I2C1_SDA                                AMB_D28  // I2C1_SDA     AMB_D28
#define I2C1_SCL                                AMB_D27 // I2C1_SCL     AMB_D127
#define I2C2_SDA                                AMB_D2 // I2C2_SDA     AMB_D12
#define I2C2_SCL                                AMB_D3 // I2C2_SCL     AMB_D13

/* IR Device pin mapping */
//#define IR_TX
//#define IR_RX

/* UART pin mapping */
#define LOG_UART_MODIFIABLE_BAUD_RATE           1
#define SERIAL_ONE_UART_MODIFIABLE_BAUD_RATE    1
#define SERIAL_TWO_UART_MODIFIABLE_BAUD_RATE    1
#define SERIAL_TRI_UART_MODIFIABLE_BAUD_RATE    1
#define LOG_TX                                  AMB_D20 // UART1_OUT    AMB_D25 / AMB_D6
#define LOG_RX                                  AMB_D21 // UART1_IN     AMB_D26 / AMB_D5
//#define LOG_CTS                                 AMB_D9  // UART1_CTS AMB_D9
//#define LOG_RTS                                 AMB_D10 // UART1_RTS AMB_D10
#define SERIAL1_TX                              AMB_D5 // UART0_OUT    AMB_D21
#define SERIAL1_RX                              AMB_D4 // UART0_IN     AMB_D22
//#define SERIAL1_CTS
//#define SERIAL1_RTS
#define SERIAL2_TX                              AMB_D0 // UART2_OUT    AMB_D19
#define SERIAL2_RX                              AMB_D0 // UART2_IN     AMB_D18
//#define SERIAL2_CTS                             AMB_D17 // UART2_CTS    AMB_D17
//#define SERIAL2_RTS                             AMB_D16 // UART2_RTS    AMB_D16
#define SERIAL3_TX                              AMB_D12  // UART3_OUT   AMB_D15
#define SERIAL3_RX                              AMB_D11  // UART3_IN    AMB_D14
//#define SERIAL3_CTS                             AMB_D12 // UART3_CTS    AMB_D12
//#define SERIAL3_RTS                             AMB_D13 // UART3_RTS    AMB_D13

/* SWD pin mapping */
#define SWD_DATA                                AMB_D3 // PA0
#define SWD_CLK                                 AMB_D2 // PA1

#endif /* _VARIANT_ARDUINO_AMEBA_ */
