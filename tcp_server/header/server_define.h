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

#ifndef __SERVER_DEFINE_H__
#define __SERVER_DEFINE_H__


/*******************************************************************************
**                               INCLUDES
*******************************************************************************/
#include <pthread.h>
#include <stdint.h>
#include <queue>
using namespace std;

/*******************************************************************************
**                                DEFINES
*******************************************************************************/
// clang-format off

#define PORT                                8888
#define MAX_CLIENT                          10

#define TOTAL_BYTE_START_SIGNAL             13
#define BYTE_HEIGHT_DATA_RECEIVE            0
#define BYTE_WIDTH_DATA_RECEIVE             4
#define BYTE_NFRAME_DATA_RECEIVE            8
#define BYTE_BRIHTNESS_DATA_RECEIVE         12

#define TOTAL_BYTE_STOP_SIGNAL              1

#define CHANNELS                            3

typedef struct 
{
    int cols;
    int rows;
    int channels;
    uint32_t total_of_pixels;
    void cal_total_of_pixels(){total_of_pixels = rows * cols * channels;}
} frame_info_t;

typedef struct
{
    frame_info_t frame_info;
    int total_of_frames;
} video_info_t; 

typedef struct
{
    uint8_t* data;
    uint32_t n_data;
} data_t;

typedef struct 
{
    video_info_t        video_info;  // Thong tin frame
    int                 brightness;  // Do sang
    int                 client_id;
    int                 socket;
} thread_arg_t;
// clang-format on
/*******************************************************************************
**                     EXTERNAL VARIABLE DECLARATIONS
*******************************************************************************/

/*******************************************************************************
**                     EXTERNAL FUNCTION DECLARATIONS
*******************************************************************************/

#endif /* __DEFINE_H__ */

/******************************** End of file *********************************/
