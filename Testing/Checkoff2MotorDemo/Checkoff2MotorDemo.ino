const int v1 = 7;
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
  pinMode(receiver, INPUT_PULLUP);

  PCICR |= B00000001;
  PCMSK0 |= B00100000;

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Turn Motor Base Motor back and forth
  if (digitalRead(startButton) == 1) {
    digitalWrite(v1, HIGH); // HIGH =  :: LOW =
    analogWrite(en1, 0);
    analogWrite(en2, 220); 
    analogWrite(en3, 0); 
    while (receiverCount < 1) {
      Serial.println(receiverCount);
    }
    //delay(2000);
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 0);
    delay(3000);
    // Turn Base Motor opposite direction
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);
    analogWrite(en2, 210);
    analogWrite(en3, 0);
    delay(750);
    digitalWrite(v1, HIGH);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 0);

    delay(3000);

    // Turn extension motor one direction
    digitalWrite(v1, HIGH); // HIGH =  :: LOW =
    analogWrite(en1, 255);
    analogWrite(en2, 0); 
    analogWrite(en3, 0);
    delay(1000);
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 0);
    delay(3000);
    // Turn Base Motor opposite direction
    digitalWrite(v1, LOW);
    analogWrite(en1, 255);
    analogWrite(en2, 0);
    analogWrite(en3, 0);
    delay(1000);
    digitalWrite(v1, HIGH);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 0);

    delay(3000);

    // turn height motor
    digitalWrite(v1, HIGH); // HIGH =  :: LOW =
    analogWrite(en1, 0);
    analogWrite(en2, 0); 
    analogWrite(en3, 100);
    delay(600);
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 0);
    delay(3000);
    // Turn height Motor opposite direction
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 100);
    delay(400);
    digitalWrite(v1, HIGH);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 0);

  }
}

ISR (PCINT0_vect) {
  receiverState = digitalRead(receiver);
  if (receiverState == 1) {
    Serial.println("check");
    receiverCount++;
  }
}
