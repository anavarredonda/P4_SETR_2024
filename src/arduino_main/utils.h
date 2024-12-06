

/*-------------------- motors ----------------------*/

// Enable/Disable motor control.
//  HIGH: motor control enabled
//  LOW: motor control disabled
#define PIN_Motor_STBY 3

// Group A Motors (Right Side)
// PIN_Motor_AIN_1: Digital output. HIGH: Forward, LOW: Backward
#define PIN_Motor_AIN_1 7
// PIN_Motor_PWMA: Analog output [0-255]. It provides speed.
#define PIN_Motor_PWMA 5

// Group B Motors (Left Side)
// PIN_Motor_BIN_1: Digital output. HIGH: Forward, LOW: Backward
#define PIN_Motor_BIN_1 8
// PIN_Motor_PWMB: Analog output [0-255]. It provides speed.
#define PIN_Motor_PWMB 6


/*-------------------- ultrasound ----------------------*/
#define TRIG_PIN 13  
#define ECHO_PIN 12 

QueueHandle_t ultrasoundQueue;


/*-------------------- infrared ----------------------*/
#define PIN_ITR20001_LEFT   A2
#define PIN_ITR20001_MIDDLE A1
#define PIN_ITR20001_RIGHT  A0

struct InfraredInfo {
  unsigned int    r_value;    // value of right infrared sensor   
  unsigned int    c_value;    // value of center infrared sensor
  unsigned int    l_value;    // value of left infrared sensor
};

QueueHandle_t infraredQueue;


/*-------------------- led ----------------------*/
#define PIN_RBGLED 4
#define NUM_LEDS 1


