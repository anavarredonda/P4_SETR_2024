#include "stub_esp.hpp"
// #include "nava_password.h"
#include "../../../cred.h"
#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define RXD2 33
#define TXD2 4
#define SERVER_IP "193.147.79.118"
#define SERVER_PORT 21883

// For WIFI connection
//SSID NAME
const char* ssid = "eduroam"; // eduroam SSID

// For MQTT use
WiFiClient wificlient; // Wifi client to connect MQTT to wifi
Adafruit_MQTT_Client mqtt(&wificlient, SERVER_IP, SERVER_PORT); // Define MQTT client

// Configure MQTT publisher
Adafruit_MQTT_Publish publisher = Adafruit_MQTT_Publish(&mqtt, MQTT_TOPIC);

String msg_from_ard; // Stores the message received from Arduino UNO
String action; // Stores action to do received from Arduino UNO
int value; // Stores interger value received from Arduino UNO
String json_msg; // Message to publish through MQTT

// ---------------------------------------------------
// Returns true if connected to MQTT
bool mqtt_connect() {
  // Stop if already connected.
  if (mqtt.connected()) {
    return true;
  }

  // If connection succesfull returns 0
  if (mqtt.connect() != 0) {
    mqtt.disconnect();
    return false;
  }
  return true;
}
// ---------------------------------------------------

void setup() {

  // Begin serial communication to show traces for debug purposes
  Serial.begin(9600);

  // Begin serial communication to communicate with Arduino UNO 
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // Connect to WIFI
  serial_send(Serial, "Starting connection proccess");
  WiFi.disconnect(true); 
  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD); 
  while (WiFi.status() != WL_CONNECTED) {
    serial_send(Serial, "Trying to connect to wifi");
    delay(500);
  }
  serial_send(Serial, "Connected to wifi");
  // Inform Arduino UNO that connection to wifi is complete
  serial_send(Serial2, CONNECTED_TO_SERVER);

  // Connect to MQTT
  while (!mqtt_connect()) {
    serial_send(Serial, "Trying to connect to MQTT");
    delay(500);
  }
  serial_send(Serial, "Connected to MQTT");
  // Inform Arduino UNO that connection to MQTT is complete
  serial_send(Serial2, CONNECTED_TO_MQTT);
}

void loop() {
  msg_from_ard = serial_recv(Serial2);

  if (msg_from_ard != "") {
    proccess_serial_msg(msg_from_ard, action, value);

    // Create the message depending on the action
    json_msg = create_json_msg(action, value);

    // Publish the message until it's correctly published
    // while (!publisher.publish(MQTT_TOPIC, json_msg.c_str())) {
    //   delay(10);
    // }
    publisher.publish(MQTT_TOPIC, json_msg.c_str());
    serial_send(Serial, "Message published");
  }
  // } else {
  //   mqtt.ping(); // To keep conexion alive
  // }

  // Make sure there is still connection to the server
  // while (!mqtt_connect()) {
  //   serial_send(Serial, "Reconnecting to MQTT");
  // }

}