#include "stub_esp.hpp"
// #include "nava_password.h"
// #include "../../../cred.h"
#include <WiFi.h>

#define RXD2 33
#define TXD2 4

String msg_from_ard;
String* action;
int* value;

void setup() {

  // Begin serial communication to show traces for debug purposes
  Serial.begin(9600);

  // Begin serial communication to communicate with Arduino UNO 
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // Connect to WIFI
  WiFi.disconnect(true); 
  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Inform Arduino UNO that connection to wifi is complete
  serial_send(Serial2, CONNECTED_TO_SERVER);
}

void loop() {
  msg_from_ard = serial_recv(Serial2);
  if (msg_from_ard != "") {
    proccess_action(msg_from_ard, action, value);

    if (action == START_LAP) {

    } else if (action == PING) {

    } else if (action == END_LAP) {

    } else if (action == OBSTACLE_DETECTED) {
      
    } else if (action == LINE_LOST) {

    } else if (action == INIT_LINE_SEARCH) {
      
    } else if (action == STOP_LINE_SEARCH) {

    } else if (action == LINE_FOUND) {

    } else if (action == VISIBLE_LINE) {

    }
  }

}