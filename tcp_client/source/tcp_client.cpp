#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT        8888
#define SERVER_IP_ADDRESS "127.0.0.1"

int connect_to_server(int socket,const char* server_ip);

int main()
{
    int socket_to_connect = 0;

    socket_to_connect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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



    printf("END\n");
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