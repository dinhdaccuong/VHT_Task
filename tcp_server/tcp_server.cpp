#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <cstdlib>
#include <signal.h>
#include <stdint.h>
#include <queue>

using namespace std;

#define TEST                                1
#define DEBUG                               1



#define PORT                                8888
#define MAX_CLIENT                          10

#define TOTAL_BYTE_START_SIGNAL             13
#define BYTE_HEIGHT_DATA_RECEIVE            0
#define BYTE_WIDTH_DATA_RECEIVE             4
#define BYTE_NFRAME_DATA_RECEIVE            8
#define BYTE_BRIHTNESS_DATA_RECEIVE         12

#define TOTAL_BYTE_STOP_SIGNAL              1


int n_client = 0; // number of client

typedef struct
{
    int height;
    int width;
    int total_of_frame;
} frame_info_t;

typedef struct
{
    uint8_t* data;
    uint32_t n_data;
} data_t;

typedef struct 
{
    queue<data_t>       q_data;      // Hang doi luu lieu
    pthread_t*          subthread;   // Thread con
    pthread_mutex_t     mt_lock;     // mutex
    frame_info_t        frame_info;  // Thong tin frame
    int                 brightness;  // Do sang
    int                 total_pixel;
    int                 client_id;
    int                 socket;
    void cal_total_pixel()
    {
        total_pixel = frame_info.height * frame_info.width;
    }
} thread_arg_t;

pthread_t new_threads[MAX_CLIENT];  
sockaddr_in sockaddr_clients[MAX_CLIENT];
thread_arg_t thread_args[MAX_CLIENT];

void *connection_handler(void *socket_desc);
void clear_all(int sign_num);
int init_server(int socket);

int convert_chars_to_int(const char* chars, int pos = 0);
void convert_int_to_chars(char* chars, int num, int pos = 0);


void print_chars(const char* chars, int n_char);
void* send_data_to_client(void *data);
void incrase_brightness(uint8_t* image, int size, int brightness);
uint8_t saturate_cast(uint8_t num, float mul);

int main()
{
    signal(SIGINT, clear_all);

#if !TEST
    int sizeof_sockaddr_in = 0;
    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // TCP/IP, IPPROTO_TCP
    if (socket < 0)
    {
        printf("Couldc not reate socket\n");
        return 1;
    }

    if(init_server(server_socket))
    {
        printf("Init server failed\n");
        return 1;
    }
    printf("Waiting for incoming connections...\n");


    sizeof_sockaddr_in = sizeof(struct sockaddr_in);
    int socket_client = 0;
    while (1)
    {
        socket_client = accept(server_socket, (struct sockaddr *)&sockaddr_clients[n_client], (socklen_t *)&sizeof_sockaddr_in);
        if (socket_client)
        {
            thread_args[n_client].client_id = n_client;
            thread_args[n_client].socket = socket_client;
            if (pthread_create(&new_threads[n_client], NULL, connection_handler, &thread_args[n_client]) < 0)
            {
                printf("Create thread failed\n");
                continue;
            }
            n_client++;
        }
    }

#endif
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


int init_server(int server_socket)
{
    struct sockaddr_in server;
    // sockaddr_in init
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

        // Bind
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Socket bind failed\n");
        return 1;
    }
    listen(server_socket, 3);

    return 0;
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


void* send_data_to_client(void *data)
{
    int n_frame_sent = 0;
    thread_arg_t* this_ta = (thread_arg_t*)data;
    int timeout = 0;
    while (1)
    {
        if(n_frame_sent == this_ta->frame_info.total_of_frame)
        {
            break;
        }

        pthread_mutex_lock(&(this_ta->mt_lock));
        
        if(!this_ta->q_data.empty())
        {
            incrase_brightness(this_ta->q_data.front().data, this_ta->total_pixel, this_ta->brightness / 100.0);

            if(!send(this_ta->socket, this_ta->q_data.front().data, this_ta->total_pixel, 0))
            {
                return NULL;
            } 
            delete(this_ta->q_data.front().data);         // Xoa vung nho da cap phat
            this_ta->q_data.pop();                        // Xoa phan tu da xu ly
            n_frame_sent++;
        }

        pthread_mutex_unlock(&(this_ta->mt_lock));       
    }
    

}

void *connection_handler(void *ta)
{
    thread_arg_t* this_ta = (thread_arg_t*)ta;

    char* p_data = new char[16];
    printf("Connection handler socket: %d\n", this_ta->socket);
    int n_data = 0;
    int total_of_pixel = 0;
    int exit = 0;
    pthread_t send_data_thread;     // Tao thread xu ly va gui du lieu ve client

    while (1)
    {

        n_data = recv(this_ta->socket, p_data, 16, 0);
        switch (n_data)
        {
        case TOTAL_BYTE_START_SIGNAL:             // start signal
            this_ta->frame_info.height = convert_chars_to_int(p_data);
            this_ta->frame_info.width = convert_chars_to_int(p_data, BYTE_WIDTH_DATA_RECEIVE);
            this_ta->frame_info.total_of_frame = convert_chars_to_int(p_data, BYTE_NFRAME_DATA_RECEIVE);
            this_ta->cal_total_pixel();
            this_ta->brightness = p_data[BYTE_BRIHTNESS_DATA_RECEIVE];
            this_ta->subthread = &send_data_thread;
            delete(p_data);
            p_data = new char[this_ta->total_pixel];
            pthread_mutex_init(&(this_ta->mt_lock), NULL);
            pthread_create(&send_data_thread, NULL, send_data_to_client, this_ta);
            
#if DEBUG
            printf("Start signal\n");
            printf("     frame height: %d\n", this_ta->frame_info.height);
            printf("     frame width:  %d\n", this_ta->frame_info.width);
            printf("     number of frame: %d\n",this_ta->frame_info.total_of_frame);
            printf("     brightness:   %d\n", this_ta->brightness);
#endif
            break;

        case TOTAL_BYTE_STOP_SIGNAL:
            pthread_join(send_data_thread, NULL);
            exit = 1;
            break;

        default:        // Mac dinh la du lieu cua anh
            data_t image;

            image.data = (uint8_t*)p_data;

            pthread_mutex_lock(&(this_ta->mt_lock));

            this_ta->q_data.push(image);
            p_data = new char[this_ta->total_pixel];    // cap phat vung nho moi cho lan nhan du lieu sau

            pthread_mutex_unlock(&(this_ta->mt_lock));

            break;
        }

        if(exit)
            break;
        usleep(10);
    }
}

void clear_all(int sign_num)
{
    printf("Preparing to exit program.....\n");
    for(int i = 0; i < n_client; i++)
    {
        //pthread_cancel(*(thread_args[i].subthread));
        pthread_cancel(new_threads[i]);
        close(thread_args[i].socket);
    }
    exit(0);
}

void print_chars(const char* chars, int n_char)
{
    for(int i = 0; i < n_char; i ++)
    {
        printf("%c", chars[i] + 48);
    }
    printf("\n");
}