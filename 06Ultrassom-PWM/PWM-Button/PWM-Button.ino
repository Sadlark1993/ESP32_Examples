/*
Toda vez que alguém apertar o botao no pino 14, o PWM do pino 13 aumenta;
toda vez que alguem apertar o botao no pino 19, o PWM do pino 13 diminui.
*/

int frequency = 5000;
int ledChannel = 0;
int resolution = 8;
long duration;
float distanceCm = 0;

#define TRIG 18 //pino trig
#define ECHO 4 //pino echo
#define SOUND_SPEED 0.035 // cm/us

int button; //+pwm
int button2; //-pwm
int button3; //show distance
int history = 1;
int history2 = 1;
int motor = 30;

void setup() {
  // put your setup code here, to run once:
  ledcSetup(ledChannel, frequency, resolution);

  ledcAttachPin(13, ledChannel); //pino do PWM
  pinMode(14, INPUT_PULLUP); //botao
  pinMode(19, INPUT_PULLUP); //botao
  pinMode(16, INPUT_PULLUP); //botao
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.begin(110200);
}

void loop() {
  // put your main code here, to run repeatedly:

  button2 = digitalRead(19); //botao diminui pwm
  button = digitalRead(14); //botao aumenta pwm
  button3 = digitalRead(16); //botao show distance

  if(!button && history){ //aumentar
    motor = motor+2;
    if(motor>255) motor = 255;
    history = 0;
    ledcWrite(ledChannel, motor); //escrevendo pwm no pino 13
    Serial.print("motor: ");
    Serial.print(motor);
    Serial.print(", Distance: ");
    Serial.println(distanceCm);
  }else if(button) history = 1;

  if(!button2 && history2){ //diminuir
    motor = motor-2;
    if(motor < 0) motor = 0;
    history2 = 0;
    ledcWrite(ledChannel, motor); //escrevendo pwm no pino 13
    Serial.print("motor: ");
    Serial.print(motor);
    Serial.print(", Distance: ");
    Serial.println(distanceCm);
  }else if(button2) history2 = 1;

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  //pulse of 10us
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  //collects the pulse width
  duration = pulseIn(ECHO, HIGH);

  distanceCm = duration*SOUND_SPEED/2;

  if(!button3){
    Serial.print("Distance: ");
    Serial.println(distanceCm);
  }
  
  delay(5);
}
