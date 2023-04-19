#include "DD_Library.h"
// ------------------------- Maintenance Tasks ---------------------------
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

// -------------------------- Standard Mode ------------------------------
// (1) - RESET LEDS
void standardModeCallback_ResetLEDs()
{
	previousT = ts.getCurrentTask();
	currentMode = STANDARD;
	ShowModeLEDTask.restart();
	// Reset LEDs
	UpdateLEDsTask.disable(); // stop constantly updating the LEDs
	memset(brightnesses, currentB, sizeof(brightnesses));
	SetLEDTask.restart();
	// SetLEDTask.forceNextIteration();
	StandardModeTask.setCallback(&standardModeCallback_SetHome);
}
// (2) - SET STP TARGETS TO HOME
void standardModeCallback_SetHome()
{
	// Reset targets
	memset(targets, HOME, sizeof(targets));
	SetSTPTargetTask.restart();
	StandardModeTask.setCallback(&standardModeCallback_GoHome);
}
// (3) - RUN STP TO HOME
void standardModeCallback_GoHome()
{
	// turn off indefinite running
	RunSTPTask.disable();
	// run until home position
	RunToSTPTask.restart();
	StandardModeTask.setCallback(&standardModeCallback_SetSTPSpeeds);
}
// (4) - SET NEW STP SPEEDS
void standardModeCallback_SetSTPSpeeds()
{
	// Set stepper speeds
	SetSTPSpeedsTask.restart();
	StandardModeTask.setCallback(&standardModeCallback_RestartSTPRun);
}
// (5) - RESTART STP RUN SPEED
void standardModeCallback_RestartSTPRun()
{
	previousT = NULL; // this is last step so don't need to come back
	// enable run task
	RunSTPTask.restart();
	// Done
	StandardModeTask.setCallback(&standardModeCallback_ResetLEDs);
}

// -------------------------- Frozen Mode ------------------------------

// (1) - RESET LEDS
void frozenModeCallback_ResetLEDs()
{
	previousT = ts.getCurrentTask();
	currentMode = FROZEN;
	ShowModeLEDTask.restart();
	// Reset LEDs
	UpdateLEDsTask.disable(); // stop constantly updating the LEDs
	memset(brightnesses, currentB, sizeof(brightnesses));
	SetLEDTask.restart();
	// SetLEDTask.forceNextIteration();
	FrozenModeTask.setCallback(&frozenModeCallback_StopSTPs);
}
// (2) - STOP STPS
void frozenModeCallback_StopSTPs()
{
	previousT = NULL; // this is last step so don't need to come back
	// disable indefinite running
	RunSTPTask.disable();
	StopSTPTask.restart();
	// Done
	FrozenModeTask.setCallback(&frozenModeCallback_ResetLEDs);
}

// -------------------------- Random Mode ------------------------------

// (1) - RESET LEDS
void randomModeCallback_ResetLEDs()
{
	previousT = ts.getCurrentTask();
	currentMode = RANDOM;
	ShowModeLEDTask.restart();
	// Reset LEDs
	UpdateLEDsTask.disable(); // stop constantly updating the LEDs
	memset(brightnesses, currentB, sizeof(brightnesses));
	SetLEDTask.restart();
	// SetLEDTask.forceNextIteration();
	RandomModeTask.setCallback(&randomModeCallback_SetSTPSpeeds);
}
// (2) - SET NEW STP SPEEDS
void randomModeCallback_SetSTPSpeeds()
{
	previousT = NULL; // this is last step so don't need to come back
	// Generate Random Speeds and directions

	// Set stepper speeds
	SetSTPSpeedsTask.restart();
	RandomModeTask.setCallback(&randomModeCallback_ResetLEDs);
}
// -------------------------- Pattern 1 Mode - Dragon Flow------------------------------

// (1) - RESET LEDS
void pattern1ModeCallback_ResetLEDs()
{
	previousT = ts.getCurrentTask();
	currentMode = PATTERN1;
	ShowModeLEDTask.restart();
	// Reset LEDs
	UpdateLEDsTask.disable();						 // stop constantly updating the LEDs
	memset(brightnesses, OFF, sizeof(brightnesses)); // turn all LEDs off
	SetLEDTask.restart();
	// SetLEDTask.forceNextIteration();
	Pattern1ModeTask.setCallback(&pattern1ModeCallback_SetHome);
}
// (2) - SET STP TARGETS TO HOME
void pattern1ModeCallback_SetHome()
{
	// Reset targets
	memset(targets, HOME, sizeof(targets));
	SetSTPTargetTask.restart();
	Pattern1ModeTask.setCallback(&pattern1ModeCallback_GoHome);
}
// (3) - RUN STP TO HOME
void pattern1ModeCallback_GoHome()
{

	// turn off indefinite running
	RunSTPTask.disable();
	// run until home position
	RunToSTPTask.restart();
	Pattern1ModeTask.setCallback(&pattern1ModeCallback_StartPattern);
}
// (4) - START LED PATTERN
void pattern1ModeCallback_StartPattern()
{
	previousT = NULL; // this is last step so don't need to come back

	UpdateLEDsTask.restart();
	Pattern1ModeTask.setCallback(&pattern1ModeCallback_ResetLEDs);
}

// -------------------------- Pattern 2 Mode - Bubbling ------------------------------

// (1) - RESET LEDS
void pattern2ModeCallback_ResetLEDs()
{
	previousT = ts.getCurrentTask();
	currentMode = PATTERN2;
	ShowModeLEDTask.restart();
	// Reset LEDs
	// UpdateLEDsTask.disable();						 // stop constantly updating the LEDs
	// memset(brightnesses, OFF, sizeof(brightnesses)); // turn all LEDs off
	// SetLEDTask.restart();
	// SetLEDTask.forceNextIteration();
	Pattern2ModeTask.setCallback(&pattern2ModeCallback_SetSTPSpeeds);
}
// (2) - SET NEW STP SPEEDS
void pattern2ModeCallback_SetSTPSpeeds()
{
	// Generate Random Speeds and directions

	// Set stepper speeds
	SetSTPSpeedsTask.restart();
	Pattern2ModeTask.setCallback(&pattern2ModeCallback_StartPattern);
}
// (3) - START LED PATTERN
void pattern2ModeCallback_StartPattern()
{
	previousT = NULL; // this is last step so don't need to come back

	UpdateLEDsTask.restart();
	RunSTPTask.restart();
	Pattern2ModeTask.setCallback(&pattern2ModeCallback_ResetLEDs);
}

// -------------------------- Pattern 3 Mode - Resting Dragon ------------------------------
// (1) - RESET LEDS
void pattern3ModeCallback_ResetLEDs()
{
	previousT = ts.getCurrentTask();
	currentMode = PATTERN3;
	ShowModeLEDTask.restart();
	// Reset LEDs
	UpdateLEDsTask.disable(); // stop constantly updating the LEDs
	memset(brightnesses, currentB, sizeof(brightnesses));
	SetLEDTask.restart();
	// SetLEDTask.forceNextIteration();
	Pattern3ModeTask.setCallback(&pattern3ModeCallback_SetHome);
}
// (2) - SET STP TARGETS TO HOME
void pattern3ModeCallback_SetHome()
{
	// Reset targets
	memset(targets, HOME, sizeof(targets));
	SetSTPTargetTask.restart();
	Pattern3ModeTask.setCallback(&pattern3ModeCallback_GoHome);
}
// (3) - RUN STP TO HOME
void pattern3ModeCallback_GoHome()
{
	previousT = NULL; // this is last step so don't need to come back
	// turn off indefinite running
	RunSTPTask.disable();
	// run until home position
	RunToSTPTask.restart();
	// done
	Pattern3ModeTask.setCallback(&pattern3ModeCallback_ResetLEDs);
}

// --------------------------- LED Methods -------------------------------

void updateLEDsCallback()
{
	// Generate new values
	switch (currentMode)
	{
	case PATTERN1:

		// Make LEDs "flow" along length of dragon
		break;
	case PATTERN2:

		// Make LEDs "bubble" randomly
		break;
	}

	// Start updating LEDs
	// previousT = NULL; // don't come back to this task immediately
	SetLEDTask.restart();
}
void setLEDCallback()
{
	if (ts.getCurrentTask()->isFirstIteration())
	{
		currentL = 0;
	}
	AFMS[currentL].setPWM(LEDPins[currentL], int(brightnesses[currentL] * LED_PWM_MAX));
	currentL = (currentL + 1) % NUM_LEDS;
	if (ts.getCurrentTask()->isLastIteration())
	{
		if (previousT != NULL)
		{
			previousT->restart();
			// previousT->forceNextIteration();
		}
	}
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
	if (ts.getCurrentTask()->getIterations() % 2 == 0)
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

// increments stepper if needed
void runSTPCallback() // iterate over
{
	steppers[currentI].runSpeed();
	currentI = (currentI + 1) % NUM_STEPPERS;
	// currentT.forceNextIteration();
}
// moves steppers until all reach target position
void runToSTPCallback()
{
	if (ts.getCurrentTask()->isFirstIteration())
	{
		numStopped = 0;
		currentI = 0;
	}
	if (!steppers[currentI].runSpeedToPosition()) // if motor does not step, then it is in position
	{
		numStopped++;
		if (numStopped >= NUM_STEPPERS)
		{
			// All motors are at targets
			// Stop this task
			ts.getCurrentTask()->disable();
			// Go back to previous task
			if (previousT != NULL)
			{
				previousT->restart();
				// previousT->forceNextIteration();
			}
		}
	}
	currentI = (currentI + 1) % NUM_STEPPERS;
}
// sets new targets for all steppers
void setSTPTargetsCallback()
{
	if (ts.getCurrentTask()->isFirstIteration())
	{
		currentI = 0;
	}
	steppers[currentI].moveTo(targets[currentI]);
	currentI = (currentI + 1) % NUM_STEPPERS;
	if (currentI >= NUM_STEPPERS) // all steppers set
	{
		// Stop this task
		ts.getCurrentTask()->disable();
		// Go back to previous task
		if (previousT != NULL)
		{
			previousT->restart();
			// previousT->forceNextIteration();
		}
	}
}
// sets new speeds for all steppers
void setSTPSpeedsCallback()
{
	if (ts.getCurrentTask()->isFirstIteration())
	{
		currentI = 0;
	}
	steppers[currentI].setSpeed(speeds[currentI]);
	currentI = (currentI + 1) % NUM_STEPPERS;
	if (currentI >= NUM_STEPPERS) // all steppers set
	{
		// Stop this task
		ts.getCurrentTask()->disable();
		// Go back to previous task
		if (previousT != NULL)
		{
			previousT->restart();
			// previousT->forceNextIteration();
		}
	}
}
// sets all target positions to current position, make sure to disable runSTP
void stopSTPCallback()
{
	if (ts.getCurrentTask()->isFirstIteration())
	{
		numStopped = 0;
		currentI = 0;
	}
	steppers[currentI].stop();
	currentI = (currentI + 1) % NUM_STEPPERS;
	numStopped++;
	if (numStopped >= NUM_STEPPERS) // all steppers stopped
	{
		stopped = true;
		// Stop this task
		ts.getCurrentTask()->disable();
		// Go back to previous task
		if (previousT != NULL)
		{
			previousT->restart();
			// previousT->forceNextIteration();
		}
	}
}

// ------------------------- Encoder and Button ISRs ----------------------------
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

// ------------------- Accel Stepper required functions --------------------------
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