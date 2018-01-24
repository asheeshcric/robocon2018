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

uint8_t LM1 = 23;
uint8_t LM2 = 22;
uint8_t RM1 = 25;
uint8_t RM2 = 31;
uint8_t LM_Enable = 7;
uint8_t RM_Enable = 8;

void setup() {
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
        /*Serial.print(" X=");
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
        Serial.println(out_hat);*/
        
        if(out_X > 100 && out_X < 140)
        {
          //Forward or Backward
          if(out_Y < 5)
          {
            Serial.println("Forward");
            forward(150, 150);
          }
          else if(out_Y > 250)
          {
            Serial.println("Backward");
            backward(150, 150);
          }
          else
          {
            //Serial.println("Stationary");
            stop_bot();
          }
        }
        else if(out_X < 10)
        {
          //Left, F-Left or B-Left
          //Serial.println("Left, F-Left or B-Left");
          if(out_Y < 5)
          {
            Serial.println("F-Left");
            forward(50, 150);
          }
          else if(out_Y > 200)
          {
            Serial.println("B-Left");
            backward(50, 150);
          }
          else
          {
            Serial.println("Left");
            left(0, 150);
          }
        }
        else if(out_X > 200)
        {
          //Right, F-Right and B-Right
          //Serial.println("Right, F-Right or B-Right");
          if(out_Y < 5)
          {
            Serial.println("F-Right");
            forward(150, 50);         }
          else if(out_Y > 250)
          {
            Serial.println("B-Right");
            backward(150, 50);
          }
          else
          {
            Serial.println("Right");
            right(150, 0);
          }
        }

        if(out_hat != 15)
        {
          switch(out_hat)
          {
            case 0:
                   Serial.println("Hat: Forward Key Pressed!");
                   break;
            case 2:
                   Serial.println("Hat: Right Key Pressed!");
                   break;
            case 4:
                   Serial.println("Hat: Backward Key Pressed!");
                   break;
            case 6:
                   Serial.println("Hat: Left Key Pressed!");
                   break;
            default:
                   Serial.println("Hat: No Key Pressed!");
                   break;
          }
        }

        if(out_butid != 0)
        {
          switch(out_butid)
          {
            case 1:
                  Serial.println("Button 1 is pressed!");
                  Serial.println("\n\n\n\n\n\n\n");
                  break;
            default:
                  Serial.println("No button is pressed!");
                  break;
          }
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


