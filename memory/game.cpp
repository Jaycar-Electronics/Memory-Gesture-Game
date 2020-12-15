#include "game.h"
#include "levels.h"
//#include "paj7620.h"
#include "title.h"

#define ITOA(x) (char)(x + (x > 9 ? 'A' - 10 : '0'))

MemoryGame::MemoryGame()
{
  //Constructor, doesn't do much
}
MemoryGame::~MemoryGame()
{
  delete oled;
  oled = 0;
}

void MemoryGame::loadOLED()
{
  if (oled == nullptr)
  {
    oled = new U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI(
        U8G2_R0, //180 rotation
        9,       //cs
        10,      //dc
        12       //reset
    );

    oled->begin();
  }
}
void MemoryGame::unloadOLED()
{
  delete oled;
}

bool MemoryGame::playerFinished()
{
  return playerLevel >= TOTAL_LEVELS;
}

/***********************************************************
 * Shows the end screen, be it win or lose
***********************************************************/
void MemoryGame::showEndScreen(EndScreen t)
{
  oled->clearBuffer();

  switch (t)
  {
  case Winner:
    oled->setFont(u8g2_font_logisoso28_tr);
    oled->setCursor(0, 28);
    oled->print(F("WINNER!"));
    oled->setCursor(110, 64);
    oled->print((char)('1' + playerLevel));

    oled->setFont(u8g2_font_t0_11b_tr);
    oled->setCursor(0, 60);
    oled->print(F("Moving on to level "));
    break;
  case Loser:
    oled->setFont(u8g2_font_t0_11b_tr);
    oled->setCursor(0, 16);
    oled->print(F("You Lose!"));
    oled->setCursor(0, 32);
    oled->print(F("Change the code"));
    oled->setCursor(0, 48);
    oled->print(F("so it's easier!"));
    break;
  case EndGame:
    oled->setFont(u8g2_font_t0_11b_tr);
    oled->setCursor(0, 32);
    oled->print(F("That's all there is"));
    oled->setCursor(0, 60);
    oled->print(F("Add some more levels in the code!"));
    break;
  }

  oled->sendBuffer();
  delay(5000);
}

/***********************************************************
 * Blocks until the gesture sensor picks up a reading, and
 *     returns that reading
***********************************************************/
uint8_t MemoryGame::blockUntilGesture()
{
  unsigned char data = 0;
  while (data == 0)
  {
    paj7620ReadReg(0x43, 1, &data);
  }
  return data;
}

/***********************************************************
 * Challenges the game level, waiting for gestures and comp-
 *     -aring to the level data
***********************************************************/
bool MemoryGame::playGameLevel()
{
  loadCurrentLevel();
  uint8_t errorCount = 0;
  bool caughtError;

  for (int i = 0; i < levelSize && errorCount < MAX_ERRORS; i++)
  {
    caughtError = false;
    //draw empty screen with bar
    oled->clearBuffer();
    oled->drawBox(0, 0, BAR_CHUNK * (MAX_ERRORS - errorCount), 8);
    oled->setFont(u8g2_font_logisoso28_tr);
    oled->setCursor(0, 64);
    oled->print(ITOA(i+1));
    oled->sendBuffer();

    //prepare bar for next scene
    oled->clearBuffer();
    oled->setFont(u8g2_font_open_iconic_check_4x_t);
    oled->setCursor(48, 48);
    oled->drawBox(0, 0, BAR_CHUNK * (MAX_ERRORS - errorCount), 8);

    uint8_t comparison = getLevelValue(i);
    uint8_t data = blockUntilGesture();

    //------------------

    if (data != comparison)
    {
      oled->print('D');
      caughtError = true;
    }
    else //succesfull
    {
      oled->print('@');
    }

    oled->sendBuffer();

    delay(1000);

    if (caughtError)
    {
      displayFlag(comparison);
      delay(500); //half a second
      errorCount++;
      i--; //decrement so we can do this one again
    }
  }

  if (errorCount < MAX_ERRORS)
  {
    playerLevel++;
    return true;
  }
  else
  {
    return false;
  }
}
/************************************************************
 * Shows the 'READY?' titlecard.
************************************************************/
void MemoryGame::showReadyCard()
{
  oled->clearBuffer();
  oled->setFont(u8g2_font_logisoso28_tr);
  oled->setCursor(10, 44);
  oled->print(F("READY?"));
  oled->sendBuffer();
}

/***********************************************************
 * Shows the 'GO!' titlecard
***********************************************************/
void MemoryGame::showGoCard()
{
  oled->clearBuffer();
  oled->setFont(u8g2_font_logisoso28_tr);
  oled->setCursor(40, 44);
  oled->print(F("GO!"));
  oled->sendBuffer();
}

/***********************************************************
 * Loads the correct level in memory depending on playerlevel
***********************************************************/
void MemoryGame::loadCurrentLevel()
{
  levelPointer = pgm_read_byte(&levels[playerLevel]);
  levelSize = strlen_P(levelPointer);
}

/***********************************************************
 * Gets the value at position for the current playerLevel
***********************************************************/
uint8_t MemoryGame::getLevelValue(uint8_t position)
{
  return pgm_read_byte(levelPointer + position);
}

/***********************************************************
 * Displays arrows depending on gesture value
***********************************************************/
void MemoryGame::displayFlag(uint8_t value)
{
  oled->clearBuffer();
  oled->setFont(u8g2_font_open_iconic_arrow_4x_t);
  oled->setCursor(48, 48);
  switch (value)
  {
  case GES_RIGHT_FLAG:
    oled->print('B');
    break;
  case GES_LEFT_FLAG:
    oled->print('A');
    break;
  case GES_UP_FLAG:
    oled->print('C');
    break;
  case GES_DOWN_FLAG:
    oled->print('@');
    break;
  case GES_CLOCKWISE_FLAG:
    oled->print('X');
    break;
  case GES_COUNT_CLOCKWISE_FLAG:
    oled->print('W');
    break;
  default:
    break;
  }
  oled->sendBuffer();
}

/***********************************************************
 * Shows the level for the player, at a speed defined by
 *     Level
***********************************************************/
void MemoryGame::showGameLevel()
{
  loadCurrentLevel();
  showReadyCard();
  delay(1000);

  for (int i = 0; i < levelSize; i++)
  {
    uint8_t value = getLevelValue(i);

    displayFlag(value);

    oled->setFont(u8g2_font_logisoso28_tr);
    oled->setCursor(0, 64);
    oled->print(ITOA(i+1));
    oled->sendBuffer();

    delay(500 + (1000 - BASE_SPEED * playerLevel));
  }
  showGoCard();
  delay(1000);
}

/***********************************************************
 * Shows the title picture made in MS Paint
***********************************************************/
void MemoryGame::showTitleCard()
{
  //XBMP is done through page'd data
  //as the memory is not quite enough
  //to shonk it all at once.
  oled->firstPage();
  do
  {
    oled->drawXBMP(0, 0, 128, 64, title_bits);
  } while (oled->nextPage());
}
