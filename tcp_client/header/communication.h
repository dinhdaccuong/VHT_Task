/**
 * @file communication.h
 * @author CuongD (cuong.dd20150481@gmail.com)
 * @brief 
 * @version 0.1
 * @date 15-12-2020
 * 
 * @copyright Copyright (c) 2020 CuongD
 * 
 */

#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

/*******************************************************************************
**                               INCLUDES
*******************************************************************************/
#include <stdio.h> 
#include <string.h>
#include <ctype.h>
#include <unistd.h> 
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "client_define.h"
using namespace cv;
/*******************************************************************************
**                                DEFINES
*******************************************************************************/
// clang-format off

// clang-format on
/*******************************************************************************
**                     EXTERNAL VARIABLE DECLARATIONS
*******************************************************************************/

/*******************************************************************************
**                     EXTERNAL FUNCTION DECLARATIONS
*******************************************************************************/
int connect_to_server(int socket,const char* server_ip);
int send_start_signal(int socket, video_info_t& video_info, int brightness = 20);
int send_stop_signal(int socket);
int send_data_frame(int socket, Mat &frame);
int receive_all_data(int socket, uint8_t* buff, uint32_t size);
int send_all_data(int socket, uint8_t* buff, uint32_t size);
#endif /* __COMMUNICATION_H__ */

/******************************** End of file *********************************/
