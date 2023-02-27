const int button1 = 4;
const int button2 = 5;
const int button3 = 6;
volatile int buttonState1;
volatile int buttonState2;
volatile int buttonState3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  PCICR |= B00000100;
  PCMSK2 |= B00010000;
  PCMSK2 |= B00100000;
  PCMSK2 |= B01000000;

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);

  //attachInterrupt(digitalPinToInterrupt(button),buttonPressed,CHANGE);

}

void loop() {
  // put your main code here, to run repeatedly:
}

ISR (PCINT2_vect) {
  buttonState1 = digitalRead(button1);
  buttonState2 = digitalRead(button2);
  buttonState3 = digitalRead(button3);
  if (buttonState1 == 1) {
    Serial.println("button 1 pressed");
  }
  if (buttonState2 == 1) {
    Serial.println("button 2 pressed");
  }
  if (buttonState3 == 1) {
    Serial.println("button 3 pressed");
  }
}