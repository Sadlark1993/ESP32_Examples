void setup() {
  pinMode(4, INPUT);
  pinMode(13, OUTPUT);
  Serial.begin(110200);
}

void loop() {
  if(analogRead(4)<800){
    digitalWrite(13, HIGH);
  }else{
    digitalWrite(13,LOW);
  }
  Serial.println(analogRead(4));
  
}
