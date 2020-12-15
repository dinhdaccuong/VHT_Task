#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>

using namespace std;
using namespace cv;

#define USING_OPENCV                1

#define TOTAL_BYTE_START_SIGNAL     13
#define TOTAL_BYTE_STOP_SIGNAL      10

#define PORT                        8888
#define SERVER_IP_ADDRESS           "127.0.0.1"             


#define PATH_TO_VIDEO               "/home/cuongd/CuongD/VHT_Task/small.avi"
#define PATH_TO_IMAGE               "/home/cuongd/CuongD/VHT_Task/size.jpeg"

typedef struct
{
    int height;
    int width;
    int total_of_frame;
} frame_info;

int connect_to_server(int socket,const char* server_ip);
int split_video_to_frame(const char* path_to_video, vector<Mat> &vect_frames);
int show_video_from_frames(vector<Mat> &vect_frames);
int send_start_signal(int socket,  frame_info& frame_i, int brightness = 20);
int send_stop_signal(int socket);
int send_data_frame(int socket, Mat &frame);

void print_chars(const char* chars, int n_char);
void convert_int_to_chars(char* chars, int num, int pos = 0);
int convert_chars_to_int(const char* chars, int pos = 0);


int get_frame_info_from_video(const char* path, frame_info& p_info);
int get_video_fps(const char* path);

void bin(int n);
void bin(char c);

int main()
{
    
    int socket_to_connect = 0;
    frame_info f_infor;
    if(get_frame_info_from_video(PATH_TO_VIDEO, f_infor))
    {
        printf("Get frame infor error\n");
        return 1;
    }
#if USING_OPENCV
    socket_to_connect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socket_to_connect < 0)
    {
        printf("Create socket error\n");
        return 1;
    }

    if(socket_to_connect < 0)
    {
        printf("Could not create socket\n");
        return 1;
    }

    if(connect_to_server(socket_to_connect, SERVER_IP_ADDRESS))
    {
        printf("Connect error\n");
        return 1;
    }

    if(send_start_signal(socket_to_connect, f_infor))
    {
        printf("Send start signal error\n");
        return 1;
    }
 #endif

    return 0;

}

int get_video_fps(const char* path)
{

    VideoCapture cap(path);
    if(!cap.isOpened())
    {
        return -1;
    }

    int fps = cap.get(CAP_PROP_FPS);

    cap.release();

    return fps;
}
void print_chars(const char* chars, int n_char)
{
    for(int i = 0; i < n_char; i ++)
    {
        printf("%c", chars[i] + 48);
    }
    printf("\n");
}

int get_frame_info_from_video(const char* path, frame_info& p_info)
{
    VideoCapture cap(path);
    if(!cap.isOpened())
    {
        return 1;
    }
    p_info.height = cap.get(CAP_PROP_FRAME_HEIGHT);
    p_info.width = cap.get(CAP_PROP_FRAME_WIDTH);
    p_info.total_of_frame = cap.get(CAP_PROP_FRAME_COUNT);

    cap.release();
    return 0;
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


int send_start_signal(int socket, frame_info& frame_i, int brightness)
{
    char start_squence[13];
    memset(start_squence, 0, 13);
    convert_int_to_chars(start_squence, frame_i.height);      // byte tu 0-3
    convert_int_to_chars(start_squence, frame_i.width, 4);      // byte tu 4-7
    convert_int_to_chars(start_squence, frame_i.total_of_frame, 8);
    start_squence[12] = brightness;

    if(send(socket, start_squence, TOTAL_BYTE_START_SIGNAL, 0) < 0)
    {
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
    for(int i = 0; i < vect_frames.size(); i++)
    {
        imshow("Frame", vect_frames[i]);
        printf("frame %d\n", i + 1);
        waitKey(25);
    }
    return 0;
}

int split_video_to_frame(const char* path_to_video, vector<Mat> &vect_frames)
{
    VideoCapture cap(path_to_video);

    if(!cap.isOpened())
    {
        return 1;
    }
    
    while(1)
    {
        Mat frame;
        cap >> frame;
        if(frame.empty())
        {
            break;
        }
        vect_frames.push_back(frame);
    }
    
    cap.release();
    return 0;
}

int connect_to_server(int socket,const char* server_ip)
{
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if(connect(socket, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        close(socket);
        return 1;
    }
    return 0;
}