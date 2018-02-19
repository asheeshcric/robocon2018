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
long interval = 2000;         // Interval the robot to move forward and backward
unsigned long currentMillis;
int panelModeCount = 0;                  // For going changing the operation in the Panel Clean Mode
int countEndFlag = 0;                    // To determine whether the count cycle is over and needs to restart again or not.


// For line following mode
int lineFollowingMode = 0;
int threshold = 78;
uint8_t L_R = 0;
uint8_t L_G = 0;
uint8_t L_B = 0;
uint8_t C_R = 0;
uint8_t C_G = 0;
uint8_t C_B = 0;
uint8_t R_R = 0;
uint8_t R_G = 0;
uint8_t R_B = 0;
int frequency = 0;

// Sensor Pins
uint8_t CS0 = 44;   // For right sensor
uint8_t CS1 = 46;
uint8_t CS2 = 48;
uint8_t CS3 = 50;
uint8_t COutput = 42;

uint8_t RS0 = 45;   // For centre sensor
uint8_t RS1 = 47;
uint8_t RS2 = 49;
uint8_t RS3 = 51;
uint8_t ROutput = 43;

uint8_t LS0 = 41;   // For Left sensor
uint8_t LS1 = 38;
uint8_t LS2 = 39;
uint8_t LS3 = 36;
uint8_t LOutput = 40;


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
  pinMode(RS0, OUTPUT);
  pinMode(RS1, OUTPUT);
  pinMode(RS2, OUTPUT);
  pinMode(RS3, OUTPUT);
  pinMode(ROutput, INPUT);

  pinMode(CS0, OUTPUT);
  pinMode(CS1, OUTPUT);
  pinMode(CS2, OUTPUT);
  pinMode(CS3, OUTPUT);
  pinMode(COutput, INPUT);

  pinMode(LS0, OUTPUT);
  pinMode(LS1, OUTPUT);
  pinMode(LS2, OUTPUT);
  pinMode(LS3, OUTPUT);
  pinMode(LOutput, INPUT);

  // Setting frequency-scaling to 20%
  digitalWrite(RS0,HIGH);
  digitalWrite(RS1,LOW);
  delay(10);
  digitalWrite(CS0,HIGH);
  digitalWrite(CS1,LOW);
  delay(10);
  digitalWrite(LS0, HIGH);
  digitalWrite(LS1, LOW);
  
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
    stop_bot();
    MODE = 1; // Line following Mode
    Serial.println("MODE 1 Selected - Line Following Mode.........");
  }
  if (digitalRead(digitalPin2) == 1) {
    stop_bot();
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
    EscServo();
    delay(15);
    lineFollowingMode = 0;
    line_follower();
    // Serial.println("Line Following Mode Running......");
  }
  else if (MODE == 2) {
    EscServo();
    delay(15);
    clean_panel();
    Serial.println("Panel Cleaning Mode Running......");
  }
  else if (MODE == 10) {
    esc.writeMicroseconds(0);
    adjust_bot();
    MODE = 0;
    Serial.println("Adjusting the bot on the big robot......");
  }
  else {
    stop_bot();
    Serial.println("The robot is on STATIONARY MODE........");
  }
}


// Line Following Mode
void line_follower() {
  // Code for Line Follower
  findColor(LS2, LS3, LOutput, 1);  // For the third sensor
  Serial.print(L_R);
  Serial.print(" ");
  Serial.print(L_G);
  Serial.print(" ");
  Serial.print(L_B);
  Serial.print("  -- --  ");
  findColor(CS2, CS3, COutput, 2);  // For the centre sensor
  Serial.print(C_R);
  Serial.print(" ");
  Serial.print(C_G);
  Serial.print(" ");
  Serial.print(C_B);
  Serial.print("  -- --  ");
  findColor(RS2, RS3, ROutput, 3);  // For the right sensor
  Serial.print(R_R);
  Serial.print(" ");
  Serial.print(R_G);
  Serial.print(" ");
  Serial.println(R_B);


  // Run line follower
  run_linefollower();
}

void run_linefollower() {
  if (L_B > threshold && C_B < threshold && R_B > threshold) {
     forward(100, 100);
     lineFollowingMode = 1;
   }
   else if (L_B < threshold && C_B > threshold && R_B > threshold) {
     left(180);
     lineFollowingMode = 2;
   }
   else if (L_B > threshold && C_B > threshold && R_B < threshold) {
     right(180);
     lineFollowingMode = 3;
   }
   else if (L_B < threshold && C_B < threshold && R_B > threshold) {
     left(255);
     //delay(300);
     lineFollowingMode = 2;
   }
   else if (L_B > threshold && C_B < threshold && R_B < threshold) {
     right(255);
     //delay(300);
     lineFollowingMode = 3;
   }
   else if (L_B < threshold && C_B < threshold && R_B < threshold) {
     forward(100, 100);
     lineFollowingMode = 1;
   }
  if (L_R > 100 && L_R < 140){
    if (C_R > 80 && C_G < 100){
      MODE = 10;
    }
    else if (C_R > 100 && C_R < 140) {
      MODE = 10;
    }
  }
}
   

// To determine the RGB value of the color
void findColor(uint8_t S2, uint8_t S3, uint8_t sensorOut, uint8_t number) {
  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  if (number == 1) {
    L_R = frequency;  
  }
  else if (number == 2) {
    C_R = frequency;
  }
  else {
    R_R = frequency;
  }
  //delay(100);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  if (number == 1) {
    L_G = frequency;  
  }
  else if (number == 2) {
    C_G = frequency;
  }
  else {
    R_G = frequency;
  }
  //delay(100);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  if (number == 1) {
    L_B = frequency;  
  }
  else if (number == 2) {
    C_B = frequency;
  }
  else {
    R_B = frequency;
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
    forward(100, 100);
    interval = 2000;
    countEndFlag = 0;
  }
  else if (panelModeCount == 2) {
    Serial.print("Right in Panel Mode.........");
    Serial.println(panelModeCount);
    backward(100, 100);
    countEndFlag = 1;
    interval = 2500;
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
  forward(100, 100);
  delay(1400);
  right(255);
  delay(1100);
  MODE = 0;
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

void sharp_right(int PWM)
{
  digitalWrite(LM1, 1);
  digitalWrite(LM2, 0);
  digitalWrite(RM1, 1);
  digitalWrite(RM2, 1);

  analogWrite(LM_Enable, PWM);
  analogWrite(RM_Enable, 0);
}

void sharp_left(int PWM)
{
  digitalWrite(LM1, 1);
  digitalWrite(LM2, 1);
  digitalWrite(RM1, 1);
  digitalWrite(RM2, 0);

  analogWrite(LM_Enable, 0);
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
