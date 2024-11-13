// mqtt_client.cpp

#include "mqtt_client.hpp"

// // WiFi and MQTT configurations
// const char* ssid = "Not_Russian_spies";
// const char* password = "Diller123";
 //const char* mqtt_server = "192.168.1.180";  // e.g., "broker.hivemq.com" or your local broker

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi(const char* ssid, const char *password) {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
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

/*void mqtt_setup() {
  setup_wifi(ssid, password);
  client.setServer(mqtt_server, 1883);  // Port 1883 is the default for MQTT
  client.setCallback(callback);
}*/

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

uint8_t *mqtt_connect_packet(const char* clientID, const char* username, const char* password) {

  uint8_t packet[50]; // buffer

  packet[0] = 0x10; // connectmessage
  packet[1] = 0x12 + strlen(clientID); // længden på det resterende af beskeden
  packet[2] = 0x00; // slut på fixed header
  packet[3] = 0x04; // længden på protokolnavnet (MQTT)
  packet[4] = 'M';
  packet[5] = 'Q';
  packet[6] = 'T';
  packet[7] = 'T';
  packet[8] = 0x05; // version 5 af MQTT protokollen
  packet[9] = 0xC2; // new session, username and password flag set.
  packet[10] = 0x00; // slut på variable header
  packet[11] = 0x3c; // keep alive in 60 sec.
  packet[12] = 0x00;
  packet[13] = strlen(clientID); // længde i bytes på clientID'et
  memcpy(&packet[14], clientID, strlen(clientID));  // ligger clientID ind fra byte 14 og frem

  int offset = 14 + strlen(clientID); // Opdater offset til slutningen af Client ID

  packet[offset] = 0x00; // Username Length MSB
  packet[offset + 1] = strlen(username); // Username Length LSB
  memcpy(&packet[offset + 2], username, strlen(username)); // Kopier Username
  offset += 2 + strlen(username); // Opdater offset

  packet[offset] = 0x00; // Password Length MSB
  packet[offset + 1] = strlen(password); // Password Length LSB
  memcpy(&packet[offset + 2], password, strlen(password)); // Kopier Password


  return packet;
}

void mqtt_connect(WiFiClient espClient, const char* brokerIP, const uint16_t brokerPort){

  if(!espClient.connect(brokerIP, brokerPort)){
    //int error_code = espClient.getLastSSLError();
    Serial.println("Kunne ikke oprette forbindelse til broker!");
    //Serial.println(error_code);
    return; // Stop hvis forbindelsen mislykkes
  }

 uint8_t *connect_package = mqtt_connect_packet("sensor1", "parking_broker", "123456");

 int packetlength = sizeof(connect_package);

  espClient.write(connect_package, packetlength);

  while(!espClient.available()){
    Serial.println("Broker have not responded!");
    delay(1000);
  }

   // Læs og tjek svaret fra broker
  uint8_t response[4];
  espClient.read(response, 4);

  // Tjek om det er et korrekt CONNACK-svar
  if (response[0] == 0x20 && response[1] == 0x02 && response[3] == 0x00) {
    Serial.println("MQTT CONNECT successful");
  } else {
    Serial.println("MQTT CONNECT failed");
  }
}
