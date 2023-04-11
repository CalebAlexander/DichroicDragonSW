// #include <Wire.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60);

Adafruit_StepperMotor *myMotor1 = AFMS.getStepper(516, 1);
Adafruit_StepperMotor *myMotor2 = AFMS.getStepper(516, 2);

void setup()
{
  Serial.begin(9600); // set up Serial library at 9600 bps
  // while (!Serial)
  //   ;
  Serial.println("Stepper test!");

  if (!AFMS.begin())
  { // create with the default frequency 1.6KHz
    // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1)
      ;
  }
  Serial.println("Motor Shield found.");

  myMotor1->setSpeed(50); // 10 rpm
  myMotor2->setSpeed(50); // 10 rpm
}

void loop()
{
  Serial.println("Single coil steps");
  myMotor1->step(516, FORWARD, SINGLE);
  myMotor1->step(516, BACKWARD, SINGLE);

  Serial.println("Single coil steps");
  myMotor2->step(516, FORWARD, SINGLE);
  myMotor2->step(516, BACKWARD, SINGLE);

  // Serial.println("Double coil steps");
  // myMotor1->step(100, FORWARD, DOUBLE);
  // myMotor1->step(100, BACKWARD, DOUBLE);

  // Serial.println("Interleave coil steps");
  // myMotor1->step(100, FORWARD, INTERLEAVE);
  // myMotor1->step(100, BACKWARD, INTERLEAVE);

  // Serial.println("Microstep steps");
  // myMotor1->step(50, FORWARD, MICROSTEP);
  // myMotor1->step(50, BACKWARD, MICROSTEP);
}
