// #include "MqttConsumer.h"

// void MqttConsumer::mqttGenericCallback(char* topic, byte* payload, unsigned int length) {
//     Serial.print("Message arrived [");
//     Serial.print(topic);
//     Serial.print("] ");
//     for (unsigned int i = 0; i < length; i++) {
//         Serial.print((char)payload[i]);
//     }
//     Serial.println();

//     // if (strcmp(topic, mqttTopic_data) == 0) {
//     //     handleTopic(payload, length);
//     // } else if (strcmp(topic, mqttTopic_peers) == 0) {
//     //     handleTopic(payload, length);
//     // } else if (strcmp(topic, ha_hubNode_data) == 0) {
//     //     handleTopic(payload, length);
//     // }

//     // Add more topics and handlers as needed
// }

// void MqttConsumer::handleTopic(byte* payload, unsigned int length) {
//     // Handle the payload for topic1
//     Serial.print("Handling topic: ");
//     for (unsigned int i = 0; i < length; i++) {
//         Serial.print((char)payload[i]);
//     }
//     Serial.println();
// }
