#pragma once

#ifndef USER_DEF_H
#define USER_DEF_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ff.h"
#include "GUI.h"
#include "DIALOG.h"
#include "string.h"
#include "stdio.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_sdram.h"
#include "stm32746g_discovery_ts.h"


#define LCD_WIDTH   480
#define LCD_HEIGHT  272
#define PIC_SIZE_X 57
#define PIC_SIZE_Y 57
//////////optimize///////////
// #define SHOW_hIem
// #define SHOW_position

/////////chessman//////////////
#define IMAGE_BK 0x03
#define IMAGE_BA 0x04
#define IMAGE_BB 0x05
#define IMAGE_BR 0x06
#define IMAGE_BN 0x07
#define IMAGE_BC 0x08
#define IMAGE_BP 0x09
#define IMAGE_RK 0x0A
#define IMAGE_RA 0x0B
#define IMAGE_RB 0x0C
#define IMAGE_RR 0x0D
#define IMAGE_RN 0x0E
#define IMAGE_RC 0x0F
#define IMAGE_RP 0x10
#define IMAGE_WHITE 0x11
#define IMAGE_BLACKWIN 0x12
#define IMAGE_REDWIN 0x13
///////////////////////////////

extern enum State MODE;
extern enum Show SHOW_MODE;
extern enum Teams current_color;
extern enum Teams winner;
extern uint8_t go_time;
extern unsigned int rand_seed;
extern uint8_t time_count;
extern int open_count;
extern int find_idx;

void CPU_CACHE_Enable(void);

void BSP_Touch_Update(void);
void LCD_initial(void);

void shuffle(void);

void chessman_init(void);
int refresh_click(int );
void check_click(void);
void check_first_color(int );
int game_rule_check(int );
uint8_t find_die(enum Teams);
void finish_check(void);

typedef enum State
{
    IDLE,
    PLAY,
    STOP,
    FINISH
} State;

typedef enum Show
{
    DISPLAY,
    NODISPLAY
} Show;

typedef enum Chess
{
    K = 6,
    A = 5,
    B = 4,
    R = 3,
    N = 2,
    C = 1,
    P = 0
} Chess;

typedef enum Teams
{
    BLACK,
    RED,
    NONE,
} Teams;

typedef struct Chesstype
{
    uint8_t image_ID;
    enum Chess chess_type_enum;
    enum Teams color;
} Chesstype;

typedef struct Chessman
{
    int X;
    int Y;
    uint8_t open;
    uint8_t click;
    uint8_t alive;
    WM_HWIN hItem;
    struct Chesstype chess_type;
} Chessman;

extern struct Chessman CHESSMAN[32];

#ifdef __cplusplus
}
#endif

#endif

