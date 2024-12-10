#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

#include "FastLED.h"

#include "stub_arduino.hpp"
#include "utils.h"

#define DELAY_READ_ULTRASOUND 20
#define DELAY_READ_INFRARED 100
#define DELAY_COMMAND_MOTORS 100
#define DELAY_SEND_MSGS 1000
#define DELAY_RECV_MSGS 1000

#define UMBRAL_COLOR 500

CRGB leds[NUM_LEDS];

void setup() {
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

  // ---------------------------------------------------------------------------

  FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, NUM_LEDS);
  FastLED.setBrightness(20);


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
  xTaskCreate(Read_Ultrasound, "read_ultrasound", 100, NULL, 4, NULL);
  xTaskCreate(Read_Infrared, "read_infrared", 100, NULL, 5, NULL);

  // Comand de motors
  xTaskCreate(Command_Motors, "command_motors", 100, NULL, 3, NULL);
  
  // send and recv msgs to ESP
  //xTaskCreate(Send_Msgs, "send_msgs", 100, NULL, 1, NULL);
  //xTaskCreate(Recv_Msgs, "recv_msgs", 100, NULL, 1, NULL);

  vTaskStartScheduler();

}

void loop() {
}

uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
{
  return (((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
}


// ----------------- Read_Ultrasound -------------------------------------------

static void Read_Ultrasound(void *pvParameters) {
  long distance = -1;
  long time;
  TickType_t xLastWakeTime;

  while(1) {
    xLastWakeTime = xTaskGetTickCount();

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);          //Enviamos un pulso de 10us
    digitalWrite(TRIG_PIN, LOW);

    time = pulseIn(ECHO_PIN, HIGH);
    distance = time / 59;  //escalar el tiempo a una distancia en cm

    xQueueSendToBack(ultrasoundQueue, &distance, 0);

    vTaskDelayUntil(&xLastWakeTime, (DELAY_READ_ULTRASOUND / portTICK_PERIOD_MS));
  }

}


// ----------------- Read_Infrared ---------------------------------------------

static void Read_Infrared(void *pvParameters) {
  struct InfraredInfo infraredinfo;
  TickType_t xLastWakeTime;

  while (1) {

    xLastWakeTime = xTaskGetTickCount();

    infraredinfo.r_value = analogRead(PIN_ITR20001_RIGHT);
    infraredinfo.c_value  = analogRead(PIN_ITR20001_MIDDLE);
    infraredinfo.l_value =  analogRead(PIN_ITR20001_LEFT);

    Serial.println(infraredinfo.r_value);
    Serial.println(infraredinfo.c_value);
    Serial.println(infraredinfo.l_value);

    xQueueSendToBack(infraredQueue, &infraredinfo, 0);

    delayMicroseconds(10);

    vTaskDelayUntil(&xLastWakeTime, (DELAY_READ_INFRARED / portTICK_PERIOD_MS));
  }

}

// ----------------- Command_Motors --------------------------------------------

static void Command_Motors(void *pvParameters) {
  TickType_t xLastWakeTime;
  long distance;
  struct InfraredInfo infraredInfo;
  int last_view_line = 0;

  //Activar control motores
  digitalWrite(PIN_Motor_STBY, HIGH);

  // right motors, backwards
  digitalWrite(PIN_Motor_AIN_1, HIGH);

  // left motors, forwards
  digitalWrite(PIN_Motor_BIN_1, HIGH);

  while (1) {

    xLastWakeTime = xTaskGetTickCount();

    // Receive info from infrared
    if( xQueueReceive( infraredQueue, &infraredInfo, portMAX_DELAY) ){

      // Receive info from ultrasound
      if( xQueueReceive( ultrasoundQueue, &distance, portMAX_DELAY) ){

        // Line in the midle
        if (infraredInfo.c_value > UMBRAL_COLOR){
          FastLED.showColor(Color(0, 255, 0));
          // Right motor
          analogWrite(PIN_Motor_PWMA, 100);

          // Left motor
          analogWrite(PIN_Motor_PWMB, 100);

        // Line in the left
        } else if (infraredInfo.l_value > UMBRAL_COLOR) {
          FastLED.showColor(Color(0, 255, 0));
          // Right motor
          analogWrite(PIN_Motor_PWMA, 125);

          // Left motor
          analogWrite(PIN_Motor_PWMB, 75);

          last_view_line = -1;

        // Line in the right
        } else if (infraredInfo.r_value > UMBRAL_COLOR) {
          FastLED.showColor(Color(0, 255, 0));
          // Right motor
          analogWrite(PIN_Motor_PWMA, 75);

          // Left motor
          analogWrite(PIN_Motor_PWMB, 125);

          last_view_line = 1;

        // Lost line
        } else {
          FastLED.showColor(Color(255, 0, 0));
          // Last view in the left
          if (last_view_line == -1) {
            // Right motor
            analogWrite(PIN_Motor_PWMA, 50);

            // Left motor
            analogWrite(PIN_Motor_PWMB, 125);

          // Last view in the right
          } else if (last_view_line == 1) {
            // Right motor
            analogWrite(PIN_Motor_PWMA, 125);

            // Left motor
            analogWrite(PIN_Motor_PWMB, 50);

          } else {
            // Right motor
            analogWrite(PIN_Motor_PWMA, 0);

            // Left motor
            analogWrite(PIN_Motor_PWMB, 0);
          } 
        }
      }
    }

    vTaskDelayUntil(&xLastWakeTime, (DELAY_COMMAND_MOTORS / portTICK_PERIOD_MS));
  }
}

// ----------------- Send_Msgs -------------------------------------------------

static void Send_Msgs(void *pvParameters) {
  TickType_t xLastWakeTime;

  xLastWakeTime = xTaskGetTickCount();

  vTaskDelayUntil(&xLastWakeTime, (DELAY_SEND_MSGS / portTICK_PERIOD_MS));

}

// ----------------- Recv_Msgs -------------------------------------------------

static void Recv_Msgs(void *pvParameters) {
  TickType_t xLastWakeTime;

  xLastWakeTime = xTaskGetTickCount();

  vTaskDelayUntil(&xLastWakeTime, (DELAY_RECV_MSGS / portTICK_PERIOD_MS));

}
