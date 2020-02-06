#include <Arduino.h>
#include "GyverButton.h"

#define ledsCount 3 // 2 (красный/зеленый) или 3 (красный/желтый/зеленый)
#define redLedPin 1
#define yellowLedPin 4
#define greenLedPin 2
#define buttonPin 3

#define lightChangeTimeoutMs 2000

bool isRed = true;
bool isGreen = false;
bool isAuto = true;

GButton button(buttonPin, LOW_PULL, NORM_OPEN);
long lastLightChangeMs = 0;

void nextLight()
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

void currentLights()
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
	pinMode(redLedPin, OUTPUT);
	if (ledsCount == 3)
	{
		pinMode(yellowLedPin, OUTPUT);
	}
	pinMode(greenLedPin, OUTPUT);
}

void loop()
{
	// button.tick();
	// if (button.isClick())
	// {
	// 	nextLight();
	// }

	if (millis() - lastLightChangeMs >= lightChangeTimeoutMs)
	{
		lastLightChangeMs = millis();
		nextLight();
	}
	currentLights();
}
