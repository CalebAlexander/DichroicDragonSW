#include "DD_Library.h"

/* Sample Methods
// steppers[0]->step(100, DIRECTION, STEPSIZE(SINGLE, DOUBLE, MICROSTEP));
// PWMs[0].setPWM(PIN, LOW(0), HIGH(4092))
// GPIO_Expander.digitalWrite(LED_PIN, !GPIO_Expander.digitalRead(BUTTON_PIN));
// Temp_Sensor.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
// rtc.update(), rtc.seconds(), rtc.minutes(), rtc.date(), rtc.dayStr(), etc
*/

Task StandardModeTask(TASK_IMMEDIATE, TASK_ONCE, &standardModeCallback, &ts, true); // Default Mode
Task FrozenModeTask(TASK_IMMEDIATE, TASK_ONCE, &randomModeCallback, &ts, false);
Task RandomModeTask(TASK_IMMEDIATE, TASK_ONCE, &randomModeCallback, &ts, false);
Task Pattern1ModeTask(TASK_IMMEDIATE, TASK_ONCE, &pattern1ModeCallback, &ts, false);
Task Pattern2ModeTask(TASK_IMMEDIATE, TASK_ONCE, &pattern2ModeCallback, &ts, false);
Task Pattern3ModeTask(TASK_IMMEDIATE, TASK_ONCE, &pattern3ModeCallback, &ts, false);

Task CheckTemperatureTask(TEMP_UPDATE_RATE *TASK_SECOND, TASK_FOREVER, &checkTemperatureCallback, &ts, true);
Task CheckTimeTask(RTC_UPDATE_RATE *TASK_SECOND, TASK_FOREVER, &checkTimeCallback, &ts, true);
Task RunSTPTask(TASK_IMMEDIATE, TASK_FOREVER, &runSTPCallback, &ts, false);
Task StopSTPTask(TASK_IMMEDIATE, NUM_STEPPERS, &stopSTPCallback, &ts, false);
Task UpdateLEDs(LED_UPDATE_RATE *TASK_MILLISECOND, TASK_FOREVER, &updateLEDsCallback, &ts, false);
Task SetLEDTask(TASK_IMMEDIATE, NUM_LEDS, &setLED, &ts, false);
Task ShowStatusLED(STATUS_LED_FLASH_RATE *TASK_MILLISECOND, TASK_FOREVER, &showStatusLED, &ts, false);
Task ShowModeLED(TASK_IMMEDIATE, TASK_ONCE, &showModeLED, &ts, false);
// --------- Task Scheduler Callbacks ----------
// Maintenance
void checkTemperatureCallback()
{
	Temp_Sensor.getEvent(humidity, temperature); // populate temp and humidity objects with fresh data

	// check temperature over threshold

	// throw error and turn things off
}
void checkTimeCallback()
{
	rtc.update();
	// Do something with this
}

// Modes
void standardModeCallback()
{
	// Reset LEDs
	memset(brightnesses, currentB, sizeof(brightnesses));

	SetLEDTask.restart();
	// Reset Steppers
	// disable run task, Stop steppers, goTo 0
	// Set stepper speeds
	// Start Steppers
	stopped = true;
	// enable run task
}
void frozenModeCallback()
{
	// Reset LEDs
	memset(brightnesses, currentB, sizeof(brightnesses));
	SetLEDTask.restart();
	// Stop Steppers
	// disable run task, Stop steppers
}
// RANDOM_MODE_UPDATE_RATE 100     //ms      -->> fastest rotation will be 10ticks/sec and all other random ones with me multiples of that
void randomModeCallback()
{
	// Reset LEDs
	memset(brightnesses, currentB, sizeof(brightnesses));
	SetLEDTask.restart();

	stopped = true;
}
// PATTERN1_MODE_UPDATE_RATE 100   //ms      -->> change brightness 10 times a second to make smooth flow appearance
void pattern1ModeCallback()
{
}
// PATTERN2_MODE_UPDATE_RATE 100   //ms      -->> fastest rotation will be 10ticks/sec and all other random ones with me multiples of that, ignore brightness knob interrupt
void pattern2ModeCallback()
{
}
// PATTERN3_MODE_UPDATE_RATE 10    //ms      -->> only executes once so doesn't really matter
void pattern3ModeCallback()
{
}

// ---------- Interrupt Service Routines ----------
void encoder1A_ISR()
{
	static unsigned long lastEncoder1InterruptTime = 0;
	unsigned long interruptTime = millis();

	if (interruptTime - lastEncoder1InterruptTime > ENCODER_DEBOUNCE_TIME)
	{ // faster than 5ms is a bounce
		if (digitalRead(ENCODER_1B_PIN) == LOW)
		{
			// right turn
			encoder1Pos--;
		}
		else
		{
			// left turn
			encoder1Pos++;
		}
	}
	encoder1Pos = max(0, min(ENCODER_TICKS, encoder1Pos));
	// update mode
	// do something
}
void encoder2A_ISR()
{
	// if (millis() - last_run > 5)
	// {
	// 	if (digitalRead(4) == 1)
	// 	{
	// 		counter++;
	// 		dir = "CW";
	// 	}
	// 	if (digitalRead(4) == 0)
	// 	{
	// 		counter--;
	// 		dir = "CCW";
	// 	}
	// 	last_run = millis();
	// }
	// update mode
	// do something
}

void encoder1Button_ISR()
{
}
void encoder2Button_ISR()
{
}

void powerSupervisor_ISR()
{
	// Update Status LED
	if (currentStatus != POWER_LOSS)
	{
		currentStatus = POWER_LOSS;
	}
	// Stop Steppers
	if (!stopped)
	{
		StopSTPTask.restart();
		return;
	}
	// Get Stepper positions
	// Write positions to EEPROM
	// Power off to LEDs and Steppers
}

void updateLEDsCallback()
{
	// Generate new values

	// Start updating LEDs
	SetLEDTask.restart();
}
void setLED()
{
	Task &currentT = ts.currentTask();
	int ledNum = currentT.getIterations() % NUM_STEPPERS;

	float brightness = brightnesses[ledNum];
	AFMS[ledNum].setPWM(LEDPins[ledNum] / PWMS_IN_USE, int(brightness * LED_PWM_MAX));
}

void showModeLED()
{
	switch (currentMode)
	{
	case STANDARD:
		// do nothing except show pattern mode
		analogWrite(STATUS_PIN_R, MODE_STANDARD_R);
		analogWrite(STATUS_PIN_G, MODE_STANDARD_G);
		analogWrite(STATUS_PIN_B, MODE_STANDARD_B);
		break;
	case FROZEN:
		analogWrite(STATUS_PIN_R, MODE_FROZEN_R);
		analogWrite(STATUS_PIN_G, MODE_FROZEN_G);
		analogWrite(STATUS_PIN_B, MODE_FROZEN_B);
		break;
	case RANDOM:
		analogWrite(STATUS_PIN_R, MODE_RANDOM_R);
		analogWrite(STATUS_PIN_G, MODE_RANDOM_G);
		analogWrite(STATUS_PIN_B, MODE_RANDOM_B);
		break;
	case PATTERN1:
		analogWrite(STATUS_PIN_R, MODE_PATTERN1_R);
		analogWrite(STATUS_PIN_G, MODE_PATTERN1_G);
		analogWrite(STATUS_PIN_B, MODE_PATTERN1_B);
		break;
	case PATTERN2:
		analogWrite(STATUS_PIN_R, MODE_PATTERN2_R);
		analogWrite(STATUS_PIN_G, MODE_PATTERN2_G);
		analogWrite(STATUS_PIN_B, MODE_PATTERN2_B);
		break;
	case PATTERN3:
		analogWrite(STATUS_PIN_R, MODE_PATTERN3_R);
		analogWrite(STATUS_PIN_G, MODE_PATTERN3_G);
		analogWrite(STATUS_PIN_B, MODE_PATTERN3_B);
		break;
	case NOMODE:
		analogWrite(STATUS_PIN_R, OFF);
		analogWrite(STATUS_PIN_G, OFF);
		analogWrite(STATUS_PIN_B, OFF);
		break;
	}
}

void showStatusLED()
{
	Task &currentT = ts.currentTask();
	if (currentT.getIterations() % 2 == 0)
	{
		// Turn LEDs ON
		switch (currentStatus)
		{
		case OK:
			// do nothing except show pattern mode
			break;
		case TEMP_HIGH:
			analogWrite(STATUS_PIN_R, STATUS_TEMP_HIGH_R);
			analogWrite(STATUS_PIN_G, STATUS_TEMP_HIGH_G);
			analogWrite(STATUS_PIN_B, STATUS_TEMP_HIGH_B);
			break;
		case BAD_COMMS:
			analogWrite(STATUS_PIN_R, STATUS_BAD_COMMS_R);
			analogWrite(STATUS_PIN_G, STATUS_BAD_COMMS_G);
			analogWrite(STATUS_PIN_B, STATUS_BAD_COMMS_B);
			break;
		case POWER_LOSS:
			analogWrite(STATUS_PIN_R, STATUS_POWER_LOSS_R);
			analogWrite(STATUS_PIN_G, STATUS_POWER_LOSS_G);
			analogWrite(STATUS_PIN_B, STATUS_POWER_LOSS_B);
			break;
		case NOSTATUS:
			analogWrite(STATUS_PIN_R, OFF);
			analogWrite(STATUS_PIN_G, OFF);
			analogWrite(STATUS_PIN_B, OFF);
			break;
		}
	}
	else
	{
		// Turn LEDs OFF
		analogWrite(STATUS_PIN_R, OFF);
		analogWrite(STATUS_PIN_G, OFF);
		analogWrite(STATUS_PIN_B, OFF);
	}
}

// --------------------------- Stepper Methods -------------------------------
void stopSTPCallback()
{
	Task &currentT = ts.currentTask();
	int stpNum = currentT.getIterations() % NUM_STEPPERS;
	steppers[stpNum].stop();
	if (currentT.isLastIteration() && goBack)
	{
		stopped = true;
	}
}
void runSTPCallback()
{
	Task &currentT = ts.currentTask();
	int stpNum = currentT.getIterations() % NUM_STEPPERS;
	steppers[stpNum].runSpeed();
	// currentT.forceNextIteration();
}
void stp1f1() { AFMS_Steppers[0]->onestep(FORWARD, SINGLE); }
void stp1b1() { AFMS_Steppers[0]->onestep(BACKWARD, SINGLE); }
void stp2f1() { AFMS_Steppers[1]->onestep(FORWARD, SINGLE); }
void stp2b1() { AFMS_Steppers[1]->onestep(BACKWARD, SINGLE); }
void stp3f1() { AFMS_Steppers[2]->onestep(FORWARD, SINGLE); }
void stp3b1() { AFMS_Steppers[2]->onestep(BACKWARD, SINGLE); }
void stp4f1() { AFMS_Steppers[3]->onestep(FORWARD, SINGLE); }
void stp4b1() { AFMS_Steppers[3]->onestep(BACKWARD, SINGLE); }
void stp5f1() { AFMS_Steppers[4]->onestep(FORWARD, SINGLE); }
void stp5b1() { AFMS_Steppers[4]->onestep(BACKWARD, SINGLE); }
void stp6f1() { AFMS_Steppers[5]->onestep(FORWARD, SINGLE); }
void stp6b1() { AFMS_Steppers[5]->onestep(BACKWARD, SINGLE); }
void stp7f1() { AFMS_Steppers[6]->onestep(FORWARD, SINGLE); }
void stp7b1() { AFMS_Steppers[6]->onestep(BACKWARD, SINGLE); }
void stp8f1() { AFMS_Steppers[7]->onestep(FORWARD, SINGLE); }
void stp8b1() { AFMS_Steppers[7]->onestep(BACKWARD, SINGLE); }
void stp9f1() { AFMS_Steppers[8]->onestep(FORWARD, SINGLE); }
void stp9b1() { AFMS_Steppers[8]->onestep(BACKWARD, SINGLE); }
void stp10f1() { AFMS_Steppers[9]->onestep(FORWARD, SINGLE); }
void stp10b1() { AFMS_Steppers[9]->onestep(BACKWARD, SINGLE); }
void stp11f1() { AFMS_Steppers[10]->onestep(FORWARD, SINGLE); }
void stp11b1() { AFMS_Steppers[10]->onestep(BACKWARD, SINGLE); }
void stp12f1() { AFMS_Steppers[11]->onestep(FORWARD, SINGLE); }
void stp12b1() { AFMS_Steppers[11]->onestep(BACKWARD, SINGLE); }
void stp13f1() { AFMS_Steppers[12]->onestep(FORWARD, SINGLE); }
void stp13b1() { AFMS_Steppers[12]->onestep(BACKWARD, SINGLE); }
void stp14f1() { AFMS_Steppers[13]->onestep(FORWARD, SINGLE); }
void stp14b1() { AFMS_Steppers[13]->onestep(BACKWARD, SINGLE); }
void stp15f1() { AFMS_Steppers[14]->onestep(FORWARD, SINGLE); }
void stp15b1() { AFMS_Steppers[14]->onestep(BACKWARD, SINGLE); }
void stp16f1() { AFMS_Steppers[15]->onestep(FORWARD, SINGLE); }
void stp16b1() { AFMS_Steppers[15]->onestep(BACKWARD, SINGLE); }
void stp17f1() { AFMS_Steppers[16]->onestep(FORWARD, SINGLE); }
void stp17b1() { AFMS_Steppers[16]->onestep(BACKWARD, SINGLE); }
void stp18f1() { AFMS_Steppers[17]->onestep(FORWARD, SINGLE); }
void stp18b1() { AFMS_Steppers[17]->onestep(BACKWARD, SINGLE); }
void stp19f1() { AFMS_Steppers[18]->onestep(FORWARD, SINGLE); }
void stp19b1() { AFMS_Steppers[18]->onestep(BACKWARD, SINGLE); }
void stp20f1() { AFMS_Steppers[19]->onestep(FORWARD, SINGLE); }
void stp20b1() { AFMS_Steppers[19]->onestep(BACKWARD, SINGLE); }