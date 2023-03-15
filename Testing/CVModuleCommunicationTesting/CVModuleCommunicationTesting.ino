const int controllerOut = 13;
const int CVin = A0;

volatile int input = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(CVin, INPUT);
  pinMode(controllerOut, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(controllerOut, HIGH);
  input = analogRead(CVin);
  if (input > 0) {
    digitalWrite(controllerOut, LOW);
    Serial.println(input);
  }
  delay(5000);
}
