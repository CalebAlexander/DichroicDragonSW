#include "DD_Library.h"

/* Sample Methods
  // steppers[0]->step(100, DIRECTION, STEPSIZE(SINGLE, DOUBLE, MICROSTEP));
  // PWMs[0].setPWM(PIN, LOW(0), HIGH(4092))
  // GPIO_Expander.digitalWrite(LED_PIN, !GPIO_Expander.digitalRead(BUTTON_PIN));
  // Temp_Sensor.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  // rtc.update(), rtc.seconds(), rtc.minutes(), rtc.date(), rtc.dayStr(), etc
*/

// --------- Task Scheduler Callbacks ----------
// Maintenance
void checkTemperatureCallback()
{
}

// void rtcCallback()
// {

// }

// Modes

// STANDARD_MODE_UPDATE_RATE 4651  //ms      -->> one rotation every hour 60min*60sec = 2400 / 516ticks = 4.651 secs/tick
void standardModeCallback()
{
  // Move Steppers without blocking
  // Update local stepper positions
}
// FROZEN_MODE_UPDATE_RATE 10      //ms      -->> only executes once so doesn't really matter
void frozenModeCallback()
{
  // Testing git
}
// RANDOM_MODE_UPDATE_RATE 100     //ms      -->> fastest rotation will be 10ticks/sec and all other random ones with me multiples of that
void randomModeCallback()
{
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
    if (digitalRead(encoder1PinB) == LOW)
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
    if (digitalRead(encoder2PinB) == LOW)
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