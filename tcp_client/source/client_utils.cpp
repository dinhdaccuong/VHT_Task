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
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdint.h>
#include <stdio.h>

#include "client_define.h"
#include "client_utils.h"


#include <queue>

using namespace std;
using namespace cv;

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

void show_video_info(video_info_t& video_info)
{
    printf("total of frame: %d\n", video_info.total_of_frames);
    printf("total of pixels: %d\n", video_info.frame_info.total_of_pixels);
}

void get_bytes_from_frame(uint8_t* bytes, Mat img)
{
    uint32_t n_bytes = img.cols * img.rows * img.channels();
    for(uint32_t i = 0; i < n_bytes; i++)
    {
        bytes[i] = img.data[i];
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

int get_video_info(const char* path, video_info_t& video_info)
{
    try
    {
        VideoCapture cap(path);   
             
        Mat m;
        if(!cap.isOpened())
        {
            return 1;
        }
        cap >> m;
        video_info.total_of_frames = cap.get(CAP_PROP_FRAME_COUNT);
        video_info.fps = cap.get(CAP_PROP_FPS);
        video_info.frame_info.channels = m.channels();
        video_info.frame_info.type = m.type();
        video_info.frame_info.cols = m.cols;
        video_info.frame_info.rows = m.rows;
        video_info.frame_info.cal_total_of_pixels();
        cap.release();
        return 0;
    }
    catch(const std::exception& e)
    {
        printf("Error when open video\n");
        return 1;
    }

}


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


void bin(char c)
{
    unsigned int k = 0;
    for(int x = 0;  x < 8; x++)
    {
        k = (c << x) & 0x80;
        printf("%d", k >> 7);
    }
    printf("\n");
}

void bin(int n)
{
    unsigned int k = 0;
    for(int x = 0;  x < 32; x++)
    {
        k = (n << x) & 0x80000000;
        printf("%d", k >> 31);
    }
    printf("\n");
}
 

int show_video_from_frames(vector<Mat> &vect_frames)
{
    namedWindow("Frame");
    moveWindow("Frame", 600, 300);

    for(int i = 0; i < vect_frames.size(); i++)
    {
        imshow("Frame", vect_frames[i]);
        waitKey(25);
    }
    return 0;
}

void copy_array(uint8_t* arr1, uint8_t* arr2, uint32_t size)
{
    uint32_t i = 0;
    for(i = 0; i < size; i++)
    {
        arr1[i] = arr2[i];
    }
}

int split_video_to_frame(const char* path_to_video, vector<Mat> &vect_frames)
{
    VideoCapture cap(path_to_video);
    int total_of_frame = cap.get(CAP_PROP_FRAME_COUNT);

    if(!cap.isOpened())
    {
        return -1;      // error
    }
    if(cap.get(CAP_PROP_FRAME_COUNT) < 1)
    {
        return 0;       // 0 frame
    }
    int frame_count = 0;
    while(1)
    {
        Mat frame;
        cap >> frame;
        vect_frames.push_back(frame);
        frame_count++;
        if(frame_count == total_of_frame)
        {
            break;
        }           
    }
    cap.release();
    return 0;
}
/******************************** End of file *********************************/

