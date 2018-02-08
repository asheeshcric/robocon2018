uint8_t LM1 = 7;
uint8_t LM2 = 8;
uint8_t RM1 = 12;
uint8_t RM2 = 10;
uint8_t LM_Enable = 6;
uint8_t RM_Enable = 11;

uint8_t Servo_Pin = 9;

uint8_t digitalPin1 = 2;
uint8_t digitalPin2 = 13;


#include <Servo.h>
Servo myservo;
Servo esc;
int pos = 90;
int step_size = 5;

void setup() {
  // put your setup code here, to run once:
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  pinMode(3, OUTPUT);     //For ESC PWM Pin
  pinMode(Servo_Pin, OUTPUT);
  pinMode(LM_Enable, OUTPUT);
  pinMode(RM_Enable, OUTPUT);
  pinMode(digitalPin1, INPUT);
  pinMode(digitalPin2, INPUT);
  

  myservo.attach(Servo_Pin);
  myservo.write(pos);
  
  esc.attach(3);
  esc.writeMicroseconds(300);
  delay(2000);
  
  Serial.begin(9600);
}



// Main Loop Starts //
void loop() {
  esc.writeMicroseconds(1320);
  if(pos == 0){
    step_size = 10;
  }
  else if(pos == 160){
    step_size = -10;
  }
  pos = pos + step_size;
  myservo.write(pos);
  line_follower();
  delay(15);
}
// Main Loop Ends //

void line_follower()
{
  int s1 = analogRead(A7);
  int s2 = analogRead(A6);
  int s3 = analogRead(A5);
  int s4 = analogRead(A4);
  int s5 = analogRead(A3);
//  Serial.println(s1,s2,s3,s4,s5);/
}

void forward(int LM_PWM, int RM_PWM)
{
  digitalWrite(LM1, 1);
  digitalWrite(LM2, 0);
  digitalWrite(RM1, 1);
  digitalWrite(RM2, 0);

  analogWrite(LM_Enable, LM_PWM);
  analogWrite(RM_Enable, RM_PWM);
}

void backward(int LM_PWM, int RM_PWM)
{
  digitalWrite(LM1, 0);
  digitalWrite(LM2, 1);
  digitalWrite(RM1, 0);
  digitalWrite(RM2, 1);

  analogWrite(LM_Enable, LM_PWM);
  analogWrite(RM_Enable, RM_PWM);
}

void right(int LM_PWM, int RM_PWM)
{
  digitalWrite(LM1, 1);
  digitalWrite(LM2, 0);
  digitalWrite(RM1, 1);
  digitalWrite(RM2, 1);

  analogWrite(LM_Enable, LM_PWM);
  analogWrite(RM_Enable, RM_PWM);
}

void left(int LM_PWM, int RM_PWM)
{
  digitalWrite(LM1, 1);
  digitalWrite(LM2, 1);
  digitalWrite(RM1, 1);
  digitalWrite(RM2, 0);

  analogWrite(LM_Enable, LM_PWM);
  analogWrite(RM_Enable, RM_PWM);
}

void stop_bot()
{
  digitalWrite(LM1, 1);
  digitalWrite(LM2, 1);
  digitalWrite(RM1, 1);
  digitalWrite(RM2, 1);

  analogWrite(LM_Enable, 0);
  analogWrite(RM_Enable, 0);
}
