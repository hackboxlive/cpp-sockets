#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main()	{
	int client, server;
	int port_num = 1500;
	bool is_exit = false;
	int buffer_size = 1024;
	char buffer[buffer_size];

	struct sockaddr_in server_addr;
	socklen_t size;

	//initialize socket

	client = socket(AF_INET, SOCK_STREAM, 0);

	if(client < 0)	{
		cout << "Error while establishing connection\n";
		exit(1);
	}
	
	cout << "Server socket connection established\n";

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(port_num);

	//binding socket

	if(bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)	{
		cout << "Error binding connection, the socket has already been established...\n";
		exit(1);
	}

	size = sizeof(server_addr);
	cout << "Looking for clients...\n";

	//listening socket
	listen(client, 1);

	//accept client
	int client_count = 1;
	server = accept(client, (struct sockaddr*)&server_addr, &size);

	//check if it's valid
	if(server < 0)	{
		cout << "Error on accepting...\n";
	}

	while(server > 0)	{
		strcpy(buffer, "Server connected...\n");
		send(server, buffer, buffer_size, 0);
		cout << "Connected with client #" << client_count << " ,evertything seems fine...\n";
		cout << "Enter ~ to end the connection\n";

		cout << "Client:\n";
		do	{
			recv(server, buffer, buffer_size, 0);
			cout << buffer << "\n";
			if(*buffer == '~')	{
				*buffer = '*';
				is_exit = true;
			}
		}while(*buffer != '*');

		do	{
			cout << "Server:\n";
			do	{
				cin >> buffer;
				send(server, buffer, buffer_size, 0);
				if(*buffer == '~')	{
					send(server, buffer, buffer_size, 0);
					*buffer = '*';
					is_exit = true;
				}
			}while(*buffer != '*');

			cout << "Client:\n";
			do	{
				recv(server, buffer, buffer_size, 0);
				cout << buffer << "\n";
				if(*buffer == '~')	{
					*buffer = '*';
					is_exit = true;
				}
			}while(*buffer != '*');
		}while(!is_exit);

		//close connection
		//inet_ntoa converts packet data to IP, which was taken from client
		cout << "Connection terminated with IP " << inet_ntoa(server_addr.sin_addr);
		close(server);
		cout << "Sayonara..\n";
		is_exit = false;
		exit(1);
	}

	close(client);
	return 0;
}