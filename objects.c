struct frame {
  char c[4];
  int color;
  int bgcolor;
};

struct object {
  int  num;  // kolik je okének animace
  struct frame frames[10];
  int delay;  // prodleva v ms
  int  points;  // body za sebrání
} objects[] = {
  // 1 v area = BLIKAJÍCÍ SRDCE
  {
    2,
    {
      {"\x03", RED, AREA_BGCOLOR},
      {" ", RED, AREA_BGCOLOR}
    },
    500,
    100
  },

  // 2 = LISTEK MENICI BARVU
  {
    3,
    {
      {"\x04", LIGHTGREEN, AREA_BGCOLOR},
      {"\x04", GREEN, AREA_BGCOLOR},
      {"\x04", YELLOW, AREA_BGCOLOR}
    },
    300,
    200
  },

  // 3 = SLUNICKO
  {
    1,
    {
      {"\x0F", YELLOW, BROWN}
    },
    1000,
    300
  },

  // 4 = STAR
  {
    2,
    {
      {"*", BLUE, AREA_BGCOLOR},
      {"*", AREA_BGCOLOR, BLUE},

    },
    1000,
    400
  },

  // 5 = POMLČKA
  {
    4,
    {
      {"-",  YELLOW, AREA_BGCOLOR},
      {"\\", YELLOW, AREA_BGCOLOR},
      {"|",  YELLOW, AREA_BGCOLOR},
      {"/",  YELLOW, AREA_BGCOLOR},
    },
    200,
    500
  },

  // 6 = HOŘÍCÍ ZEĎ (nedávat do herního plánu)
  {
    4,
    {
      {" ",     RED,     YELLOW},
      {"\xB2",  YELLOW,  RED},
      {"\xB1",  YELLOW, RED},
      {"\xB0",  YELLOW,  RED}
    },
    300,
    0
  },

  // 7 = OTEVŘENÉ DVEŘE
  {
    2,
    {
      {"\x1D",  WHITE,  GREEN},
      {" ",     WHITE,  GREEN},
    },
    500,
    0
  }

};








