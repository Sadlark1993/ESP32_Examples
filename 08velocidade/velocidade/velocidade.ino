/*
Toda vez que alguém apertar o botao no pino 14, o PWM do pino 13 aumenta;
toda vez que alguem apertar o botao no pino 19, o PWM do pino 13 diminui.
*/

int frequency = 5000;
int ledChannel = 0;
int resolution = 8;
long duration = 1;
float distanceCm = 0;
double velocidade = 0; //cm/s

float distanceSum;
float actualDistance;
float lastDistance = 0;
int rotationFlag = 1;
int indice = 0;
int rotationCount = 0;

#define TRIG 18 //pino trig
#define ECHO 4 //pino echo
#define SOUND_SPEED 0.035 // cm/us

int button; //+pwm
int button2; //-pwm
int button3; //show distance
int history = 1;
int history2 = 1;
int motor = 30;

//interrupcao
#define LED_INT 17
hw_timer_t *My_timer = NULL;                      //variavel ponteiro do tipo hw_timer_t
void IRAM_ATTR onTimer(){                         //rotina de interrupcao
  timerAlarmDisable(My_timer);
  digitalWrite(LED_INT, !digitalRead(LED_INT));   //da um toggle no led

  Serial.print("RPS: ");
  Serial.println(rotationCount);
  rotationCount = 0;

  timerAlarmEnable(My_timer);
}

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

  //******************configurando interrupcao - inicio****************
  pinMode(LED_INT, OUTPUT);
  My_timer = timerBegin(0, 80, true);             //funcao q inicializa o timer. Argumentos: numero do timer, prescaler, incrementar
  timerAttachInterrupt(My_timer, &onTimer, true); //acoplando a rotina de interrupcao onTimer ao timer My_timer

/*
  Aqui eh a funcao q implementa o contador. Vai contar ate esse numero e gerar a interrucao.
  O valor true significa q ele vai recomecar ao final da contagem, gerando interrupcoes frequentimente.
  Como o preescaler esta com valor 80, o numero nesse contador representa microsegundos.
*/
  timerAlarmWrite(My_timer, 1000000, true);
  timerAlarmEnable(My_timer);                     //ativando a interrupcao (enable)
  //*****************configurando interrupcao - fim***********************

}

void loop() {
  // put your main code here, to run repeatedly:

  button2 = digitalRead(19); //botao diminui pwm
  button = digitalRead(14); //botao aumenta pwm
  button3 = digitalRead(16); //botao show distance

/*

  if(indice<8){
    digitalWrite(TRIG, LOW);                        //limpa o trig
    delayMicroseconds(2);

    //pulse of 10us
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(5);
    digitalWrite(TRIG, LOW);

    //collects the pulse width
    duration = pulseIn(ECHO, HIGH);
    distanceSum = distanceSum+duration*SOUND_SPEED/2;

    indice++;

  }else{
    distanceCm = distanceSum/8;
    distanceSum = 0;
    indice = 0;
  }
*/


  digitalWrite(TRIG, LOW);                        //limpa o trig
  delayMicroseconds(2);

  //pulse of 10us
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIG, LOW);

  //collects the pulse width
  duration = pulseIn(ECHO, HIGH);
  distanceCm = duration*SOUND_SPEED/2;

  if(distanceCm>7){
    rotationFlag = 0;
  }else if(!rotationFlag){
    rotationCount++;
    rotationFlag = 1;
  }
  



  if(!button && history){ //aumentar
    motor = motor+2;
    if(motor>255) motor = 255;
    history = 0;
    ledcWrite(ledChannel, motor); //escrevendo pwm no pino 13

    Serial.print("motor: ");
    Serial.print(motor);
    Serial.print(", Distance: ");
    Serial.print(distanceCm);
    Serial.print(", Velocidade: ");
    Serial.println(velocidade);
  }else if(button) history = 1;

  if(!button2 && history2){ //diminuir
    motor = motor-2;
    if(motor < 0) motor = 0;
    history2 = 0;
    ledcWrite(ledChannel, motor); //escrevendo pwm no pino 13

    Serial.print("motor: ");
    Serial.print(motor);
    Serial.print(", Distance: ");
    Serial.print(distanceCm);
    Serial.print(", Velocidade: ");
    Serial.println(velocidade);
  }else if(button2) history2 = 1;

  if(!button3){

    Serial.print("Distance: ");
    Serial.print(distanceCm);
    Serial.print(", Velocidade: ");
    Serial.println(velocidade);
  }
  
  delay(2);
}