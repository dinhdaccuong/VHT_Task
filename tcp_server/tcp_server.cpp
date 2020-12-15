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

#define TEST                                0
#define DEBUG                               1



#define PORT                                8888
#define MAX_CLIENT                          10

#define TOTAL_BYTE_START_SIGNAL             13
#define BYTE_HEIGHT_DATA_RECEIVE            0
#define BYTE_WIDTH_DATA_RECEIVE             4
#define BYTE_NFRAME_DATA_RECEIVE            8
#define BYTE_BRIHTNESS_DATA_RECEIVE         12
int n_client = 0; // number of client

typedef struct
{
    int height;
    int width;
    int total_of_frame;
} frame_info_t;

typedef struct
{
    uint8_t* p_data;
    uint32_t n_data;
} data_t;

typedef struct 
{
    queue<data_t>       q_data;      // Hang doi luu lieu
    pthread_t*          subthread;   // Thread con
    pthread_mutex_t     mt_lock;     // mutex
    frame_info_t        frame_info;  // Thong tin frame
    int                 brightness;  // Do sang
    int                 client_id;
    int                 socket;
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

void* send_data_to_client(void *data)
{

}

void *connection_handler(void *ta)
{
    thread_arg_t* this_ta = (thread_arg_t*)ta;

    char data_to_receive[16];
    printf("Connection handler socket: %d\n", this_ta->socket);
    int n_data = 0;
    int exit = 0;
    while (1)
    {

        n_data = recv(this_ta->socket, data_to_receive, 16, 0);
        switch (n_data)
        {
        case TOTAL_BYTE_START_SIGNAL:             // start signal
            this_ta->frame_info.height = convert_chars_to_int(data_to_receive);
            this_ta->frame_info.width = convert_chars_to_int(data_to_receive, BYTE_WIDTH_DATA_RECEIVE);
            this_ta->frame_info.total_of_frame = convert_chars_to_int(data_to_receive, BYTE_NFRAME_DATA_RECEIVE);
            this_ta->brightness = data_to_receive[BYTE_BRIHTNESS_DATA_RECEIVE];
#if DEBUG
            printf("Start signal\n");
            printf("     frame height: %d\n", this_ta->frame_info.height);
            printf("     frame width:  %d\n", this_ta->frame_info.width);
            printf("     number of frame: %d\n",this_ta->frame_info.total_of_frame);
            printf("     brightness:   %d\n", this_ta->brightness);
#endif
            exit = 1;
            break;
        
        default:
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