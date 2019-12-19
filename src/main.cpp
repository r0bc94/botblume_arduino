#include <Arduino.h>

#include "network_manager/network_manager.h"
#include "water_level_measure/water_level_measure.h"
#include "types/message.h"

#define POLL_DELAY 100
#define STATE_DELAY 60000
#define TOPIC "/blume/1/status"
#define SUB_TOPIC "/blume/1/wasserstand"

const char* ssid     = "~PrettyFlyForAWiFi~";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "TN8Y=UL8Dz84n6v#";     // The password of the Wi-Fi network

const char *mqttIp = "192.168.1.161";
const int mqttPort = 1883;

NetworkManager netman;

void callback(const char *topic, byte *payload, unsigned int length);
void createMessageAndSend(boolean requested);

void setup() {
  Serial.begin(9600);
  netman = NetworkManager();
  
  Serial.println("Connecting to wifi");
  netman.connectWiFi(ssid, password);
  netman.connectMqtt(mqttIp, mqttPort);
  netman.subscribeToTopic(SUB_TOPIC, callback);
  // put your setup code here, to run once:
}

void callback(const char *topic, byte *payload, unsigned int length) {
  createMessageAndSend(true);
}

void createMessageAndSend(boolean requested) {
  struct Message *msg = getMeasurement(A0);
  msg->requested = requested;

  Serial.println(msg->originalValue);
  Serial.println(msg->percentage);
  Serial.println(msg->requested);
  Serial.println();

  MessageSendState result = netman.sendMessage(msg, TOPIC);
  switch(result) {
    case MSG_MQTT_NOT_CONNECTED:
      Serial.println("MQTT Connection Lost");
      netman.reconnectMqtt();
      break;

    case MSG_WIFI_NOT_CONNECTED:
      Serial.println("WIFI Connection Lost");
      break;

    case MSG_SUCCESS:
      Serial.println("Success");
      break;

    default:
      Serial.println("Unknown Error");
  }

  free(msg);
}

unsigned int counter = 0;
void loop() {
  netman.checkMqtt();
  delay(POLL_DELAY);
  counter += 1;

  if (counter * POLL_DELAY == STATE_DELAY) {
    Serial.println("Send State");
    createMessageAndSend(false);
    counter = 0;
  }
  // put your main code here, to run repeatedly:
}
