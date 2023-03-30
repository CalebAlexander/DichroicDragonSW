// ---------- Import libraries ----------
#include "DD_Library.h"

// ---------- Instantiate Objects ----------
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
};

Adafruit_MCP23X17 GPIO_Expander1 = Adafruit_MCP23X17();
Adafruit_MCP23X17 GPIO_Expander2 = Adafruit_MCP23X17();
Adafruit_SHT4x Temp_Sensor = Adafruit_SHT4x();

volatile int encoder1Pos = ENCODER_TICKS / 2;
volatile int encoder1ButtonState = 0;
volatile int encoder2Pos = ENCODER_TICKS / 2;
volatile int encoder2ButtonState = 0;

volatile int currentI = 0;
volatile int currentS = STANDARD_MODE_STEP_SIZE;
volatile int currentP = 0;
volatile int currentD = FORWARD;
void (*currentF)(void) = NULL;
volatile float currentB = LED_DEFAULT_BRIGHTNESS;
volatile bool goBack = false;

sensors_event_t *temperature = NULL;
sensors_event_t *humidity = NULL;

Status currentStatus = OK;
Mode currentMode = STANDARD;

Scheduler ts;

void setup()
{
	// --------- ON START-UP ----------

	// Set up communications
	Serial.begin(9600); // set up Serial library at 9600 bps
	while (!Serial)
		;

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
		steppers[i]->setSpeed(STEPPER_RPM);		// 10 rpm
		steppers[i + 1]->setSpeed(STEPPER_RPM); // 10 rpm
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