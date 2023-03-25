const int v1 = 7;
const int en1 = A0;
const int en2 = A1;
const int en3 = A2;

void setup() {
  // put your setup code here, to run once:
  pinMode(v1, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);
  pinMode(en3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Turn Motor 1 on 
  digitalWrite(v1, HIGH); // HIGH =  :: LOW =
  analogWrite(en1, 150);
  analogWrite(en2, 0); 
  analogWrite(en3, 0);
  delay(2000);
  analogWrite(en1, 0);

  // turn Motor 2 on
  analogWrite(en1, 0);
  analogWrite(en2, 100); 
  analogWrite(en3, 0);
  delay(2000);
  analogWrite(en2, 0);

  // turn Motor 3 on 
  analogWrite(en1, 0);
  analogWrite(en2, 0); 
  analogWrite(en3, 255);
  delay(2000);
  analogWrite(en3, 0);

  delay(5000);
}
