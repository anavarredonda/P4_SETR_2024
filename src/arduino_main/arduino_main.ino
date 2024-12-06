#include <Arduino_FreeRTOS.h>

#include "stub_arduino.hpp"
#include "utils.h"

#define DELAY_READ_ULTRASOUND 1000
#define DELAY_READ_INFRARED 1000
#define DELAY_COMMAND_MOTORS 1000
#define DELAY_SEND_MSGS 1000



int main() {
  // Inicializar el hardware de Arduino
  init();
  Serial.begin(9600);

  /* Parametros funcion xTaskCreate
  MyTask1: función que ejecutar la tarea
  “TaskLed1”: Nombre identificativo
  100: Numero de palabras reservadas en la pila.
  NULL: Parámetros pasados a la función dela tarea
  1: Prioridad asignada a la tarea
  NULL: puntero opcional para creación de tarea.
  */

  // Read Sensors
  xTaskCreate(Read_Ultrasound, "read_ultrasound", 100, NULL, 1, NULL);
  xTaskCreate(Read_Infrared, "read_infrared", 100, NULL, 1, NULL);

  // Comand de motors
  xTaskCreate(Command_Motors, "command_motors", 100, NULL, 1, NULL);
  // Send msgs to ESP
  xTaskCreate(Send_Msgs, "send_msgs", 100, NULL, 1, NULL);

  vTaskStartScheduler();

}

static void Read_Ultrasound(void *pvParameters) {

  vTaskDelayUntil(&xLastWakeTime, (DELAY_READ_ULTRASOUND / portTICK_PERIOD_MS));

}

static void Read_Infrared(void *pvParameters) {

  vTaskDelayUntil(&xLastWakeTime, (DELAY_READ_INFRARED / portTICK_PERIOD_MS));

}

static void Command_Motors(void *pvParameters) {

  vTaskDelayUntil(&xLastWakeTime, (DELAY_COMMAND_MOTORS / portTICK_PERIOD_MS));

}

static void Send_Msgs(void *pvParameters) {

  vTaskDelayUntil(&xLastWakeTime, (DELAY_SEND_MSGS / portTICK_PERIOD_MS));

}

