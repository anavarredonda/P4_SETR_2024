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



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Configurar pines como salida
  pinMode(PIN_Motor_STBY, OUTPUT);
  pinMode(PIN_Motor_AIN_1, OUTPUT);
  pinMode(PIN_Motor_PWMA, OUTPUT);
  pinMode(PIN_Motor_BIN_1, OUTPUT);
  pinMode(PIN_Motor_PWMB, OUTPUT);

  //Activar control motores
  digitalWrite(PIN_Motor_STBY, HIGH);

  // Motores derecha, modo adelante
  digitalWrite(PIN_Motor_AIN_1, HIGH);

  // Motores izquierda, modo adelante
  digitalWrite(PIN_Motor_BIN_1, HIGH);

}

void loop() {

  // Motor derecha
  analogWrite(PIN_Motor_PWMA, 100);

  //Motor izquierda
  analogWrite(PIN_Motor_PWMB, 100);

}
