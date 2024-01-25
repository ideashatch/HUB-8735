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

#define HUB8735_ULTRA

#define MAIN_THREAD_STACK_SIZE                  (4096 * 4)

#define TOTAL_GPIO_PIN_NUM                      (31)
#define TOTAL_PWM_PIN_NUM                       (10)

/* Digital pin mapping refer to g_APinDescription */
#define AMB_D0                                  0  // PF_2 G
#define AMB_D1                                  1  // PF_1 B
#define AMB_D2                                  2  // PF_0 G
#define AMB_D3                                  3  // PE_4 B
#define AMB_D4                                  4  // PE_3 G
#define AMB_D5                                  5  // PE_2 B
#define AMB_D6                                  6  // PE_1 G 
#define AMB_D7                                  7  // PD_17 B
#define AMB_D8                                  8  // PD_15 G

#define AMB_D9                                  9  // PA_2 B                
#define AMB_D10                                 10 // PA_3 G                
#define AMB_D11                                 11 // PF_14 B               
#define AMB_D12                                 12 // PF_15 G	//Button
#define AMB_D13                                 13 // PF_10 B               
#define AMB_D14                                 14 // PA_5	//Sensor power on
#define AMB_D15                                 15 // PD_16 B
#define AMB_D16                                 16 // PF_3 G	//LOG_RX

#define AMB_D17                                 17 // PF_4 B	//LOG_TX
#define AMB_D18                                 18 // PF_5 G
#define AMB_D19                                 19 // PF_6 B
#define AMB_D20                                 20 // PF_7 G
#define AMB_D21                                 21 // PF_8 B
#define AMB_D22                                 22 // PF_11 G
#define AMB_D23                                 23 // PF_12 B
#define AMB_D24                                 24 // PF_13 G

#define AMB_D25                                 25 // PE_6_LEDA green
#define AMB_D26                                 26 // PF_9 LEDB blue
#define AMB_D27                                 27 // PA_0 TP
#define AMB_D28                                 28 // PA_1 TP
#define AMB_D29                                 29 // PE_5 TP
 

/* Analog pin mapping */
#define A0                                      AMB_D2 // PF_0    3.3V    CH0
#define A1                                      AMB_D1 // PF_1    3.3V    CH1
#define A2                                      AMB_D0  // PF_2    3.3V    CH2
//#define A3                                      AMB_D27 // PF_3    3.3V    CH3    LOG_RX not recomanded to be used as ADC
#define A4                                      AMB_D27 // PA_0    3.3V    CH4
#define A5                                      AMB_D28 // PA_1    3.3V    CH5
#define A6                                      AMB_D9 // PA_2    3.3V    CH6
#define A7                                      AMB_D10 // PA_3    3.3V    CH7

/* LED pin mapping */
//HUB8735 no LED
#define LED_B                                   AMB_D26 //  Blue
#define LED_G                                   AMB_D25 //  Green
#define LED_BUILTIN                             LED_B   //  Blue
#define LED_BUILTIN2                            LED_G   //  Blue

/* Push button mapping */
//HUB 8735 need jump to button
#define PUSH_BTN                                AMB_D12 // PF_15

/* ADC/DAC pin mapping */
#define FEATURE_ADC
//#define FEATURE_DAC

/* SPI pin mapping */ 
// Master and Slave
#define SPI_MOSI                                AMB_D4 // SPI_0_MOSI PE_3  
#define SPI_MISO                                AMB_D5 // SPI_0_MISO PE_2  
#define SPI_SCLK                                AMB_D6 // SPI_0_SCL  PE_1  
#define SPI_SS                                  AMB_D3 // SPI_0_CS0  PE_4  
#define SPI1_MOSI                               AMB_D20  // SPI_1_MOSI  PF_7 
#define SPI1_MISO                               AMB_D18  // SPI_1_MISO PF_5 
#define SPI1_SCLK                               AMB_D19  // SPI_1_SCL  PF_6  
#define SPI1_SS                                 AMB_D21  // SPI_1_CS0  PF_8

/* TwoWire/I2C pin mapping */
#define I2C_SDA                                 AMB_D3//AMB_D28 // I2C0_SDA     PE_4
#define I2C_SCL                                 AMB_D4//AMB_D27 // I2C0_SCL     PE_3
#define I2C1_SDA                                AMB_D0  // I2C1_SDA    PF_2
#define I2C1_SCL                                AMB_D1 // I2C1_SCL     PF_1
#define I2C2_SDA                                AMB_D28//AMB_D3 // I2C2_SDA      PA_1
#define I2C2_SCL                                AMB_D27//AMB_D4 // I2C2_SCL      PA_0

/* IR Device pin mapping */
//#define IR_TX
//#define IR_RX

/* UART pin mapping */
#define LOG_UART_MODIFIABLE_BAUD_RATE           1
#define SERIAL_ONE_UART_MODIFIABLE_BAUD_RATE    1
#define SERIAL_TWO_UART_MODIFIABLE_BAUD_RATE    1
#define SERIAL_TRI_UART_MODIFIABLE_BAUD_RATE    1
#define LOG_TX                                  AMB_D17 // UART1_OUT    PF_4
#define LOG_RX                                  AMB_D16 // UART1_IN     PF_3
//#define LOG_CTS                                 AMB_D9  // UART1_CTS AMB_D9
//#define LOG_RTS                                 AMB_D10 // UART1_RTS AMB_D10
#define SERIAL1_TX                              AMB_D9 // UART0_OUT    PA_2
#define SERIAL1_RX                              AMB_D10 // UART0_IN     PA_3
//#define SERIAL1_CTS
//#define SERIAL1_RTS
#define SERIAL2_TX                              AMB_D8 // UART2_OUT    PD_15
#define SERIAL2_RX                              AMB_D15 // UART2_IN     PD_16
//#define SERIAL2_CTS                             AMB_D17 // UART2_CTS    AMB_D17
//#define SERIAL2_RTS                             AMB_D16 // UART2_RTS    AMB_D16
#define SERIAL3_TX                              AMB_D6  // UART3_OUT   PE_1
#define SERIAL3_RX                              AMB_D5  // UART3_IN    PE_2


/* SWD pin mapping */
#define SWD_DATA                                AMB_D27 // PA_0
#define SWD_CLK                                 AMB_D28 // PA_1

#endif /* _VARIANT_ARDUINO_AMEBA_ */
