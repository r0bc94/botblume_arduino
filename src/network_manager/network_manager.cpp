#include "network_manager.h"

NetworkManager::NetworkManager() {
  mqttConnectionMade = false;
} 

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

MQTTConnectState NetworkManager::connectMqtt(const char *mqttAddress, int mqttPort) {
  this->mqttClient.setServer(mqttAddress, mqttPort);

  Serial.printf("Connecting to the MQTT Server on: %s:%d\n", mqttAddress, mqttPort);
  MQTTConnectState returnCode = this->connectMqtt();
  this->mqttConnectionMade = true;

  return returnCode;
}

MQTTConnectState NetworkManager::reconnectMqtt() {
  if (!this->mqttConnectionMade) {
    Serial.println("MQTT - Connection was never established!");
    return MQTT_CON_NO_CON_MADE; 
  }
  
  Serial.println("Trying to reestablish the Connection with the MQTT - Server");
  return this->connectMqtt();
}

bool NetworkManager::subscribeToTopic(const char *topic, callbackFunction cbFunc) {
  this->mqttClient.setCallback(cbFunc);
  return this->mqttClient.subscribe(topic);
}

bool NetworkManager::checkMqtt() {
  return this->mqttClient.loop();
}

boolean NetworkManager::isMqttConnected() {
  return this->mqttClient.connected();
}

MessageSendState NetworkManager::sendMessage(struct Message *message, const char *topic) {
  if (WiFi.status() != WL_CONNECTED) {
    return MSG_WIFI_NOT_CONNECTED;
  }

  if (!this->mqttClient.connected()) {
    if (this->mqttConnectionMade) {
      Serial.println("MQTT - Connection lost, trying to reconnect");
      MQTTConnectState constate = this->reconnectMqtt();
      if (constate != MQTTConnectState::MQTT_CON_SUCCESS) {
        return MSG_MQTT_NOT_CONNECTED;
      }
    } else {
      Serial.println("The Connection with the MQTT - Server was not established!");
      Serial.println("Please connect to the MQTT - Server first, before using this method.");
      return MSG_MQTT_NOT_CONNECTED;
    }
  }  

  String messageToSend = this->buildMqttMessage(message);
  bool sendState = this->mqttClient.publish(topic, messageToSend.c_str());

  return sendState ? MSG_SUCCESS : MSG_UKNOWN_ERROR;
}

String NetworkManager::buildMqttMessage(struct Message *message) {
  String str = String();
  str = String(message->percentage) + "\n" + String(message->originalValue);
  return str;
}

MQTTConnectState NetworkManager::connectMqtt() {

 if (WiFi.status() != WL_CONNECTED) {
  return MQTT_CON_NO_WIFI;
 }

 unsigned int mqttConnectionCounter = 1;
  while(!this->mqttClient.connected()) {
    Serial.printf("%d ", mqttConnectionCounter);
    this->mqttClient.connect("esp");
    delay(1000);

    if (mqttConnectionCounter == MAX_MQTT_CONNECTION_RETRY_COUNT) {
      return MQTT_CON_TIMEOUT;
    }
  } 

  Serial.printf("MQTT - Connection established!\n");

  return MQTT_CON_SUCCESS;
}
