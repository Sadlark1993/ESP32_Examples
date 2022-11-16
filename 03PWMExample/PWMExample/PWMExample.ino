/*
PWM Exemple
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
int history = 1;
int motor = 0;

void setup() {
  // put your setup code here, to run once:
  ledcSetup(ledChannel, frequency, resolution); //setando pwm
  //ledcSetup(ledChannel1, frequency1, resolution1);

  ledcAttachPin(13, ledChannel); //setando pino do pwm
  pinMode(14, INPUT_PULLUP); //setando pullup no pino do botao
}

void loop() {
  // put your main code here, to run repeatedly:

  button = digitalRead(14);
  if(!button && history){ //ao apertar botao no pino 14, o ESP executa isso
    motor = motor+30;
    if(motor>255) motor = 255;
    history = 0;
  }else if(button) history = 1;

  ledcWrite(ledChannel, motor); //Escrevendo PWM no pino 13
  delay(5);
}
