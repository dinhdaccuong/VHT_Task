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


#define TEST 0

#define PORT 8888
#define BUFFER_TO_RECEIVE_SIZE 1024 
#define MAX_CLIENT 10


int n_client = 0; // number of client

pthread_t new_threads[MAX_CLIENT]; 
pthread_t main_thread; 
int new_sockets[MAX_CLIENT];
sockaddr_in sockaddr_clients[MAX_CLIENT];

void *connection_handler(void *socket_desc);
void clear_all(int sign_num);
int init_server(int socket);

int convert_chars_to_int(const char* chars, int pos = 0);
void convert_int_to_chars(char* chars, int num, int pos = 0);


void print_chars(const char* chars, int n_char);

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

    while (1)
    {
        new_sockets[n_client] = accept(server_socket, (struct sockaddr *)&sockaddr_clients[n_client], (socklen_t *)&sizeof_sockaddr_in);
        if (new_sockets[n_client] > 0)
        {
            if (pthread_create(&new_threads[n_client], NULL, connection_handler, &new_sockets[n_client]) < 0)
            {
                printf("Create thread failed\n");
                continue;
            }
            n_client++;
        }
    }

#endif
    // char p[4];
    // int num = 320;
    // convert_int_to_chars(p, num, 0);
    // int num2 = convert_chars_to_int(p, 0);

    // printf("%d\n", num2);
    // printf("END\n");

    return 0;
}

void convert_int_to_chars(char* chars, int num, int pos)
{
    memset(chars, 0, sizeof(int));
    for(int i = sizeof(int) - 1; i >= 0; i--)
    {
        chars[i + pos] = num;
        num = num >> 8;
    }
}

int convert_chars_to_int(const char* chars, int pos)
{
    int num = 0;
    for(int i = 0; i < 4; i++)
    {
        num |= chars[i + pos];
        if(i < 3)
            num = num << 8;
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
void *connection_handler(void *socket_desc)
{
    int socket = *(int *)socket_desc;
    char data_to_receicve[1024];
    char data_to_send[1024];
    strcpy(data_to_send, "Phan Thu Hang");
    printf("Connection handler socket: %d\n", socket);
    int n_data = 0;
    int exit = 0;
    while (1)
    {

        n_data = recv(socket, data_to_receicve, 1024, 0);
        switch (n_data)
        {
        case 9:             // start signal
            printf("Start signal\n");
            // printf("     frame height: %d\n", convert_chars_to_int(data_to_receicve));
            // printf("     frame width:  %d\n", convert_chars_to_int(data_to_receicve, 4));
            // printf("     brightness:   %d\n", (int)data_to_receicve[9]);
            print_chars(data_to_receicve, 9);

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
        pthread_cancel(new_threads[i]);
        close(new_sockets[i]);
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