#ifndef _LEVELS_H__
#define _LEVELS_H__

#include "paj7620.h"
#define FINISHED 0
#define TOTAL_LEVELS 2
/************************************************

Note:

1. Each level must be defined similar to below
2. Each level must finish with FINISHED
3. Each level must be in the levels PROGMEM array.


**************************************************/

//each level must be defined similar to below, then
// end with FINISHED

const uint8_t level1[] PROGMEM = {
    GES_COUNT_CLOCKWISE_FLAG,
    GES_LEFT_FLAG,
    FINISHED};

const uint8_t level2[] PROGMEM = {
    GES_RIGHT_FLAG,
    GES_LEFT_FLAG,
    GES_UP_FLAG,
    GES_DOWN_FLAG,
    FINISHED};

const uint8_t *const levels[TOTAL_LEVELS] PROGMEM = {
    level1,
    level2};

#endif