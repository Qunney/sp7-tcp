#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <netinet/in.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_CLOSE_CONNECTION_SYMBOL '#'
#define DEFAULT_PORT 5555
#define BUFFER_SIZE 1024

using namespace std;


bool find_stop_symbol(const char* msg);

int main(int argc, char const* argv[]) {
	
	int client, servlen, port;
	
	struct sockaddr_in server_address;

	string host = SERVER_IP;
	
	argc >= 3 ? port = atoi(argv[2]) : port = DEFAULT_PORT;

	client = socket(AF_INET, SOCK_STREAM, 0);
	if(client < 0) {
		cout << "ClientError: establishing socket error!";
		exit(0);
	}

	server_address.sin_port=htons(port);
	server_address.sin_family= AF_INET;
	inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

	cout << "\nClient socket created\n";

	int ret = connect(client,
				     (struct sockaddr *)&server_address,
				      sizeof(server_address));
	if (ret == 0) {
		cout << "ClientInfo: try connect to server \n"
			 << inet_ntoa(server_address.sin_addr)
			 << "port :" << port << endl;
	}		
	cout << "Waiting confiation from server...\n";
	char buffer[BUFFER_SIZE];
	recv(client, buffer, BUFFER_SIZE, 0);
	cout << "Connected to the client!" 
	     << endl
	     << "Enter " << SERVER_CLOSE_CONNECTION_SYMBOL << " for close connection\n";
	while(true) {
		cout << "client:";
		cin.getline(buffer, BUFFER_SIZE);
		send(client, buffer, BUFFER_SIZE, 0);
		if(find_stop_symbol(buffer)){
			break;
		}

		cout << "server: ";
		recv(client, buffer, BUFFER_SIZE, 0);
		cout << buffer << endl;
		if (find_stop_symbol(buffer)){
			break;
		}
	
	}
	cout << "\n Good bye!" << endl;
	close(client);
	return 0;
}

bool find_stop_symbol(const char* msg) {
	for(int i=0; i < strlen(msg); ++i){
		if(msg[i] == SERVER_CLOSE_CONNECTION_SYMBOL){
			return true;
		}
	}
	return false;
}