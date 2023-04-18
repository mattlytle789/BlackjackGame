const int receiver = 13;
int count = 0;
volatile int receiverState = 0;
volatile int receiverCount = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(receiver, INPUT_PULLUP);
  Serial.begin(9600);

  PCICR |= B00000001;
  PCMSK0 |= B00100000;
}

void loop() {
  // put your main code here, to run repeatedly:
  /*if (digitalRead(receiver)) {
    count++;
    Serial.println(count);
    delay(500);
  }*/
  Serial.println(digitalRead(receiver));
}
/*
ISR (PCINT0_vect) {
  receiverState = digitalRead(receiver);
  Serial.println(receiverState);
  if (receiverState == 1) {
  }
}*/
