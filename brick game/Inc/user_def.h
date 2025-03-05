#pragma once

#ifndef USER_DEF_H
#define USER_DEF_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32746g_discovery.h"
#include "stm32746g_discovery_sdram.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "stdlib.h"
#include "time.h"

#define SCREEN_W 480
#define SCREEN_H 272
#define CENTER_W 240
#define CENTER_H 136
#define CIRCLE_R 5
#define HITBOARD_W 30
#define HITBOARD_H 5
#define HITBOARD_X 210
#define HITBOARD_Y 232
#define HITBOARD_COLOR ((uint32_t)LCD_COLOR_LIGHTMAGENTA)
#define RECT_W 56
#define RECT_H 10
#define RECT_D 40
#define RECT_G 4
#define RECT_NUM 38 // 0 ~ 23
#define TIMMER_Y 12
#define VELOCITY (int)49
#define TIMMER_SPEED (int)199
#define SENSITIVITY 4 // move hitboard sensitivity

    typedef enum DIR
    {
        LEFT_UP,
        LEFT_DOWN,
        RIGHT_UP,
        RIGHT_DOWN
    } DIR;

    typedef struct BALL
    {
        int16_t X;
        int16_t Y;
        DIR dir;
    } BALL;

    typedef struct BLOCK
    {
        uint16_t X;
        uint16_t Y;
        uint8_t vaild;
    } BLOCK;

    typedef struct hitboard
    {
        int16_t X;
    } hitboard;

    // typedef struct TIMER
    // {
    //     char *min;
    //     char *sec;
    // } TIMER;

    typedef enum State
    {
        NEW_GAME,
        BALL_MOVE,
        JUDGE_BOUNDARY,
        JUDGE_BLOCK,
        WAIT_USER,
        WAIT_TOUCH,
        BLOCK_MOVE,
        LOSS_GAME,
        WIN_GAME
    } State;

    typedef enum Time
    {
        TIME_PLUS,
        TIME_STOP
    } Time;

    void init(void);
    void hitboard_press(TS_StateTypeDef *);
    void move_hitboard(TS_StateTypeDef *);
    void ball_move(void);
    void judge_boundary(void);
    void judge_block(void);
    void timer_count(void);
    void win_game(void);
    void loss_game(void);
    void waituserbutton(void);

#ifdef __cplusplus
}
#endif
#endif
