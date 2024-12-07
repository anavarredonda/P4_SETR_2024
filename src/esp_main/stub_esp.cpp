#include "stub_esp.hpp"

// #define JSON_SIZE 128

// Sends the message adding {}
void serial_send(HardwareSerial& serial, String msg)
{
  Serial.print("{");
  Serial.print(msg);
  Serial.print("}");
}

// Returns the recv msg without {}
// MSG example: {ACTION[value]}
String serial_recv(HardwareSerial& serial)
{
  String recv_msg = "";
  bool start_char_recv = false;
  bool end_char_recv = false;

  while (serial.available()) {
    char readByte = serial.read();  

    // Receive the msg
    if (start_char_recv && !end_char_recv) {
      recv_msg += readByte;
    }

    // Check if the starting msg char has been received
    if (readByte == '{') {
      start_char_recv = true;
    
    // Check if the ending msg char has been received
    } else if (readByte == '}') {
      end_char_recv = true;
      break;
    }

  }

  return recv_msg;
}

// Updates action and value pointers with the ones in the msg
void proccess_serial_msg(String msg, String& action, int& value)
{
  // Get the inedex where the int value is stored
  int begin_value_index = msg.indexOf('[');
  int end_value_index = msg.indexOf(']');

  // Check if there is an int value or not in this msg
  if (begin_value_index != -1 && end_value_index != -1) {
    // Update the action to do with the new action
    action = msg.substring(0, begin_value_index);
    // Get a substring which represents the int value
    String temp_value = msg.substring(begin_value_index + 1, end_value_index);
    // Update the int value
    value = temp_value.toInt();

  } else {
    // In case there is no int value in this msg
    action = msg;
    // Update int value to zero to make sure it has a value
    value = 0;
  }
}

// // Creates the json message in String format
// String create_json_msg(String action, int value) {
//   // Create json
//   StaticJsonDocument<JSON_SIZE> jsondoc;

//   // Fill json parameters
//   jsondoc["team_name"] = TEAM_NAME;
//   jsondoc["id"] = TEAM_ID;
//   jsondoc["action"] = action;

//   if (action == PING || action == END_LAP) {
//     jsondoc["time"] = value;
  
//   } else if (action == OBSTACLE_DETECTED) {
//     jsondoc["distance"] = value;
  
//   } else if (action == VISIBLE_LINE) {
//     jsondoc["value"] = value;
//   }

//   String jsonString;
//   serializeJson(jsondoc, jsonString);

//   return jsonString;  // Returns json in String format
// }

// Returns the json message in String format
String create_json_msg(String action, int value) {
  String jsonString = "{";
  // Add fixed parameters
  jsonString += "\"team_name\":\"" + String(TEAM_NAME) + "\",";
  jsonString += "\"id\":\"" + String(TEAM_ID) + "\",";
  jsonString += "\"action\":\"" + action + "\",";

  if (action == PING || action == END_LAP) {
    jsonString += "\"time\":" + String(value);
  
  } else if (action == OBSTACLE_DETECTED) {
    jsonString += "\"distance\":" + String(value);
  
  } else if (action == VISIBLE_LINE) {
    jsonString += "\"value\":" + String(value);
  }

  // End json msg
  jsonString += "}";

  return jsonString;  // Returns json msg in string format
}
