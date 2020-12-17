/**
 * @file server_utils.cpp
 * @author CuongD (cuong.dd20150481@gmail.com)
 * @brief 
 * @version 0.1
 * @date 17-12-2020
 * 
 * @copyright Copyright (c) 2020 CuongD
 * 
 */

/*******************************************************************************
**                                INCLUDES
*******************************************************************************/


#include <stdio.h>

#include <stdint.h>

#include <sys/socket.h>

#include <server_utils.h>

/*******************************************************************************
**                       INTERNAL MACRO DEFINITIONS
*******************************************************************************/
// clang-format off
#define MAX(x, y) (x > y ? x : y)
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


void convert_int_to_chars(uint8_t *chars, int num, int pos)
{
    for (int i = sizeof(int) - 1; i >= 0; i--)
    {
        chars[i + pos] = num;
        num = num >> 8;
    }
}

int convert_chars_to_int(const uint8_t *chars, int pos)
{
    int num = 0;
    int temp = 0;
    for (int i = 0; i < 4; i++)
    {
        temp |= chars[i + pos];
        temp &= 0x000000FF;
        num |= temp;

        if (i < 3)
        {
            num = num << 8;
        }
        temp = 0;
    }
    return num;
}

uint8_t saturate_cast(uint8_t num)
{
    if (num > 255)
        return 255;
    return num;
}

void incrase_brightness(uint8_t *image, int img_size, int brightness)
{
    for (int i = 0; i < img_size; i++)
    {
        image[i] = saturate_cast(image[i] + (image[i] * (brightness / 100.0)));
    }
}

void print_chars(const uint8_t *chars, int n_char)
{
    for (int i = 0; i < n_char; i++)
    {
        printf("%c", chars[i] + 48);
    }
    printf("\n");
}

void copy_array(uint8_t *arr1, uint8_t *arr2, uint32_t size)
{
    uint32_t i = 0;
    for (i = 0; i < size; i++)
    {
        arr1[i] = arr2[i];
    }
}
int send_all_data(int socket, uint8_t *buff, uint32_t size)
{
    const char *pbuf = (const char *)buff;

    while (size > 0)
    {
        int sent = send(socket, pbuf, size, 0);
        if (sent < 1)
        {
            printf("Can't write to socket");
            return -1;
        }

        pbuf += sent;
        size -= sent;
    }

    return 0;
}

int receive_all_data(int socket, uint8_t *buff, uint32_t size)
{
    char *pbuf = (char *)buff;
    uint32_t total = 0;

    while (size > 0)
    {
        int rval = recv(socket, pbuf, size, 0);
        if (rval < 0)
        {
            printf("Can't read from socket");
            return -1;
        }

        if (rval == 0)
        {
            printf("Socket disconnected");
            return 0;
        }

        pbuf += rval;
        size -= rval;
        total += rval;
    }

    return total;
}

void show_image_data(uint8_t* image, uint32_t size)
{
    printf("[ ");
    for(uint32_t i = 0; i < size; i++)
    {
        printf("%d ", image[i]);
    }
    printf(" ]\n");
}


/******************************** End of file *********************************/
