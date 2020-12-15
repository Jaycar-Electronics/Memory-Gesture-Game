#ifndef _GAME_H__
#define _GAME_H__

#include <U8g2lib.h>

#define BASE_SPEED 200

#define MAX_ERRORS 5
#define BAR_CHUNK (128 / MAX_ERRORS)

enum EndScreen {
  Winner = 0,
  Loser,
  EndGame
};

class MemoryGame
{

private:
  uint8_t playerLevel = 0;
  uint8_t errorCount = 0;

  unsigned char * levelPointer = nullptr;
  size_t levelSize = 0;

  U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI *oled = nullptr;

public:
  MemoryGame();
  ~MemoryGame();


  void loadOLED();
  void unloadOLED();

  void showEndScreen(EndScreen type);

  bool playGameLevel();
  void showGameLevel();

  void showReadyCard();
  void showGoCard();
  void showTitleCard();

  bool playerFinished();

  void loadCurrentLevel();
  uint8_t getLevelValue(uint8_t position);

  void displayFlag(uint8_t value);

  uint8_t blockUntilGesture();

};

#endif