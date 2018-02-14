//uint8_t LM1 = 7;
//uint8_t LM2 = 8;
//uint8_t RM1 = 12;
//uint8_t RM2 = 10;
//uint8_t LM_Enable = 6;
//uint8_t RM_Enable = 11;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int s1 = analogRead(A7);
  Serial.println(s1);
  
}

//void forward(int LM_PWM, int RM_PWM)
//{
//  digitalWrite(LM1, 1);
//  digitalWrite(LM2, 0);
//  digitalWrite(RM1, 1);
//  digitalWrite(RM2, 0);
//
//  analogWrite(LM_Enable, LM_PWM);
//  analogWrite(RM_Enable, RM_PWM);
//}
//
//void backward(int LM_PWM, int RM_PWM)
//{
//  digitalWrite(LM1, 0);
//  digitalWrite(LM2, 1);
//  digitalWrite(RM1, 0);
//  digitalWrite(RM2, 1);
//
//  analogWrite(LM_Enable, LM_PWM);
//  analogWrite(RM_Enable, RM_PWM);
//}
//
//void right(int LM_PWM, int RM_PWM)
//{
//  digitalWrite(LM1, 1);
//  digitalWrite(LM2, 0);
//  digitalWrite(RM1, 1);
//  digitalWrite(RM2, 1);
//
//  analogWrite(LM_Enable, LM_PWM);
//  analogWrite(RM_Enable, RM_PWM);
//}
//
//void left(int LM_PWM, int RM_PWM)
//{
//  digitalWrite(LM1, 1);
//  digitalWrite(LM2, 1);
//  digitalWrite(RM1, 1);
//  digitalWrite(RM2, 0);
//
//  analogWrite(LM_Enable, LM_PWM);
//  analogWrite(RM_Enable, RM_PWM);
//}
//
//void stop_bot()
//{
//  digitalWrite(LM1, 1);
//  digitalWrite(LM2, 1);
//  digitalWrite(RM1, 1);
//  digitalWrite(RM2, 1);
//
//  analogWrite(LM_Enable, 0);
//  analogWrite(RM_Enable, 0);
//}
