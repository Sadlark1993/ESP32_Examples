/*
  Codigo para testar sensor de distancia ultrasom
  pinos:
    echo: D4
    trig: D18
*/

#define TRIG 18 //pino trig
#define ECHO 4 //pino echo
#define SOUND_SPEED 0.035 // cm/us

long duration;
float distanceCm;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

}

void loop() {
  //cleans TRIG
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  //pulse of 10us
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  //collects the puls width
  duration = pulseIn(ECHO, HIGH);

  distanceCm = duration*SOUND_SPEED/2;
  Serial.print("Distance: ");
  Serial.println(distanceCm);

  delay(50);

}
