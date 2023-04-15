// Motor 1 : High = Out, Low = In :: Motor 2 : High = Left, Low = Right :: Motor 3 : High = Raise, Low = Lower
// Delay to extend to full extension 1000
// Delay to retract from full extension 1400
const int v1 = 7; 
const int en1 = 3; 
const int en2 = 10;
const int en3 = 11;
const int startButton = 5;

void setup() {
  // put your setup code here, to run once:
  pinMode(startButton, INPUT_PULLUP);
  pinMode(v1, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(en3, OUTPUT);
}

void loop() {
  if (digitalRead(startButton)) {
    // Extend arm
    digitalWrite(v1, HIGH);
    analogWrite(en1, 255);
    delay(1000);
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);

    delay(1000);

    digitalWrite(v1, HIGH);
    analogWrite(en2, 0);
    analogWrite(en3, 140);
    delay(725);
    digitalWrite(v1, LOW);
    analogWrite(en2, 0);
    analogWrite(en3, 0);

    delay(1000);

    // Retract arm
    digitalWrite(v1, LOW);
    analogWrite(en1, 255);
    delay(1050);
    digitalWrite(v1, HIGH);
    analogWrite(en1, 0);

    delay(1000);

    // Drop Arm
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 130);
    delay(300);
    digitalWrite(v1, HIGH);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 0);

  }
}
