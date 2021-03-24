/*
  Pojmenovat stavy pro finish + přidat stav
  Udělat automatický pohyb.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#undef  CLOCKS_PER_SEC
//#define CLOCKS_PER_SEC  15000

#include "rlutil.h"

// Stavy pro state
#define ST_WAITING   4
#define ST_PLAYING   0
#define ST_WIN       1
#define ST_GAMEOVER  2
#define ST_EXIT      3
int state = ST_PLAYING; /* 0 = hra běží, 1 = hra skončila úspěchem, 2 = hra skončila neúspěchem, 3 = uživatel skončil */
time_t timer = 0;  // počet milisekund od začátku běju programu

struct {
  int score;
  int remobjects;  // počet nesebraných
  int length;      // délka hada
  int door_x, door_y;
  int move_ms; // interval automatického pohybu
  int last_step; // krok, ve kterém jsme se naposledy automaticky hýbli
  time_t start;   // hodnota z timer v okamžiku stisku první klávesy
} game;

struct {
  int print;
  int maxscore;
  int objects;
  int w;
  int h;
} debug;

#include "area.c"



// a = area
int area[HEIGHT][WIDTH] = { // 16, 10
  {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
  {W,0,0,0,0,0,0,W,0,0,0,0,0,0,0,0,W},
  {W,W,W,W,W,0,0,W,0,W,W,D,W,W,0,0,W},
  {W,0,0,0,W,2,0,W,0,W,1,0,0,W,0,0,W},
  {W,0,0,0,W,0,0,W,0,0,0,W,5,W,0,0,W},
  {W,0,W,W,W,0,0,W,0,W,W,W,0,W,W,0,W},
  {W,0,0,0,W,3,0,0,0,W,0,0,1,0,0,0,W},
  {W,0,0,2,0,0,0,W,0,0,0,W,W,W,W,0,W},
  {W,0,W,0,0,H,0,W,0,4,0,W,0,0,0,0,W},
  {W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W+END},
};



struct {
  int key;
  int dx;
  int dy;
} arrow_move[4] = {
  {KEY_UP, 0, -1},
  {KEY_DOWN, 0, 1},
  {KEY_RIGHT, 1, 0},
  {KEY_LEFT, -1, 0}
};


int main()
{
  struct areaSize size;
  int move_i; // index do pole arrow_move[], který udává směr pohybu
  int step; // pořadí současného kroku

  saveDefaultColor();

  setBackgroundColor(PRG_BGCOLOR);
  cls();
  hidecursor();
  size = getAreaSize(area);
  game.score = 0;
  debug.print = 0;
  debug.maxscore = size.maxscore;
  debug.objects = size.objects;
  debug.w = size.w;
  debug.h = size.h;
  game.remobjects = size.objects;
  game.length = 0;
  game.door_x = size.door_x;
  game.door_y = size.door_y;
  game.move_ms = 400;

  state = ST_WAITING;
  game.start = -1;
  game.last_step = -1;

  printArea(area, size.w, size.h, 0);

  while (1) {
    msleep(10);
    //timer = clock() / (double) CLOCKS_PER_SEC * 1000;
    timer = clock() * 1000 / CLOCKS_PER_SEC;

    if (kbhit()) {
      int key, i;

      key = getkey();

      for (i = 0; i < 4; i++) {
        if (arrow_move[i].key == key) {
          if (state == ST_WAITING) {
            game.start = timer;
            state = ST_PLAYING;
          }
          move_i = i;
          break;
        }
      }
      // pokud je v i 4, tak v key nebyla sipka
      if (i == 4) {
        if (key == KEY_ESCAPE) {
          break; // ukončíme while(1)
        }
        else if (key == KEY_ENTER) {
          if (state == ST_WIN || state == ST_GAMEOVER) // ENTER po skočení hry
            break;

          debug.print = !(debug.print);
          if (!debug.print) {
            setBackgroundColor(PRG_BGCOLOR);
            cls();
          }
          printArea(area, size.w, size.h, 0);
        }
      }
    }
    else {
      printArea(area, size.w, size.h, 1);
    }

    if (state == ST_PLAYING) {
      step = (timer - game.start) / game.move_ms;
      if (game.last_step < step) {
        moveHead(area, size.w, size.h, arrow_move[move_i].dx, arrow_move[move_i].dy);
        printArea(area, size.w, size.h, 0);
        game.last_step = step;
      }
    }
  }

  setBackgroundColor(BLACK);
  cls();
  resetColor();
  showcursor();

  // Ty jsou pouze kvuli ladeni
  setColor(GREY);
  setBackgroundColor(BLACK);

  return 0;
}
