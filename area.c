/*
  Rezervovaná jména promìnných
  ----------------------------

  a[radek][sloupec] = area = herní plocha
  a[i][j]
  a[height][width]
  w = width = šíøka herního pole
  h = height = výška horního pole
    0 <= i < h
    0 <= j < w
*/



#define  WIDTH    80
#define  HEIGHT   25



// NEMÌNIT FREE!!
// V definici herního pole spoléháme na nulu.
#define  FREE   0
#define  WALL   100
#define  W      WALL
#define  DOOR   200
#define  DOOR_OPEN   7   // index do pole objects[]
#define  WALL_FIRE   6   // index do pole objects[]
#define  D      DOOR
#define  HEAD   300
#define  H      HEAD

#define  OBJ_FROM  1
#define  OBJ_TO    99

#define  BODY_FROM  1000
#define  BODY_TO    1099

#define   END      999999

#define  AREA_BGCOLOR    MAGENTA
#define  PRG_BGCOLOR     BLACK

#include "objects.c"


struct areaSize {
  int w;
  int h;
  int objects;
  int maxscore;
  int door_x, door_y;
};

void printnl()
{
  printf("                                                                  \n");
}

/*
  Funkce vytiskne herní pole.
  save = 0 = chceme překreslit celé herní pole
       = 1 = chceme pouze překreslit animované prvky
*/
void printArea(int a[][WIDTH], int w, int h, int save)
{
  int i, j;

  gotoxy(1, 1);
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      // nechceme pouze animaci a pole není objekt
      if (a[i][j] >= OBJ_FROM && a[i][j] <= OBJ_TO) {
        int oi; // object index
        int fi; // frame index

        oi = a[i][j] - OBJ_FROM;
        // objects[oi].delay
        fi = (timer / objects[oi].delay) % objects[oi].num;

        setBackgroundColor(objects[oi].frames[fi].bgcolor);
        setColor(objects[oi].frames[fi].color);
        if (save)
          gotoxy(j + 1, i + 1);
        printf("%s", objects[oi].frames[fi].c);
      }
      else {
        if (save)
          continue;

        if (a[i][j] == FREE) {
          setBackgroundColor(AREA_BGCOLOR);
          printf(" ");
        }
        else if (a[i][j] == WALL) {
          setBackgroundColor(RED);
          printf(" ");
        }
        else if (a[i][j] == DOOR) {
          setBackgroundColor(GREEN);
          setColor(WHITE);
          printf("X");
        }
        else if (a[i][j] == HEAD) {
          setBackgroundColor(AREA_BGCOLOR);
          setColor(WHITE);
          printf("\x02");
        }
        else if (a[i][j] >= BODY_FROM && a[i][j] <= BODY_TO) {
          setBackgroundColor(AREA_BGCOLOR);
          setColor(BLUE);
          printf("%d", (a[i][j] - BODY_FROM + 1) % 10);
        }
        else {
          setBackgroundColor(AREA_BGCOLOR);
          setColor(WHITE);
          printf("?");
        }
      }
    }
    if (!save)
      printf("\n");
  }

  if (save)
    gotoxy(1, h + 1);

  setColor(GREY);
  setBackgroundColor(PRG_BGCOLOR);
  printf("Score: %d\n", game.score);
  printnl();

  if (state == ST_WIN) {
    printf("Vitezstvi! Hra skoncila.\n");
    printf("Stiskni ENTER pro konec.\n");
    printnl();
  }
  else if (state == ST_GAMEOVER) {
    printf("Pozor zed! Hra skoncila.\n");
    printf("Stiskni ENTER pro konec.\n");
    printnl();
  }

  // Pozor! Při zkrácení hodnoty se hodnota nepřepíše
  if (debug.print) {
    printnl();
    printnl();
    printf("Max. score: %d\n", debug.maxscore);
    printf("Celkem objektu: %d\n", debug.objects);
    printf("Zbyva objektu: %d  \n", game.remobjects);
    printf("Delka: %d  \n", game.length);
    printf("State: %d\n", state);
    printf("Timer: %d s %03d ms; Clock: %d\n", timer / 1000, timer % 1000, clock());
  }
}

struct areaSize getAreaSize(int a[][WIDTH])
{
  struct areaSize size;
  int i, j;
  int end = 0;

  size.objects = 0;
  size.maxscore = 0;
  size.door_x = 0;
  size.door_y = 0;
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      if (a[i][j] >= END) {
        size.w = j + 1;
        size.h = i + 1;
        a[i][j] -= END;
        end = 1;
      }

      if (a[i][j] >= OBJ_FROM && a[i][j] <= OBJ_TO) {
        size.objects++;
        size.maxscore += objects[ a[i][j] - OBJ_FROM ].points;
      }
      else if (a[i][j] == DOOR) {
        size.door_x = j;
        size.door_y = i;
      }

      if (end) {
        return size;
      }
    }
  }

  size.w = WIDTH;
  size.h = HEIGHT;
  return size;
}

/*
  Pokud to jde, tak procedura pohne s hlavou hada.
  direction:
    0 = TOP
    1 = RIGHT
    2 = BOTTOM
    3 = LEFT
*/
void moveHead(int a[][WIDTH], int w, int h, int dx, int dy)
{
  int x, y, add;

  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      if (a[y][x] == HEAD) {
        if (y + dy >= 0 && y + dy < h && x + dx >= 0 && x + dx < w) {
          add = 0;
          if (a[y + dy][x + dx] == FREE)
            ;
          else if (a[y + dy][x + dx] == DOOR_OPEN) {
            state = ST_WIN;
          }
          else if (a[y + dy][x + dx] >= OBJ_FROM && a[y + dy][x + dx] <= OBJ_TO) {
            game.score += objects[ a[y + dy][x + dx] - OBJ_FROM].points;
            game.remobjects--;
            add = 1;
            if (game.remobjects == 0) {
              a[game.door_y][game.door_x] = DOOR_OPEN;
            }
          }
          else if (a[y + dy][x + dx] == WALL || a[y + dy][x + dx] == DOOR) {
            a[y + dy][x + dx] = WALL_FIRE;
            state = ST_GAMEOVER;
            return;
          }
          else
            return;

          moveBody(a, w, h, add);
          if (add)
            game.length++;
          a[y + dy][x + dx] = HEAD;
        }

        return;
      }
    }
  }
}

/*
  Posune tělo o jeden článek.
  První článek přepíše hlavičku.
  add = 0 => poslední článek zanikne
  add = 1 => poslední článek zůstane

  game.length = současná délka těla

*/
void moveBody(int a[][WIDTH], int w, int h, int add)
{
  int x, y;

  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      if (a[y][x] == HEAD) {
        if (game.length == 0 && add == 0)
          a[y][x] = FREE;
        else
          a[y][x] = BODY_FROM;
      }
      else if (a[y][x] >= BODY_FROM && a[y][x] <= BODY_TO) {
        if (add == 0 && a[y][x] == BODY_FROM + game.length - 1)
          a[y][x] = FREE;
        else
          a[y][x]++;
      }
    }
  }
}
