const int v1 = 7; // Motor 1 : High = Out, Low = In :: Motor 2 : High = Left, Low = Right :: Motor 3 : High = Raise, Low = Lower
const int en1 = 3; 
const int en2 = 10;
const int en3 = 11;
const int startButton = 5;
const int receiver = 13;
volatile int receiverState = 0;
int receiverCount = 0;
bool readFlag = true;

void setup() {
  // put your setup code here, to run once:
  pinMode(startButton, INPUT_PULLUP);
  pinMode(v1, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);
  pinMode(en3, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(startButton) == 1) {
    // Drop Arm
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 110);
    delay(400);
    digitalWrite(v1, HIGH);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 0);

    delay(1000);

    // Drag card
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);
    analogWrite(en2, 230);
    analogWrite(en3, 0);
    delay(675);
    digitalWrite(v1, HIGH);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 0);

    delay(1000);

    digitalWrite(v1, HIGH);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 110);
    delay(800);
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 0);
  }
}
