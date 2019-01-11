#ifndef WiFiManager_h
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#define WiFiManager_h

class SetupWiFi
{
  public:
    SetupWiFi();
    void SETUP();
    void RUN();
    void RECONNECT();
    bool CONNECTION();
};

extern SetupWiFi wifiManager;
extern String macID;

#endif
