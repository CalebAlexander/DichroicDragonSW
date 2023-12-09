// ---------- Import libraries ----------
#include "DD_Library.h"

// ---------- Instantiate Objects ----------
Adafruit_MotorShield AFMS[] = {
	Adafruit_MotorShield(PWM_1_ADDRESS), // PWM1
	Adafruit_MotorShield(PWM_2_ADDRESS), // PWM2
	Adafruit_MotorShield(PWM_3_ADDRESS), // PWM3 blah
	Adafruit_MotorShield(PWM_4_ADDRESS), // PWM4
	Adafruit_MotorShield(PWM_5_ADDRESS), // PWM5
	Adafruit_MotorShield(PWM_6_ADDRESS), // PWM6
	Adafruit_MotorShield(PWM_7_ADDRESS), // PWM7
	Adafruit_MotorShield(PWM_8_ADDRESS), // PWM8
	Adafruit_MotorShield(PWM_9_ADDRESS), // PWM9
	Adafruit_MotorShield(PWM_10_ADDRESS) // PWM10
};

Adafruit_StepperMotor *AFMS_Steppers[] = {
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

AccelStepper steppers[] = {
	AccelStepper(stp1f1, stp1b1),
	AccelStepper(stp2f1, stp2b1),
	AccelStepper(stp3f1, stp3b1),
	AccelStepper(stp4f1, stp4b1),
	AccelStepper(stp5f1, stp5b1),
	AccelStepper(stp6f1, stp6b1),
	AccelStepper(stp7f1, stp7b1),
	AccelStepper(stp8f1, stp8b1),
	AccelStepper(stp9f1, stp9b1),
	AccelStepper(stp10f1, stp10b1),
	AccelStepper(stp11f1, stp11b1),
	AccelStepper(stp12f1, stp12b1),
	AccelStepper(stp13f1, stp13b1),
	AccelStepper(stp14f1, stp14b1),
	AccelStepper(stp15f1, stp15b1),
	AccelStepper(stp16f1, stp16b1),
	AccelStepper(stp17f1, stp17b1),
	AccelStepper(stp18f1, stp18b1),
	AccelStepper(stp19f1, stp19b1),
	AccelStepper(stp20f1, stp20b1),
};

const int LEDPins[] = {
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

float brightnesses[] = {
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
};
float speeds[] = {
	STEPPER_DEFAULT_RPM, // STP1
	STEPPER_DEFAULT_RPM, // STP2
	STEPPER_DEFAULT_RPM, // STP3
	STEPPER_DEFAULT_RPM, // STP4
	STEPPER_DEFAULT_RPM, // STP5
	STEPPER_DEFAULT_RPM, // STP6
	STEPPER_DEFAULT_RPM, // STP7
	STEPPER_DEFAULT_RPM, // STP8
	STEPPER_DEFAULT_RPM, // STP9
	STEPPER_DEFAULT_RPM, // STP10
	STEPPER_DEFAULT_RPM, // STP11
	STEPPER_DEFAULT_RPM, // STP12
	STEPPER_DEFAULT_RPM, // STP13
	STEPPER_DEFAULT_RPM, // STP14
	STEPPER_DEFAULT_RPM, // STP15
	STEPPER_DEFAULT_RPM, // STP16
	STEPPER_DEFAULT_RPM, // STP17
	STEPPER_DEFAULT_RPM, // STP18
	STEPPER_DEFAULT_RPM, // STP19
	STEPPER_DEFAULT_RPM	 // STP20
};
long targets[] = {
	HOME, // STP1
	HOME, // STP2
	HOME, // STP3
	HOME, // STP4
	HOME, // STP5
	HOME, // STP6
	HOME, // STP7
	HOME, // STP8
	HOME, // STP9
	HOME, // STP10
	HOME, // STP11
	HOME, // STP12
	HOME, // STP13
	HOME, // STP14
	HOME, // STP15
	HOME, // STP16
	HOME, // STP17
	HOME, // STP18
	HOME, // STP19
	HOME, // STP20
};

Adafruit_MCP23X17 GPIO_Expander1 = Adafruit_MCP23X17();
Adafruit_MCP23X17 GPIO_Expander2 = Adafruit_MCP23X17();
Adafruit_SHT4x Temp_Sensor = Adafruit_SHT4x();
ExternalEEPROM myMem = ExternalEEPROM();

volatile int encoder1Pos = ENCODER_TICKS / 2;
volatile int encoder1ButtonState = 0;
volatile int encoder2Pos = ENCODER_TICKS / 2;
volatile int encoder2ButtonState = 0;

volatile int currentI = 0;
volatile int currentL = 0;
volatile int currentP = 0;
volatile int currentD = FORWARD;
void (*currentF)(void) = NULL;
volatile float currentB = LED_DEFAULT_BRIGHTNESS;
volatile bool goBack = false;
volatile bool stopped = true;
volatile int numStopped = 0;

sensors_event_t *temperature = NULL;
sensors_event_t *humidity = NULL;

Status currentStatus = OK;
Mode currentMode = STANDARD;

Scheduler ts;
// Mode Tasks
Task StandardModeTask(TASK_IMMEDIATE, TASK_ONCE, &standardModeCallback_ResetLEDs, &ts, true); // Default Mode
Task FrozenModeTask(TASK_IMMEDIATE, TASK_ONCE, &frozenModeCallback_ResetLEDs, &ts, false);
Task RandomModeTask(TASK_IMMEDIATE, TASK_ONCE, &randomModeCallback_ResetLEDs, &ts, false);
Task Pattern1ModeTask(TASK_IMMEDIATE, TASK_ONCE, &pattern1ModeCallback_ResetLEDs, &ts, false);
Task Pattern2ModeTask(TASK_IMMEDIATE, TASK_ONCE, &pattern2ModeCallback_ResetLEDs, &ts, false);
Task Pattern3ModeTask(TASK_IMMEDIATE, TASK_ONCE, &pattern3ModeCallback_ResetLEDs, &ts, false);

// Miscellaneous Tasks
Task CheckTemperatureTask(TEMP_UPDATE_RATE *TASK_SECOND, TASK_FOREVER, &checkTemperatureCallback, &ts, true);
Task CheckTimeTask(RTC_UPDATE_RATE *TASK_SECOND, TASK_FOREVER, &checkTimeCallback, &ts, true);
Task ShowStatusLEDTask(STATUS_LED_FLASH_RATE *TASK_MILLISECOND, TASK_FOREVER, &showStatusLED, &ts, false);
Task ShowModeLEDTask(TASK_IMMEDIATE, TASK_ONCE, &showModeLED, &ts, false);
Task ActivityLEDTask(ACTIVITY_LED_RATE *TASK_MILLISECOND, TASK_FOREVER, &activityLEDCallback, &ts, true);

// Stepper Tasks
Task RunSTPTask(TASK_IMMEDIATE, TASK_FOREVER, &runSTPCallback, &ts, false);
Task RunToSTPTask(TASK_IMMEDIATE, TASK_FOREVER, &runToSTPCallback, &ts, false);
Task StopSTPTask(TASK_IMMEDIATE, NUM_STEPPERS, &stopSTPCallback, &ts, false);
Task SetSTPSpeedsTask(TASK_IMMEDIATE, NUM_STEPPERS, &setSTPSpeedsCallback, &ts, false);
Task SetSTPTargetTask(TASK_IMMEDIATE, NUM_STEPPERS, &setSTPTargetsCallback, &ts, false);

// LED Tasks
Task UpdateLEDsTask(LED_UPDATE_RATE *TASK_MILLISECOND, TASK_FOREVER, &updateLEDsCallback, &ts, false);
Task SetLEDTask(TASK_IMMEDIATE, NUM_LEDS, &setLEDCallback, &ts, false);
Task *previousT = NULL;

void setup()
{
	// --------- ON START-UP ----------

	// Set up communications
	Serial.begin(9600); // set up Serial library at 9600 bps
	while (!Serial)
		;

	Wire.begin();
	Wire.setClock(400000);

	// Set up Steppers and PWMs
	Serial.println("--------- Setting up Steppers and PWMs -------------");
	for (int i = 0; i < NUM_PWMS; i++)
	{
		if (!AFMS[i].begin())
		{ // create with the default frequency 1.6KHz
			// if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
			Serial.println("Could not find Motor Shield " + String(i) + ". Check wiring.");
			while (1)
				;
		}
		Serial.println("Motor Shield " + String(i) + " found.");
		steppers[2 * i].setMaxSpeed(STEPPER_MAX_RPM);	   // 10 rpm
		steppers[2 * i].setSpeed(STEPPER_DEFAULT_RPM);	   // 10 rpm
		steppers[2 * i + 1].setMaxSpeed(STEPPER_MAX_RPM);  // 10 rpm
		steppers[2 * i + 1].setSpeed(STEPPER_DEFAULT_RPM); // 10 rpm
	}
	Serial.println("Found Steppers and PWMs");

	// Set up GPIO Port Expander1
	Serial.println("--------- Setting up GPIO Port Expander1 -------------");
	if (!GPIO_Expander1.begin_I2C())
	{
		// if (!mcp.begin_SPI(CS_PIN)) {
		Serial.println("Error. Couldn't set up GPIO Port Expander1");
		while (1)
			;
	}
	Serial.println("Found GPIO Expander1");

	// Set up GPIO Port Expander2
	Serial.println("--------- Setting up GPIO Port Expander2 -------------");
	if (!GPIO_Expander2.begin_I2C())
	{
		// if (!mcp.begin_SPI(CS_PIN)) {
		Serial.println("Error. Couldn't set up GPIO Port Expander2");
		while (1)
			;
	}
	Serial.println("Found GPIO Expander2");

	// Set up Temperature Sensor
	Serial.println("--------- Setting up Temperature Sensor -------------");
	if (!Temp_Sensor.begin())
	{
		Serial.println("Couldn't find SHT4x");
		while (1)
			;
	}
	Serial.println("Found SHT4x sensor");
	Serial.print("Serial number 0x");
	Serial.println(Temp_Sensor.readSerial(), HEX);

	// You can have 3 different precisions, higher precision takes longer
	Temp_Sensor.setPrecision(SHT4X_LOW_PRECISION);
	switch (Temp_Sensor.getPrecision())
	{
	case SHT4X_HIGH_PRECISION:
		Serial.println("High precision");
		break;
	case SHT4X_MED_PRECISION:
		Serial.println("Med precision");
		break;
	case SHT4X_LOW_PRECISION:
		Serial.println("Low precision");
		break;
	}

	// You can have 6 different heater settings
	// higher heat and longer times uses more power
	// and reads will take longer too!
	Temp_Sensor.setHeater(SHT4X_NO_HEATER);
	switch (Temp_Sensor.getHeater())
	{
	case SHT4X_NO_HEATER:
		Serial.println("No heater");
		break;
	case SHT4X_HIGH_HEATER_1S:
		Serial.println("High heat for 1 second");
		break;
	case SHT4X_HIGH_HEATER_100MS:
		Serial.println("High heat for 0.1 second");
		break;
	case SHT4X_MED_HEATER_1S:
		Serial.println("Medium heat for 1 second");
		break;
	case SHT4X_MED_HEATER_100MS:
		Serial.println("Medium heat for 0.1 second");
		break;
	case SHT4X_LOW_HEATER_1S:
		Serial.println("Low heat for 1 second");
		break;
	case SHT4X_LOW_HEATER_100MS:
		Serial.println("Low heat for 0.1 second");
		break;
	}

	// Read from NV memory
	if (myMem.begin() == false)
	{
		Serial.println("No memory detected. Freezing.");
		while (1)
			;
	}
	Serial.println("Memory detected!");

	myMem.setMemorySize(512 * 1024 / 8); // Qwiic EEPROM is the 24512C (512k bit)

	// Setup RTC
	rtc.begin(); // Call rtc.begin() to initialize the library
	// (Optional) Sets the SQW output to a 1Hz square wave.
	// (Pull-up resistor is required to use the SQW pin.)
	rtc.writeSQW(SQW_SQUARE_1);
	rtc.autoTime();

	// Set initial states
	// Set operating mode
	// Turn on LED Power and Motor Power
	// Add tasks to scheduler
	pinMode(ENCODER_1BTN_PIN, INPUT);
	pinMode(ENCODER_1A_PIN, INPUT);
	pinMode(ENCODER_1B_PIN, INPUT);
	pinMode(ENCODER_2BTN_PIN, INPUT);
	pinMode(ENCODER_2A_PIN, INPUT);
	pinMode(ENCODER_2B_PIN, INPUT);
	pinMode(PWR_SUPERVISOR_PIN, INPUT);
	pinMode(ACTIVITY_LED_PIN, OUTPUT);
	pinMode(STP_PWR_PIN, HIZ);
	pinMode(LED_PWR_PIN, HIZ);

	pinMode(SQW_INPUT_PIN, INPUT_PULLUP);
	pinMode(SQW_OUTPUT_PIN, OUTPUT);
	digitalWrite(SQW_OUTPUT_PIN, digitalRead(SQW_INPUT_PIN));

	// Set up hardware interrupts
	attachInterrupt(digitalPinToInterrupt(ENCODER_1BTN_PIN), encoder1Button_ISR, FALLING);
	attachInterrupt(digitalPinToInterrupt(ENCODER_1A_PIN), encoder1A_ISR, RISING);
	attachInterrupt(digitalPinToInterrupt(ENCODER_2BTN_PIN), encoder2Button_ISR, FALLING);
	attachInterrupt(digitalPinToInterrupt(ENCODER_2A_PIN), encoder2A_ISR, RISING);
	attachInterrupt(digitalPinToInterrupt(PWR_SUPERVISOR_PIN), powerSupervisor_ISR, FALLING);
	// --------------------------------
}

void loop()
{
	// start scheduler
	ts.execute();
}