#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

#include "stub_arduino.hpp"
#include "utils.h"

#define DELAY_READ_ULTRASOUND 1000
#define DELAY_READ_INFRARED 1000
#define DELAY_COMMAND_MOTORS 1000
#define DELAY_SEND_MSGS 1000

void setup() {
  // Inicializar el hardware de Arduino
  init();
  Serial.begin(9600);

  // ----------------- PIN MODES ------------------------------
  //Infrarrojo
  pinMode(PIN_ITR20001_LEFT, INPUT);
  pinMode(PIN_ITR20001_MIDDLE, INPUT);
  pinMode(PIN_ITR20001_RIGHT, INPUT);

  //motores
  pinMode(PIN_Motor_STBY, OUTPUT);
  pinMode(PIN_Motor_AIN_1, OUTPUT);
  pinMode(PIN_Motor_PWMA, OUTPUT);
  pinMode(PIN_Motor_BIN_1, OUTPUT);
  pinMode(PIN_Motor_PWMB, OUTPUT);

  //ultrasonidos
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // ----------------------------------------------------------

  // Creacion de colas
  infraredQueue = xQueueCreate(5, sizeof(struct InfraredInfo)); //Cola infrarojo
  ultrasoundQueue = xQueueCreate(5, sizeof(int)); //Cola ultrasonidos

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

void loop() {
}

// ----------------- Read_Ultrasound ------------------------------------------

static void Read_Ultrasound(void *pvParameters) {
  long distance = -1;
  long time;
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(TRIG_PIN, LOW);

  time = pulseIn(ECHO_PIN, HIGH);
  distance = time / 59;  //escalar el tiempo a una distancia en cm

  xQueueSendToBack(ultrasoundQueue, &distance, 0);

  vTaskDelayUntil(&xLastWakeTime, (DELAY_READ_ULTRASOUND / portTICK_PERIOD_MS));

}


// ----------------- Read_Infrared -------------------------------------------

static void Read_Infrared(void *pvParameters) {
  struct InfraredInfo infraredinfo;
  TickType_t xLastWakeTime;

  xLastWakeTime = xTaskGetTickCount();

  infraredinfo.r_value = analogRead(PIN_ITR20001_RIGHT);
  infraredinfo.c_value  = analogRead(PIN_ITR20001_MIDDLE);
  infraredinfo.l_value =  analogRead(PIN_ITR20001_LEFT);

  xQueueSendToBack(infraredQueue, &infraredinfo, 0);

  vTaskDelayUntil(&xLastWakeTime, (DELAY_READ_INFRARED / portTICK_PERIOD_MS));

}

// ----------------- Command_Motors --------------------------------------------

static void Command_Motors(void *pvParameters) {
  TickType_t xLastWakeTime;

  xLastWakeTime = xTaskGetTickCount();

  vTaskDelayUntil(&xLastWakeTime, (DELAY_COMMAND_MOTORS / portTICK_PERIOD_MS));

}

// ----------------- Send_Msgs --------------------------------------------

static void Send_Msgs(void *pvParameters) {
  TickType_t xLastWakeTime;

  xLastWakeTime = xTaskGetTickCount();

  vTaskDelayUntil(&xLastWakeTime, (DELAY_SEND_MSGS / portTICK_PERIOD_MS));

}
