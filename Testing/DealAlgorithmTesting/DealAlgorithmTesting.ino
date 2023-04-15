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
  pinMode(receiver, INPUT_PULLUP);
  pinMode(v1, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);
  pinMode(en3, OUTPUT);

  PCICR |= B00000001;
  PCMSK0 |= B00100000;

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(startButton) == 1) {
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

    delay(1000);

    // Drag card to card reader
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);
    analogWrite(en2, 200);
    analogWrite(en3, 0);
    /*while (receiverCount < 2) {
      Serial.println(receiverCount);
      if (receiverCount > 1) {
        break;
      }
    }*/
    delay(450);
    digitalWrite(v1, HIGH);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 0);
    receiverCount = 0;

    delay(1000);
    Serial.println(receiverCount);
    
    // drag card to player
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);
    analogWrite(en2, 200);
    analogWrite(en3, 0);
    while (receiverCount < 1) {
      Serial.println(receiverCount);
      /*if (receiverCount > 1) {
        break;
      }*/
    }
    digitalWrite(v1, HIGH);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 0);
    receiverCount = 0;

    delay(1000);

    // extend arm
    digitalWrite(v1, HIGH);
    analogWrite(en1, 255);
    analogWrite(en2, 0);
    analogWrite(en3, 0);
    delay(1150);
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 0);

    delay(1000);

    // Raise Arm
    digitalWrite(v1, HIGH);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 140);
    delay(725);
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 0);

    delay(1000);

    // retract arm
    digitalWrite(v1, LOW);
    analogWrite(en1, 255);
    analogWrite(en2, 0);
    analogWrite(en3, 0);
    delay(1015);
    digitalWrite(v1, HIGH);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 0);

    delay(1000);

    // Swing arm 
    digitalWrite(v1, HIGH);
    analogWrite(en1, 0);
    analogWrite(en2, 200);
    analogWrite(en3, 0);
    while (receiverCount < 1) {
      Serial.println(receiverCount);
    }
    digitalWrite(v1, LOW);
    analogWrite(en1, 0);
    analogWrite(en2, 0);
    analogWrite(en3, 0);
    receiverCount = 0;
  }
}

ISR (PCINT0_vect) {
  receiverState = digitalRead(receiver);
  Serial.println("check");
  if (receiverState == 1) {
    receiverCount++;
  }
}
