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
#include <server_utils.h>
#include <server_define.h>
using namespace std;

#define TEST                                0
#define DEBUG                               1

int n_client = 0; // number of client



pthread_t new_threads[MAX_CLIENT];  
sockaddr_in sockaddr_clients[MAX_CLIENT];
thread_arg_t thread_args[MAX_CLIENT];

void *connection_handler(void *socket_desc);
void clear_all(int sign_num);
int init_server(int socket);
int server_socket = 0;
int main()
{
    signal(SIGINT, clear_all);

#if !TEST
    int sizeof_sockaddr_in = 0;
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // TCP/IP, IPPROTO_TCP
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


void *connection_handler(void *ta)
{
    thread_arg_t* this_ta = (thread_arg_t*)ta;

    uint8_t* data_to_recv = new uint8_t[13];
    printf("Connection handler socket: %d\n", this_ta->socket);
    
    int n_data_recv = 0;
    int n_data_sent = 0;
    uint32_t data_size = 0;
    int frame_count = 0;

    n_data_recv = recv(this_ta->socket, data_to_recv, 13, 0);

    if(n_data_recv != 13)
    {
        return NULL;
    }

    this_ta->video_info.frame_info.rows = convert_chars_to_int(data_to_recv);
    this_ta->video_info.frame_info.cols = convert_chars_to_int(data_to_recv, BYTE_WIDTH_DATA_RECEIVE);
    this_ta->video_info.total_of_frames = convert_chars_to_int(data_to_recv, BYTE_NFRAME_DATA_RECEIVE);
    this_ta->video_info.frame_info.channels = 3;
    this_ta->video_info.frame_info.cal_total_of_pixels();
    this_ta->brightness = data_to_recv[12];
    delete(data_to_recv);
    data_size = this_ta->video_info.frame_info.total_of_pixels;
    data_to_recv = new uint8_t[data_size];
    
#if 1
            printf("Start signal\n");
            printf("     frame height: %d\n", this_ta->video_info.frame_info.rows);
            printf("     frame width:  %d\n", this_ta->video_info.frame_info.cols);
            printf("     number of frame: %d\n",this_ta->video_info.total_of_frames);
            printf("     brightness:   %d\n", this_ta->brightness);
#endif
    while(1)
    {
        memset(data_to_recv, 0, data_size);
        n_data_recv = receive_all_data(this_ta->socket, data_to_recv, data_size);
        //show_image_data(data_to_recv, 20);
        //printf("n_data_recv = %d\n", n_data_recv);
        if(n_data_recv != data_size)
        {
            printf("Receive error\n");
            return NULL;
        }

        //printf("SERVER: Receive frame %d\n\n", frame_count);
        
        incrase_brightness(data_to_recv, data_size, this_ta->brightness);

        n_data_sent = send_all_data(this_ta->socket, data_to_recv, data_size);
        if(n_data_sent < 0)
        {
            printf("Sent error\n");
            return NULL;            
        }
        //show_image_data(data_to_recv, 20);
        //printf("n_data_sent = %d\n", n_data_sent);
       // printf("SERVER: Sent frame %d\n\n", frame_count);
        frame_count ++;

        if(frame_count == this_ta->video_info.total_of_frames)
        {    
            break;
        }
            
    }

    printf("Complete client %d\n", this_ta -> client_id);
    delete(data_to_recv);
    close(this_ta->socket);
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
    close(server_socket);
    exit(0);
}

