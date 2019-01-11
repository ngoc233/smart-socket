#include "LedManager.h"
#include "Arduino.h"

#define DEVICE_1 12 
#define DEVICE_2 14
#define DEVICE_3 16 
#define LED_STA 13

LedManager::LedManager(){
  
}
void LedManager::SETUP(){
  pinMode(DEVICE_1,OUTPUT);
  pinMode(DEVICE_2,OUTPUT);
  pinMode(DEVICE_3,OUTPUT);
}

void LedManager::ONLED(){
      digitalWrite(LED_STA,HIGH);
      Serial.println("bat den");      
}

void LedManager::OFFLED(){
      digitalWrite(LED_STA,LOW);
      Serial.println("tat den ");
}


LedManager ledManager = LedManager();
