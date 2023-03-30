#include "DD_Library.h"

/* Sample Methods
// steppers[0]->step(100, DIRECTION, STEPSIZE(SINGLE, DOUBLE, MICROSTEP));
// PWMs[0].setPWM(PIN, LOW(0), HIGH(4092))
// GPIO_Expander.digitalWrite(LED_PIN, !GPIO_Expander.digitalRead(BUTTON_PIN));
// Temp_Sensor.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
// rtc.update(), rtc.seconds(), rtc.minutes(), rtc.date(), rtc.dayStr(), etc
*/

Task CheckTemperatureTask(TEMP_UPDATE_RATE *TASK_SECOND, TASK_FOREVER, &checkTemperatureCallback, &ts, true);

Task StandardModeTask(STANDARD_MODE_UPDATE_RATE *TASK_MILLISECOND, TASK_FOREVER, &standardModeCallback, &ts, true); // Default Mode
Task FrozenModeTask(TASK_IMMEDIATE, TASK_ONCE, &randomModeCallback, &ts, false);
Task RandomModeTask(RANDOM_MODE_UPDATE_RATE *TASK_MILLISECOND, TASK_ONCE, &randomModeCallback, &ts, false);
Task Pattern1ModeTask(PATTERN1_MODE_UPDATE_RATE *TASK_MILLISECOND, TASK_FOREVER, &pattern1ModeCallback, &ts, false);
Task Pattern2ModeTask(PATTERN2_MODE_UPDATE_RATE *TASK_MILLISECOND, TASK_FOREVER, &pattern2ModeCallback, &ts, false);
Task Pattern3ModeTask(TASK_IMMEDIATE, TASK_ONCE, &pattern3ModeCallback, &ts, false);

// --------- Task Scheduler Callbacks ----------
// Maintenance
void checkTemperatureCallback()
{
	Temp_Sensor.getEvent(humidity, temperature); // populate temp and humidity objects with fresh data

	// check temperature over threshold

	// throw error and turn things off
}

void updateStepper(int I, int S, int D)
{
	// STBY OFF
	steppers[I]->step(S, D, SINGLE);
	// STBY ON
	switch (D)
	{
	case FORWARD:
		stepper_pos[I] = (stepper_pos[I] + S) % STEPPER_STEPS;
		break;
	case BACKWARD:
		stepper_pos[I] = (stepper_pos[I] - S) % STEPPER_STEPS;
		break;
	}
}
void updateLED(int I, float B)
{
	AFMS[I].setPWM(LEDPins[I] / PWMS_IN_USE, int(B * LED_PWM_MAX));
	brightnesses[I] = B;
}
void standardUpdateAllSteppersCallback()
{
	updateStepper(currentI, currentS, currentD);
	currentI++;
	Task &currentT = ts.currentTask();
	if (currentI < NUM_STEPPERS)
	{
		// done moving steppers
		currentT.forceNextIteration();
		return;
	}
	currentI = 0;
	currentT.setCallback(currentF);
	if (goBack)
	{
		currentT.forceNextIteration();
	}
}
void standardUpdateAllLEDsCallback()
{
	updateLED(currentI, currentB);
	currentI++;
	Task &currentT = ts.currentTask();
	if (currentI < NUM_LEDS)
	{
		// done moving steppers
		currentT.forceNextIteration();
		return;
	}
	currentI = 0;
	currentT.setCallback(currentF);
	if (goBack)
	{
		currentT.forceNextIteration();
	}
}
void randomUpdateAllSteppersCallback()
{
}
void randomUpdateAllLEDsCallback()
{
}

// Modes

// STANDARD_MODE_UPDATE_RATE 4651  //ms      -->> one rotation every hour 60min*60sec = 2400 / 516ticks = 4.651 secs/tick
void standardModeCallback()
{
	currentI = 0;
	currentS = STANDARD_MODE_STEP_SIZE;
	Task &currentT = ts.currentTask();
	currentD = FORWARD;
	currentF = standardModeCallback;
	// If first iteration, set LEDs
	if (currentT.getIterations() == 0)
	{
		StandardModeTask.setCallback(&standardUpdateAllLEDsCallback);
		goBack = true; // come back to this task to update steppers after LEDs
		StandardModeTask.forceNextIteration();
	}
	// Move Steppers without blocking
	StandardModeTask.setCallback(&standardUpdateAllSteppersCallback);
	goBack = false;
	StandardModeTask.forceNextIteration();
}

// FROZEN_MODE_UPDATE_RATE 10      //ms      -->> only executes once so doesn't really matter
void frozenModeCallback()
{
	currentI = 0;
	currentS = 0;
	currentD = -1;
	currentF = frozenModeCallback;
}
// RANDOM_MODE_UPDATE_RATE 100     //ms      -->> fastest rotation will be 10ticks/sec and all other random ones with me multiples of that
void randomModeCallback()
{
	currentI = 0;
	currentS = STANDARD_MODE_STEP_SIZE;
	// Task &currentT = ts.currentTask();
	currentD = FORWARD;
	currentF = standardModeCallback;

	RandomModeTask.setCallback(&randomUpdateAllSteppersCallback);
	goBack = false;
	RandomModeTask.forceNextIteration();
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
	static unsigned long lastEncoder2InterruptTime = 0;
	unsigned long interruptTime = millis();

	if (interruptTime - lastEncoder2InterruptTime > ENCODER_DEBOUNCE_TIME)
	{ // faster than 5ms is a bounce
		if (digitalRead(ENCODER_2B_PIN) == LOW)
		{
			// right turn
			encoder2Pos--;
		}
		else
		{
			// left turn
			encoder2Pos++;
		}
	}
	encoder2Pos = max(0, min(ENCODER_TICKS, encoder2Pos));
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
}