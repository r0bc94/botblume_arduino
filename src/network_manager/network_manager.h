#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "../types/message.h"

#define MAX_WIFI_CONNECTION_RETRY_COUNT   1000
#define MAX_MQTT_CONNECTION_RETRY_COUNT   100

enum MQTTConnectState {
  MQTT_CON_SUCCESS,
  MQTT_CON_TIMEOUT,
  MQTT_CON_NO_WIFI,
};

enum MessageSendState {
  MSG_SUCCESS,
  MSG_WIFI_NOT_CONNECTED,
  MSG_MQTT_NOT_CONNECTED,
  MSG_UKNOWN_ERROR,
};

class NetworkManager {
  private:
    WiFiClient wifiClient;
    PubSubClient mqttClient;

    char *ssid;
    char *password;

    bool wifiConnected;

    String buildMqttMessage(struct Message *message);

  public:
    NetworkManager();

    /**
     * Establishes the connection with a wifi network.
     * 
     * @param ssid: ssid of the network
     * @param password: password of the network
     */
    wl_status_t connectWiFi(const char *ssid, const char *password);

    /**
     * Establishes the connection to a given MQTT - Server. 
     * 
     * @param mqttAddress: Address of the MQTT Server
     * @param mqttPort: Port on which the MQTT Server runs
     */
    MQTTConnectState connectMqtt(const char *mqttAddress, const int mqttPort);
    
    /**
      * Returns the current status of the wifi connection.
      */
    wl_status_t getWiFiStatus();

    /**
      * Returns the current status of the connection to the given mqtt server.
      */
    boolean isMqttConnected();

    /**
      * Publishes the provided message to the connected MQTT - Server.
      * 
      * @param message message which should be send to the MQTT - Server.
      * @param topic defines the topic for the published message.
      */
    MessageSendState sendMessage(struct Message *message, const char *topic);
};
