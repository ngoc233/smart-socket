#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ESP8266mDNS.h>
// my class

#include "WiFiManager.h"
#include "MQTTManager.h"
#include "LedManager.h"

#define buttonPIN 4

bool stationStatus = false;
int timeStation = 0;
byte pinStatus = 0;
bool SCstatus = true;
byte countButton = 0 ;
MQTTClass mqtt;

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
bool ngoc233 = true;

void setup() {
   Serial.begin(115200);
   pinMode(buttonPIN, OUTPUT);
   pinMode(buttonPIN, INPUT);
   digitalWrite(buttonPIN,HIGH);
   // mqtt
   mqtt.SETUP();
   mqtt.RUN();
   mqtt.SETCALLBACK();
}

void loop() {
  // put your main code here, to run repeatedly:
  pinStatus = digitalRead(buttonPIN);
  if(pinStatus == HIGH)
  {
    // count time and up countButton variable
    if((unsigned long)(millis() - timeStation) >= 1000)
    {
      countButton++;
      timeStation = millis();
    }
    // if hold on 5second run smart config
    if(countButton >= 5  && SCstatus && countButton < 30 && ngoc233)
    {
      WiFi.disconnect(true);
      Serial.println("run smart config");
      // don't smart config seconds time
      SCstatus = false;
      wifiManager.SETUP();
      wifiManager.RUN();
      //check had wifi to loop mqtt client
      stationStatus = true;
      ngoc233 = false;
    }else if(countButton >= 30)
    {
       // if time is over 30s, be going to setup smart config
       Serial.println("reset smart config");
       SCstatus = true;
       countButton = 0;
    }
  }else
  {
    SCstatus = true;
    countButton = 0;
  }
  if(stationStatus)
  {
    wifiManager.RECONNECT();
  }
  if (wifiManager.CONNECTION())
  {
    mqtt.LOOP();
  }

  
}
