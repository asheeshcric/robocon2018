#include "hidjoystickrptparser.h"

JoystickReportParser::JoystickReportParser(JoystickEvents *evt) :
joyEvents(evt),
oldHat(0xDE),
oldButtons(0) {
        for (uint8_t i = 0; i < RPT_GEMEPAD_LEN; i++)
                oldPad[i] = 0xD;
}

void JoystickReportParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
        bool match = true;

        // Checking if there are changes in report since the method was last called
        for (uint8_t i = 0; i < RPT_GEMEPAD_LEN; i++)
                if (buf[i] != oldPad[i]) {
                        match = false;
                        break;
                }

        // Calling Game Pad event handler
        if (!match && joyEvents) {
                joyEvents->OnGamePadChanged((const GamePadEventData*)buf);

                for (uint8_t i = 0; i < RPT_GEMEPAD_LEN; i++) oldPad[i] = buf[i];
        }

        uint8_t hat = (buf[5] & 0xF);

        // Calling Hat Switch event handler
        if (hat != oldHat && joyEvents) {
                joyEvents->OnHatSwitch(hat);
                oldHat = hat;
        }

        uint16_t buttons = (0x0000 | buf[6]);
        buttons <<= 4;
        buttons |= (buf[5] >> 4);
        uint16_t changes = (buttons ^ oldButtons);

        // Calling Button Event Handler for every button changed
        if (changes) {
                for (uint8_t i = 0; i < 0x0C; i++) {
                        uint16_t mask = (0x0001 << i);

                        if (((mask & changes) > 0) && joyEvents) {
                                if ((buttons & mask) > 0)
                                        joyEvents->OnButtonDn(i + 1);
                                else
                                        joyEvents->OnButtonUp(i + 1);
                        }
                }
                oldButtons = buttons;
        }
}

void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt) {
        /*Serial.print("X1: ");
        PrintHex<uint8_t > (evt->X, 0x80);
        Serial.print("\tY1: ");
        PrintHex<uint8_t > (evt->Y, 0x80);
        Serial.print("\tX2: ");
        PrintHex<uint8_t > (evt->Z1, 0x80);
        Serial.print("\tY2: ");
        PrintHex<uint8_t > (evt->Z2, 0x80);
        Serial.print("\tRz: ");
        PrintHex<uint8_t > (evt->Rz, 0x80);
        Serial.println("");*/

        inp_X = evt->X;
        inp_Y = evt->Y;
        inp_Z1 = evt->Z1;
        inp_Z2 = evt->Z2;
        inp_Rz = evt->Rz;
}

void JoystickEvents::OnHatSwitch(uint8_t hat) {
        /*Serial.print("Hat Switch: ");
        PrintHex<uint8_t > (hat, 0x80);
        Serial.println("");*/
        inp_hat = hat;
}

void JoystickEvents::OnButtonUp(uint8_t but_id) {
        Serial.print("Up: ");
        Serial.println(but_id, DEC);
        /*if(but_id == 6)
        {
          Serial.println("Button 6 is PRESSED! \n\n\n\n\n\n\n");
        }
        else if(but_id == 8)
        {
          Serial.println("Button 8 is PRESSED! \n\n\n\n\n\n");
        }*/
}

void JoystickEvents::OnButtonDn(uint8_t but_id) {
        Serial.print("Dn: ");
        Serial.println(but_id, DEC);
        if(but_id == 6)
        {
          Serial.println("Button 6 is PRESSED!");
          Change_Height(2);
        }
        else if(but_id == 8)
        {
          Serial.println("Button 8 is PRESSED!");
          Change_Height(-2);
        }
}

void JoystickEvents::Change_Height(int stepsize)
{
  //Code to decrease or increase the height of

   Serial.print("Height Changed by: ");
   Serial.println(stepsize);
}

