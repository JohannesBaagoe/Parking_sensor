#include "mqtt_client.hpp"

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;
static int counter = 0;

void connectToWifi() {
    Serial.println("Connecting to Wi-Fi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
    mqttClient.setClientId("Device1");
    mqttClient.setCredentials("parking_broker", "123456");

    while (!mqttClient.connected()) {
        Serial.println("Connecting to MQTT...");
        mqttClient.connect();
        delay(100);
    }
}

void WiFiEvent(WiFiEvent_t event) {
    Serial.printf("[WiFi-event] event: %d\n", event);
    switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        connectToMqtt();
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
        xTimerStart(wifiReconnectTimer, 0);
        break;
    }
}

void onMqttConnect(bool sessionPresent) {
    Serial.println("Connected to MQTT.");
    String payload = "message" + String(counter++);
    uint16_t packetIdPub2 = mqttClient.publish("Sensor1", 1, true, payload.c_str());
    Serial.print("Publishing at QoS 1, packetId: ");
    Serial.println(packetIdPub2);

    esp_sleep_enable_timer_wakeup(5 * 1000000); // Sleep after sending
    Serial.println("Going to deep sleep now.");
    esp_deep_sleep_start();
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    Serial.println("Disconnected from MQTT.");

    if (WiFi.isConnected()) {
        xTimerStart(mqttReconnectTimer, 0);
    }
}

void onMqttPublish(uint16_t packetId) {
    Serial.println("Publish acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
}
