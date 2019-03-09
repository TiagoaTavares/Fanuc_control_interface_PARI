/**
 * @file pcolor.c
 * @author Tiago Tavares (tiagoatavares@ua.pt)
 * @brief 
 * @version 0.1
 * @date 2018-12-26
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#include <stdio.h>
#include "myutils.h"

/**
 * @brief  Set the color of text that follows upon the next printf.
 *
 * @param  attr Type of attribute (RESET, BRIGHT, BLINK, etc...)
 * @param  fg Color of foreground
 * @param  bg Color of background
 * @return Nothing
 */
void textcolor(int attr, int fg, int bg)
{
        printf("%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
}

/**
 * @brief  printf of an horizontal separator red line
 *
 * @return Nothing
 */
void PrintRedLine(void)
{
        int n;
        textcolor(RESET, RED, WHITE);   //completar a função
        for(n=0; n< 40; n++)
                printf("=");
        printf("\n");
}

/**
 * @brief  printf of an horizontal separator green line
 *
 * @return Nothing
 */
void PrintGreenLine(void)
{
        int n;
        textcolor(RESET, GREEN, WHITE);   //completar a função
        for(n=0; n< 40; n++)
                printf("=");
        printf("\n");
}


/**
 * @brief  Reset color text
 *
 * @return Nothing
 */
void ResetTextColors(void)
{
        textcolor(RESET, WHITE, WHITE); //completar a função
}

/**
 * @brief  Reset color text to red
 *
 * @return Nothing
 */
void TextColorRed(void)
{
        textcolor(RESET, RED, WHITE); //completar a função
}



/**
 * @brief  Make somesort of highlight text
 *
 * @return Nothing
 */
void HighLightText(void)
{
        textcolor(RESET, BLUE, YELLOW); //completar a função
}
