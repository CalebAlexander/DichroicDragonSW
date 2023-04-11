#define ENCODER_DEBOUNCE_TIME 5
#define ENCODER_TICKS 12

#include <Arduino.h>
#include <Adafruit_MotorShield.h>

const int encoderAPin = 3;
const int encoderBPin = 4;
const int encoderBtnPin = 2;
const int LEDPin = 0;
volatile uint16_t encoderPos = ENCODER_TICKS / 2;
volatile bool LEDStatus = false;

String dir = "";
unsigned long last_run = 0;

void encoderA_ISR();
void encoderBtn_ISR();

Adafruit_MotorShield AFMS(0x60); // Default address, no jumpers

void setup()
{
  Serial.begin(9600); // set up Serial library at 9600 bps
  while (!Serial)
    ;
  Serial.println("Starting Encoder + LED Test");
  AFMS.begin();

  // Set up hardware interrupts
  attachInterrupt(digitalPinToInterrupt(encoderAPin), encoderA_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderBtnPin), encoderBtn_ISR, RISING);
}

void loop()
{
  // put your main code here, to run repeatedly:
}

void encoderA_ISR()
{
  Serial.println("Encoder ISR");
  if (millis() - last_run > 5)
  {

    if (digitalRead(encoderBPin) == LOW)
    {
      encoderPos = min(encoderPos + 1, ENCODER_TICKS);
      dir = "CCW";
    }
    if (digitalRead(encoderBPin) == HIGH)
    {
      encoderPos = max(encoderPos - 1, 1);
      dir = "CW";
    }
    last_run = millis();
    AFMS.setPWM(LEDPin, 4096 / encoderPos);
    Serial.println("LED Brightness Set to: \t" + String(encoderPos));
  }
}

void encoderBtn_ISR()
{
  Serial.println("Btn ISR");
  LEDStatus = !LEDStatus;
  if (LEDStatus == true)
  {
    AFMS.setPWM(LEDPin, 4096 / encoderPos);
  }
  else
  {
    AFMS.setPWM(LEDPin, 0);
  }
  Serial.println("LED Status: \t" + String(LEDStatus));
}