/*
    C socket server example
*/
#include<stdlib.h>
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#define SIZEOF_BUFF 100

const char *pLight1gpio = "/sys/class/gpio/gpio60/value\0";
const char *pLight2gpio = "/sys/class/gpio/gpio48/value\0";
char buff[SIZEOF_BUFF];

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[200];
    char format[] = "LIGHT[%04d][%04d]SWITCH\0";

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 5005 );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
	for(;;)
	{
    	//accept connection from an incoming client
    	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    	if (client_sock < 0)
    	{
        	perror("accept failed");
        	return 1;
    	}
    	puts("Connection accepted");

    	//Receive a message from client
    	while( (read_size = recv(client_sock , client_message , 200 , 0)) > 0 )
    	{
        	//Send the message back to client
        	char format[] = "LIGHT[%04d][%04d]SWITCH\0";
			unsigned int light;
			unsigned int status;
			const char *pGpio = NULL;

//			printf("Received %s\n", client_message);
			sscanf(client_message, format, &light, &status);
			printf("Read: %d, %d\n", light, status);
			switch(light)
			{
				case 1:
					pGpio = pLight1gpio;
					break;
				case 2:
					pGpio = pLight2gpio;
					break;
				default:
					break;
			}

			if(pGpio != NULL)
			{
				if(status != 0){
					status = 1;
				}
				memset(buff, 0, SIZEOF_BUFF);
				snprintf(buff, SIZEOF_BUFF, "echo %d > %s \n", status, pGpio);
				printf("%s\n", buff);
				system(buff);
			}
    	}

    	if(read_size == 0)
    	{
        	puts("Client disconnected");
        	fflush(stdout);
    	}
    	else if(read_size == -1)
    	{
        	perror("recv failed");
    	}
	}
    return 0;
}
