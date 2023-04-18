const int enMotor1 = 3;
const int enMotor2 = 10;
const int enMotor3 = 11;
const int motorPolarity = A3;
const int CVout = 7;
const int dealButton = 9;
bool cardReadyFlag = false;
volatile int receiverCount = 0;
volatile int receiverState = 0;
const int receiver = 13;

void setup() {
  pinMode(enMotor1, OUTPUT);
  pinMode(enMotor2, OUTPUT);
  pinMode(enMotor3, OUTPUT);
  pinMode(motorPolarity, OUTPUT);
  pinMode(CVout, OUTPUT);
  pinMode(dealButton, INPUT_PULLUP);
  pinMode(receiver, INPUT_PULLUP);
  PCICR |= B00000001;
  PCMSK0 |= B00100000;
  Serial.begin(115200);
}

void loop() {
  if (digitalRead(dealButton) == 1) {
    // Drop Arm
    digitalWrite(motorPolarity, LOW);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 100);
    delay(350);
    digitalWrite(motorPolarity, HIGH);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);
    delay(300);
    /*// Drag card
    digitalWrite(motorPolarity, LOW);
    analogWrite(enMotor2, 210);
    analogWrite(enMotor3, 0);
    delay(420);
    digitalWrite(motorPolarity, HIGH);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);
    delay(300);*/

    while (Serial.available() > 0) {
      Serial.read();
    }
    digitalWrite(CVout, HIGH);
    receivePlayerCard();
    digitalWrite(CVout, LOW);
    
    // Drag arm to player 1
    digitalWrite(motorPolarity, LOW);
    analogWrite(enMotor2, 200);
    analogWrite(enMotor3, 0);
    while (receiverCount < 1) {
      continue;
    }
    digitalWrite(motorPolarity, HIGH);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);
    delay(300);
    // Extend arm
    digitalWrite(motorPolarity, HIGH);
    analogWrite(enMotor1, 110);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);
    delay(475);
    digitalWrite(motorPolarity, LOW);
    analogWrite(enMotor1, 0);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);
    delay(300);
    // Raise arm
    digitalWrite(motorPolarity, HIGH);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 140);
    delay(545);
    digitalWrite(motorPolarity, LOW);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);
    delay(300);
    // Retract arm 
    digitalWrite(motorPolarity, LOW);
    analogWrite(enMotor1, 100);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);
    delay(555);
    digitalWrite(motorPolarity, HIGH);
    analogWrite(enMotor1, 0);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);
    delay(300);
    // Return to deck
    receiverCount = 0;
    digitalWrite(motorPolarity, HIGH);
    analogWrite(enMotor2, 200);
    analogWrite(enMotor3, 0);
    while (receiverCount < 1) {
      continue;
    }
    digitalWrite(motorPolarity, LOW);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);
    delay(300);
    // go to reader
    digitalWrite(motorPolarity, LOW);
    analogWrite(enMotor2, 220);
    analogWrite(enMotor3, 0);
    delay(380);
    digitalWrite(motorPolarity, HIGH);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);    
  }
  receiverCount = 0;
}

void receivePlayerCard() {
  Serial.println("reading card...");
  int input  = '0';
  while (true) {
    if (Serial.available()) {
      break;
    }
  }
  input = Serial.read();
  switch (input) {
    case 'A' :
      Serial.println("Received A");
      break;
    case '2' :
      Serial.println("Received 2");
      break;
    case '3' :
      Serial.println("Received 3");
      break;
    case '4' :
      Serial.println("Received 4");
      break;
    case '5' :
      Serial.println("Received 5");
      break;
    case '6' :
      Serial.println("Received 6");
      break;
    case '7' :
      Serial.println("Received 7");
      break;
    case '8' :
      Serial.println("Received 8");
      break;
    case '9' :
      Serial.println("Received 9");
      break;
    case 'T' :
      Serial.println("Received 10");
      break;
    case 'J' :
      Serial.println("Received J");
      break;
    case 'Q' :
      Serial.println("Received Q");
      break;
    case 'K' :
      Serial.println("Received K");
      break;
    default :
      Serial.println("ERROR! Bad Card Read");
      break;
  }
}

ISR (PCINT0_vect) {
    receiverState = digitalRead(receiver);
    if (receiverState == 1) {
        receiverCount++;
    }
}