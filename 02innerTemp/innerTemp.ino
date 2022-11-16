
extern "C"{ //extern "C" is used when we are using a function written in C language inside a C++ language code
  uint8_t temprature_sens_read();
}

  uint8_t temprature_sens_read();

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  Serial.print("Temperatura em graus Celcius: ");
  Serial.print((temprature_sens_read()-32)/1.8);
  Serial.println(" C");
  delay(5000);
}
