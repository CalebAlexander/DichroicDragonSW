// ---------- Import libraries ----------
#include <Arduino.h>
#include "DD_Library.h"

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

		// Configure PWMs
		PWMs[i].begin();
		PWMs[i].setOscillatorFrequency(27000000);
		PWMs[i].setPWMFreq(1600);
	}
	Serial.println("Found Steppers and PWMs");

	// Set up GPIO Port Expander
	Serial.println("--------- Setting up GPIO Port Expander -------------");
	if (!GPIO_Expander.begin_I2C())
	{
		// if (!mcp.begin_SPI(CS_PIN)) {
		Serial.println("Error. Couldn't set up GPIO Port Expander");
		while (1)
			;
	}
	Serial.println("Found GPIO Expander");

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

	// Set up RTC
	Serial.println("--------- Setting up RTC -------------");
	rtc.begin(); // Call rtc.begin() to initialize the library
	// Now set the time...
	// You can use the autoTime() function to set the RTC's clock and
	// date to the compiliers predefined time. (It'll be a few seconds
	// behind, but close!)
	rtc.autoTime();

	// Read from NV memory
	// Set initial states
	// Set operating mode
	// Turn on LED Power and Motor Power
	// Add tasks to scheduler

	// Set up input pins
	// pinMode(powerButtonPin, INPUT);
	pinMode(encoder1ButtonPin, INPUT); // consider using internal pull ups to help debounce?
	pinMode(encoder1APin, INPUT);
	pinMode(encoder1BPin, INPUT);
	pinMode(encoder2ButtonPin, INPUT);
	pinMode(encoder2APin, INPUT);
	pinMode(encoder2BPin, INPUT);

	// Set up hardware interrupts
	attachInterrupt(digitalPinToInterrupt(encoder1ButtonPin), encoder1Button_ISR(), FALLING);
	attachInterrupt(digitalPinToInterrupt(encoder1APin), encoder1A_ISR(), RISING);
	attachInterrupt(digitalPinToInterrupt(encoder2ButtonPin), encoder2Button_ISR(), FALLING);
	attachInterrupt(digitalPinToInterrupt(encoder2APin), encoder2A_ISR(), RISING);
	attachInterrupt(digitalPinToInterrupt(powerSupervisorPin), powerSupervisor_ISR(), FALLING);
	// --------------------------------
}

void loop()
{
	// start scheduler
	ts.execute();
}