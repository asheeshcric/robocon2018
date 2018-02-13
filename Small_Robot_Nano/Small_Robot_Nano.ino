uint8_t LM1 = 7;
uint8_t LM2 = 8;
uint8_t RM1 = 12;
uint8_t RM2 = 10;
uint8_t LM_Enable = 6;
uint8_t RM_Enable = 11;

uint8_t Servo_Pin = 9;

uint8_t digitalPin1 = 2;    // For LineFollower
uint8_t digitalPin2 = 13;   // For Cleaning Solar Panel

uint8_t MODE = 0;           // To decide the mode in which the robot operates

unsigned long previousMillis = 0;   // For timing purposes to clean the panel
const long interval = 2000;         // Interval the robot to move forward and backward
unsigned long currentMillis;
int panelMode = 1;                  // For going backward and forward


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
  MODE = 0;   // For STOP MODE
  

  myservo.attach(Servo_Pin);
  myservo.write(pos);
  
  esc.attach(3);
  esc.writeMicroseconds(300);
  delay(2000);
  
  Serial.begin(9600);
}



// Main Loop Starts //
void loop() {
  if (digitalRead(digitalPin1) == 1) {
    MODE = 1; // Line following Mode
    Serial.println("MODE 1 Selected - Line Following Mode.........");
  }
  if (digitalRead(digitalPin2) == 1) {
    MODE = 2; // Panel cleaning Mode
    currentMillis = millis();
    Serial.println("MODE 2 Selected - Panel Cleaning Mode.........");
  }
  decide_Mode();    // Operating the robot according to the MODE setup
  
}
// Main Loop Ends //



// Run ESC and Servo
void EscServo() {
  esc.writeMicroseconds(1320);
  if(pos == 0){
    step_size = 10;
  }
  else if(pos == 180){
    step_size = -10;
  }
  pos = pos + step_size;
  myservo.write(pos);
}


// Function that defines the operation of the robot according to the MODE
void decide_Mode() {
  if (MODE == 1) {
    EscServo();
    delay(15);
    line_follower();
    Serial.println("Line Following Mode Running......");
  }
  else if (MODE == 2) {
    EscServo();
    delay(15);
    clean_panel();
    Serial.println("Panel Cleaning Mode Running......");
  }
  else if (MODE == 10) {
    adjust_bot();
    Serial.println("Adjusting the bot on the big robot......");
  }
  else {
    stop_bot();
    Serial.println("The robot is on STATIONARY MODE........");
  }
}


// Line Following Mode
void line_follower()
{
  // Code for Line Follower
  int s1 = analogRead(A7);
  int s2 = analogRead(A6);
  int s3 = analogRead(A5);
  int s4 = analogRead(A4);
  int s5 = analogRead(A3);
  Serial.print(s1);
  Serial.print(" ");
  Serial.print(s2);
  Serial.print(" ");
  Serial.print(s3);
  Serial.print(" ");
  Serial.print(s4);
  Serial.print(" ");
  Serial.print(s5);
  Serial.println(" ");
}


// Panel Cleaning Mode
void clean_panel() {
  // Code for Cleaning the Solar Panel
  if (panelMode == 1) {
    forward(255, 255);
  }
  else if (panelMode == -1) {
    backward(255, 255);
  }

  // Changing the panelMode
  if (currentMillis - previousMillis >= interval) {
    panelMode = panelMode * -1;
    previousMillis = currentMillis;
  }
}

// Robot Adjust Mode
void adjust_bot() {
  // Code for adjusting the bot on the Big Robot
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
