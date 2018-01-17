#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

int main()	{
	int client, server;
	int port_num = 1500;
	bool is_exit = false;
	int buffer_size = 1024;
	char buffer[buffer_size];
	char *ip = "127.0.0.1";

	struct sockaddr_in server_addr;

	client = socket(AF_INET, SOCK_STREAM, 0);

	if(client < 0)	{
		cout << "Error while establishing connection\n";
		exit(1);
	}

	cout << "Socket client has been created..\n";

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_num);

	if(connect(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0)	{
		cout << "Connection to the server port number : " << port_num << endl;
	}

	cout << "Awaiting confirmation from the server...\n";
	recv(client, buffer, buffer_size, 0);
	cout << "Connection established...\n";
	cout << "Enter ~ to end connection...\n";
	do	{
		cout << "Client:\n";
		do	{
			cin >> buffer;
			send(client, buffer, buffer_size, 0);
			if(*buffer == '~')	{
				send(client, buffer, buffer_size, 0);
				*buffer = '*';
				is_exit = true;
			}
		}while(*buffer != '*');

		cout << "Server:\n";
		do	{
			recv(client, buffer, buffer_size, 0);
			cout << buffer << "\n";
			if(*buffer == '~')	{
				*buffer = '*';
				is_exit = true;
			}
		}while(*buffer != '*');
		cout << endl;
	}while(!is_exit);

	//close
	cout << "Connection terminated. Sayonara!..\n";
	close(client);
	return 0;
}