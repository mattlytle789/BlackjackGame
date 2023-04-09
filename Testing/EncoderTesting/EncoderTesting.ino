const int receiver = 13;
int count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(receiver, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(receiver)) {
    count++;
    Serial.println(count);
    delay(500);
  }
}
