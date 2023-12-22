#include <WiFi.h>
#include "SSLLineNotify.h"
#include "VideoStream.h"
#include "AmebaFatFS.h"

char ssid[] = "yourssid"; // your network SSID (name)
char pass[] = "password";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

char server[] = "notify-api.line.me";    // name address for Google (using DNS)

#define CHANNEL 0

VideoSetting config(VIDEO_FHD, CAM_FPS, VIDEO_JPEG, 1);

LineNotify client;

void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ;
    }

    //new
    Camera.configVideoChannel(CHANNEL, config);
    Camera.videoInit();
    Camera.channelBegin(CHANNEL);
      
    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        delay(10000);
    }
    Serial.println("Connected to wifi");
    printWifiStatus();

    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:
	  client.setLineToken("-------------------------------------------");		//Enter your license

    if (client.connect(server, 443)) {
        Serial.println("connected to server");
        //client.send("Line Notify message",0);						//Enter your message
        //client.sendStricker("Stricker Test",520,2,0);
        client.sendCameraImage("Camera Test", client, Camera, CHANNEL,0);	
        //client.sendSDImage("SD Image Test", client, "1.jpg" ,0);	//Need put 1.jpg file in SD card
        //client.sendurlImage("URL Image Test", "https://upload.wikimedia.org/wikipedia/commons/thumb/3/3a/Cat03.jpg/240px-Cat03.jpg", "https://upload.wikimedia.org/wikipedia/commons/thumb/3/3a/Cat03.jpg/240px-Cat03.jpg" ,0);
    }   
    else
    {
        Serial.println("connected to server failed");
    }
    delay(100);
}

void loop() {
    // if there are incoming bytes available
    // from the server, read them and print them:
    while (client.available()) {
        char c = client.read();
        Serial.write(c);
    }

    // if the server's disconnected, stop the client:
    if (!client.connected()) {
        Serial.println();
        Serial.println("disconnecting from server.");
        client.stop();
        // do nothing forevermore:
        while (true);
    }
}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
