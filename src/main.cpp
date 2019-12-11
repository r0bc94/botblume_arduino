#include <Arduino.h>

#include "network_manager/network_manager.h"
#include "water_level_measure/water_level_measure.h"
#include "types/message.h"

const char* ssid     = "~PrettyFlyForAWiFi~";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "TN8Y=UL8Dz84n6v#";     // The password of the Wi-Fi network

const char *mqttIp = "192.168.1.161";
const int mqttPort = 1883;

NetworkManager netman;

void setup() {
  Serial.begin(9600);
  netman = NetworkManager();
  
  Serial.println("Connecting to wifi");
  netman.connectWiFi(ssid, password);
  netman.connectMqtt(mqttIp, mqttPort);

  // put your setup code here, to run once:
}

void loop() {
  struct Message *msg = getMeasurement(A0);
  Serial.println(msg->originalValue);
  Serial.println(msg->percentage);
  Serial.println();

  netman.sendMessage(msg, "test");
  free(msg);

  delay(2000);
  // put your main code here, to run repeatedly:
}