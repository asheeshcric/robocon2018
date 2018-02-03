uint8_t LM1 = 7;
uint8_t LM2 = 8;
uint8_t RM1 = 12;
uint8_t RM2 = 10;
uint8_t LM_Enable = 6;
uint8_t RM_Enable = 11;
uint8_t ESC1 = 3;
uint8_t ESC2 = 5;

uint8_t Servo_Pin = 9;

uint8_t digitalPin1 = 2;
uint8_t digitalPin2 = 13;

//For ESCs:
int STATE=1;
int Arming_time=00;
int Pulse=1000;


#include <Servo.h>
Servo myservo;
Servo esc1;
Servo esc2;
int pos = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  pinMode(ESC1, OUTPUT);
  pinMode(ESC2, OUTPUT);
  pinMode(Servo_Pin, OUTPUT);
  pinMode(LM_Enable, OUTPUT);
  pinMode(RM_Enable, OUTPUT);
  pinMode(digitalPin1, INPUT);
  pinMode(digitalPin2, INPUT);
  
  
  myservo.attach(Servo_Pin);

  //Setting up ESC:
  setup_ESC(ESC1);
  setup_ESC(ESC2);  
  Serial.begin(9600);
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    run_ESC(ESC1);
    run_ESC(ESC2);   
    forward(200, 200);         
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    run_ESC(ESC1);
    run_ESC(ESC2);   
    forward(200, 200);
  }
}

//Function that sets up the ESCs
void setup_ESC(int pin){
  for(Arming_time=0; Arming_time<500;Arming_time += 1)
   {  digitalWrite(pin,HIGH);
      delayMicroseconds(1100);
      digitalWrite(pin,LOW);
      delay(20-(Pulse/1000));
   }
}

//Function to run the ESC motors:
void run_ESC(int pin){
  for( Pulse=1150; Pulse <=1400; Pulse +=1)
 {  digitalWrite(pin,HIGH);
    delayMicroseconds(Pulse);
    digitalWrite(pin,LOW);
    delay(20-(Pulse/1000));
 }
// for( Pulse=1400; Pulse >=1150; Pulse -=1)
// {  digitalWrite(pin,HIGH);
//    delayMicroseconds(Pulse);
//    digitalWrite(pin,LOW);
//    delay(20-(Pulse/1000));
// }
}


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
