#ifndef _SYSTEM_H__
#define _SYSTEM_H__

#include <arduino.h>
#define GES_REACTION_TIME 500
#define GES_ENTRY_TIME 800
#define GES_QUIT_TIME 1000

#define OLED_PWR 6
#define OLED_GND 7
#define GES_PWR A2
#define GES_GND A3

void (*resetSelf)(void) = 0; //shortcut to reset the arduino.

void setupPins()
{
  pinMode(OLED_PWR, OUTPUT);
  pinMode(OLED_GND, OUTPUT);
  pinMode(GES_PWR, OUTPUT);
  pinMode(GES_GND, OUTPUT);
  digitalWrite(OLED_PWR, HIGH);
  digitalWrite(OLED_GND, LOW);
  digitalWrite(GES_PWR, HIGH);
  digitalWrite(GES_GND, LOW);
}

#endif