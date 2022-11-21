/*
  Interrupcao

  formula da frequencia do timer: timer speed (Hz) = clock speed (Mhz) / prescaler
  a velocidade de clock no ESP32 eh de 80MHz

*/

#define LED 17
hw_timer_t *My_timer = NULL;                      //variavel ponteiro do tipo hw_timer_t
void IRAM_ATTR onTimer(){                         //rotina de interrupcao
  digitalWrite(LED, !digitalRead(LED));           //da um toggle no led
}

void setup() {
  pinMode(LED, OUTPUT);
  My_timer = timerBegin(0, 80, true);             //funcao q inicializa o timer. Argumentos: numero do timer, prescaler, incrementar
  timerAttachInterrupt(My_timer, &onTimer, true); //acoplando a rotina de interrupcao onTimer ao timer My_timer

/*
  Aqui eh a funcao q implementa o contador. Vai contar ate esse numero e gerar a interrucao.
  O valor true significa q ele vai recomecar ao final da contagem, gerando interrupcoes frequentimente.
  Como o preescaler esta com valor 80, o numero nesse contador representa microsegundos.
*/
  timerAlarmWrite(My_timer, 200000, true);
  timerAlarmEnable(My_timer);                     //ativando a interrupcao (enable)

}

void loop() {
  

}
