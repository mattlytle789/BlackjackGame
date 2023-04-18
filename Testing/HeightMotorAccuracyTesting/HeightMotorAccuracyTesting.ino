// Motor 1 : High = Out, Low = In :: Motor 2 : High = Left, Low = Right :: Motor 3 : High = Raise, Low = Lower
const int v1 = A3; 
const int en1 = 3; 
const int en2 = 10;
const int en3 = 11;
const int startButton = 9;

int count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(startButton, INPUT_PULLUP);
  pinMode(v1, OUTPUT);
  pinMode(en3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(startButton)) {
      // Lower arm 
      digitalWrite(v1, LOW);
      analogWrite(en3, 110);
      delay(375);
      digitalWrite(v1, HIGH);
      analogWrite(en3, 0);
        
      delay(1000);

      // Raise arm 
      digitalWrite(v1, HIGH);
      analogWrite(en3, 140);
      delay(450);
      digitalWrite(v1, LOW);
      analogWrite(en3, 0);

      delay(1000);
      //count++;
      //Serial.println(count);
    //}
  }

}
