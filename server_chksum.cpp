#include<iostream>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
using namespace std;

#define PORT 5555

int serverskt, check;
int clientskt;

void closeAll(){
	close(serverskt);
	close(clientskt);
}

bool checksum(char *msg);

int main(){
	int counter = 0;
	struct sockaddr_in server, client;
	char msg[1000] = {' '};
	char ack[3] = {'A','C','K'};
	char nack[4]= {'N','A','C','K'};

	serverskt = socket(AF_INET, SOCK_STREAM, 0);
	if(serverskt<0){
		cout<<"Error creating socket\n\n";
		closeAll();
		return 0;
	}
	cout<<"Socket Created\n";

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	check = bind(serverskt, (struct sockaddr *) &server, sizeof(server));
	if(check<0){
		cout<<"Error binding the socket\n\n";
		closeAll();
		return 0;
	}
	cout<<"Binding successful\n";

	cout<<"Listening..............\n" <<endl;
	check = listen(serverskt, 5);
	if(check<0){
		cout<<"Error listening\n\n";
		closeAll();
		return 0;
	}

	socklen_t len = sizeof(client);
	clientskt = accept(serverskt, (struct sockaddr *) &client, &len);
	if(clientskt<0){
		cout<<"Error accepting the request\n\n";
		closeAll();
		return 0;
	}	

	check = recv(clientskt, msg, sizeof(msg), 0);
	if(check<0){
		cout<<"Error receiving the message\n\n";
		closeAll();
		return 0;
	}
	cout<<"Message has been received\n";

	// Checksum Detection
	if(checksum(msg)){
		check = send(clientskt, ack, sizeof(ack), 0);
		if(check<0){
			cout<<"Error sending the message\n\n";
			closeAll();
			return 0;
		}    
	} else {
		check = send(clientskt, nack, sizeof(nack), 0);
		if(check<0){
			cout<<"Error sending the message\n\n";
			closeAll();
			return 0;
		}
	}

	closeAll();
	return 0;
}

bool checksum(char *msg){
	char a1,a2,a3,a4, b1,b2,b3,b4, c1,c2,c3,c4;
	int ct=0;
	int i=strlen(msg)-1; char s1[i], s2[i], s3[i], s4[i]; char carry='0'; int ctr=0;
	while(i>=0){
		if(ctr==0){
			a1=msg[i--]; 	a2=msg[i--]; 	a3=msg[i--]; 	a4=msg[i--];
			b1=msg[i--];	b2=msg[i--];	b3=msg[i--];	b4=msg[i--];

			s1[ct]=a1^b1^carry;
			carry = a1&b1 | b1&carry | carry&a1;

			s2[ct]=a2^b2^carry;
			carry = a2&b2 | b2&carry | carry&a2;

			s3[ct]=a3^b3^carry;
			carry = a3&b3 | b3&carry | carry&a3;

			s4[ct]=a4^b4^carry;
			carry = a4&b4 | b4&carry | carry&a4;
			ctr++; ct++;
			cout<<carry;
		} else{
			a1=msg[i--];	a2=msg[i--];	a3=msg[i--];	a4=msg[i--];
			b1=s1[ct-1];	b2=s2[ct-1];	b3=s3[ct-1];	b4=s4[ct-1];

			s1[ct]=a1^b1^carry;
			carry = a1&b1 | b1&carry | carry&a1;

			s2[ct]=a2^b2^carry;
			carry = a2&b2 | b2&carry | carry&a2;

			s3[ct]=a3^b3^carry;
			carry = a3&b3 | b3&carry | carry&a3;

			s4[ct]=a4^b4^carry;
			carry = a4&b4 | b4&carry | carry&a4;
			ct++;				
		}

		if(s1[ct-1]=='1' && s2[ct-1]=='1' && s3[ct-1]=='1' && s4[ct-1]=='1' && carry=='0')
			return true;
	}
	return false;
}
