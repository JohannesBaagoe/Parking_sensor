// mqtt_client.cpp

#include "mqtt_client.hpp"

// WiFi and MQTT configurations
const char* ssid = "Not_Russian_spies";
const char* password = "Diller123";
const char* mqtt_server = "192.168.163.180";  // e.g., "broker.hivemq.com" or your local broker

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client", "parking_broker", "123456")) {  // You can use a unique client ID
      Serial.println("connected");
      // Once connected, subscribe to a topic
      client.subscribe("Sensor2");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqtt_setup() {
  setup_wifi();
  client.setServer(mqtt_server, 1883);  // Port 1883 is the default for MQTT
  client.setCallback(callback);
}

void mqtt_loop() {
  int counter = 0;
  while (1) {
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
    // Publish a message every 5 seconds
    static unsigned long lastMsg = 0;
    unsigned long now = millis();

    if (now - lastMsg > 5000) {
      lastMsg = now;
      if (counter % 3 == 1) {
        client.publish("Sensor2", "true");
      } else {
        client.publish("Sensor2", "false");
      }
    }
    counter++;
  }
}
