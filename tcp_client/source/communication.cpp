/**
 * @file communication.cpp
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
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h> 
#include <stdio.h> 

#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>

#include <string.h>
#include <ctype.h>

#include "client_utils.h"
#include "client_define.h"
#include "communication.h"

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

int send_start_signal(int socket, video_info_t& video_info, int brightness)
{
    char start_squence[TOTAL_BYTE_START_SIGNAL];
    memset(start_squence, 0, TOTAL_BYTE_START_SIGNAL);
    convert_int_to_chars(start_squence, video_info.frame_info.rows);      // byte tu 0-3
    convert_int_to_chars(start_squence, video_info.frame_info.cols, 4);      // byte tu 4-7
    convert_int_to_chars(start_squence, video_info.total_of_frames, 8);
    start_squence[12] = brightness;
    int n_data_send = send(socket, start_squence, 13, 0);

    if(n_data_send < 0)
    {
        printf("Errorrrr\n");
        return 1;
    }
    return 0;
}
int send_stop_signal(int socket)
{

}
int send_data_frame(int socket, Mat &frame)
{

}

int connect_to_server(int socket,const char* server_ip)
{
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if(connect(socket, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        close(socket);
        return 1;
    }
    return 0;
}

int send_all_data(int socket, uint8_t* buff, uint32_t size)
{
    const char *pbuf = (const char *) buff;

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

int receive_all_data(int socket, uint8_t* buff, uint32_t size)
{
    char *pbuf = (char *) buff;
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
/******************************** End of file *********************************/
