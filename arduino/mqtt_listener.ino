#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 4);
IPAddress server(192, 168, 0, 3);

EthernetClient ethClient;
PubSubClient client(ethClient);
const int arduinoPins[8] = { 2, 3, 4, 5, 6, 7, 8, 9 };

void setup() {
    Serial.begin(9600);
    for (int i = 0; i < 8; i++) {
        pinMode(arduinoPins[i], OUTPUT);
    }
    Ethernet.begin(mac, ip);
    client.setServer(server, 1883);
    client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
    payload[length] = '\0';
    String topicStr(topic);
    String payloadStr((char*)payload);
    int pinIndex = topicStr.charAt(8) - '0';  // Cambiado a 8 para "ardu_pinX"
    if (pinIndex >= 0 && pinIndex < 8) {
        digitalWrite(arduinoPins[pinIndex], payloadStr == "on" ? HIGH : LOW);
        Serial.print("Pin ");
        Serial.print(pinIndex + 1);
        Serial.print(" ");
        Serial.println(payloadStr);
    }
}

void reconnect() {
    while (!client.connected()) {
        if (client.connect("ArduinoClient")) {
            for (int i = 0; i < 8; i++) {
                String topic = "ardu_pin" + String(i);  // Cambiado a "ardu_pinX"
                client.subscribe(topic.c_str());
            }
        } else {
            delay(2000);
        }
    }
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}
