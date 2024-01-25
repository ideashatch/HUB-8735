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

/* AMB82 (RTL8735B) */

#include "variant.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "us_ticker_api.h"
#include "wiring_digital.h"
__weak void _init(void) { }
void __libc_init_array(void);

/*
 * Pins descriptions
 */
PinDescription g_APinDescription[TOTAL_GPIO_PIN_NUM]=
{
    {NC,  	TYPE_DIGITAL,               NOT_INITIAL                    				, MODE_NOT_INITIAL}, // AMB_D0     
    {NC,  	TYPE_DIGITAL,               NOT_INITIAL          			 			, MODE_NOT_INITIAL}, // AMB_D1     
    {PA_1,  TYPE_DIGITAL | TYPE_ANALOG, PIO_GPIO | PIO_GPIO_IRQ 		  | PIO_ADC	, MODE_NOT_INITIAL}, // AMB_D2    	SWD_CLK         *I2C2_SDA 
    {PA_0,  TYPE_DIGITAL | TYPE_ANALOG, PIO_GPIO | PIO_GPIO_IRQ 		  | PIO_ADC	, MODE_NOT_INITIAL}, // AMB_D3    	SWD_DATA        *I2C2_CL 	
    {PA_3, 	TYPE_DIGITAL | TYPE_ANALOG, PIO_GPIO | PIO_GPIO_IRQ 		  | PIO_ADC	, MODE_NOT_INITIAL}, // AMB_D4		SERIAL1_RX
    {PA_2, 	TYPE_DIGITAL | TYPE_ANALOG, PIO_GPIO | PIO_GPIO_IRQ 		  | PIO_ADC	, MODE_NOT_INITIAL}, // AMB_D5     	SERIAL1_TX
    {PF_0, 	TYPE_DIGITAL | TYPE_ANALOG, PIO_GPIO | PIO_GPIO_IRQ 		  | PIO_ADC	, MODE_NOT_INITIAL}, // AMB_D6     	
    {PF_8, 	TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM          	, MODE_NOT_INITIAL}, // AMB_D7		SPI1_SS
    {PF_7, 	TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM          	, MODE_NOT_INITIAL}, // AMB_D8		SPI1_MOSI	
    {PF_6,  TYPE_DIGITAL, 				PIO_GPIO | PIO_GPIO_IRQ | PIO_PWM          	, MODE_NOT_INITIAL}, // AMB_D9     	SPI1_CLK
    {PF_5,  TYPE_DIGITAL, 				PIO_GPIO | PIO_GPIO_IRQ           			, MODE_NOT_INITIAL}, // AMB_D10    	SPI1_MISO
    {PE_2,  TYPE_DIGITAL, 				PIO_GPIO | PIO_GPIO_IRQ           			, MODE_NOT_INITIAL}, // AMB_D11		SERIAL3_RX
    {PE_1,  TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                    	, MODE_NOT_INITIAL}, // AMB_D12  	SERIAL3_TX  
    {NC,  	TYPE_DIGITAL,               NOT_INITIAL                    				, MODE_NOT_INITIAL}, // AMB_D13    
    {NC,  	TYPE_DIGITAL,               NOT_INITIAL                    				, MODE_NOT_INITIAL}, // AMB_D14    
    {NC,  	TYPE_DIGITAL,               NOT_INITIAL                    				, MODE_NOT_INITIAL}, // AMB_D15    
    {PA_5, 	TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                    	, MODE_NOT_INITIAL}, // AMB_D16    
    {PF_10, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ           | PIO_ADC	, MODE_NOT_INITIAL}, // AMB_D17   	test Button pin  
    {NC, 	TYPE_DIGITAL,               NOT_INITIAL                    				, MODE_NOT_INITIAL}, // AMB_D18    
    {NC, 	TYPE_DIGITAL,               NOT_INITIAL                    				, MODE_NOT_INITIAL}, // AMB_D19    
    {PF_4, 	TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                    	, MODE_NOT_INITIAL}, // AMB_D20		LOG_TX	
    {PF_3,	TYPE_DIGITAL, 				PIO_GPIO | PIO_GPIO_IRQ           | PIO_ADC	, MODE_NOT_INITIAL}, // AMB_D21    	LOG_RX
    {NC,  	TYPE_DIGITAL,				NOT_INITIAL           						, MODE_NOT_INITIAL}, // AMB_D22    
    {NC,  	TYPE_DIGITAL,               NOT_INITIAL          						, MODE_NOT_INITIAL}, // *AMB_D23   
    {NC,  	TYPE_DIGITAL,               NOT_INITIAL                   				, MODE_NOT_INITIAL}, // *AMB_D24   
    {NC,  	TYPE_DIGITAL,               NOT_INITIAL                    				, MODE_NOT_INITIAL}, // *AMB_D25   
    {NC,  	TYPE_DIGITAL,               NOT_INITIAL                    				, MODE_NOT_INITIAL}, // *AMB_D26   
    {PF_1,  TYPE_DIGITAL | TYPE_ANALOG, PIO_GPIO | PIO_GPIO_IRQ 		  | PIO_ADC	, MODE_NOT_INITIAL}, // *AMB_D27   	*LOG_RTS        I2C1_SCL
    {PF_2,  TYPE_DIGITAL | TYPE_ANALOG, PIO_GPIO | PIO_GPIO_IRQ 		  | PIO_ADC	, MODE_NOT_INITIAL}, // *AMB_D28   	*LOG_CTS        I2C1_SDA
    {PE_6,  TYPE_DIGITAL | TYPE_ANALOG, PIO_GPIO | PIO_GPIO_IRQ 		  | PIO_ADC	, MODE_NOT_INITIAL}, // *AMB_D29   	*LOG_CTS        I2C1_SDA
    {PF_9,  TYPE_DIGITAL | TYPE_ANALOG, PIO_GPIO | PIO_GPIO_IRQ 		  | PIO_ADC	, MODE_NOT_INITIAL}, // *AMB_D30   	*LOG_CTS        I2C1_SDA
    //{PF_10, TYPE_DIGITAL,               PIO_GPIO | PIO_GPIO_IRQ                    , MODE_NOT_INITIAL}  // *AMB_D29  
};

void *gpio_pin_struct[TOTAL_GPIO_PIN_NUM] = {NULL};
void *gpio_irq_handler_list[TOTAL_GPIO_PIN_NUM] = {NULL};

#ifdef __cplusplus
} // extern C
#endif

void serialEvent() __attribute__((weak));
bool Serial_available() __attribute__((weak));

// ----------------------------------------------------------------------------

void serialEventRun(void)
{
    if (Serial_available && serialEvent && Serial_available()) serialEvent();
}

void ameba_init(void)
{
    // Initialize C library
    __libc_init_array();

    us_ticker_init();
}
// ----------------------------------------------------------------------------

void wait_for_debug(void) {
    while (((CoreDebug->DHCSR) & CoreDebug_DHCSR_C_DEBUGEN_Msk) == 0) {
        asm("nop");
    }
    delay(1000);
}
