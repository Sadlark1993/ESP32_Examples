

#include "PID_v1.h"

int frequency = 5000;
int ledChannel = 0;
int resolution = 8;
long duration = 1;
float distanceCm = 0;
//double velocidade = 0; //cm/s
int encoder = 0;

float distanceSum;
float actualDistance;
float lastDistance = 0;
int rotationFlag = 1;
int indice = 0;
int rotationCount = 0;
double rotationCount2 =0;
int flagInt = 0;

//#define TRIG 18 //pino trig
#define ECHO 4 //pino receptor infravermelho
#define ALVO 8 //setpoint
//#define SOUND_SPEED 0.035 // cm/us
int echo;

int button; //+pwm
int button2; //-pwm
int button3; //show distance
int history = 0.8;
int history2 = 1;
double motor = 40;

double setpoint; //para o PID
double kp= 2.4;
double pi = 0.6;
double kd = 0.1;
PID myPID(&rotationCount2, &motor, &setpoint, kp, pi, kd, DIRECT);

//interrupcao -----------------------------------------------------------------------------------
#define LED_INT 17
hw_timer_t *My_timer = NULL;                      //variavel ponteiro do tipo hw_timer_t
void IRAM_ATTR onTimer(){                         //rotina de interrupcao
  timerAlarmDisable(My_timer);
  digitalWrite(LED_INT, !digitalRead(LED_INT));   //da um toggle no led

  
  rotationCount2 = rotationCount;
  rotationCount = 0;

  flagInt=1;

  timerAlarmEnable(My_timer);
}

void setup() {
  
  
  ledcSetup(ledChannel, frequency, resolution);

  ledcAttachPin(13, ledChannel); //pino do PWM
  pinMode(14, INPUT_PULLUP); //botao
  pinMode(19, INPUT_PULLUP); //botao
  pinMode(16, OUTPUT);
  //pinMode(TRIG, OUTPUT);
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
  timerAlarmWrite(My_timer, 50000, true);
  timerAlarmEnable(My_timer);                     //ativando a interrupcao (enable)
  //*****************configurando interrupcao - fim***********************

  /*
    O disco tem 24 dentes. O alvo é mais ou menos 5 voltas por segundo. -> 120 dentes por segundo.
    A cada 100ms, devem passar 12 dentes pelo sensor.
  */
  setpoint = ALVO;
  myPID.SetMode(AUTOMATIC);


}

void loop() {
  // put your main code here, to run repeatedly:

  //button2 = digitalRead(19); //botao diminui pwm
  button = digitalRead(14); //botao aumenta pwm
  //button3 = digitalRead(16); //botao show distance


  if(!button){ //desligar
    timerAlarmDisable(My_timer);
    ledcWrite(ledChannel, 0);
    history = 0;
    while( digitalRead(19)){
      echo = analogRead(ECHO);
      if(echo<1000){
        digitalWrite(16, HIGH);
      } else {
        digitalWrite(16, LOW);
      }
      delay(2);
    }
    timerAlarmEnable(My_timer);
  }

  
  echo = analogRead(ECHO);
  if(echo<800){
    encoder = 0;
  }
  if(echo>800 && !encoder){
    encoder = 1;
    rotationCount++;
  }

  

  //Serial.println(analogRead(ECHO));


  if(flagInt){
    ledcWrite(ledChannel, motor);
    myPID.Compute();
    Serial.print("RPS: ");
    Serial.println(rotationCount2);
    Serial.print("motor: ");
    Serial.println(motor);
    flagInt = 0;
    if(rotationCount2>ALVO) digitalWrite(16, HIGH);
    else digitalWrite(16, LOW);
  }


  //delay(1);
  delayMicroseconds(50);
}