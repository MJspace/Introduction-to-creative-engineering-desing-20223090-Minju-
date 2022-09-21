#define PIN_LED 7  

void setup() {
  // put your setup code here, to run once:
  pinMode(7,OUTPUT);
}
void loop() {
  digitalWrite(7,LOW);
  delay(1000);
  for(int i=0; i<=5; i+=1){
    digitalWrite(7,HIGH);
    delay(100);
    digitalWrite(7,LOW);
    delay(100);
  }
  delay(10);
  digitalWrite(7,HIGH);
  while(1) {}
}


