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
#include <TaskSchedulerDeclarations.h>
// #include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
// #include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MCP23X17.h>
#include <Adafruit_SHT4x.h>
#include <FastLED.h>
// #include <SparkFunDS1307RTC.h>

extern enum Mode {
	STANDARD, // Standard - White - Default
	FROZEN,	  // Frozen - Blue
	RANDOM,	  // Random - Yellow
	PATTERN1, // Dragon Flow - Purple
	PATTERN2, // Bubbling - Pink
	PATTERN3  // Resting Dragon - Green
} currentMode;

extern enum Status {
	OK,
	TEMP_HIGH,
	BAD_COMMS
} currentStatus;

// typedef struct DD_Library
// {
// 	Adafruit_MotorShield AFMS;

// };

// ---------- Instantiate Objects ----------
// Array of AFMS (PWM controllers)
extern Adafruit_MotorShield AFMS[NUM_PWMS];

// Array of Stepper Motors
extern Adafruit_StepperMotor *steppers[NUM_STEPPERS];
// LED Pins
extern const int LEDPins[NUM_LEDS];
// GPIO Port Expander
extern Adafruit_MCP23X17 GPIO_Expander1;
extern Adafruit_MCP23X17 GPIO_Expander2;
// Temperature Sensor
extern Adafruit_SHT4x Temp_Sensor;
// RTC
// Object in library
// Buttons
// Encoders

// ---------- Instantiate Variables ----------
extern volatile int encoder1Pos;
extern volatile int encoder1ButtonState;
extern volatile int encoder2Pos;
extern volatile int encoder2ButtonState;
extern volatile int currentI;	// current stepper to be moved
extern volatile int currentS;	// number of steps to be moved
extern volatile int currentP;	// position to be moved to
extern volatile int currentD;	// current direction
extern void (*currentF)(void);	// current function
extern volatile float currentB; // Default brightness
extern volatile bool goBack;

// Current Time
// Array of Stepper positions
extern volatile uint16_t stepper_pos[NUM_STEPPERS]; // 0 - 516
// Array of LED brightnesses
extern volatile float brightnesses[NUM_LEDS]; // 0 - 4095
// Current Temperature
extern sensors_event_t *humidity;
extern sensors_event_t *temperature;

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

extern Scheduler ts;

// Status Tasks
extern Task CheckTemperatureTask;
extern Task StandardModeTask;
extern Task FrozenModeTask;
extern Task RandomModeTask;
extern Task Pattern1ModeTask;
extern Task Pattern2ModeTask;
extern Task Pattern3ModeTask;

// Task TenHzTask(100 * TASK_MILLISECOND, 10);

// Utility Tasks
// Task UpdateStepperTask(TASK_IMMEDIATE, NUM_STEPPERS, &updateStepperCallback, &ts, false);
// Task UpdateAllLEDSTask(TASK_IMMEDIATE, NUM_LEDS, &updateAllLEDsCallback, &ts, false);
// ask UpdateModeLEDTask(TASK_IMMEDIATE, NUM_MODE_LEDS, &updateModeLEDCallback, &ts, false);

#endif