#define PIN_ITR20001_LEFT   A2
#define PIN_ITR20001_MIDDLE A1
#define PIN_ITR20001_RIGHT  A0

int l = 0, r = 0, m = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_ITR20001_LEFT, INPUT);
  pinMode(PIN_ITR20001_MIDDLE, INPUT);
  pinMode(PIN_ITR20001_RIGHT, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  l = analogRead(PIN_ITR20001_LEFT);
  m = analogRead(PIN_ITR20001_MIDDLE);
  r = analogRead(PIN_ITR20001_RIGHT);
  Serial.print("Lectura izq: ");
  Serial.println(l);
  Serial.print("Lectura medio: ");
  Serial.println(m);
  Serial.print("Lectura dcha: ");
  Serial.println(r);
  Serial.println("--------------------------");
  delay(1000);
}
