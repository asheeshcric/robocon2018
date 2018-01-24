void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 

}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t s1 = analogRead(A0);
  uint8_t s2= analogRead(A1);
  uint8_t s3 = analogRead(A2);
  uint8_t s4 = analogRead(A3);
  uint8_t s5 = analogRead(A4);
  Serial.print(s1);
  Serial.print("  ");
  Serial.print(s2);
  Serial.print("  ");
  Serial.print(s3);
  Serial.print("  ");
  Serial.print(s4);
  Serial.print("  ");
  Serial.print(s5);
  Serial.println("  ");

}
