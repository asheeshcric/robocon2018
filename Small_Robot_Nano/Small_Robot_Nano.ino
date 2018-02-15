uint8_t RM1 = 23;
uint8_t RM2 = 22;
uint8_t LM1 = 31;
uint8_t LM2 = 25;
uint8_t LM_Enable = 8;
uint8_t RM_Enable = 7;

uint8_t Servo_Pin = 6;

uint8_t digitalPin1 = 52;    // For LineFollower
uint8_t digitalPin2 = 53;   // For Cleaning Solar Panel

uint8_t MODE = 0;           // To decide the mode in which the robot operates

unsigned long previousMillis = 0;   // For timing purposes to clean the panel
const long interval = 1500;         // Interval the robot to move forward and backward
unsigned long currentMillis;
int panelModeCount = 0;                  // For going changing the operation in the Panel Clean Mode
int countEndFlag = 0;                    // To determine whether the count cycle is over and needs to restart again or not.


// For line following mode
int lineFollowingMode = 0;
uint8_t RR1 = 0;
uint8_t GG1 = 0;
uint8_t BB1 = 0;
uint8_t RR2 = 0;
uint8_t GG2 = 0;
uint8_t BB2 = 0;
int frequency = 0;

// Sensor Pins
uint8_t AS0 = 44;
uint8_t AS1 = 46;
uint8_t AS22 = 48;
uint8_t AS3 = 50;
uint8_t AOutput = 42;

uint8_t BS0 = 45;
uint8_t BS1 = 47;
uint8_t BS2 = 49;
uint8_t BS3 = 51;
uint8_t BOutput = 43;

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

  // Setting up the pins for the sensor
  pinMode(AS0, OUTPUT);
  pinMode(AS1, OUTPUT);
  pinMode(AS22, OUTPUT);
  pinMode(AS3, OUTPUT);
  pinMode(AOutput, INPUT);

  pinMode(BS0, OUTPUT);
  pinMode(BS1, OUTPUT);
  pinMode(BS2, OUTPUT);
  pinMode(BS3, OUTPUT);
  pinMode(BOutput, INPUT);

  // Setting frequency-scaling to 20%
  digitalWrite(AS0,HIGH);
  digitalWrite(AS1,LOW);
  delay(100);
  digitalWrite(BS0,HIGH);
  digitalWrite(BS1,LOW);
  
  myservo.attach(Servo_Pin);
  myservo.write(pos);
  
  esc.attach(5);
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
    previousMillis = 0;
    panelModeCount = 0;
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
//    /EscServo();
    delay(15);
    lineFollowingMode = 0;
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
  findColor(AS0, AS1, AS22, AS3, AOutput, 1);  // For the first sensor
  Serial.print(RR1);
  Serial.print(" ");
  Serial.print(GG1);
  Serial.print(" ");
  Serial.print(BB1);
  Serial.print("  -- --  ");
  findColor(BS0, BS1, BS2, BS3, BOutput, 2);  // For the second sensor
  Serial.print(RR2);
  Serial.print(" ");
  Serial.print(GG2);
  Serial.print(" ");
  Serial.println(BB2);
  if (BB1 > 50 && BB2 < 50) {
    right(150);
    lineFollowingMode = 3;
  }
  else if (BB1 < 50 && BB2 > 50) {
    left(150);
    lineFollowingMode = 2;
  }
  else if (BB1 > 50 && BB2 > 50) {
    forward(200, 200);
    lineFollowingMode = 1;
  }
  else if (BB1 < 50 && BB2 < 50) {
    forward(200, 200);
    lineFollowingMode = 1;
  }
  else {
    if (lineFollowingMode == 1) {
      backward(200, 200);
    }
    else if (lineFollowingMode == 2) {
      right(150);
    }
    else {
      left(150);
    }

  }
}

// To determine the RGB value of the color
void findColor(uint8_t S0, uint8_t S1, uint8_t S2, uint8_t S3, uint8_t sensorOut, uint8_t number) {
  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  if (number == 1) {
    RR1 = frequency;  
  }
  else if (number == 2) {
    RR2 = frequency;
  }
  //delay(100);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  if (number == 1) {
    GG1 = frequency;  
  }
  else {
    GG2 = frequency;
  }
  //delay(100);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  if (number == 1) {
    BB1 = frequency;  
  }
  else {
    BB2 = frequency;
  }
  //delay(100);
}


// Panel Cleaning Mode
void clean_panel() {
  // Track the timer
  currentMillis = millis();
  
  // Code for Cleaning the Solar Panel
  if (panelModeCount == 1) {
    Serial.print("Forward in Panel Mode.........");
    Serial.println(panelModeCount);
    forward(255, 255);
    countEndFlag = 0;
  }
  else if (panelModeCount == 2) {
    Serial.print("Right in Panel Mode.........");
    Serial.println(panelModeCount);
    right(255);
    countEndFlag = 0;
  }
  else if (panelModeCount == 3) {
    Serial.print("Left in Panel Mode........");
    Serial.print(panelModeCount);
    left(255);
    countEndFlag = 0;
  }
  else if (panelModeCount == 4) {
    Serial.print("Backward in Panel Mode........");
    Serial.print(panelModeCount);
    backward(255, 255);
    countEndFlag = 1;
  }

  // Changing the panelMode
  Serial.print("Current Millis = ");
  Serial.println(currentMillis);
  Serial.print("Previous Millis = ");
  Serial.println(previousMillis);
  unsigned long time_past = currentMillis - previousMillis;
  Serial.print("Time Past = ");
  Serial.println(time_past);
  if (time_past >= interval) {
    previousMillis = currentMillis;
    Serial.print("Previous Millis changed to = ");
    Serial.println(previousMillis);

    // To make sure that the count cycle is restarted again for the next phase....
    if (countEndFlag == 1) {
      panelModeCount = 0;
    }
    panelModeCount = panelModeCount + 1;
    Serial.print("PANEL MODE COUNT CHANGED  TO = ");
    Serial.println(panelModeCount);
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

void right(int PWM)
{
  digitalWrite(LM1, 1);
  digitalWrite(LM2, 0);
  digitalWrite(RM1, 0);
  digitalWrite(RM2, 1);

  analogWrite(LM_Enable, PWM);
  analogWrite(RM_Enable, PWM);
}

void left(int PWM)
{
  digitalWrite(LM1, 0);
  digitalWrite(LM2, 1);
  digitalWrite(RM1, 1);
  digitalWrite(RM2, 0);

  analogWrite(LM_Enable, PWM);
  analogWrite(RM_Enable, PWM);
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
