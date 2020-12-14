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
int main()
{
    int socket_desc = 0;
    struct sockaddr_in server;

    socket_desc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // TCP/IP, IPPROTO_TCP
    if(socket_desc == -1)
    {
        printf("Could not create socket\n");
    }

    server.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if(connect(socket_desc, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        printf("Connection error\n");
        close(socket_desc);
        return 1;
    }
    printf("Connected\n");
    char data_to_send[1024];
    strcpy(data_to_send, "Dinh Dac Cuong");

    printf("Send data to server Y/n: \n");
    char c = 'y';
    
    scanf("%c", &c);
    if(c == 'y' || c == 'Y')
    {
        if(send(socket_desc, data_to_send, sizeof(data_to_send), 0) < 0)
        {
            printf("Send failed");
            return 1;
        }

        printf("Data was sent successfully\n");
    }
    
    strcpy(data_to_send, "Phan Thu Hang");
    printf("Send data to server Y/n: \n");


    if(c == 'y' || c == 'Y')
    {
        if(send(socket_desc, data_to_send, sizeof(data_to_send), 0) < 0)
        {
            printf("Send failed");
            return 1;
        }

        printf("Data was sent successfully\n");
    }


    printf("END\n");
    return 0;
}