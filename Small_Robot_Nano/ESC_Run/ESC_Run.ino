Servo esc; // Define the ESC as a servo object

int arm = 1000; // defines pulse width of 1000 us     //850us to 2400us is the generally pulse width range used for esc
int speedvalue;


void setup()
{
  esc.attach(3);
  esc.writeMicroseconds(arm);
  // This command sends a pulse train
  // from pin 9 that continues until
  // the pin is called to do something else.

  /* Once armed the setup could also be used to specify the
  run speed of the motor. The commented out lines provide
  a 2 second delay between changes in speed.*/
  
}

void loop()
{

  /*
  Calls a sub to throttle up motor from 0 rpm to a steady running value.
  The if statement is used to run the throttle up once.
  */
  delay(150);
  esc.writeMicroseconds(1200);
  delay(150);
  esc.writeMicroseconds(1300);
  delay(150);
  
  
 // esc.detach(); // Disengage ESC from pin
}


