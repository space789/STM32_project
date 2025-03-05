#include "user_def.h"

uint8_t go_time = 0;
struct Chessman CHESSMAN[32];
enum State MODE = IDLE;
enum Show SHOW_MODE = DISPLAY;
unsigned int rand_seed;
uint8_t time_count = 0;
int open_count = 0;
int Chess_onclick_idx;
int find_idx;
enum Teams current_color;
enum Teams winner;

////////////////////////////////////////////////////////////

void BSP_Touch_Update(void)
{
  GUI_PID_STATE TS_State;
  static TS_StateTypeDef prev_state;
  TS_StateTypeDef ts;
  TS_State.Layer = 0;

  BSP_TS_GetState(&ts);

  // click down
  if (prev_state.touchDetected == 0x00 && ts.touchDetected == 0x01)
  {
    TS_State.Pressed = 1;
    TS_State.x = ts.touchX[0];
    TS_State.y = ts.touchY[0];
    GUI_TOUCH_StoreStateEx(&TS_State);
  }
  // released
  else if (prev_state.touchDetected == 0x01 && ts.touchDetected == 0x00)
  {
    TS_State.Pressed = 0;
    TS_State.x = 0;
    TS_State.y = 0;
    GUI_TOUCH_StoreStateEx(&TS_State);
  }

  prev_state = ts;
}

void LCD_initial(void)
{
  /* LCD Initialization */
  BSP_LCD_Init();

  /* LCD Initialization */
  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
  BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS + (BSP_LCD_GetXSize() * BSP_LCD_GetYSize() * 4));

  /* Enable the LCD */
  BSP_LCD_DisplayOn();

  /* Select the LCD Background Layer  */
  BSP_LCD_SelectLayer(0);

  /* Clear the Background Layer */
  BSP_LCD_Clear(LCD_COLOR_BLACK);

  /* Select the LCD Foreground Layer  */
  BSP_LCD_SelectLayer(1);

  /* Clear the Foreground Layer */
  BSP_LCD_Clear(LCD_COLOR_BLACK);

  /* Configure the transparency for foreground and background :
     Increase the transparency */
  BSP_LCD_SetTransparency(0, 255);
  BSP_LCD_SetTransparency(1, 0);

  BSP_LCD_SelectLayer(0);
  // BSP_LCD_Clear(LCD_COLOR_BLACK);

  // BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
  // BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  // //	BSP_LCD_DisplayStringAt(10,10,(uint8_t *)"test",CENTER_MODE);

  // // if use GUI need to change HAL pixel format to RGB565!!!
  // extern LTDC_HandleTypeDef hLtdcHandler;
  // hLtdcHandler.LayerCfg[0].PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
}

void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

void chessman_init(void)
{
  int i = 0, j = 0;
  for (; i < 32; i++)
  {
    CHESSMAN[i].click = 0;
    CHESSMAN[i].open = 0;
    CHESSMAN[i].alive = 1;
    CHESSMAN[i].X = (int)(i % 8);
    CHESSMAN[i].Y = (int)(i / 8);
#ifdef SHOW_position
    printf("CHESSMAN[%d].X = %d \r\n", i, (int)(i % 8));
    printf("CHESSMAN[%d].Y = %d \r\n", i, (int)(i / 8));
#endif
    if (i < 16)
      CHESSMAN[i].chess_type.color = BLACK;
    else
      CHESSMAN[i].chess_type.color = RED;
  }
  winner = NONE;
  current_color = NONE;
  CHESSMAN[0].chess_type.image_ID = IMAGE_BK;
  CHESSMAN[0].chess_type.chess_type_enum = K;
  CHESSMAN[1].chess_type.image_ID = IMAGE_BA;
  CHESSMAN[1].chess_type.chess_type_enum = A;
  CHESSMAN[2].chess_type.image_ID = IMAGE_BA;
  CHESSMAN[2].chess_type.chess_type_enum = A;
  CHESSMAN[3].chess_type.image_ID = IMAGE_BB;
  CHESSMAN[3].chess_type.chess_type_enum = B;
  CHESSMAN[4].chess_type.image_ID = IMAGE_BB;
  CHESSMAN[4].chess_type.chess_type_enum = B;
  CHESSMAN[5].chess_type.image_ID = IMAGE_BR;
  CHESSMAN[5].chess_type.chess_type_enum = R;
  CHESSMAN[6].chess_type.image_ID = IMAGE_BR;
  CHESSMAN[6].chess_type.chess_type_enum = R;
  CHESSMAN[7].chess_type.image_ID = IMAGE_BN;
  CHESSMAN[7].chess_type.chess_type_enum = N;
  CHESSMAN[8].chess_type.image_ID = IMAGE_BN;
  CHESSMAN[8].chess_type.chess_type_enum = N;
  CHESSMAN[9].chess_type.image_ID = IMAGE_BC;
  CHESSMAN[9].chess_type.chess_type_enum = C;
  CHESSMAN[10].chess_type.image_ID = IMAGE_BC;
  CHESSMAN[10].chess_type.chess_type_enum = C;
  CHESSMAN[11].chess_type.image_ID = IMAGE_BP;
  CHESSMAN[11].chess_type.chess_type_enum = P;
  CHESSMAN[12].chess_type.image_ID = IMAGE_BP;
  CHESSMAN[12].chess_type.chess_type_enum = P;
  CHESSMAN[13].chess_type.image_ID = IMAGE_BP;
  CHESSMAN[13].chess_type.chess_type_enum = P;
  CHESSMAN[14].chess_type.image_ID = IMAGE_BP;
  CHESSMAN[14].chess_type.chess_type_enum = P;
  CHESSMAN[15].chess_type.image_ID = IMAGE_BP;
  CHESSMAN[15].chess_type.chess_type_enum = P;
  CHESSMAN[16].chess_type.image_ID = IMAGE_RK;
  CHESSMAN[16].chess_type.chess_type_enum = K;
  CHESSMAN[17].chess_type.image_ID = IMAGE_RA;
  CHESSMAN[17].chess_type.chess_type_enum = A;
  CHESSMAN[18].chess_type.image_ID = IMAGE_RA;
  CHESSMAN[18].chess_type.chess_type_enum = A;
  CHESSMAN[19].chess_type.image_ID = IMAGE_RB;
  CHESSMAN[19].chess_type.chess_type_enum = B;
  CHESSMAN[20].chess_type.image_ID = IMAGE_RB;
  CHESSMAN[20].chess_type.chess_type_enum = B;
  CHESSMAN[21].chess_type.image_ID = IMAGE_RR;
  CHESSMAN[21].chess_type.chess_type_enum = R;
  CHESSMAN[22].chess_type.image_ID = IMAGE_RR;
  CHESSMAN[22].chess_type.chess_type_enum = R;
  CHESSMAN[23].chess_type.image_ID = IMAGE_RN;
  CHESSMAN[23].chess_type.chess_type_enum = N;
  CHESSMAN[24].chess_type.image_ID = IMAGE_RN;
  CHESSMAN[24].chess_type.chess_type_enum = N;
  CHESSMAN[25].chess_type.image_ID = IMAGE_RC;
  CHESSMAN[25].chess_type.chess_type_enum = C;
  CHESSMAN[26].chess_type.image_ID = IMAGE_RC;
  CHESSMAN[26].chess_type.chess_type_enum = C;
  CHESSMAN[27].chess_type.image_ID = IMAGE_RP;
  CHESSMAN[27].chess_type.chess_type_enum = P;
  CHESSMAN[28].chess_type.image_ID = IMAGE_RP;
  CHESSMAN[28].chess_type.chess_type_enum = P;
  CHESSMAN[29].chess_type.image_ID = IMAGE_RP;
  CHESSMAN[29].chess_type.chess_type_enum = P;
  CHESSMAN[30].chess_type.image_ID = IMAGE_RP;
  CHESSMAN[30].chess_type.chess_type_enum = P;
  CHESSMAN[31].chess_type.image_ID = IMAGE_RP;
  CHESSMAN[31].chess_type.chess_type_enum = P;
  /////////////////////////////////
  Chess_onclick_idx = 100; // set default
}

void check_click(void)
{
  int i = 0;
  for (; i < 32; i++)
  {
    if (CHESSMAN[i].click)
    {
      if (SHOW_MODE == DISPLAY)
      {
        WM_ShowWin(CHESSMAN[i].hItem);
      }
      else
      {
        WM_HideWin(CHESSMAN[i].hItem);
      }
    }
  }
}

void shuffle(void)
{
  int i, k = 0, l;
  struct Chesstype chesstype_temp;
  for (; k < 1000; k++)
  {
    srand(rand_seed);
    i = rand() % 32;
    if (!CHESSMAN[i].open)
    {
      chesstype_temp = CHESSMAN[i].chess_type;
      srand(rand_seed);
      l = rand() % 32;
      if (!CHESSMAN[l].open)
      {
        CHESSMAN[i].chess_type = CHESSMAN[l].chess_type;
        CHESSMAN[l].chess_type = chesstype_temp;
      }
    }
  }
}

void check_first_color(int index)
{
  open_count += 1;
  if (open_count == 1)
  {
    if (CHESSMAN[index].chess_type.color == BLACK)
      current_color = RED;
    else
      current_color = BLACK;
  }
  else
  {
    if (current_color == BLACK)
      current_color = RED;
    else
      current_color = BLACK;
  }
  int i = 0;
  for (; i < 32; i++)
  {
    if (CHESSMAN[i].click == 1)
    {
      WM_ShowWin(CHESSMAN[i].hItem);
      CHESSMAN[i].click = 0;
    }
  }
  Chess_onclick_idx = 100; // set default
}

int refresh_click(int index)
{
  int i = 0;
  if (!CHESSMAN[index].click && CHESSMAN[index].open && CHESSMAN[index].chess_type.color == current_color && CHESSMAN[index].alive)
  {
    for (; i < 32; i++)
    {
      if (CHESSMAN[i].click == 1)
      {
        // if (SHOW_MODE == NODISPLAY)
        //   GUI_Delay(10);                    // TODO: Find method to fix flash image
        WM_ShowWin(CHESSMAN[i].hItem);
        CHESSMAN[i].click = 0;
      }
    }
    Chess_onclick_idx = index;
    CHESSMAN[index].click = 1;
  }
  if (CHESSMAN[index].open && Chess_onclick_idx != 100)
  {
    return game_rule_check(index);
  }
  return 100; // default
}

int game_rule_check(int index)
{
  struct Chesstype chesstype_temp;
  struct Chessman chess_temp;
  int count_chess = 0;
  int index_temp = 0;
  if ((CHESSMAN[Chess_onclick_idx].X + 1 == CHESSMAN[index].X && CHESSMAN[Chess_onclick_idx].Y == CHESSMAN[index].Y) ||
      (CHESSMAN[Chess_onclick_idx].X - 1 == CHESSMAN[index].X && CHESSMAN[Chess_onclick_idx].Y == CHESSMAN[index].Y) ||
      (CHESSMAN[Chess_onclick_idx].X == CHESSMAN[index].X && CHESSMAN[Chess_onclick_idx].Y + 1 == CHESSMAN[index].Y) ||
      (CHESSMAN[Chess_onclick_idx].X == CHESSMAN[index].X && CHESSMAN[Chess_onclick_idx].Y - 1 == CHESSMAN[index].Y))
  {
    if (((CHESSMAN[Chess_onclick_idx].chess_type.chess_type_enum >= CHESSMAN[index].chess_type.chess_type_enum) ||
         (CHESSMAN[Chess_onclick_idx].chess_type.chess_type_enum == P && CHESSMAN[index].chess_type.chess_type_enum == K)) &&
         !(CHESSMAN[Chess_onclick_idx].chess_type.chess_type_enum == K && CHESSMAN[index].chess_type.chess_type_enum == P) &&
        CHESSMAN[index].alive && CHESSMAN[Chess_onclick_idx].alive && CHESSMAN[index].chess_type.color != current_color &&
        CHESSMAN[Chess_onclick_idx].chess_type.chess_type_enum != C)
    {
      chesstype_temp = CHESSMAN[index].chess_type;
      CHESSMAN[index].chess_type = CHESSMAN[Chess_onclick_idx].chess_type;
      CHESSMAN[Chess_onclick_idx].chess_type = chesstype_temp;
      CHESSMAN[Chess_onclick_idx].alive = 0;
      CHESSMAN[Chess_onclick_idx].chess_type.image_ID = IMAGE_WHITE;
      CHESSMAN[Chess_onclick_idx].click = 0;
      WM_ShowWin(CHESSMAN[Chess_onclick_idx].hItem);
      WM_ShowWin(CHESSMAN[index].hItem);
      if (current_color == BLACK)
        current_color = RED;
      else
        current_color = BLACK;
      return Chess_onclick_idx;
    }
    if (!CHESSMAN[index].alive && CHESSMAN[Chess_onclick_idx].alive)
    {
      chesstype_temp = CHESSMAN[index].chess_type;
      CHESSMAN[index].chess_type = CHESSMAN[Chess_onclick_idx].chess_type;
      CHESSMAN[Chess_onclick_idx].chess_type = chesstype_temp;
      CHESSMAN[Chess_onclick_idx].alive = 0;
      CHESSMAN[index].alive = 1;
      CHESSMAN[Chess_onclick_idx].click = 0;
      WM_ShowWin(CHESSMAN[Chess_onclick_idx].hItem);
      WM_ShowWin(CHESSMAN[index].hItem);
      if (current_color == BLACK)
        current_color = RED;
      else
        current_color = BLACK;
      return Chess_onclick_idx;
    }
  }
  // the rule for C to eat
  if (CHESSMAN[Chess_onclick_idx].chess_type.chess_type_enum == C && CHESSMAN[index].alive &&
      CHESSMAN[Chess_onclick_idx].chess_type.color != CHESSMAN[index].chess_type.color)
  {
    if (CHESSMAN[Chess_onclick_idx].Y == CHESSMAN[index].Y)
    {
      count_chess = 0;
      index_temp = index;
      index_temp += 1;
      while (Chess_onclick_idx > index_temp)
      {
        if (CHESSMAN[index_temp].alive)
        {
          count_chess += 1;
        }
        index_temp += 1;
      }
      if (count_chess == 1)
      {
        chesstype_temp = CHESSMAN[index].chess_type;
        CHESSMAN[index].chess_type = CHESSMAN[Chess_onclick_idx].chess_type;
        CHESSMAN[Chess_onclick_idx].chess_type = chesstype_temp;
        CHESSMAN[Chess_onclick_idx].alive = 0;
        CHESSMAN[Chess_onclick_idx].chess_type.image_ID = IMAGE_WHITE;
        CHESSMAN[Chess_onclick_idx].click = 0;
        WM_ShowWin(CHESSMAN[Chess_onclick_idx].hItem);
        WM_ShowWin(CHESSMAN[index].hItem);
        if (current_color == BLACK)
          current_color = RED;
        else
          current_color = BLACK;
        return Chess_onclick_idx;
      }
      ///////////////////////////////////////////
      count_chess = 0;
      index_temp = index;
      index_temp -= 1;
      while (Chess_onclick_idx < index_temp)
      {
        if (CHESSMAN[index_temp].alive)
        {
          count_chess += 1;
        }
        index_temp -= 1;
      }
      if (count_chess == 1)
      {
        chesstype_temp = CHESSMAN[index].chess_type;
        CHESSMAN[index].chess_type = CHESSMAN[Chess_onclick_idx].chess_type;
        CHESSMAN[Chess_onclick_idx].chess_type = chesstype_temp;
        CHESSMAN[Chess_onclick_idx].alive = 0;
        CHESSMAN[Chess_onclick_idx].chess_type.image_ID = IMAGE_WHITE;
        CHESSMAN[Chess_onclick_idx].click = 0;
        WM_ShowWin(CHESSMAN[Chess_onclick_idx].hItem);
        WM_ShowWin(CHESSMAN[index].hItem);
        if (current_color == BLACK)
          current_color = RED;
        else
          current_color = BLACK;
        return Chess_onclick_idx;
      }
    }
    if (CHESSMAN[Chess_onclick_idx].X == CHESSMAN[index].X)
    {
      count_chess = 0;
      index_temp = index;
      index_temp += 8;
      while (Chess_onclick_idx > index_temp)
      {
        if (CHESSMAN[index_temp].alive)
        {
          count_chess += 1;
        }
        index_temp += 8;
      }
      if (count_chess == 1)
      {
        chesstype_temp = CHESSMAN[index].chess_type;
        CHESSMAN[index].chess_type = CHESSMAN[Chess_onclick_idx].chess_type;
        CHESSMAN[Chess_onclick_idx].chess_type = chesstype_temp;
        CHESSMAN[Chess_onclick_idx].alive = 0;
        CHESSMAN[Chess_onclick_idx].chess_type.image_ID = IMAGE_WHITE;
        CHESSMAN[Chess_onclick_idx].click = 0;
        WM_ShowWin(CHESSMAN[Chess_onclick_idx].hItem);
        WM_ShowWin(CHESSMAN[index].hItem);
        if (current_color == BLACK)
          current_color = RED;
        else
          current_color = BLACK;
        return Chess_onclick_idx;
      }
      ///////////////////////////////////////////
      count_chess = 0;
      index_temp = index;
      index_temp -= 8;
      while (Chess_onclick_idx < index_temp)
      {
        if (CHESSMAN[index_temp].alive)
        {
          count_chess += 1;
        }
        index_temp -= 8;
      }
      if (count_chess == 1)
      {
        chesstype_temp = CHESSMAN[index].chess_type;
        CHESSMAN[index].chess_type = CHESSMAN[Chess_onclick_idx].chess_type;
        CHESSMAN[Chess_onclick_idx].chess_type = chesstype_temp;
        CHESSMAN[Chess_onclick_idx].alive = 0;
        CHESSMAN[Chess_onclick_idx].chess_type.image_ID = IMAGE_WHITE;
        CHESSMAN[Chess_onclick_idx].click = 0;
        WM_ShowWin(CHESSMAN[Chess_onclick_idx].hItem);
        WM_ShowWin(CHESSMAN[index].hItem);
        if (current_color == BLACK)
          current_color = RED;
        else
          current_color = BLACK;
        return Chess_onclick_idx;
      }
    }
  }

  return 100; // default
}

void finish_check(void)
{
  int i = 0;
  int count_red = 0;
  int count_black = 0;
  for (; i < 32; i++)
  {
    if (!CHESSMAN[i].alive && CHESSMAN[i].chess_type.color == RED)
    {
      count_red++;
    }
    if (!CHESSMAN[i].alive && CHESSMAN[i].chess_type.color == BLACK)
    {
      count_black++;
    }
  }
  if (count_red == 16)
  {
    winner = BLACK;
    go_time = 0;
    MODE = FINISH;
  }
  if (count_black == 16)
  {
    winner = RED;
    go_time = 0;
    MODE = FINISH;
  }
}

uint8_t find_die(enum Teams color)
{
  int cal = 0;
  if (color == BLACK)
  {
    while (find_idx < 32)
    {
      if (!CHESSMAN[find_idx].alive && CHESSMAN[find_idx].chess_type.color == BLACK)
      {
        cal = ((6 - CHESSMAN[find_idx].chess_type.chess_type_enum) + 0x03);
        find_idx++;
        return cal;
      }
      find_idx++;
    }
  }
  if (color == RED)
  {
    while (find_idx < 32)
    {
      if (!CHESSMAN[find_idx].alive && CHESSMAN[find_idx].chess_type.color == RED)
      {
        cal = ((6 - CHESSMAN[find_idx].chess_type.chess_type_enum) + 0x0A);
        find_idx++;
        return cal;
      }
      find_idx++;
    }
  }
  return IMAGE_WHITE; // for default
}
