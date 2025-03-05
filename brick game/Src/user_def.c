#include "user_def.h"

State MODE;
Time TIME_STATE;
BLOCK block[RECT_NUM];
// TIMER timer;
BALL ball;
hitboard hitboard_cur;
TS_StateTypeDef TS_State_pre;
uint8_t game_strat;
uint8_t moveing_flag; // still touch
uint8_t block_count;
uint8_t rand_seed;
uint8_t time_count;
// uint8_t flag = 0;
char now_time[5] = " 0:0 ";

/*---------------------------
        NEW_GAME
----------------------------*/
void init(void)
{
    int i, j, k;
    srand(100);
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    k = 0;
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 3; j++)
        {
            block[k].X = (uint16_t)(i * (RECT_W + RECT_G));
            block[k].Y = (uint16_t)(j * (RECT_H + RECT_G) + RECT_D);
            block[k].vaild = 1;
            BSP_LCD_FillRect(i * (RECT_W + RECT_G), j * (RECT_H + RECT_G) + RECT_D, RECT_W, RECT_H);
            k++;
        }
    }
    now_time[0] = ' ';
    now_time[1] = '0';
    now_time[3] = '0';
    now_time[4] = ' ';
    BSP_LCD_FillCircle(CENTER_W, CENTER_H, CIRCLE_R);
    //
    BSP_LCD_SetTextColor(HITBOARD_COLOR);
    //
    BSP_LCD_FillRect(CENTER_W - (HITBOARD_W / 2), HITBOARD_Y, HITBOARD_W, HITBOARD_H);
    //
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    //
    BSP_LCD_DisplayStringAt(0, TIMMER_Y, (uint8_t *)now_time, CENTER_MODE);
    hitboard_cur.X = (uint16_t)(CENTER_W - (HITBOARD_W / 2));
    ball.X = CENTER_W;
    ball.Y = CENTER_H;
    game_strat = 0;
    rand_seed = 0;
    time_count = 0;
    block_count = 0;
    TIME_STATE = TIME_STOP;
    MODE = WAIT_TOUCH;
}

void waituserbutton(void)
{
}

/*---------------------------
        WAIT_TOUCH
----------------------------*/
void hitboard_press(TS_StateTypeDef *TS_State)
{
    srand(rand_seed);
    if (((*TS_State).touchX[0] > hitboard_cur.X &&
         (*TS_State).touchX[0] < (uint16_t)(hitboard_cur.X + (int16_t)HITBOARD_W) &&
         ((*TS_State).touchY[0] > (uint16_t)HITBOARD_Y)) ||
        moveing_flag)
    {
        TS_State_pre = *TS_State;
        if (!game_strat)
        {
            switch (rand() % 4)
            {
            case 0:
                ball.dir = RIGHT_UP;
                break;
            case 1:
                ball.dir = RIGHT_DOWN;
                break;
            case 2:
                ball.dir = LEFT_UP;
                break;
            case 3:
                ball.dir = LEFT_DOWN;
                break;
            default:
                break;
            }
            game_strat = 1;
        }
        if (MODE == BALL_MOVE || MODE == JUDGE_BOUNDARY || MODE == JUDGE_BLOCK)
        {
        }
        else
        {
            MODE = BLOCK_MOVE;
        }
    }
    BSP_TS_ResetTouchData(TS_State);
}

/*---------------------------
        BLOCK_MOVE
----------------------------*/
void move_hitboard(TS_StateTypeDef *TS_State)
{
    moveing_flag = 1;
    if ((*TS_State).touchX[0] == 0x0000 && (*TS_State).touchY[0] == 0x0000)
    {
        moveing_flag = 0;
    }
    else
    {
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_FillRect(hitboard_cur.X, HITBOARD_Y, HITBOARD_W, HITBOARD_H);
        // BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        //
        BSP_LCD_SetTextColor(HITBOARD_COLOR);
        //
        hitboard_cur.X = hitboard_cur.X + ((*TS_State).touchX[0] - TS_State_pre.touchX[0]) * (int16_t)SENSITIVITY;
        if (hitboard_cur.X < 0)
        {
            hitboard_cur.X = 0;
        }
        else if ((hitboard_cur.X + (int16_t)HITBOARD_W) > (int16_t)SCREEN_W)
        {
            hitboard_cur.X = (int16_t)(SCREEN_W - HITBOARD_W);
        }
        BSP_LCD_SetTextColor(HITBOARD_COLOR);
        BSP_LCD_FillRect(hitboard_cur.X, HITBOARD_Y, HITBOARD_W, HITBOARD_H);
        //
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        //
    }
    BSP_TS_ResetTouchData(TS_State);
    if (MODE == BALL_MOVE || MODE == JUDGE_BOUNDARY || MODE == JUDGE_BLOCK)
    {
    }
    else
    {
        MODE = WAIT_TOUCH;
    }
}

/*---------------------------
        BALL_MOVE
----------------------------*/
void ball_move(void)
{
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillCircle(ball.X, ball.Y, CIRCLE_R);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    switch (ball.dir)
    {
    case RIGHT_UP:
        ball.X++;
        ball.Y--;
        if (ball.Y <= 0)
        {
            ball.Y = 0;
        }
        break;
    case RIGHT_DOWN:
        ball.X++;
        ball.Y++;
        break;
    case LEFT_UP:
        ball.X--;
        if (ball.X <= 0)
        {
            ball.X = 0;
        }
        ball.Y--;
        if (ball.Y <= 0)
        {
            ball.Y = 0;
        }
        break;
    case LEFT_DOWN:
        ball.X--;
        if (ball.X <= 0)
        {
            ball.X = 0;
        }
        ball.Y++;
        break;
    default:
        break;
    }
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillCircle(ball.X, ball.Y, CIRCLE_R);
    //
    BSP_LCD_SetTextColor(HITBOARD_COLOR);
    //
    BSP_LCD_FillRect(hitboard_cur.X, HITBOARD_Y, HITBOARD_W, HITBOARD_H);
    //
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    //
    MODE = JUDGE_BOUNDARY;
}

/*---------------------------
        JUDGE_BOUNDARY
----------------------------*/
void judge_boundary(void)
{
    if ((ball.Y - (uint16_t)CIRCLE_R) == 0)
    {
        switch (ball.dir)
        {
        case RIGHT_UP:
            ball.dir = RIGHT_DOWN;
            break;
        case LEFT_UP:
            ball.dir = LEFT_DOWN;
            break;
        default:
            break;
        }
    }
    if ((ball.X - (uint16_t)CIRCLE_R) == 0)
    {
        switch (ball.dir)
        {
        case LEFT_UP:
            ball.dir = RIGHT_UP;
            break;
        case LEFT_DOWN:
            ball.dir = RIGHT_DOWN;
            break;
        default:
            break;
        }
    }
    if ((ball.X + (uint16_t)CIRCLE_R) == SCREEN_W)
    {
        switch (ball.dir)
        {
        case RIGHT_UP:
            ball.dir = LEFT_UP;
            break;
        case RIGHT_DOWN:
            ball.dir = LEFT_DOWN;
            break;
        default:
            break;
        }
    }
    if ((ball.Y + (uint16_t)CIRCLE_R) == HITBOARD_Y &&
        (ball.X + (uint16_t)CIRCLE_R) >= hitboard_cur.X &&
        (hitboard_cur.X + HITBOARD_W) >= (ball.X - (uint16_t)CIRCLE_R))
    {
        switch (ball.dir)
        {
        case RIGHT_DOWN:
            ball.dir = RIGHT_UP;
            break;
        case LEFT_DOWN:
            ball.dir = LEFT_UP;
            break;
        default:
            break;
        }
    }
    //
    BSP_LCD_SetTextColor(HITBOARD_COLOR);
    //
    BSP_LCD_FillRect(hitboard_cur.X, HITBOARD_Y, HITBOARD_W, HITBOARD_H);
    //
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    //
    if (ball.Y + (uint16_t)CIRCLE_R == SCREEN_H)
    {
        MODE = LOSS_GAME;
    }
    else
    {
        MODE = JUDGE_BLOCK;
    }
}

/*---------------------------
        JUDGE_BLOCK
----------------------------*/
void judge_block(void)
{
    int i = 0;
    for (; i < RECT_NUM; i++)
    {
        // block down boundary
        if ((ball.Y - (uint16_t)CIRCLE_R) == (block[i].Y + (uint16_t)RECT_H) &&
            ball.X >= (block[i].X - 5) &&
            (block[i].X + (uint16_t)RECT_W + 5) >= ball.X &&
            (block[i].vaild == 1))
        {
            switch (ball.dir)
            {
            case LEFT_UP:
                ball.dir = LEFT_DOWN;
                break;
            case RIGHT_UP:
                ball.dir = RIGHT_DOWN;
            default:
                break;
            }
            block[i].vaild = 0;
            block_count++;
            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
            BSP_LCD_FillRect(block[i].X, block[i].Y, RECT_W, RECT_H);
            BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        }
        // block up boundary
        if ((ball.Y + (uint16_t)CIRCLE_R) == block[i].Y &&
            ball.X >= (block[i].X - 5) &&
            (block[i].X + (uint16_t)RECT_W + 5) >= ball.X &&
            (block[i].vaild == 1))
        {
            switch (ball.dir)
            {
            case LEFT_DOWN:
                ball.dir = LEFT_UP;
                break;
            case RIGHT_DOWN:
                ball.dir = RIGHT_UP;
            default:
                break;
            }
            block[i].vaild = 0;
            block_count++;
            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
            BSP_LCD_FillRect(block[i].X, block[i].Y, RECT_W, RECT_H);
            BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        }
        // block left boundary
        if ((ball.X + (uint16_t)CIRCLE_R) == block[i].X &&
            ball.Y >= block[i].Y &&
            (block[i].Y + (uint16_t)RECT_H) >= ball.Y &&
            (block[i].vaild == 1))
        {
            switch (ball.dir)
            {
            case RIGHT_DOWN:
                ball.dir = LEFT_DOWN;
                break;
            case RIGHT_UP:
                ball.dir = LEFT_UP;
            default:
                break;
            }
            block[i].vaild = 0;
            block_count++;
            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
            BSP_LCD_FillRect(block[i].X, block[i].Y, RECT_W, RECT_H);
            BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        }
        // block right boundary
        if ((ball.X - (uint16_t)CIRCLE_R) == (block[i].X + (uint16_t)RECT_W) &&
            ball.Y >= block[i].Y &&
            (block[i].Y + (uint16_t)RECT_H) >= ball.Y &&
            (block[i].vaild == 1))
        {
            switch (ball.dir)
            {
            case LEFT_DOWN:
                ball.dir = RIGHT_DOWN;
                break;
            case LEFT_UP:
                ball.dir = RIGHT_UP;
            default:
                break;
            }
            block[i].vaild = 0;
            block_count++;
            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
            BSP_LCD_FillRect(block[i].X, block[i].Y, RECT_W, RECT_H);
            BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        }
    }
    if (block_count == RECT_NUM)
    {
        MODE = WIN_GAME;
    }
    else
    {
        MODE = WAIT_TOUCH;
    }
}
/*---------------------------
        WIN_GAME
----------------------------*/
void win_game(void)
{
    game_strat = 0;
    BSP_LCD_FillCircle(ball.X, ball.Y, CIRCLE_R);
    BSP_LCD_DisplayStringAt(0, TIMMER_Y, (uint8_t *)now_time, CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, CENTER_H, (uint8_t *)"Win", CENTER_MODE);
}
/*---------------------------
        LOSS_GAME
----------------------------*/
void loss_game(void)
{
    game_strat = 0;
    BSP_LCD_FillCircle(ball.X, ball.Y, CIRCLE_R);
    BSP_LCD_DisplayStringAt(0, TIMMER_Y, (uint8_t *)now_time, CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, CENTER_H, (uint8_t *)"Game_Over", CENTER_MODE);
}

/*---------------------------
        TIMER
----------------------------*/
// this one need to be better modify use sturct pointer
void timer_count(void)
{
    if (now_time[3] < '9' && now_time[4] == ' ')
    {
        now_time[3] += 1;
    }
    else if (now_time[3] == '9' && now_time[4] == ' ')
    {
        now_time[3] = '1';
        now_time[4] = '0';
    }
    else if (now_time[4] < '9' && now_time[3] > '0')
    {
        now_time[4] += 1;
    }
    else if (now_time[3] < '5' && now_time[4] == '9')
    {
        now_time[3] += 1;
        now_time[4] = '0';
    }
    else if (now_time[3] == '5' && now_time[4] == '9' && now_time[1] < '9')
    {
        now_time[3] = '0';
        now_time[4] = ' ';
        now_time[1] += 1;
    }
    else if (now_time[3] == '5' && now_time[4] == '9' && now_time[1] == '9' && now_time[0] == ' ')
    {
        now_time[3] = '0';
        now_time[4] = ' ';
        now_time[1] = '0';
        now_time[0] = '1';
    }
    else if (now_time[3] == '5' && now_time[4] == '9' && now_time[1] == '9' && now_time[0] < '9')
    {
        now_time[3] = '0';
        now_time[4] = ' ';
        now_time[1] = '0';
        now_time[0] += 1;
    }
    else if (now_time[3] == '5' && now_time[4] == '9' && now_time[1] == '9' && now_time[0] == '9')
    {
        now_time[3] = '0';
        now_time[4] = ' ';
        now_time[1] = '0';
        now_time[0] = ' ';
    }
    BSP_LCD_DisplayStringAt(0, TIMMER_Y, (uint8_t *)now_time, CENTER_MODE);
    TIME_STATE = TIME_STOP;
}
