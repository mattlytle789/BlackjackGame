const int enMotor2 = 10;
const int enMotor3 = 11;
const int motorPolarity = A3;
const int CVout = 7;
const int dealButton = 9;
bool cardReadyFlag = false;

void setup() {
  pinMode(enMotor2, OUTPUT);
  pinMode(enMotor3, OUTPUT);
  pinMode(motorPolarity, OUTPUT);
  pinMode(CVout, OUTPUT);
  pinMode(dealButton, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  if (digitalRead(dealButton) == 1) {
    // Drop Arm
    digitalWrite(motorPolarity, LOW);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 110);
    delay(400);
    digitalWrite(motorPolarity, HIGH);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);
    delay(300);
    // Drag card
    digitalWrite(motorPolarity, LOW);
    analogWrite(enMotor2, 190);
    analogWrite(enMotor3, 0);
    delay(440);
    digitalWrite(motorPolarity, HIGH);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);
    delay(300);
    // Raise arm 
    digitalWrite(motorPolarity, HIGH);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 140);
    delay(600);
    digitalWrite(motorPolarity, LOW);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);

    digitalWrite(CVout, HIGH);
    receivePlayerCard();
    digitalWrite(CVout, LOW);
  }
}

void receivePlayerCard() {
  Serial.println("reading card...");
  int input  = '0';
  while (true) {
    if (Serial.available()) {
      break;
    }
  }
  Serial.println("check");
  input = Serial.read();
  switch (input) {
    case '1' :
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