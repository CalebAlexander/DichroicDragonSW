#ifndef DD_LIBRARY_H
#define DD_LIBRARY_H

// ---------- Define constants ----------
#define ENCODER_1BTN_PIN 4
#define ENCODER_1A_PIN 5
#define ENCODER_1B_PIN 6
#define ENCODER_2BTN_PIN 7
#define ENCODER_2A_PIN 8
#define ENCODER_2B_PIN 9
#define PWR_SUPERVISOR_PIN 10

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

#define STANDARD_MODE_STEP_SIZE 1

#define LED_DEFAULT_BRIGHTNESS 0.5
#define LED_PWM_MIN 0
#define LED_PWM_MAX 4092
#define PWMS_IN_USE 5

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

#define _TASK_SLEEP_ON_IDLE_RUN // Enable 1 ms SLEEP_IDLE powerdowns between runs if no callback methods were invoked during the pass

// #define PRINT_USA_DATE
// #define SQW_INPUT_PIN 2   // Input pin to read SQW
// #define SQW_OUTPUT_PIN 13 // LED to indicate SQW's state
#include <Arduino.h>
#include <Wire.h>
#include <TaskScheduler.h>
// #include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
// #include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MCP23X17.h>
#include <Adafruit_SHT4x.h>
#include <FastLED.h>
// #include <SparkFunDS1307RTC.h>

enum Mode
{
	STANDARD, // Standard - White - Default
	FROZEN,	  // Frozen - Blue
	RANDOM,	  // Random - Yellow
	PATTERN1, // Dragon Flow - Purple
	PATTERN2, // Bubbling - Pink
	PATTERN3  // Resting Dragon - Green
};

enum Status
{
	OK,
	TEMP_HIGH,
	BAD_COMMS
};

// typedef struct DD_Library
// {
// 	Adafruit_MotorShield AFMS;

// };

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
// Adafruit_PWMServoDriver PWMs[] = {
// 	Adafruit_PWMServoDriver(PWM_1_ADDRESS), // PWM1
// 	Adafruit_PWMServoDriver(PWM_2_ADDRESS), // PWM2
// 	Adafruit_PWMServoDriver(PWM_3_ADDRESS), // PWM3
// 	Adafruit_PWMServoDriver(PWM_4_ADDRESS), // PWM4
// 	Adafruit_PWMServoDriver(PWM_5_ADDRESS), // PWM5
// 	Adafruit_PWMServoDriver(PWM_6_ADDRESS), // PWM6
// 	Adafruit_PWMServoDriver(PWM_7_ADDRESS), // PWM7  (NOT USED)
// 	Adafruit_PWMServoDriver(PWM_8_ADDRESS), // PWM8  (NOT USED)
// 	Adafruit_PWMServoDriver(PWM_9_ADDRESS), // PWM9  (NOT USED)
// 	Adafruit_PWMServoDriver(PWM_10_ADDRESS) // PWM10 (NOT USED)
// };

// LED Pins
const int LEDPins[]{
	0,	// LED1 (PWM1)
	1,	// LED2 (PWM1)
	14, // LED3 (PWM1)
	15, // LED4 (PWM1)
	0,	// LED5 (PWM2)
	1,	// LED6 (PWM2)
	14, // LED7 (PWM2)
	15, // LED8 (PWM2)
	0,	// LED9 (PWM3)
	1,	// LED10 (PWM3)
	14, // LED11 (PWM3)
	15, // LED12 (PWM3)
	0,	// LED13 (PWM4)
	1,	// LED14 (PWM4)
	14, // LED15 (PWM4)
	15, // LED16 (PWM4)
	0,	// LED17 (PWM5)
	1,	// LED18 (PWM5)
	14, // LED19 (PWM5)
	15, // LED20 (PWM5)
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
volatile int encoder1Pos = ENCODER_TICKS / 2;
volatile int encoder1ButtonState = 0;
volatile int encoder2Pos = ENCODER_TICKS / 2;
volatile int encoder2ButtonState = 0;
volatile int currentI = 0;						 // current stepper to be moved
volatile int currentS = STANDARD_MODE_STEP_SIZE; // number of steps to be moved
volatile int currentP = 0;						 // position to be moved to
volatile int currentD = FORWARD;
void (*currentF)(void);
volatile float currentB = LED_DEFAULT_BRIGHTNESS; // Default brightness
volatile bool goBack = false;

// Current Time
// Array of Stepper positions
volatile uint16_t stepper_pos[] = {
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
volatile float brightnesses[] = {
	LED_DEFAULT_BRIGHTNESS, // LED1
	LED_DEFAULT_BRIGHTNESS, // LED2
	LED_DEFAULT_BRIGHTNESS, // LED3
	LED_DEFAULT_BRIGHTNESS, // LED4
	LED_DEFAULT_BRIGHTNESS, // LED5
	LED_DEFAULT_BRIGHTNESS, // LED6
	LED_DEFAULT_BRIGHTNESS, // LED7
	LED_DEFAULT_BRIGHTNESS, // LED8
	LED_DEFAULT_BRIGHTNESS, // LED9
	LED_DEFAULT_BRIGHTNESS, // LED10
	LED_DEFAULT_BRIGHTNESS, // LED11
	LED_DEFAULT_BRIGHTNESS, // LED12
	LED_DEFAULT_BRIGHTNESS, // LED13
	LED_DEFAULT_BRIGHTNESS, // LED14
	LED_DEFAULT_BRIGHTNESS, // LED15
	LED_DEFAULT_BRIGHTNESS, // LED16
	LED_DEFAULT_BRIGHTNESS, // LED17
	LED_DEFAULT_BRIGHTNESS, // LED18
	LED_DEFAULT_BRIGHTNESS, // LED19
	LED_DEFAULT_BRIGHTNESS	// LED20
};							// 0 - 4095
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
void pattern1ModeCallback(); // Dragon Flow - Purple
void pattern2ModeCallback(); // Bubbling - Pink
void pattern3ModeCallback(); // Resting Dragon - Green

void updateStepper(int, int, int);
void updateLED(int, float);

void standardUpdateAllSteppersCallback();
void standardUpdateAllLEDsCallback();
void randomUpdateAllSteppersCallback();
void randomUpdateAllLEDsCallback();
void pattern1UpdateAllSteppersCallback();
void pattern1UpdateAllLEDsCallback();
void pattern2UpdateAllSteppersCallback();
void pattern2UpdateAllLEDsCallback();
void pattern3UpdateAllSteppersCallback();
void pattern3UpdateAllLEDsCallback();

void encoder1A_ISR();
void encoder2A_ISR();
void encoder1Button_ISR();
void encoder2Button_ISR();
void powerSupervisor_ISR();

Scheduler ts;
// Status Tasks
Task CheckTemperatureTask(TEMP_UPDATE_RATE *TASK_SECOND, TASK_FOREVER, &checkTemperatureCallback, &ts, true);
// Task RTCTask(RTC_UPDATE_RATE *TASK_MINUTE, TASK_FOREVER, &rtcCallback, &ts, true);
//  Mode Tasks
Task StandardModeTask(STANDARD_MODE_UPDATE_RATE *TASK_MILLISECOND, TASK_FOREVER, &standardModeCallback, &ts, true); // Default Mode
Task FrozenModeTask(TASK_IMMEDIATE, TASK_ONCE, &randomModeCallback, &ts, false);
Task RandomModeTask(RANDOM_MODE_UPDATE_RATE *TASK_MILLISECOND, TASK_ONCE, &randomModeCallback, &ts, false);
Task Pattern1ModeTask(PATTERN1_MODE_UPDATE_RATE *TASK_MILLISECOND, TASK_FOREVER, &pattern1ModeCallback, &ts, false);
Task Pattern2ModeTask(PATTERN2_MODE_UPDATE_RATE *TASK_MILLISECOND, TASK_FOREVER, &pattern2ModeCallback, &ts, false);
Task Pattern3ModeTask(TASK_IMMEDIATE, TASK_ONCE, &pattern3ModeCallback, &ts, false);

// Task TenHzTask(100 * TASK_MILLISECOND, 10);

// Utility Tasks
// Task UpdateStepperTask(TASK_IMMEDIATE, NUM_STEPPERS, &updateStepperCallback, &ts, false);
// Task UpdateAllLEDSTask(TASK_IMMEDIATE, NUM_LEDS, &updateAllLEDsCallback, &ts, false);
// ask UpdateModeLEDTask(TASK_IMMEDIATE, NUM_MODE_LEDS, &updateModeLEDCallback, &ts, false);

#endif