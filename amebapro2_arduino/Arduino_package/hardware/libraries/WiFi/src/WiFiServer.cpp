/*
  WiFiServer.cpp - Library for Arduino Wifi shield.
  Copyright (c) 2011-2014 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <string.h>
#include "server_drv.h"
#include "WiFi.h"
#include "WiFiClient.h"
#include "WiFiServer.h"

WiFiServer::WiFiServer():
    _port(1883)
{
}

WiFiServer::WiFiServer(uint16_t port)
{
    _port = port;
}

WiFiServer::WiFiServer(tProtMode portMode):
    _port(1883)
{
    _portMode = portMode;
}

WiFiServer::WiFiServer(tBlockingMode blockMode):
    _port(1883)
{
    _is_blocked = blockMode;
}

WiFiServer::WiFiServer(uint16_t port, tProtMode portMode)
{
    _port = port;
    _portMode = portMode;
}

WiFiServer::WiFiServer(uint16_t port, tProtMode portMode, tBlockingMode blockMode)
{
    _port = port;
    _portMode = portMode;
    _is_blocked = blockMode;
}

WiFiServer::~WiFiServer()
{
    stop();
}

void WiFiServer::begin()
{
    _is_connected = false;
    _sock_ser = serverdrv.startServer(_port, _portMode, _is_blocked);
    if (_sock_ser < 0) {
        _is_connected = false;
        printf("\r\n[ERROR] Socket connect failed \n");
    } else {
        _is_connected = true;
        // printf("\r\n[INFO] Socket connect successfully \n");
    }
}

WiFiClient WiFiServer::available(uint8_t* status)
{
    int client_fd = (int)(status);

    client_fd = serverdrv.getAvailable(_sock_ser);

    return WiFiClient(client_fd);
}

#if 0
int WiFiServer::available(int server_fd) {
    int client_fd;

    client_fd = serverdrv.getAvailable(server_fd);
    _sock_ser = client_fd;

    return client_fd;
}

uint8_t WiFiServer::connected() {
    if ((_sock_ser < 0) || (_sock_ser == 0xFF)) {
        _is_connected = false;
        return 0;
    } else {
        if (_is_connected) {
            return 1;
        } else {
            stop();
            return 0;
        }
    }
}

int WiFiServer::recv(uint8_t* buf, size_t size) {
    uint16_t _size = size;
    int ret;
    int err;

    ret = serverdrv.recvData(_sock_ser, buf, _size);
    if (ret <= 0) {
        err = serverdrv.getLastErrno(_sock_ser);
        if (err != 0) {
            _is_connected = false;
        }
    }

    return ret;
}
#endif

size_t WiFiServer::write(uint8_t b)
{
    return write(&b, 1);
}

void WiFiServer::stop()
{
    if (_sock_ser < 0) {
        return;
    }
    serverdrv.stopSocket(_sock_ser);
    _is_connected = false;
    _sock_ser = -1;
}

// set WiFi server to blocking/non-blocking mode
void WiFiServer::setBlockingMode()
{
    _is_blocked = BLOCKING_MODE;
}

void WiFiServer::setNonBlockingMode()
{
    _is_blocked = NON_BLOCKING_MODE;
}

#if 0
size_t WiFiServer::write(const uint8_t *buf, size_t size) {
    if (_sock_ser < 0) {
        setWriteError();
        return 0;
    }
    if (size == 0) {
        setWriteError();
        return 0;
    }

    if (!serverdrv.sendData(_sock_ser, buf, size)) {
        setWriteError();
        return 0;
    }

    return size;
}

void WiFiServer::end() {
    stop();
}

void WiFiServer::close() {
    stop();
}

// extend API from RTK
int WiFiServer::setTimeout(int timeout) {
    if (connected()) {
        recvTimeout = timeout;
        serverdrv.setSockRecvTimeout(_sock_ser, recvTimeout);
    }
    return 0;
}

// IPv6 related
int WiFiServer::enableIPv6() {
    return serverdrv.enableIPv6();
}

int WiFiServer::getIPv6Status() {
    return serverdrv.getIPv6Status();
}
#endif
