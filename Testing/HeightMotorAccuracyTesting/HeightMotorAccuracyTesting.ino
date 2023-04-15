// Motor 1 : High = Out, Low = In :: Motor 2 : High = Left, Low = Right :: Motor 3 : High = Raise, Low = Lower
const int v1 = 7; 
const int en1 = 3; 
const int en2 = 10;
const int en3 = 11;
const int startButton = 5;

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
    //while (true) {
      // lower arm 
      digitalWrite(v1, LOW);
      analogWrite(en3, 130);
      delay(300);
      digitalWrite(v1, HIGH);
      analogWrite(en3, 0);
        
      delay(1000);

      // raise arm 
      digitalWrite(v1, HIGH);
      analogWrite(en3, 130);
      delay(800);
      digitalWrite(v1, LOW);
      analogWrite(en3, 0);

      delay(1000);
      count++;
      Serial.println(count);
    //}
  }

}
