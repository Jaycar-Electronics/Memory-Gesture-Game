#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "paj7620.h"

#include "levels.h"
#include "title.h"
#include "game.h"
#include "system.h"

#define DEBUG(x) Serial.println(F(x))

MemoryGame *game;

void setup()
{
  setupPins();
  Serial.begin(9600);
  DEBUG("Hello, everything is fine");
  game = new MemoryGame();
  DEBUG("Things are still ok after initialising game");

  uint8_t error = paj7620Init();
  if (error)
  {
    DEBUG("INIT ERROR");
    Serial.println(error);
  }
  DEBUG("STARTING OLED");
  game->loadOLED();
  DEBUG("SHOWING TITLE CARD");
  game->showTitleCard();
  DEBUG("Blocking");
  game->blockUntilGesture();
}

void loop()
{
  DEBUG("Showing game Level");
  game->showGameLevel();
  DEBUG("Playing game Level");
  if (game->playGameLevel())
  {

    if (game->playerFinished())
    {
      DEBUG("PLAYER FINISHED");
      game->showEndScreen(EndGame);
    }
    else
    {
      DEBUG("PLAYER ADVANCES");
      game->showEndScreen(Winner);
    }
    delay(3000);
    if (game->playerFinished())
      resetSelf();
  }
  else
  {
    DEBUG("PLAYER LOSER");
    game->showEndScreen(Loser);
    delay(3000);
    resetSelf();
  }
}

//------------------------------------------------------------------------------
