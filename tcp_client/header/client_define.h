/**
 * @file define.h
 * @author CuongD (cuong.dd20150481@gmail.com)
 * @brief 
 * @version 0.1
 * @date 15-12-2020
 * 
 * @copyright Copyright (c) 2020 CuongD
 * 
 */

#ifndef __CLIENT_DEFINE_H__
#define __CLIENT_DEFINE_H__


/*******************************************************************************
**                               INCLUDES
*******************************************************************************/
#include <stdint.h>
/*******************************************************************************
**                                DEFINES
*******************************************************************************/
// clang-format off
#define TOTAL_BYTE_START_SIGNAL     13
#define TOTAL_BYTE_STOP_SIGNAL      10

#define PORT                        8888
#define SERVER_IP_ADDRESS           "127.0.0.1"             


#define PATH_TO_VIDEO               "/home/cuongd/CuongD/VHT_Task/small.avi"
#define PATH_TO_IMAGE               "/home/cuongd/CuongD/VHT_Task/size.jpeg"

typedef struct
{
    int channels;
    int cols;
    int rows;
    int type;
    uint32_t total_of_pixels;
    void cal_total_of_pixels(){total_of_pixels = cols * rows * channels;};
} frame_info_t;

typedef struct
{
    frame_info_t frame_info;
    int total_of_frames;
    int fps;
} video_info_t;

// clang-format on
/*******************************************************************************
**                     EXTERNAL VARIABLE DECLARATIONS
*******************************************************************************/

/*******************************************************************************
**                     EXTERNAL FUNCTION DECLARATIONS
*******************************************************************************/

#endif /* __DEFINE_H__ */

/******************************** End of file *********************************/
