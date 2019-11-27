#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "../types/message.h"

class NetworkManager {
  private:
    WiFiClient wifiClient;
    PubSubClient mqttClient;

    char *ssid;
    char *password;

  public:
    NetworkManager(char *ssid, char *password):
      ssid(ssid),
      password(password){};

    // todo: add correct return type
    /**
     * Establishes the connection to a given MQTT - Server. 
     * 
     * @param mqttAddress: Address of the MQTT Server
     * @param mqttPort: Port on which the MQTT Server runs
     */
    void connectMqtt(char *mqttAddress, int mqttPort);
    
    // todo add correct return type
    /**
      * Returns the current status of the wifi connection.
      */
    void getWiFiStatus();

    // todo add correct return type
    /**
      * Returns the current status of the connection to the given mqtt server.
      */
    void getMqttStatus();

    // todo: add an enum which describes the message status.
    /**
      * Publishes the provided message to the connected MQTT - Server.
      * 
      * @param message message which should be send to the MQTT - Server.
      * @param topic defines the topic for the published message.
      */
    void sendMessage(struct Message *message, char *topic);
};
