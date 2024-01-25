#ifndef _CAMERA_LED_H_
#define _CAMERA_LED_H_

#define CameraLED_VERSION                  // software version of this library

#define MIN_PULSE_WIDTH         1     // the shortest pulse sent to a led
#define MAX_PULSE_WIDTH         1000    // the longest pulse sent to a led 
#define DEFAULT_PULSE_WIDTH     125    // default pulse width when led is attached
#define REFRESH_INTERVAL        1000   // minumim time to refresh camera leds in microseconds 

class CameraLED
{
public:
    CameraLED();

    // attach the given pin to the next free channel, sets pinMode, returns channel number or 0 if failure
    uint8_t attach(int pin);

    // as above but also sets min and max values for writes.
    uint8_t attach(int pin, int min, int max);

    void detach();

    // Write pulse width in microseconds
    void writeMicroseconds(int value);
    
    // returns current pulse width in microseconds for this camera led (was read_us() in first release)
    int readMicroseconds();

    // return true if this camera led is attached, otherwise false
    bool attached();

private:
    uint32_t cameraledPin;    // index into the channel data for this camera led
    uint32_t min;         // min pulse width
    uint32_t max;         // max pulse width
    int currentWidth;
};

#endif
