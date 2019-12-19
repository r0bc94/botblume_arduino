#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "../types/message.h"

#define MAX_WIFI_CONNECTION_RETRY_COUNT   1000
#define MAX_MQTT_CONNECTION_RETRY_COUNT   100
#define PROPERTY_DELIMITER ";"

typedef void (*callbackFunction)(const char*, byte*, unsigned int);

enum MQTTConnectState {
  MQTT_CON_SUCCESS,
  MQTT_CON_TIMEOUT,
  MQTT_CON_NO_WIFI,
  MQTT_CON_NO_CON_MADE,
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
    bool mqttConnectionMade; 

    String buildMqttMessage(struct Message *message);
    MQTTConnectState connectMqtt();

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
     * Subscribes to a given topic and registers the passed callback
     * function. 
     * 
     * Keep in mind, that the callback function is globally set for
     * each topic.
     * 
     * @param topic Topic on which the passed callback fuction should be
     * executed.
     * @param cbFunc Function which should be executed each time a new
     * message arrives.
     * @return true, if the topic subscription was successfull.
     */
    bool subscribeToTopic(const char *topic, callbackFunction cbFunc);

    /**
     * This is a wrapper around the loop() - Function of the
     * MQTT - Client. It should regularly be called to obtain 
     * new mqtt messages.
     * 
     * @return true, if the mqtt - client is still connected to the
     * mqtt - server.
     */
    bool checkMqtt();

    /**
     * Tries to reestablishes the MQTT - Connection with the given server.
     * Keep in mind that you should have calles connectMqtt at least one time
     * before using this method. 
     * 
     * @returns State which indicates, if the connection attempt was successful.
     */
    MQTTConnectState reconnectMqtt(); 
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
