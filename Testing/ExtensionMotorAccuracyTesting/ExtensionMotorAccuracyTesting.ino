// Motor 1 : High = Out, Low = In :: Motor 2 : High = Left, Low = Right :: Motor 3 : High = Raise, Low = Lower
// Delay to extend to full extension 1000
// Delay to retract from full extension 1400
const int v1 = A3; 
const int en1 = 3; 
const int en2 = 10;
const int en3 = 11;
const int startButton = 9;

void setup() {
  // put your setup code here, to run once:
  pinMode(startButton, INPUT_PULLUP);
  pinMode(v1, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(en3, OUTPUT);
}

void loop() {
  if (digitalRead(startButton)) {
    while(true) {
      // Extend arm
      digitalWrite(v1, HIGH);
      analogWrite(en1, 120);
      delay(480);
      digitalWrite(v1, LOW);
      analogWrite(en1, 0);
      /*
      // Retract arm
      digitalWrite(v1, LOW);
      analogWrite(en1, 120);
      delay(500);
      digitalWrite(v1, HIGH);
      analogWrite(en1, 0);
      */
      delay(500);
      /*
      // Raise Arm Extended
      digitalWrite(v1, HIGH);
      analogWrite(en3, 140);
      delay(520);
      digitalWrite(v1, LOW);
      analogWrite(en3, 0);
      */
      /*
      // Raise Arm Retracted
      digitalWrite(v1, HIGH);
      analogWrite(en3, 140);
      delay(450);
      digitalWrite(v1, LOW);
      analogWrite(en3, 0);
      */

      delay(500);

      // Retract arm
      digitalWrite(v1, LOW);
      analogWrite(en1, 120);
      delay(555);
      digitalWrite(v1, HIGH);
      analogWrite(en1, 0);
      /*
      // Extend Arm 
      digitalWrite(v1, HIGH);
      analogWrite(en1, 110);
      delay(430);
      digitalWrite(v1, LOW);
      analogWrite(en1, 0);   
      */ 

      delay(500);
      /*
      // Drop Arm
      digitalWrite(v1, LOW);
      analogWrite(en3, 100);
      delay(300);
      digitalWrite(v1, HIGH);
      analogWrite(en3, 0);
      */
    }
  }
}
