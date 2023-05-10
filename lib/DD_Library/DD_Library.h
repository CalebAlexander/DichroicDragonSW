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
#define ACTIVITY_LED_PIN 11
#define STP_PWR_PIN A6
#define LED_PWR_PIN A7

#define NUM_PWMS 10
#define NUM_STEPPERS 20
#define NUM_LEDS 20
#define NUM_STATUS_LEDS 3
#define ENCODER_TICKS 12
#define ENCODER_DEBOUNCE_TIME 5 // ms

// Standard - White
#define MODE_STANDARD_R 255
#define MODE_STANDARD_G 255
#define MODE_STANDARD_B 255
// Frozen - Blue
#define MODE_FROZEN_R 61
#define MODE_FROZEN_G 67
#define MODE_FROZEN_B 235
// Random - Cyan
#define MODE_RANDOM_R 61
#define MODE_RANDOM_G 206
#define MODE_RANDOM_B 235
// Pattern 1 (Dragon Flow) - Purple
#define MODE_PATTERN1_R 136
#define MODE_PATTERN1_G 61
#define MODE_PATTERN1_B 235
// Pattern 2 (Bubbling) - Pink
#define MODE_PATTERN2_R 235
#define MODE_PATTERN2_G 61
#define MODE_PATTERN2_B 220
// Pattern 3 (Resting Dragon) - Green
#define MODE_PATTERN3_R 9
#define MODE_PATTERN3_G 217
#define MODE_PATTERN3_B 75
// Temp High - Orange
#define STATUS_TEMP_HIGH_R 245
#define STATUS_TEMP_HIGH_G 114
#define STATUS_TEMP_HIGH_B 115
// Bad Comms - Yellow
#define STATUS_BAD_COMMS_R 235
#define STATUS_BAD_COMMS_G 206
#define STATUS_BAD_COMMS_B 61
// Power Loss - Red
#define STATUS_POWER_LOSS_R 250
#define STATUS_POWER_LOSS_G 40
#define STATUS_POWER_LOSS_B 40

#define OFF 0
#define HOME 0

#define STATUS_PIN_R 9
#define STATUS_PIN_G 10
#define STATUS_PIN_B 11

#define STEPPER_STEPS 516
#define STEPPER_DEFAULT_RPM 10
#define STEPPER_MAX_RPM 100
#define STEPPER_MAX_ACCELERATION 10

// ----------- Defaults -----------------
#define LED_DEFAULT_BRIGHTNESS 0.5
#define LED_PWM_MIN 0
#define LED_PWM_MAX 4092
#define PWMS_IN_USE 5
#define LED_UPDATE_RATE 10		   // ms
#define STATUS_LED_FLASH_RATE 0.25 // ms

#define TEMP_UPDATE_RATE 1	   // s
#define EEPROM_UPDATE_RATE 60  // s
#define RTC_UPDATE_RATE 10	   // s
#define ACTIVITY_LED_RATE 0.25 // s
#define HIZ INPUT
#define PRINT_USA_DATE
#define SQW_INPUT_PIN 2	  // Input pin to read SQW
#define SQW_OUTPUT_PIN 13 // LED to indicate SQW's state

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
#include <AccelStepper.h>
// #include <MultiStepper.h>
#include <Adafruit_MotorShield.h>
// #include <Adafruit_PWMServoDriver.h>
#include <Adafruit_MCP23X17.h>
#include <Adafruit_SHT4x.h>
#include <FastLED.h>
#include <SparkFun_External_EEPROM.h>
#include <SparkFunDS1307RTC.h>

extern enum Mode {
	STANDARD, // Standard - White - Default
	FROZEN,	  // Frozen - Blue
	RANDOM,	  // Random - Yellow
	PATTERN1, // Dragon Flow - Purple
	PATTERN2, // Bubbling - Pink
	PATTERN3, // Resting Dragon - Green
	NOMODE,	  // OFF
} currentMode;

extern enum Status {
	OK,
	TEMP_HIGH,
	BAD_COMMS,
	POWER_LOSS,
	NOSTATUS,
} currentStatus;

// typedef struct DD_Library
// {
// 	Adafruit_MotorShield AFMS;

// };

// ---------- Instantiate Objects ----------
// Array of AFMS (PWM controllers)
extern Adafruit_MotorShield AFMS[NUM_PWMS];

// Array of Stepper Motors
extern Adafruit_StepperMotor *AFMS_Steppers[NUM_STEPPERS];
// Array of Accell Steppers
extern AccelStepper steppers[NUM_STEPPERS];
// extern MultiStepper multistepper1;
// extern MultiStepper multistepper2;

// LED Pins
extern const int LEDPins[NUM_LEDS];

// GPIO Port Expander
extern Adafruit_MCP23X17 GPIO_Expander1;
extern Adafruit_MCP23X17 GPIO_Expander2;
// Temperature Sensor
extern Adafruit_SHT4x Temp_Sensor;
// EEPROM
extern ExternalEEPROM EEPROM;
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
extern volatile int currentL;	// current LED
extern void (*currentF)(void);	// current function
extern volatile float currentB; // Default brightness
extern volatile bool stopped;
extern volatile int numStopped;

// Current Time
// Array of Stepper positions
extern volatile uint16_t stepper_pos[NUM_STEPPERS]; // 0 - 516
// Array of LED brightnesses
extern float brightnesses[NUM_LEDS]; // 0.0 - 1.0
extern float speeds[NUM_STEPPERS];
extern long targets[NUM_STEPPERS];
// Current Temperature
extern sensors_event_t *humidity;
extern sensors_event_t *temperature;

// ---------- Function Prototypes ----------

void checkTemperatureCallback();
void checkTimeCallback();
void activityLEDCallback();
void LEDPowerOn();
void LEDPowerOff();
void STPPowerOn();
void STPPowerOff();

// Standard Mode - White - Default
void standardModeCallback_ResetLEDs();
void standardModeCallback_SetHome();
void standardModeCallback_GoHome();
void standardModeCallback_SetSTPSpeeds();
void standardModeCallback_RestartSTPRun();
// Frozen Mode - Blue
void frozenModeCallback_ResetLEDs();
void frozenModeCallback_StopSTPs();
// Random - Cyan
void randomModeCallback_ResetLEDs();
void randomModeCallback_SetSTPSpeeds();
// Dragon Flow - Purple
void pattern1ModeCallback_ResetLEDs();
void pattern1ModeCallback_SetHome();
void pattern1ModeCallback_GoHome();
void pattern1ModeCallback_StartPattern();
// Bubbling - Pink
void pattern2ModeCallback_ResetLEDs();
void pattern2ModeCallback_SetSTPSpeeds();
void pattern2ModeCallback_StartPattern();
// Resting Dragon - Green
void pattern3ModeCallback_ResetLEDs();
void pattern3ModeCallback_SetHome();
void pattern3ModeCallback_GoHome();

void encoder1A_ISR();
void encoder2A_ISR();
void encoder1Button_ISR();
void encoder2Button_ISR();
void powerSupervisor_ISR();

void updateLEDsCallback();
void setLEDCallback();
void showModeLED();
void showStatusLED();

void updateLEDsCallback();

// Stepper Callbacks
void runSTPCallback();
void runToSTPCallback();
void setSTPTargetsCallback();
void setSTPSpeedsCallback();
void stopSTPCallback();

// LED Callbacks

// Accel Stepper required functions
void stp1f1();
void stp1b1();
void stp2f1();
void stp2b1();
void stp3f1();
void stp3b1();
void stp4f1();
void stp4b1();
void stp5f1();
void stp5b1();
void stp6f1();
void stp6b1();
void stp7f1();
void stp7b1();
void stp8f1();
void stp8b1();
void stp9f1();
void stp9b1();
void stp10f1();
void stp10b1();
void stp11f1();
void stp11b1();
void stp12f1();
void stp12b1();
void stp13f1();
void stp13b1();
void stp14f1();
void stp14b1();
void stp15f1();
void stp15b1();
void stp16f1();
void stp16b1();
void stp17f1();
void stp17b1();
void stp18f1();
void stp18b1();
void stp19f1();
void stp19b1();
void stp20f1();
void stp20b1();

extern Scheduler ts;

// Mode Tasks
extern Task StandardModeTask;
extern Task FrozenModeTask;
extern Task RandomModeTask;
extern Task Pattern1ModeTask;
extern Task Pattern2ModeTask;
extern Task Pattern3ModeTask;

// Miscellaneous Tasks
extern Task CheckTimeTask;
extern Task CheckTemperatureTask;
extern Task ShowStatusLEDTask;
extern Task ShowModeLEDTask;
extern Task ActivityLEDTask;

// Stepper Tasks
extern Task SetSTPTargetTask;
extern Task SetSTPSpeedsTask;
extern Task RunSTPTask;
extern Task RunToSTPTask;
extern Task StopSTPTask;

// LED Tasks
extern Task UpdateLEDsTask;
extern Task SetLEDTask;

// Reference Task
extern Task *previousT;

#endif