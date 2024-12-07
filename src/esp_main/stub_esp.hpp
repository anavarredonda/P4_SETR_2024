#include <Arduino.h>

// Possible actions to send
#define CONNECTED_TO_SERVER "CTS"
#define START_LAP "SL"
#define END_LAP "EL"
#define OBSTACLE_DETECTED "OD"
#define LINE_LOST "LL"
#define PING "P"
#define INIT_LINE_SEARCH "ILS"
#define STOP_LINE_SEARCH "STLS"
#define LINE_FOUND "LF"
#define VISIBLE_LINE "VL"

// Group identifiers
#define GROUP_NAME Francesco
#define GROUP_ID 3

void serial_send(HardwareSerial&, String);

String serial_recv(HardwareSerial&);

void proccess_action(String, String*, int*);
