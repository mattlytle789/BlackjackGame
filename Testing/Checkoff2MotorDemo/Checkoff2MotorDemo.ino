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
  pinMode(en2, OUTPUT);
  pinMode(en3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Turn Motor Base Motor back and forth
  if (digitalRead(startButton) == 1) {
    digitalWrite(v1, HIGH); // HIGH =  :: LOW =
    analogWrite(en1, 0);
    analogWrite(en2, 255); 
    delay(2000);
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    delay(3000);
    // Turn Base Motor opposite direction
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);
    analogWrite(en2, 255);
    delay(2000);
    digitalWrite(v1, HIGH);
    analogWrite(en1, 0);
    analogWrite(en2, 0);

    delay(3000);

    // Turn extension motor one direction
    digitalWrite(v1, HIGH); // HIGH =  :: LOW =
    analogWrite(en1, 255);
    analogWrite(en2, 0); 
    delay(2000);
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    delay(3000);
    // Turn Base Motor opposite direction
    digitalWrite(v1, LOW);
    analogWrite(en1, 255);
    analogWrite(en2, 0);
    delay(2000);
    digitalWrite(v1, HIGH);
    analogWrite(en1, 0);
    analogWrite(en2, 0);

  }
}
