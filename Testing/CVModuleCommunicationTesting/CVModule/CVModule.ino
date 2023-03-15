const int controllerIn = 7;
const int CVout = A3;

volatile int input = 0;

int count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(controllerIn, INPUT);
  pinMode(CVout, OUTPUT);
  randomSeed(A0);
}

void loop() {
  // put your main code here, to run repeatedly:
  input = digitalRead(controllerIn);
  if (input == 1) {
    analogWrite(CVout, random(13)+1);
  }
  else {
    digitalWrite(CVout, LOW);
  }
}
