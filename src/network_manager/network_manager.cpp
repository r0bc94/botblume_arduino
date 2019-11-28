#include "network_manager.h"

NetworkManager::NetworkManager() {} 

wl_status_t NetworkManager::connectWiFi(const char *ssid, const char *password) {
  WiFi.begin(ssid, password);
  Serial.printf("Connecting to %s ... \n", ssid);

  unsigned waitCounter = 1;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.printf("%d ", waitCounter);
    waitCounter += 1;

    if (waitCounter == MAX_WIFI_CONNECTION_RETRY_COUNT) {
      return WL_CONNECT_FAILED;
    }
  } 

  /**
   * We actually have to set the pubsub client here, since the
   * WiFi Client needs to be configured before creating the 
   * PubSubClient.
   */
  this->mqttClient = PubSubClient(this->wifiClient);
  this->wifiConnected = true;

  Serial.printf("WiFi Connection Established!\nMy IP - Address is:");
  Serial.print(WiFi.localIP());
  Serial.println();

  return WL_CONNECTED;
}

wl_status_t NetworkManager::getWiFiStatus() {
  return WiFi.status();
}

bool NetworkManager::connectMqtt(const char *mqttAddress, int mqttPort) {
  this->mqttClient.setServer(mqttAddress, mqttPort);

  Serial.printf("Connecting to the MQTT Server on: %s:%d\n", mqttAddress, mqttPort);

  unsigned int mqttConnectionCounter = 1;
  while(!this->mqttClient.connected()) {
    Serial.printf("%d ", mqttConnectionCounter);
    this->mqttClient.connect("esp");
    delay(1000);

    if (mqttConnectionCounter == MAX_MQTT_CONNECTION_RETRY_COUNT) {
      return false;
    }
  } 

  Serial.printf("MQTT - Connection established!\n");

  return true;
}

boolean NetworkManager::isMqttConnected() {
  return this->mqttClient.connected();
}

MessageSendState NetworkManager::sendMessage(struct Message *message, char *topic) {
  if (!this->wifiConnected) {
    return MSG_WIFI_NOT_CONNECTED;
  }

  if (!this->mqttClient.connected()) {
    return MSG_MQTT_NOT_CONNECTED;
  } 

  const char *messageToSend = this->buildMqttMessage(message);
  bool sendState = this->mqttClient.publish(topic, messageToSend);

  return sendState ? MSG_SUCCESS : MSG_UKNOWN_ERROR;
}

const char *NetworkManager::buildMqttMessage(struct Message *message) {
  return (String(message->percentage) + "\n" + String(message->originalValue)).c_str();
}
