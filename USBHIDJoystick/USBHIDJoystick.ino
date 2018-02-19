#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>

// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include "hidjoystickrptparser.h"

USB Usb;
USBHub Hub(&Usb);
HIDUniversal Hid(&Usb);
JoystickEvents JoyEvents;
JoystickReportParser Joy(&JoyEvents);

GamePadEventData data1;

uint8_t out_X;
uint8_t out_Y;
uint8_t out_Z1;
uint8_t out_Z2;
uint8_t out_Rz;
uint8_t out_hat;
uint8_t out_butid;

uint8_t LM1 = 25;
uint8_t LM2 = 23;
uint8_t RM1 = 22;
uint8_t RM2 = 31;
uint8_t LM_Enable = 7;
uint8_t RM_Enable = 8;
uint8_t Forward_PWM = 100;


uint8_t LiftM1 = 3;
uint8_t LiftM2 = 5;
uint8_t Lift_Enable = 6;

void setup() {
        pinMode(LM1, OUTPUT);
        pinMode(LM2, OUTPUT);
        pinMode(RM1, OUTPUT);
        pinMode(RM2, OUTPUT);
        pinMode(LM_Enable, OUTPUT);
        pinMode(RM_Enable, OUTPUT);
        pinMode(LiftM1, OUTPUT);
        pinMode(LiftM2, OUTPUT);
        pinMode(Lift_Enable, OUTPUT);
        Forward_PWM = 100;
        Serial.begin(115200);
#if !defined(__MIPSEL__)
        while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
        Serial.println("Start");

        if (Usb.Init() == -1)
                Serial.println("OSC did not start.");

        delay(200);

        if (!Hid.SetReportParser(0, &Joy))
                ErrorMessage<uint8_t > (PSTR("SetReportParser"), 1);
}

void loop() {
        Usb.Task();
        out_X = JoyEvents.inp_X;
        out_Y = JoyEvents.inp_Y;
        out_Z1 = JoyEvents.inp_Z1;
        out_Z2 = map(JoyEvents.inp_Z2, 0, 0xFF, 0, 255);
        out_Rz = map(JoyEvents.inp_Rz, 0, 0xFF, 0, 255);
        out_hat = JoyEvents.inp_hat;
        Serial.print(" X=");
        Serial.print(out_X);
        Serial.print(" Y=");
        Serial.print(out_Y);
        Serial.print(" Z1=");
        Serial.print(out_Z1);
        Serial.print(" Z2=");
        Serial.print(out_Z2);
        Serial.print(" Rz=");
        Serial.println(out_Rz);
        Serial.print("Hat Switch = ");
        Serial.println(out_hat);
        delay(100);
        

        if(out_hat != 15)
        {
          switch(out_hat)
          {
            case 0:
                   Serial.println("Hat: Forward Key Pressed! PWM is POWERED UP!");
                   Forward_PWM = 200;
                   break;
            case 2:
                   Serial.println("Hat: Right Key Pressed!");
                   break;
            case 4:
                   Serial.println("Hat: Backward Key Pressed! PWM is POWERED DOWN!");
                   Forward_PWM = 100;
                   break;
            case 6:
                   Serial.println("Hat: Left Key Pressed!");
                   break;
            default:
                   Serial.println("Hat: No Key Pressed!");
                   break;
          }
        }
//
//        if(out_butid != 0)
//        {
//          switch(out_butid)
//          {
//            case 1:
//                  Serial.println("Button 1 is pressed!");
//                  Serial.println("\n\n\n\n\n\n\n");
//                  break;
//            default:
//                  Serial.println("No button is pressed!");
//                  break;
//          }
//        }

    
    if (out_X > 110 && out_X < 135 && out_Y > 110 && out_Y < 135 && out_Z1 < 50){
      // Lift Motor goes up
      Serial.println("Lift Motor Goes UP");
      LiftMotor(1, 255);
    }
    else if (out_X > 110 && out_X < 135 && out_Y > 110 && out_Y < 135 && out_Z1 > 180){
      // Lift Motor goes down
      Serial.println("Lift Motor Goes DOWN");
      LiftMotor(0, 255);
    }
    else if (out_X < 10 && out_Y < 10 && out_Z1 < 10){
      // Forward-Left
      Serial.println("Forward-Left");
      forward(100, 150);
    }
    else if (out_X > 200 && out_Y < 10 && out_Z1 > 128){
      // Forward-Right
      Serial.println("Forward-Right");
      forward(150, 100);
    }
    else if (out_X < 10 && out_Y > 150 && out_Z1 < 10){
      // Backward-Left
      Serial.println("Backward-Left");
      backward(100, 150);
    }
    else if (out_X > 150 && out_Y > 150 && out_Z1 > 150){
      // Backward-Right
      Serial.println("Backward-Right");
      backward(150, 100);
    }
    else if (out_X < 10 && out_Y > 120 && out_Y < 130 && out_Z1 < 10){
      // Left
      Serial.println("Left");
      left(150);
    }
    else if (out_X > 200 && out_Y > 120 && out_Y < 130 && out_Z1 > 200){
      // Right
      Serial.println("Right");
      right(150);
    }
    else if (out_X > 120 && out_X < 130 && out_Y < 10 && out_Z1 > 120 && out_Z1 < 130){
      // Forward
      Serial.println("Forward");
      forward(Forward_PWM, Forward_PWM);
    }
    else if (out_X > 120 && out_X < 130 && out_Y > 200 && out_Z1 > 120 && out_Z1 < 130){
        // Backward
        Serial.println("Backward");
        backward(100, 100);
    }
    else {
      stop_bot();
    }
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

  
  digitalWrite(47, 0);
  digitalWrite(49, 1);
  analogWrite(6, 100);
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
//  delay(400);/
  digitalWrite(LM1, 1);
  digitalWrite(LM2, 1);
  digitalWrite(RM1, 1);
  digitalWrite(RM2, 1);

  digitalWrite(LiftM1, 1);
  digitalWrite(LiftM2, 1);

  analogWrite(LM_Enable, 0);
  analogWrite(RM_Enable, 0);
}

void LiftMotor(int direct, int pwm)
{
  if(direct == 1){            //For lifting up
    digitalWrite(LiftM1, 1);
    digitalWrite(LiftM2, 0);
    analogWrite(Lift_Enable, pwm);
  }
  else{                       //For lifting down
    digitalWrite(LiftM1, 0);
    digitalWrite(LiftM2, 1);
    analogWrite(Lift_Enable, pwm);
  }
}

