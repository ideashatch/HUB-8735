/* 
Only for HUB8735_Ultra board use.
This example control the CAMERA LED brightness

Press button on HUB 8735 Ultra to add LED brightness 
*/

#include <CameraLED.h>

// create servo object to control a servo
// servo objects can be created correspond to PWM pins

CameraLED myled;
#define LED_PWM    13	//GPF_10
int button = PUSH_BTN;
int count = 1;
bool bisPress = false;

void setup() {
    Serial.begin(115200);
      
    myled.attach(LED_PWM,1,2000);
    myled.writeMicroseconds(200);  //1/8 duty
    
    pinMode(button, INPUT_PULLUP);
}

void loop() {

    if ((digitalRead(button) == false) && (!bisPress))
    {
        if (count < 8) {
        // turn on LED
          count++;
        } else {
            // turn off LED
            count = 0;
        }
        Serial.print("Count: ");
        Serial.println(count);
        myled.writeMicroseconds(200*count);
        bisPress = true;  
    }
    else if ((digitalRead(button) == true) && (bisPress))
    {
        bisPress = false;
    }
    delay(100);
}
