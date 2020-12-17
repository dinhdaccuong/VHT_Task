#include <unistd.h> 
#include <stdio.h> 

#include <stdlib.h> 
#include <string.h>
#include <ctype.h>


#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>


#include "client_define.h"
#include "client_utils.h"
#include "communication.h"
#include <string>

using namespace std;
using namespace cv;

#define USING_OPENCV                1

video_info_t video_info;

char path_to_video[1024];
char server_ip[20];
char brightness[5];


void* send_image_to_server(void* data);


void show_image_data(uint8_t* image, uint32_t size);
void show_image_data(Mat mat, uint32_t size);
int socket_to_connect = 0;

int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        printf("Command invalid\n");
        return 1;
    }

    strcpy(server_ip, argv[1]);
    strcpy(path_to_video, argv[2]);
    strcpy(brightness, argv[3]);


    
    if(get_video_info(path_to_video, video_info))
    {
        printf("Get frame infor error\n");
        return 1;
    }

    socket_to_connect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socket_to_connect < 0)
    {
        printf("Create socket error\n");
        return 1;
    }

    if(connect_to_server(socket_to_connect, server_ip))
    {
        printf("Connect error\n");
        return 1;
    }

    if(send_start_signal(socket_to_connect, video_info, atoi(brightness)) > 0)
    {
        printf("Start signal error\n");
        return 1;
    }

    

    uint32_t data_size = video_info.frame_info.total_of_pixels;
    uint8_t* data_to_send = new uint8_t[data_size];
    uint8_t* data_to_recv = new uint8_t[data_size];
    uint8_t** data_to_save = new uint8_t*[data_size];

    vector <Mat> vect_images_recv;
    vector <Mat> vect_images_send;
    
    int frame_count = 0;
    uint32_t n_data_recv = 0;
    uint32_t n_data_sent = 0;

    if(split_video_to_frame(path_to_video, vect_images_send) > 0)
    {
        printf("Split video error\n");
        return 1;
    }
    

    while (1)
    {
        
        memset(data_to_send, 0, data_size);
        
        get_bytes_from_frame(data_to_send, vect_images_send[frame_count]);
        show_image_data(data_to_send, 20);
        n_data_sent = send_all_data(socket_to_connect, data_to_send, data_size);
        
        printf("Dinh Dac Cuong\n"); 
        
        printf("n_data_sent = %d\n", n_data_sent);

        if(n_data_sent < 0)
        {
            printf("Sent image error\n");
            return 1;
        }
        printf("CLIENT: Sent frame %d\n", frame_count + 1);


        memset(data_to_recv, 0, data_size);
        n_data_recv = receive_all_data(socket_to_connect, data_to_recv, data_size);
        show_image_data(data_to_recv, 20);
        printf("n_data_recv = %d\n", n_data_recv); 
        if(n_data_recv != data_size)
        {
            printf("Receive errorDDDDDDDD\n");
            return 1;
        }
        printf("CLIENT: Received frame %d\n\n", frame_count + 1);

        data_to_save[frame_count] = new uint8_t[video_info.frame_info.total_of_pixels];
        
        
        copy_array(data_to_save[frame_count], data_to_recv, video_info.frame_info.total_of_pixels);

        Mat mat = Mat(video_info.frame_info.rows, video_info.frame_info.cols, video_info.frame_info.type, data_to_save[frame_count]);
        vect_images_recv.push_back(mat);

        frame_count++;

        if(frame_count == video_info.total_of_frames)
        {
            printf("Break\n");
            break;
        }

    }


    printf("COMPELETE!!\n");
    show_video_from_frames(vect_images_recv);
    
    waitKey(1000);


    for(int i = 0; i < video_info.total_of_frames; i++)
    {
        delete(data_to_save[i]);
    }

    delete(data_to_save);
    delete(data_to_recv);
    delete(data_to_send);
    close(socket_to_connect);

    return 0;
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

void show_image_data(Mat mat, uint32_t size)
{
    printf("[ ");
    for(uint32_t i = 0; i < size; i++)
    {
        printf("%d ", mat.data[i]);
    }
    printf(" ]\n");
}

