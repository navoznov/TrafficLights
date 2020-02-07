#include <Arduino.h>
#include "GyverButton.h"

// Hardware settigs
#define ledsCount 3 // 2 (красный/зеленый) или 3 (красный/желтый/зеленый)
#define redLedPin 1
#define yellowLedPin 4
#define greenLedPin 2
#define buttonPin 3

// Software settings
#define lightChangeTimeoutMs 2000
#define isButtonControl true
#define isAutoControl true

bool isRed = true;
bool isGreen = false;
bool isAutoMode = true;

GButton button(buttonPin, LOW_PULL, NORM_OPEN);
long lastLightChangeMs = 0;

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
			isRed = false;
		}
		else if (isGreen)
		{
			isRed = true;
			isGreen = false;
		}
		else
		{
			isGreen = true;
		}
	}
}

void showCurrentLights()
{
	digitalWrite(redLedPin, isRed ? HIGH : LOW);
	if (ledsCount == 3)
	{
		digitalWrite(yellowLedPin, !isRed && !isGreen ? HIGH : LOW);
	}
	digitalWrite(greenLedPin, isGreen ? HIGH : LOW);
}

void setup()
{
	isAutoMode = isAutoControl;
	lastLightChangeMs = millis();

	pinMode(redLedPin, OUTPUT);
	if (ledsCount == 3)
	{
		pinMode(yellowLedPin, OUTPUT);
	}
	pinMode(greenLedPin, OUTPUT);
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
		if (millis() - lastLightChangeMs >= lightChangeTimeoutMs)
		{
			lastLightChangeMs = millis();
			setNextLights();
		}
	}
	else if (button.isClick())
	{
		setNextLights();
	}

	showCurrentLights();
}
