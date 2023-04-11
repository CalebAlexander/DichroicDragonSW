// Shows how to run three Steppers at once with varying speeds
//
// Requires the Adafruit_Motorshield v2 library
//   https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
// And AccelStepper with AFMotor support
//   https://github.com/adafruit/AccelStepper

// This tutorial is for Adafruit Motorshield v2 only!
// Will not work with v1 shields

#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include <TaskScheduler.h>

// ==== Scheduler ==============================
Scheduler ts;

void taskTempCallback();
void taskSteppersCallback();

// ==== Task definitions ========================
Task tTemp(1 * TASK_MILLISECOND, TASK_FOREVER, &taskTempCallback, &ts, true);
Task tSteppers(TASK_IMMEDIATE, TASK_FOREVER /* times */, &taskSteppersCallback, &ts, true);

// Adafruit_MotorShield AFMSbot(0x61); // Rightmost jumper closed
Adafruit_MotorShield AFMStop(0x60); // Default address, no jumpers

// Connect two steppers with 200 steps per revolution (1.8 degree)
// to the top shield
Adafruit_StepperMotor *myStepper1 = AFMStop.getStepper(516, 1);
Adafruit_StepperMotor *myStepper2 = AFMStop.getStepper(516, 2);

// Connect one stepper with 200 steps per revolution (1.8 degree)
// to the bottom shield
// Adafruit_StepperMotor *myStepper3 = AFMSbot.getStepper(200, 2);

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
void forwardstep1()
{
  myStepper1->onestep(FORWARD, SINGLE);
  // Serial.println("STP1 Forward Single");
}
void backwardstep1()
{
  myStepper1->onestep(BACKWARD, SINGLE);
  // Serial.println("STP1 Forward Single");
}
// wrappers for the second motor!
void forwardstep2()
{
  myStepper2->onestep(FORWARD, SINGLE);
  // Serial.println("STP2 Forward Double");
}
void backwardstep2()
{
  myStepper2->onestep(BACKWARD, SINGLE);
  // Serial.println("STP2 Forward Double");
}
// wrappers for the third motor!
// void forwardstep3()
// {
//   myStepper3->onestep(FORWARD, INTERLEAVE);
// }
// void backwardstep3()
// {
//   myStepper3->onestep(BACKWARD, INTERLEAVE);
// }

// Now we'll wrap the 3 steppers in an AccelStepper object
AccelStepper stepper1(forwardstep1, backwardstep1);
AccelStepper stepper2(forwardstep2, backwardstep2);
// AccelStepper stepper3(forwardstep3, backwardstep3);

void setup()
{

  Serial.begin(9600);

  while (!Serial)
    ;
  // AFMSbot.begin(); // Start the bottom shield
  Serial.println("Starting stepper motor test...");
  AFMStop.begin(); // Start the top shield

  stepper1.setMaxSpeed(100.0);
  stepper1.setSpeed(100.0);
  // stepper1.setAcceleration(100.0);
  // stepper1.moveTo(516);

  stepper2.setMaxSpeed(200.0);
  stepper2.setSpeed(200.0);
  // stepper2.setAcceleration(100.0);
  // stepper2.moveTo(516);

  // stepper3.setMaxSpeed(300.0);
  // stepper3.setAcceleration(100.0);
  // stepper3.moveTo(1000000);
}

void loop()
{
  // Change direction at the limits
  // if (stepper1.distanceToGo() == 0)
  //   stepper1.moveTo(-stepper1.currentPosition());

  // if (stepper2.distanceToGo() == 0)
  //   stepper2.moveTo(-stepper2.currentPosition());

  // if (stepper3.distanceToGo() == 0)
  //   stepper3.moveTo(-stepper3.currentPosition());
  ts.execute();
  // stepper3.run();
}

void taskTempCallback()
{
  Serial.println("Temp is blah blah");
}
void taskSteppersCallback()
{
  Serial.println("STP1 Pos: " + String(stepper1.currentPosition()) + "/tSTP2 Pos: " + String(stepper2.currentPosition()));
  stepper1.runSpeed();
  stepper2.runSpeed();
}
