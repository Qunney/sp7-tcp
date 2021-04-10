#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <netinet/in.h>

using namespace std;

#define DEFAULT_PORT 5555
#define BUFFER_SIZE 1024
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'

bool find_stop_symbol(const char* msg);

int main(int argc, char const* argv[]) {
	int server, client, port;
	struct sockaddr_in server_address;
	argc >= 2 ? port = atoi(argv[1]) : port = DEFAULT_PORT;
	client = socket(AF_INET, SOCK_STREAM, 0);
	if(client < 0) {
		cout << "ServerError: socket establishing error!";
		exit(0);
	}
	cout <<  "ServerInfo: Socket for server was successfully created\n";
	server_address.sin_port=htons(port);
	server_address.sin_family= AF_INET;
	server_address.sin_addr.s_addr = htons(INADDR_ANY);

	int ret = bind(client,
				   (struct sockaddr *)&server_address,
				   sizeof(server_address));	
	if (ret < 0) {
		cout << "ServerError: can't binding.\n";
		return -1;
	}	
	socklen_t size = sizeof(server_address);
	cout << "ServerInfo: Listening DEFAULT_PORT.\n";
	listen(client, 1);

	server = accept(client, 
					(struct sockaddr *)&server_address, 
					&size);
	
	if(server < 0 ) {
		cout << "ServerError: Can't accepting client";	
	}

	char buffer[BUFFER_SIZE];
	bool isExit = false;
	while(server > 0) {
		strcpy(buffer, "=> Server Connected!\n");
		send(server, buffer, BUFFER_SIZE, 0);
		cout << "Connected to the client!" 
		     << endl
		     << "Enter " << CLIENT_CLOSE_CONNECTION_SYMBOL << "for close connection\n";
		cout << "client: ";
		recv(server, buffer, BUFFER_SIZE, 0);
		cout << buffer << endl;
		if(find_stop_symbol(buffer)){
			isExit=true;
		}
		while(!isExit) {
			cout << "server:";
			cin.getline(buffer, BUFFER_SIZE);
			send(server, buffer, BUFFER_SIZE, 0);
			if(find_stop_symbol(buffer)){
				break;
			}

			cout << "client: ";
			recv(server, buffer, BUFFER_SIZE, 0);
			cout << buffer << endl;
			if (find_stop_symbol(buffer)){
				break;
			}
		
		}
		cout << "\n Good bye !" << endl;
		isExit = false;
		exit(1);
	}
	return 0;
}

bool find_stop_symbol(const char* msg) {
	for(int i=0; i < strlen(msg); ++i){
		if(msg[i] == CLIENT_CLOSE_CONNECTION_SYMBOL){
			return true;
		}
	}
	return false;
}