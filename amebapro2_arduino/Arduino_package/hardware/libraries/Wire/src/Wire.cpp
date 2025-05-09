/*
 * TwoWire.h - TWI/I2C library for Arduino Due
 * Copyright (c) 2011 Cristian Maglie <c.maglie@arduino.cc>
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Arduino.h"
#include "Wire.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "PinNames.h"
#include "i2c_api.h"
#include "wait_api.h"
#include "ex_api.h"
// #include "i2c_slave.h"

i2c_t i2cwire0;
i2c_t i2cwire1;

#ifdef __cplusplus
}
#endif

TwoWire::TwoWire(void *pWireObj, uint32_t dwSDAPin, uint32_t dwSCLPin)
{
    this->SDA_pin = dwSDAPin;
    this->SCL_pin = dwSCLPin;
    this->user_onReceive = NULL;
    this->user_onRequest = NULL;
    this->pI2C = pWireObj;

    this->rxBufferIndex = 0;
    this->rxBufferLength = 0;
    this->txAddress = 0;
    this->txBufferLength = 0;
    this->twiClock = this->TWI_CLOCK;
}

int MBED_I2C_SLAVE_ADDR0 = 0x00;
int I2C_DATA_LENGTH = 1;    // 125
int flag = 0;

void TwoWire::begin()
{
    amb_ard_pin_check_fun(SDA_pin, PIO_I2C);
    amb_ard_pin_check_fun(SCL_pin, PIO_I2C);

    SDA_pin = (PinName)g_APinDescription[SDA_pin].pinname;
    SCL_pin = (PinName)g_APinDescription[SCL_pin].pinname;

    i2c_init(((i2c_t *)this->pI2C), ((PinName)this->SDA_pin), ((PinName)this->SCL_pin));
    i2c_frequency(((i2c_t *)this->pI2C), this->twiClock);
}

void TwoWire::begin(uint8_t address = 0)
{
    amb_ard_pin_check_fun(SDA_pin, PIO_I2C);
    amb_ard_pin_check_fun(SCL_pin, PIO_I2C);

    SDA_pin = (PinName)g_APinDescription[SDA_pin].pinname;
    SCL_pin = (PinName)g_APinDescription[SCL_pin].pinname;

    i2c_init(((i2c_t *)this->pI2C), ((PinName)this->SDA_pin), ((PinName)this->SCL_pin));
    i2c_frequency(((i2c_t *)this->pI2C), this->twiClock);
}

void TwoWire::begin(int address)
{
    begin((uint8_t)address);
}

void TwoWire::end()
{
    i2c_reset((i2c_t *)this->pI2C);
}

void TwoWire::setClock(uint32_t frequency)
{
    twiClock = frequency;
    i2c_frequency(((i2c_t *)this->pI2C), this->twiClock);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
    if (quantity > BUFFER_LENGTH) {
        quantity = BUFFER_LENGTH;
    }

    // perform blocking read into buffer
    i2c_read(((i2c_t *)this->pI2C), ((int)address), reinterpret_cast<char *>(this->rxBuffer), ((int)quantity), ((int)sendStop));

    // i2c_read error;
    // if (read != 0) {
    //     printf("\r\n[ERROR] requestFrom: read=%d, quantity=%d \n", read, quantity);
    // }

    /*//i2c_read error;
    if (read != quantity) {
        printf("\r\n[ERROR] requestFrom: read=%d, quantity=%d \n", read, quantity);

        return read;
    }
    rxBufferLength = read;*/

    // set rx buffer iterator vars
    rxBufferIndex = 0;
    rxBufferLength = quantity;

    return quantity;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
    return requestFrom(((uint8_t)address), ((uint8_t)quantity), ((uint8_t) true));
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
    return requestFrom(((uint8_t)address), ((uint8_t)quantity), ((uint8_t) true));
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
    return requestFrom(((uint8_t)address), ((uint8_t)quantity), ((uint8_t)sendStop));
}

void TwoWire::beginTransmission(uint8_t address)
{
    // save address of target and empty buffer
    // If target address changes, wait for 50us to avoid losing next data packet, tested ok down to 10us

    // Top comment is original comment, currently for I2C Scanner, this delay is creating a huge scan time, removed to make the scanning much faster.
    // Seems like unnecessary delay: To be tested by QC.
    if (txAddress != address) {
        txAddress = address;
        //    delay(50);
    }
    txBufferLength = 0;
}

void TwoWire::beginTransmission(int address)
{
    beginTransmission(((uint8_t)address));
}

//  Originally, 'endTransmission' was an f(void) function.
//  It has been modified to take one parameter indicating
//  whether or not a STOP should be performed on the bus.
//  Calling endTransmission(false) allows a sketch to
//  perform a repeated start.
//
//  WARNING: Nothing in the library keeps track of whether
//  the bus tenure has been properly ended with a STOP. It
//  is very possible to leave the bus in a hung state if
//  no call to endTransmission(true) is made. Some I2C
//  devices will behave oddly if they do not see a STOP.
uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
    int length;
    uint8_t error = 0;
    i2c_reset(((i2c_t *)this->pI2C));
    // toggle flag to normal
    if (flag == 1) {
        flag = 0;
    }

    i2c_init(((i2c_t *)this->pI2C), ((PinName)this->SDA_pin), ((PinName)this->SCL_pin));
    i2c_frequency(((i2c_t *)this->pI2C), this->twiClock);
    i2c_set_user_callback(((i2c_t *)this->pI2C), I2C_TX_COMPLETE, i2c_callback_set_flag);
    length = i2c_write(((i2c_t *)this->pI2C), ((int)this->txAddress), reinterpret_cast<char *>(this->txBuffer), ((int)this->txBufferLength), ((int)sendStop));
    hal_delay_us(this->txBufferLength * 200);

    if ((txBufferLength > 0) && (length <= 0)) {
        error = 1;
    }
    if (flag == 0) {
        error = -1;    // Error for wrong slave address
    } else {
        error = 0;
    }
    txBufferLength = 0;    // empty buffer

    return error;
}

// This provides backwards compatibility with the original
// definition, and expected behaviour, of endTransmission
uint8_t TwoWire::endTransmission(void)
{
    return endTransmission(true);
}

void TwoWire::i2c_callback_set_flag(void *userdata)
{
    flag = 1;
}

size_t TwoWire::write(uint8_t data)
{
    if (txBufferLength >= BUFFER_LENGTH) {
        return 0;
    }
    txBuffer[txBufferLength++] = data;
    return 1;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
    for (size_t i = 0; i < quantity; ++i) {
        if (txBufferLength >= BUFFER_LENGTH) {
            return i;
        }
        txBuffer[txBufferLength++] = data[i];
    }
    return quantity;
}

int TwoWire::available(void)
{
    return (rxBufferLength - rxBufferIndex);
}

int TwoWire::read(void)
{
    if (rxBufferIndex < rxBufferLength) {
        return rxBuffer[rxBufferIndex++];
    }
    return -1;
}

int TwoWire::peek(void)
{
    if (rxBufferIndex < rxBufferLength) {
        return rxBuffer[rxBufferIndex];
    }
    return -1;
}

void TwoWire::flush(void)
{
    // Do nothing, use endTransmission(..) to force
    // data transfer.
}

void TwoWire::onReceiveService(uint8_t *inBytes, size_t numBytes, bool stop, void *arg)
{
    // status = SLAVE_RECV;

    stop = stop;

    TwoWire *wire = (TwoWire *)arg;
    if (!wire->user_onReceive) {
        return;
    }
    for (uint8_t i = 0; i < numBytes; ++i) {
        wire->rxBuffer[i] = inBytes[i];
    }
    wire->rxBufferIndex = 0;
    wire->rxBufferLength = numBytes;
    wire->user_onReceive(numBytes);
}

void TwoWire::onRequestService(void *arg)
{
    // status = SLAVE_SEND;

    TwoWire *wire = (TwoWire *)arg;
    if (!wire->user_onRequest) {
        return;
    }
    wire->txBufferLength = 0;
    wire->user_onRequest();    // user callback normally write data into txbuffer
    // if (wire->txBufferLength) {
    //     wire->slaveWrite((uint8_t*)wire->txBuffer, wire->txBufferLength);
    //     wire->slaveWrite((uint8_t*)txBuffer, txBufferLength);
    // }
}

#if 0
void TwoWire::onReceive(void(*function)(int)) {
    user_onReceive = function;
}

void TwoWire::onRequest(void(*function)(void)) {
    user_onRequest = function;
}

size_t TwoWire::slaveWrite(int buffer) {
    return slaveWrite((uint8_t *)&buffer, 1);
}

size_t TwoWire::slaveWrite(char *buffer) {
    return slaveWrite((uint8_t *)buffer, (size_t)sizeof(buffer));
}

size_t TwoWire::slaveWrite(uint8_t *buffer, size_t len) {
    return i2cSlaveWrite(buffer, len, RECV_TIMEOUT);
}

#endif

#ifdef HUB8735_ULTRA
TwoWire Wire  = TwoWire((void *)(&i2cwire0), I2C_SDA, I2C_SCL);
TwoWire Wire1  = TwoWire((void *)(&i2cwire1), I2C1_SDA, I2C1_SCL);
#else
TwoWire Wire  = TwoWire((void *)(&i2cwire0), I2C1_SDA, I2C1_SCL);
TwoWire Wire1  = TwoWire((void *)(&i2cwire1), I2C_SDA, I2C_SCL);
#endif