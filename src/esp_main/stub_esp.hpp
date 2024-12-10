#include <Arduino.h>
#include <ArduinoJson.h>

// Possible actions to send
#define CONNECTED_TO_SERVER "CTS"
#define CONNECTED_TO_MQTT "CTMQ"
#define START_LAP "START_LAP"
#define END_LAP "END_LAP"
#define OBSTACLE_DETECTED "OBSTACLE_DETECTED"
#define LINE_LOST "LINE_LOST"
#define PING "PING"
#define INIT_LINE_SEARCH "INIT_LINE_SEARCH"
#define STOP_LINE_SEARCH "STOP_LINE_SEARCH"
#define LINE_FOUND "LINE_FOUND"
#define VISIBLE_LINE "VISIBLE_LINE"

// Group identifiers
#define TEAM_NAME "Francesco"
#define TEAM_ID 3
#define MQTT_TOPIC "/SETR/2024/3/"

// Sends a message through the provided serial port
void serial_send(HardwareSerial& serial, String msg);

// Returns the message received in that serial port,
// returns "" if there is no message to receive
String serial_recv(HardwareSerial& serial);

// Updates the action and value values with the ones received
void proccess_serial_msg(String msg, String& action, int& value);

// Returns the json message in String format
// for the action and the value provided
String create_json_msg(String action, int value);
