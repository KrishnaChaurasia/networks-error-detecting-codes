#include<iostream>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/types.h>
#include<netdb.h>
using namespace std;

#define PORT 5555

int main(int argc, char *argv[])
{
	int clientskt, check;
	struct sockaddr_in server;
	struct hostent *hp;
	char msg[1000]={' '};
	char msgrcv[1000]={' '};

	clientskt = socket(AF_INET, SOCK_STREAM, 0);
	if(clientskt<0){
		cout<<"Error creating the socket\n";
		return 0;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);

	hp = gethostbyname(argv[1]);
	if(hp<0){
		cout<<"Server address/ip missing\n\n";
		return 0;
	}

	bcopy ((char *)hp->h_addr, (char *)&server.sin_addr.s_addr, hp->h_length);

	check = connect(clientskt, (struct sockaddr *) &server, sizeof(server));
	if(check<0){
		cout<<"Error connecting\n\n";
		return 0;
	}
	cout<<"Connection estd\n\n";

	msg[0]='1'; msg[1]='1'; msg[2]='0'; msg[3]='1';
	msg[4]='0'; msg[5]='0'; msg[6]='0'; msg[7]='1';
	msg[8]='1'; msg[9]='0'; msg[10]='1'; msg[11]='0';
	//msg[8]='0'; msg[9]='0'; msg[10]='0'; msg[11]='1';
	//msg[12]='0'; msg[13]='0'; msg[14]='0'; msg[15]='0';
	//msg[16]='0'; msg[17]='1'; msg[18]='0'; msg[19]='0';

	check = send(clientskt, msg, sizeof(msg), 0);
	if(check<0){
		cout<<"Error sending the message\n\n";
		return 0;
	}
	cout<<"Message has been sent\n";

	check = recv(clientskt, msgrcv, sizeof(msgrcv), 0);
	if(check<0){
		cout<<"Error reading the message\n\n";
		return 0;
	}
	cout<<"Message received is : " <<msgrcv <<"\n";

	return 0;
}
