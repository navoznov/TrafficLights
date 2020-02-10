#include <Arduino.h>
#include "GyverButton.h"

// Hardware settigs
#define LEDS_COUNT 3 // 2 (красный/зеленый) или 3 (красный/желтый/зеленый)

#define RED_LED_PIN 1
#define YELLOW_LED_PIN 2
#define GREEN_LED_PIN 3

#define BUTTON_PIN 4

// Software settings
#define LIGHT_CHANGE_TIMEOUT_MS 2000
#define SWITCH_WITH_BLINKS true
#define IS_BUTTON_CONTROL true
#define SHOULD_START_IN_AUTO_MODE true

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

GButton button(BUTTON_PIN, LOW_PULL, NORM_OPEN);
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
	if (LEDS_COUNT == 2)
	{
		isRed = !isRed;
		isGreen = !isGreen;
	}
	else if (LEDS_COUNT == 3)
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
			blinkLight(GREEN_LED_PIN, GREEN_LIGHT_BLINK_COUNT, GREEN_LIGHT_BLINK_TIMEOUT_MS);
			isRed = false;
			isYellow = true;
			isGreen = false;
			nextLightChangeTimeMs = millis() + YELLOW_LIGHT_TIMEOUT_MS;
		}
	}
}

void showCurrentLights()
{
	digitalWrite(RED_LED_PIN, isRed ? HIGH : LOW);
	if (LEDS_COUNT == 3)
	{
		digitalWrite(YELLOW_LED_PIN, isYellow ? HIGH : LOW);
	}
	digitalWrite(GREEN_LED_PIN, isGreen ? HIGH : LOW);
}

void setup()
{
	isAutoMode = SHOULD_START_IN_AUTO_MODE;

	pinMode(RED_LED_PIN, OUTPUT);
	if (LEDS_COUNT == 3)
	{
		pinMode(YELLOW_LED_PIN, OUTPUT);
	}
	pinMode(GREEN_LED_PIN, OUTPUT);
	nextLightChangeTimeMs = millis() + RED_LIGHT_TIMEOUT_MS;

	pinMode(BUTTON_PIN, INPUT_PULLUP);
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
