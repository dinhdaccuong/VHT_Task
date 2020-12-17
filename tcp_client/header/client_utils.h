/**
 * @file utils.h
 * @author CuongD (cuong.dd20150481@gmail.com)
 * @brief 
 * @version 0.1
 * @date 15-12-2020
 * 
 * @copyright Copyright (c) 2020 CuongD
 * 
 */

#ifndef __UTILS_H__
#define __UTILS_H__


/*******************************************************************************
**                               INCLUDES
*******************************************************************************/

#include "opencv2/opencv.hpp"

#include <queue>
#include <vector>

#include <client_define.h>

using namespace cv;
using namespace std;
/*******************************************************************************
**                                DEFINES
*******************************************************************************/
// clang-format off

// clang-format on
/*******************************************************************************
**                     EXTERNAL VARIABLE DECLARATIONS
*******************************************************************************/
int show_video_from_frames(vector<Mat> &vect_frames);
int split_video_to_frame(const char* path_to_video, vector<Mat> &vect_frames);
void print_chars(const char* chars, int n_char);
void convert_int_to_chars(char* chars, int num, int pos = 0);
int convert_chars_to_int(const char* chars, int pos = 0);
int get_video_info(const char* path, video_info_t& video_info);
void get_bytes_from_frame(uint8_t* bytes, Mat img);
void show_video_info(video_info_t& video_info);
void bin(int n);
void bin(char c);
void copy_array(uint8_t* arr1, uint8_t* arr2, uint32_t size);
/*******************************************************************************
**                     EXTERNAL FUNCTION DECLARATIONS
*******************************************************************************/

#endif /* __UTILS_H__ */

/******************************** End of file *********************************/
