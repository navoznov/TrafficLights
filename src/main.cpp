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

#define RED_LIGHT_TIMEOUT_MS 10000
#define RED_YELLOW_LIGHT_TIMEOUT_MS 2500
#define GREEN_LIGHT_TIMEOUT_MS 10000
#define GREEN_LIGHT_BLINK_TIMEOUT_MS 600
#define GREEN_LIGHT_BLINK_COUNT 6
#define YELLOW_LIGHT_TIMEOUT_MS 2500

#define LIGHTS_COUNT 5;
uint8_t light = 0; // текущий режим: 0 красный, 1 красный и желтый, 2 зеленый, 3 мигающий зеленый, 4 желтый

bool isRed = true;
bool isYellow = false;
bool isGreen = false;

bool isAutoMode = true;
uint8_t greenBlinkingCounter = 0;
unsigned long nextLightChangeTimeMs = 0;

GButton button(BUTTON_PIN, LOW_PULL, NORM_OPEN);

void gotoNextLight()
{
	// greenBlinkingCounter = 0;
	// nextLightChangeTimeMs = 0;
}

void setLights()
{
	if (LEDS_COUNT == 2)
	{
		isRed = !isRed;
		isGreen = !isGreen;
	}
	else if (LEDS_COUNT == 3)
	{
		// текущий режим: 0 красный, 1 красный и желтый, 2 зеленый, 3 мигающий зеленый, 4 желтый
		if (light == 0) // красный
		{
			isRed = true;
			isYellow = false;
			isGreen = false;
			nextLightChangeTimeMs = millis() + RED_YELLOW_LIGHT_TIMEOUT_MS;
			light++;
		}
		else if (light == 1) // красный + желтый
		{
			isRed = true;
			isYellow = true;
			isGreen = false;
			nextLightChangeTimeMs = millis() + RED_YELLOW_LIGHT_TIMEOUT_MS;
			light++;
		}
		else if (light == 2) // зеленый
		{
			isRed = false;
			isYellow = false;
			isGreen = true;
			nextLightChangeTimeMs = millis() + GREEN_LIGHT_TIMEOUT_MS;
			light++;
		}
		else if (light == 3) // зеленый мигающий
		{
			nextLightChangeTimeMs = millis() + GREEN_LIGHT_BLINK_TIMEOUT_MS;
			if (greenBlinkingCounter >= GREEN_LIGHT_BLINK_COUNT)
			{
				greenBlinkingCounter = 0;
				light++;
			}
			else
			{
				greenBlinkingCounter++;
				isGreen = !isGreen;
			}
		}
		else if (light == 4)
		{
			isRed = false;
			isYellow = true;
			isGreen = false;
			nextLightChangeTimeMs = millis() + YELLOW_LIGHT_TIMEOUT_MS;
			light++;
		}

		light = light % LIGHTS_COUNT;
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

	if (IS_BUTTON_CONTROL)
	{
		pinMode(BUTTON_PIN, INPUT_PULLUP);
	}
}

void loop()
{
	button.tick();

	if (button.isDouble())
	{
		isAutoMode = !isAutoMode;
		return;
	}

	if (isAutoMode && millis() >= nextLightChangeTimeMs)
	{
		setLights();
	}
	if (button.isClick())
	{
		// todo: подумать как избавтиься от этого костыля
		if(light == 3) greenBlinkingCounter = GREEN_LIGHT_BLINK_COUNT;
		setLights();
	}
	

	showCurrentLights();
}
