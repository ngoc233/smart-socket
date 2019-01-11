#ifndef MQTTManager_h
#include "Arduino.h"
#define MQTTManager_h

class MQTTClass{
  public:
    MQTTClass();
    void SETUP();
    void RUN();
    void SETCALLBACK();
    void LOOP();
};

extern MQTTClass mqttManager;
#endif
