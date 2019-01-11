#include "MQTTManager.h"
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "WiFiManager.h"

#define mqtt_server "m10.cloudmqtt.com" // could mqtt server
#define mqtt_topic_pub "demo"   // could mqtt topic pub
#define mqtt_topic_sub "demo"   // could mqtt topic sub
#define mqtt_user "vpfiytsx"    // could mqtt user
#define mqtt_pwd "q2M1pcYye1IV" // could mqtt password
uint16_t mqtt_port = 10261; //could mqtt port

// tạo hằng để lưu new MQTT
const char* charMQTTServer;
const char* charMQTTPub;
const char* charMQTTSub;
const char* charMQTTUser;
const char* charMQTTPwd;

// biến để lưu độ dài
byte serverLength, pubLength, subLength, userLength, pwdLength, portLength;

WiFiClient espClient;
PubSubClient client(espClient);

MQTTClass::MQTTClass(){
  
}
void MQTTClass::SETUP(){
 // if have length value on rom-95 -> write
  if (EEPROM.read(95) == 255 || EEPROM.read(95) == 0)
  {
    // để mặc định các giá trị
    charMQTTServer = mqtt_server;
    charMQTTPub = mqtt_topic_pub;
    charMQTTSub = mqtt_topic_sub;
    charMQTTUser = mqtt_user;
    charMQTTPwd = mqtt_pwd;
    Serial.println("set default MQTT");
    Serial.println(charMQTTServer);
  }
  else
  {
    portLength = EEPROM.read(94);
    serverLength = EEPROM.read(95);
    pubLength = EEPROM.read(96);
    subLength = EEPROM.read(97);
    userLength = EEPROM.read(98);
    pwdLength = EEPROM.read(99);

    //get server from eeprom
    String svString, pubString, subString, userString, pwdString, portString;

    // lấy độ dài và giá trị của new cloud
    int countMQTTLength = 100;
    for (int i = 0; i < serverLength; i++)
    {
      svString += char(EEPROM.read(countMQTTLength));
      countMQTTLength++;
    }
    for (int i = 0; i < pubLength; i++)
    {
      pubString += char(EEPROM.read(countMQTTLength));
      countMQTTLength++;
    }
    for (int i = 0; i < subLength; i++)
    {
      subString += char(EEPROM.read(countMQTTLength));
      countMQTTLength++;
    }
    for (int i = 0; i < userLength; i++)
    {
      userString += char(EEPROM.read(countMQTTLength));
      countMQTTLength++;
    }
    for (int i = 0; i < pwdLength; i++)
    {
      pwdString += char(EEPROM.read(countMQTTLength));
      countMQTTLength++;
    }
    for (int i = 0; i < portLength; i++)
    {
      portString += char(EEPROM.read(countMQTTLength));
      countMQTTLength++;
    }

    // make json object and set value for new cloud
    StaticJsonBuffer<500> jsonEEPBuffer;
    JsonObject& jsonEEPROM  = jsonEEPBuffer.createObject();
    jsonEEPROM["server"] = svString;
    jsonEEPROM["publish"] = pubString;
    jsonEEPROM["subcribe"] = subString;
    jsonEEPROM["user"] = userString;
    jsonEEPROM["pwd"] = pwdString;
    jsonEEPROM["port"] = portString;

    mqtt_port = portString.toInt();
    charMQTTServer = jsonEEPROM["server"];
    charMQTTPub = jsonEEPROM["publish"];
    charMQTTSub = jsonEEPROM["subcribe"];
    charMQTTUser = jsonEEPROM["user"];
    charMQTTPwd = jsonEEPROM["pwd"];
  }
}

void MQTTClass::RUN(){
    client.setServer(charMQTTServer, mqtt_port);       
}

void callback(char* topic, byte* payload, unsigned int length) {
  String stringMessage;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    char receivedChar = (char)payload[i];
    stringMessage += (String)receivedChar;
  }
  Serial.print("string mess la : ");
  Serial.println(stringMessage);
  StaticJsonBuffer<5000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(stringMessage);
  int modeMess = (int)root["mode"];
  String modeLength = root["mode"].asString();
  JsonArray& arrayMode = (root["id"]);
  for(int i=0; i<arrayMode.size();i++)
  {
    String deviceID = arrayMode[i]["idDevice"];
    if(modeLength.length() > 0)
    {
      if(modeMess == 0)
      {
        Serial.println("of device 1");
        Serial.println(macID);
      }
      else if(modeMess  == 1)
      {
        Serial.println("on device");
      }else if(modeMess == 2)
      {
         Serial.println("off all device");
      }
    }
  }
}

void MQTTClass::SETCALLBACK(){
  client.setCallback(callback);
}

void reconnect(){
  // Chờ tới khi kết nối
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Thực hiện kết nối với mqtt user và pass
    delay(500);
    Serial.println(charMQTTUser);
    delay(500);
    Serial.println(charMQTTPwd);
    delay(500);
    Serial.println(charMQTTSub);
    delay(500);
    Serial.println(charMQTTPub);
    delay(500);
    
    Serial.println("khong hieu");
    if (client.connect("ESP8266Client233", charMQTTUser, charMQTTPwd)) {
      Serial.println("connected");
      // Khi kết nối sẽ publish thông báo
      client.publish(charMQTTPub, "ESP_reconnected");
      // ... và nhận lại thông tin này
      client.subscribe(charMQTTSub);
      Serial.println();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Đợi 5s
      delay(5000);
    }
  }
}

void MQTTClass::LOOP(){
  if (!client.connected()) {
      reconnect();
    }
    client.loop();
}
MQTTClass mqttManager = MQTTClass();
