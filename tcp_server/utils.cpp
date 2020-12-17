/**
 * @file utils.cpp
 * @author CuongD (cuong.dd20150481@gmail.com)
 * @brief 
 * @version 0.1
 * @date 15-12-2020
 * 
 * @copyright Copyright (c) 2020 CuongD
 * 
 */

/*******************************************************************************
**                                INCLUDES
*******************************************************************************/
#include <utils.h>
#include <stdio.h>
/*******************************************************************************
**                       INTERNAL MACRO DEFINITIONS
*******************************************************************************/
// clang-format off

// clang-format on
/*******************************************************************************
**                      COMMON VARIABLE DEFINITIONS
*******************************************************************************/

/*******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/

/*******************************************************************************
**                      INTERNAL FUNCTION PROTOTYPES
*******************************************************************************/

/*******************************************************************************
**                          FUNCTION DEFINITIONS
*******************************************************************************/
void convert_int_to_chars(char* chars, int num, int pos)
{
    for(int i = sizeof(int) - 1; i >= 0; i--)
    {
        chars[i + pos] = num;
        num = num >> 8;
    }
}

int convert_chars_to_int(const char* chars, int pos)
{
    int num = 0;
    int temp = 0;
    for(int i = 0; i < 4; i++)
    {
        temp |= chars[i + pos];
        temp &= 0x000000FF;
        num |= temp;

        if(i < 3)
        {
            num = num << 8;
        }
        temp = 0;      
    }
    return num;
}

uint8_t saturate_cast(uint8_t num, float mul)
{
    if(num * mul > 255) 
        return 255;
    return num * mul;
}

void incrase_brightness(uint8_t* image, int img_size, int brightness)
{
    for(int i = 0; i < img_size; i++)
    {
        image[i] = saturate_cast(image[i], brightness/100.0);
    }
}

void print_chars(const char* chars, int n_char)
{
    for(int i = 0; i < n_char; i ++)
    {
        printf("%c", chars[i] + 48);
    }
    printf("\n");
}

/******************************** End of file *********************************/
