#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <paj7620.h>

#include "levels.h"
#include "title.h"

#define GES_REACTION_TIME 500
#define GES_ENTRY_TIME 800
#define GES_QUIT_TIME 1000

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI oled(
    U8G2_R0, //180 rotation
    9,       //cs
    10,      //dc
    12       //reset
);

#define OLED_PWR 6
#define OLED_GND 7
#define GES_PWR A2
#define GES_GND A3

void setup()
{

  pinMode(OLED_PWR, OUTPUT);
  pinMode(OLED_GND, OUTPUT);
  pinMode(GES_PWR, OUTPUT);
  pinMode(GES_GND, OUTPUT);

  digitalWrite(OLED_PWR, HIGH);
  digitalWrite(OLED_GND, LOW);
  digitalWrite(GES_PWR, HIGH);
  digitalWrite(GES_GND, LOW);

  uint8_t error = 0;
  Serial.begin(9600);
  oled.begin();

  error = paj7620Init(); // initialize Paj7620 registers
  if (error)
  {
    Serial.print("INIT ERROR,CODE:");
    Serial.println(error);
  }
  else
  {
    Serial.println("INIT OK");
  }
  oled.firstPage();
  do
  {
    oled.drawXBMP(0, 0, 128, 64, title_bits);
  } while (oled.nextPage());
  while (1)
    ;
  oled.clearBuffer();                  // clear the internal memory
  oled.setFont(u8g2_font_ncenB08_tr);  // choose a suitable font
  oled.drawStr(0, 10, "Hello World!"); // write something to the internal memory
  oled.sendBuffer();                   // transfer internal memory to the display
}
uint8_t *getPointerToLevel(uint8_t index)
{
  return (uint8_t *)pgm_read_byte(&levels[index]);
}
size_t getLevelSize(uint8_t *levelPtr)
{
  return strlen_P(levelPtr);
}
uint8_t getValueForLevelAt(uint8_t *levelPtr, uint8_t index)
{
  //im sure there's many problems here.
  return pgm_read_byte(levelPtr + index);
}

typedef uint8_t *levelPtr;

void loop()
{

  levelPtr current_level = getPointerToLevel(0);

  size_t level_size = getLevelSize(current_level);

  for (size_t i = 0; i < level_size; i++)
  {

    unsigned char value = getValueForLevelAt(current_level, i);
    char letter = 'u';

    switch (value)
    {
    case GES_RIGHT_FLAG:
      letter = 'R';
      break;
    case GES_LEFT_FLAG:
      letter = 'L';
      break;
    case GES_UP_FLAG:
      letter = 'U';
      break;
    case GES_DOWN_FLAG:
      letter = 'D';
      break;
    default:
      Serial.print(i);
      break;
    }

    oled.clearBuffer();                 // clear the internal memory
    oled.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    oled.setCursor(10, 10);
    oled.print(letter);
    oled.setCursor(30, 30);
    oled.print(u8x8_u8toa(i, 3));
    oled.sendBuffer(); // transfer internal memory to the display
    delay(500);
  }

  // first show the pattern.

  // then check the pattern

  uint8_t data = 0, data1 = 0, error;

  error = paj7620ReadReg(0x43, 1, &data); // Read Bank_0_Reg_0x43/0x44 for gesture result.
  if (!error)
  {
    switch (data) // When different gestures be detected, the variable 'data' will be set to different values by paj7620ReadReg(0x43, 1, &data).
    {
    case GES_RIGHT_FLAG:
      Serial.println("Right");
      break;
    case GES_LEFT_FLAG:
      Serial.println("Left");
      break;
    case GES_UP_FLAG:
      Serial.println("Up");
      break;
    case GES_DOWN_FLAG:
      Serial.println("Down");
      break;
    case GES_FORWARD_FLAG:
      Serial.println("Forward");
      break;
    case GES_BACKWARD_FLAG:
      Serial.println("Backward");
      break;
    case GES_CLOCKWISE_FLAG:
      Serial.println("Clockwise");
      break;
    case GES_COUNT_CLOCKWISE_FLAG:
      Serial.println("anti-clockwise");
      break;
    default:
      paj7620ReadReg(0x44, 1, &data1);
      if (data1 == GES_WAVE_FLAG)
      {
        Serial.println("wave");
      }
      break;
    }
  }
  else
  {
    Serial.println("Error!");
    Serial.println(error);
  }
  delay(100);
}
