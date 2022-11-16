/*
Toda vez que alguém apertar o botao no pino 14, o PWM do pino 13 aumenta;
toda vez que alguem apertar o botao no pino 19, o PWM do pino 13 diminui.
*/

int frequency = 5000;
int ledChannel = 0;
int resolution = 8;

//for a second pin
/*
int frequency1 = 6000;
int ledChannel1 = 1;
int resolution1 = 8;
*/

int button;
int button2;
int history = 1;
int history2 = 1;
int motor = 0;

void setup() {
  // put your setup code here, to run once:
  ledcSetup(ledChannel, frequency, resolution);
  //ledcSetup(ledChannel1, frequency1, resolution1);

  ledcAttachPin(13, ledChannel); //pino do PWM
  //ledcAttachPin(4, ledChannel1);
  pinMode(14, INPUT_PULLUP); //botao
  pinMode(19, INPUT_PULLUP); //botao
}

void loop() {
  // put your main code here, to run repeatedly:

  button2 = digitalRead(19); //botao diminui pwm
  button = digitalRead(14); //botao aumenta pwm

  if(!button && history){ //aumentar
    motor = motor+2;
    if(motor>255) motor = 255;
    history = 0;
    ledcWrite(ledChannel, motor); //escrevendo pwm no pino 13
  }else if(button) history = 1;

  if(!button2 && history2){ //diminuir
    motor = motor-2;
    if(motor < 0) motor = 0;
    history2 = 0;
    ledcWrite(ledChannel, motor); //escrevendo pwm no pino 13
  }else if(button2) history2 = 1;

  //ledcWrite(ledChannel, motor);
  delay(5);
}
