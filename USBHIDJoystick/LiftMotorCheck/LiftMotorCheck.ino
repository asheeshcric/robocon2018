uint8_t LiftM1 = 3;
uint8_t LiftM2 = 5;
uint8_t Lift_Enable = 6;

void setup() {
  // put your setup code here, to run once:
  pinMode(LiftM1, OUTPUT);
  pinMode(LiftM2, OUTPUT);
  pinMode(Lift_Enable, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LiftM1, 1);
  digitalWrite(LiftM2, 0);
  analogWrite(Lift_Enable, 100);
  delay(1000);
  digitalWrite(LiftM1, 0);
  digitalWrite(LiftM2, 1);
  analogWrite(Lift_Enable, 100);
  delay(1000);
}
