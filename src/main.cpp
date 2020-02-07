#include <Arduino.h>
#include "GyverButton.h"

// Hardware settigs
#define ledsCount 3 // 2 (красный/зеленый) или 3 (красный/желтый/зеленый)

#define redLedPin 1
#define yellowLedPin 2
#define greenLedPin 3

#define buttonPin 4

// Software settings
#define LIGHT_CHANGE_TIMEOUT_MS 2000
#define SWITCH_WITH_BLINKS true
#define isButtonControl true
#define shouldStartInAutoMode true

#define	RED_LIGHT_TIMEOUT_MS 10000
#define	RED_YELLOW_LIGHT_TIMEOUT_MS 2000
#define	GREEN_LIGHT_TIMEOUT_MS 10000
#define GREEN_LIGHT_BLINK_TIMEOUT_MS 750
#define	GREEN_LIGHT_BLINK_COUNT 3
#define	YELLOW_LIGHT_TIMEOUT_MS 2000

bool isRed = true;
bool isYellow = false;
bool isGreen = false;
bool isGreenBlinking = false;

bool isAutoMode = true;

GButton button(buttonPin, LOW_PULL, NORM_OPEN);
unsigned long nextLightChangeTimeMs = 0;

void blinkLight(uint8_t pin, uint8_t count, int timeoutMs)
{
	for (size_t i = 0; i < count * 2; i++)
	{
		digitalWrite(pin, i % 2);
		delay(timeoutMs);
	}
}

void setNextLights()
{
	if (ledsCount == 2)
	{
		isRed = !isRed;
		isGreen = !isGreen;
	}
	else if (ledsCount == 3)
	{
		if (isRed)
		{
			if (isYellow)
			{
				isRed = false;
				isYellow = false;
				isGreen = true;
				nextLightChangeTimeMs = millis() + GREEN_LIGHT_TIMEOUT_MS;
			}
			else
			{
				isRed = true;
				isYellow = true;
				isGreen = false;
				nextLightChangeTimeMs = millis() + RED_YELLOW_LIGHT_TIMEOUT_MS;
			}
		}
		else if(isYellow)
		{
			isRed = true;
			isYellow = false;
			isGreen = false;
			nextLightChangeTimeMs = millis() + RED_LIGHT_TIMEOUT_MS;
		}
		else if (isGreen)
		{
			blinkLight(greenLedPin, GREEN_LIGHT_BLINK_COUNT, GREEN_LIGHT_BLINK_TIMEOUT_MS);
			isRed = false;
			isYellow = true;
			isGreen = false;
			nextLightChangeTimeMs = millis() + YELLOW_LIGHT_TIMEOUT_MS;
		}
	}
}

void showCurrentLights()
{
	digitalWrite(redLedPin, isRed ? HIGH : LOW);
	if (ledsCount == 3)
	{
		digitalWrite(yellowLedPin, isYellow ? HIGH : LOW);
	}
	digitalWrite(greenLedPin, isGreen ? HIGH : LOW);
}

void setup()
{
	isAutoMode = shouldStartInAutoMode;

	pinMode(redLedPin, OUTPUT);
	if (ledsCount == 3)
	{
		pinMode(yellowLedPin, OUTPUT);
	}
	pinMode(greenLedPin, OUTPUT);
	nextLightChangeTimeMs = millis() + RED_LIGHT_TIMEOUT_MS;
}

void loop()
{
	button.tick();

	if (button.isDouble())
	{
		isAutoMode = !isAutoMode;
		return;
	}

	if (isAutoMode)
	{
		if (millis() >= nextLightChangeTimeMs)
		{
			setNextLights();
		}
	}
	if (button.isClick())
	{
		setNextLights();
	}

	showCurrentLights();
}
