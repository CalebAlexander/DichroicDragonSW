#ifndef DD_LIBRARY_H
#define DD_LIBRARY_H

// ---------- Define constants ----------
#define NUM_PWMS 10
#define NUM_STEPPERS 20
#define NUM_LEDS 20
#define NUM_MODE_LEDS 3
#define ENCODER_TICKS 12
#define ENCODER_DEBOUNCE_TIME 5 // ms

#define STEPPER_STEPS 516
#define STEPPER_RPM 10

// ----------- Defaults -----------------
#define STANDARD_MODE_UPDATE_RATE 4651 // ms      -->> one rotation every hour 60min*60sec = 2400 / 516ticks = 4.651 secs/tick
#define FROZEN_MODE_UPDATE_RATE 10	   // ms      -->> only executes once so doesn't really matter
#define RANDOM_MODE_UPDATE_RATE 100	   // ms      -->> fastest rotation will be 10ticks/sec and all other random ones with me multiples of that
#define PATTERN1_MODE_UPDATE_RATE 100  // ms      -->> change brightness 10 times a second to make smooth flow appearance
#define PATTERN2_MODE_UPDATE_RATE 100  // ms      -->> fastest rotation will be 10ticks/sec and all other random ones with me multiples of that, ignore brightness knob interrupt
#define PATTERN3_MODE_UPDATE_RATE 10   // ms      -->> only executes once so doesn't really matter

#define TEMP_UPDATE_RATE 1	  // s
#define EEPROM_UPDATE_RATE 60 // s
// #define RTC_UPDATE_RATE 10          // s

#define PWM_1_ADDRESS 0x61
#define PWM_2_ADDRESS 0x62
#define PWM_3_ADDRESS 0x63
#define PWM_4_ADDRESS 0x64
#define PWM_5_ADDRESS 0x65
#define PWM_6_ADDRESS 0x66
#define PWM_7_ADDRESS 0x67
#define PWM_8_ADDRESS 0x68
#define PWM_9_ADDRESS 0x69
#define PWM_10_ADDRESS 0x6A

// #define PRINT_USA_DATE
// #define SQW_INPUT_PIN 2   // Input pin to read SQW
// #define SQW_OUTPUT_PIN 13 // LED to indicate SQW's state
#include <Arduino.h>
#include <Wire.h>
#include <TaskScheduler.h>
#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MCP23X17.h>
#include <Adafruit_SHT4x.h>
// #include <SparkFunDS1307RTC.h>

enum Mode = {
	STANDARD, // Standard - White - Default
	FROZEN,	  // Frozen - Blue
	RANDOM,	  // Random - Yellow
	PATTERN1, // Dragon Flow - Purple
	PATTERN2, // Bubbling - Pink
	PATTERN3  // Resting Dragon - Green
}

enum Status = {OK, TEMP_HIGH, BAD_COMMS}

// ---------- Instantiate Objects ----------
// Array of AFMS (PWM controllers)
Adafruit_MotorShield AFMS[] = {
	Adafruit_MotorShield(PWM_1_ADDRESS), // PWM1
	Adafruit_MotorShield(PWM_2_ADDRESS), // PWM2
	Adafruit_MotorShield(PWM_3_ADDRESS), // PWM3
	Adafruit_MotorShield(PWM_4_ADDRESS), // PWM4
	Adafruit_MotorShield(PWM_5_ADDRESS), // PWM5
	Adafruit_MotorShield(PWM_6_ADDRESS), // PWM6
	Adafruit_MotorShield(PWM_7_ADDRESS), // PWM7
	Adafruit_MotorShield(PWM_8_ADDRESS), // PWM8
	Adafruit_MotorShield(PWM_9_ADDRESS), // PWM9
	Adafruit_MotorShield(PWM_10_ADDRESS) // PWM10
};
// Array of Stepper Motors
Adafruit_StepperMotor *steppers[] = {
	AFMS[0].getStepper(STEPPER_STEPS, 1), // STP1
	AFMS[0].getStepper(STEPPER_STEPS, 2), // STP2
	AFMS[1].getStepper(STEPPER_STEPS, 1), // STP3
	AFMS[1].getStepper(STEPPER_STEPS, 2), // STP4
	AFMS[2].getStepper(STEPPER_STEPS, 1), // STP5
	AFMS[2].getStepper(STEPPER_STEPS, 2), // STP6
	AFMS[3].getStepper(STEPPER_STEPS, 1), // STP7
	AFMS[3].getStepper(STEPPER_STEPS, 2), // STP8
	AFMS[4].getStepper(STEPPER_STEPS, 1), // STP9
	AFMS[4].getStepper(STEPPER_STEPS, 2), // STP10
	AFMS[5].getStepper(STEPPER_STEPS, 1), // STP11
	AFMS[5].getStepper(STEPPER_STEPS, 2), // STP12
	AFMS[6].getStepper(STEPPER_STEPS, 1), // STP13
	AFMS[6].getStepper(STEPPER_STEPS, 2), // STP14
	AFMS[7].getStepper(STEPPER_STEPS, 1), // STP15
	AFMS[7].getStepper(STEPPER_STEPS, 2), // STP16
	AFMS[8].getStepper(STEPPER_STEPS, 1), // STP17
	AFMS[8].getStepper(STEPPER_STEPS, 2), // STP18
	AFMS[9].getStepper(STEPPER_STEPS, 1), // STP19
	AFMS[9].getStepper(STEPPER_STEPS, 2)  // STP20
};
// Array of PCA9685 (LED Control)
Adafruit_PWMServoDriver PWMs[] = {
	Adafruit_PWMServoDriver(PWM_1_ADDRESS), // PWM1
	Adafruit_PWMServoDriver(PWM_2_ADDRESS), // PWM2
	Adafruit_PWMServoDriver(PWM_3_ADDRESS), // PWM3
	Adafruit_PWMServoDriver(PWM_4_ADDRESS), // PWM4
	Adafruit_PWMServoDriver(PWM_5_ADDRESS), // PWM5
	Adafruit_PWMServoDriver(PWM_6_ADDRESS), // PWM6
	Adafruit_PWMServoDriver(PWM_7_ADDRESS), // PWM7  (NOT USED)
	Adafruit_PWMServoDriver(PWM_8_ADDRESS), // PWM8  (NOT USED)
	Adafruit_PWMServoDriver(PWM_9_ADDRESS), // PWM9  (NOT USED)
	Adafruit_PWMServoDriver(PWM_10_ADDRESS) // PWM10 (NOT USED)
};
// GPIO Port Expander
Adafruit_MCP23X17 GPIO_Expander;
// Temperature Sensor
Adafruit_SHT4x Temp_Sensor = Adafruit_SHT4x();
// RTC
// Object in library
// Buttons
// Encoders

// ---------- Instantiate Variables ----------
// Pin Constants
// const int powerButtonPin = 3;
const int encoder1ButtonPin = 4;
const int encoder1APin = 5;
const int encoder1BPin = 6;
const int encoder2ButtonPin = 7;
const int encoder2APin = 8;
const int encoder2BPin = 9;
const int powerSupervisorPin = 10;

// const int LED_POWER_BUTTON_R =

volatile int encoder1Pos = ENCODER_TICKS / 2;
volatile int encoder1ButtonState = 0;
volatile int encoder2Pos = ENCODER_TICKS / 2;
volatile int encoder2ButtonState = 0;

// Current Time
// Array of Stepper positions
uint16_t stepper_pos[] = {
	0, // STP1
	0, // STP2
	0, // STP3
	0, // STP4
	0, // STP5
	0, // STP6
	0, // STP7
	0, // STP8
	0, // STP9
	0, // STP10
	0, // STP11
	0, // STP12
	0, // STP13
	0, // STP14
	0, // STP15
	0, // STP16
	0, // STP17
	0, // STP18
	0, // STP19
	0  // STP20
};	   // 0 - 516
// Array of LED brightnesses
uint16_t brightnesses[] = {
	0, // LED1
	0, // LED2
	0, // LED3
	0, // LED4
	0, // LED5
	0, // LED6
	0, // LED7
	0, // LED8
	0, // LED9
	0, // LED10
	0, // LED11
	0, // LED12
	0, // LED13
	0, // LED14
	0, // LED15
	0, // LED16
	0, // LED17
	0, // LED18
	0, // LED19
	0  // LED20
};	   // 0 - 4095
// Current Temperature
sensors_event_t humidity, temperature;

Status currentStatus = OK;
Mode currentMode = STANDARD;

// ---------- Function Prototypes ----------

void checkTemperatureCallback();
// void rtcCallback();

void standardModeCallback(); // Standard - White - Default
void frozenModeCallback();	 // Frozen - Blue
void randomModeCallback();	 // Random - Yellow
void pattern1Callback();	 // Dragon Flow - Purple
void pattern2Callback();	 // Bubbling - Pink
void pattern3Callback();	 // Resting Dragon - Green

void updateAllSteppersCallback();
void updateAllLEDsCallback();
void updateModeLEDCallback();

void encoder1A_ISR();
void encoder2A_ISR();
void encoder1Button_ISR();
void encoder2Button_ISR();
void powerSupervisor_ISR();

Scheduler ts;
// Status Tasks
Task CheckTemperatureTask(TEMP_UPDATE_RATE *TASK_SECOND, TASK_FOREVER, &checkTemperatureCallback, &ts, true);
Task RTCTask(RTC_UPDATE_RATE *TASK_MINUTE, TASK_FOREVER, &rtcCallback, &ts, true);
// Mode Tasks
Task StandardModeTask(STANDARD_MODE_UPDATE_RATE *TASK_MILLISECOND, TASK_FOREVER, &standardModeCallback, &ts, true); // Default Mode
Task FrozenModeTask(TASK_IMMEDIATE, TASK_ONCE, &randomModeCallback, &ts, false);
Task RandomModeTask(RANDOM_MODE_UPDATE_RATE *TASK_MILLISECOND, TASK_FOREVER, &randomModeCallback, &ts, false);
Task Pattern1ModeTask(PATTERN1_MODE_UPDATE_RATE *TASK_MILLISECOND, TASK_FOREVER, &pattern1ModeCallback, &ts, false);
Task Pattern2ModeTask(PATTERN2_MODE_UPDATE_RATE *TASK_MILLISECOND, TASK_FOREVER, &pattern2ModeCallback, &ts, false);
Task Pattern3ModeTask(TASK_IMMEDIATE, TASK_ONCE, &pattern3ModeCallback, &ts, false);
// Utility Tasks
Task UpdateAllSteppersTask(TASK_IMMEDIATE, NUM_STEPPERS, &updateAllSteppersCallback, &ts, false);
Task UpdateAllLEDSTask(TASK_IMMEDIATE, NUM_LEDS, &updateAllLEDsCallback, &ts, false);
Task UpdateModeLEDTask(TASK_IMMEDIATE, NUM_MODE_LEDS, &updateModeLEDCallback, &ts, false);

#endif