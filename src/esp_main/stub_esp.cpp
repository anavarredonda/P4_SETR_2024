#include "stub_esp.hpp"

// Sends the message adding {}
void serial_send(HardwareSerial& Serial, String msg)
{
  Serial.print("{");
  Serial.print(msg);
  Serial.print("}");
}

// Returns the recv msg without {}
String serial_recv(HardwareSerial& Serial)
{
  String recv_msg = "";
  bool start_char_recv = false;
  bool end_char_recv = false;

  while (Serial.available()) {
    char readByte = Serial.read();  

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
void proccess_action(String msg, String* action, int* value)
{
  // Get the inedex where the int value is stored
  int begin_value_index = msg.indexOf('[');
  int end_value_index = msg.indexOf(']');

  // Check if there is an int value or not in this msg
  if (begin_value_index != -1 && end_value_index != -1) {
    // Update the action to do with the new action
    *action = msg.substring(0, begin_value_index);
    // Get a substring which represents the int value
    String temp_value = msg.substring(begin_value_index + 1, end_value_index);
    // Update the int value
    *value = temp_value.toInt();

  } else {
    // In case there is no int value in this msg
    *action = msg;
    // Update int value to zero to make sure it has a value
    *value = 0;
  }

}




