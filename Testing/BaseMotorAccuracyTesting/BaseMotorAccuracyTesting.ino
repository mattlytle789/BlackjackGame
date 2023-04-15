// Motor 1 : High = Out, Low = In :: Motor 2 : High = Left, Low = Right :: Motor 3 : High = Raise, Low = Lower
const int v1 = 7; 
const int en1 = 3; 
const int en2 = 10;
const int en3 = 11;
const int startButton = 5;
const int receiver = 13;
volatile int receiverState = 0;
int receiverCount = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(startButton, INPUT_PULLUP);
  pinMode(receiver, INPUT_PULLUP);
  pinMode(en2, OUTPUT);
  pinMode(v1, OUTPUT);

  PCICR |= B00000001;
  PCMSK0 |= B00100000;

  Serial.begin(9600);
}

void loop() {
  if (digitalRead(startButton)) {
    //turn motor right
    digitalWrite(v1, LOW); 
    analogWrite(en2, 210);
    while (receiverCount < 3) {
      Serial.println(receiverCount);
    }
    digitalWrite(v1, HIGH);
    analogWrite(en2, 0);
  }
  receiverCount = 0;
}

ISR (PCINT0_vect) {
  receiverState = digitalRead(receiver);
  if (receiverState == 1) {
    receiverCount++;
  }
}
