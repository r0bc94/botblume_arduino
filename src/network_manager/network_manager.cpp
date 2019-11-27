#include "network_manager.h"

NetworkManager::NetworkManager() {
  this->mqttClient = PubSubClient(this->wifiClient);
} 

wl_status_t NetworkManager::connectWiFi(char *ssid, char *password) {
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

  this->wifiConnected = true;
  Serial.printf("WiFi Connection Established!\nMy IP - Address is: %s\n", WiFi.localIP());

  return WL_CONNECTED;
}

wl_status_t NetworkManager::getWiFiStatus() {
  return WiFi.status();
}

void NetworkManager::connectMqtt(char *mqttAddress, int mqttPort) {
  this->mqttClient.setServer(mqttAddress, mqttPort);

  Serial.printf("Connecting to the MQTT Server on: %s:%d\n", mqttAddress, mqttPort);

  unsigned int mqttConnectionCounter = 1;
  while(!this->mqttClient.connected()) {
    Serial.printf("%d ", mqttConnectionCounter);
    this->mqttClient.connect("esp");
    delay(1000);
  } 

  Serial.printf("MQTT - Connection established!\n");
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

  char *messageToSend = this->buildMqttMessage(message);
  bool sendState = this->mqttClient.publish(topic, messageToSend);

  return sendState ? MSG_SUCCESS : MSG_UKNOWN_ERROR;
}

char *NetworkManager::buildMqttMessage(struct Message *message) {
  return (char*) (message->percentage + "\n" + message->originalValue);
}
