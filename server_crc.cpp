#include<iostream>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
using namespace std;

#define PORT 8000

int serverSocket, check;
int client1Socket;

void closeAll(){
close(serverSocket);
close(client1Socket);
}

bool crc(char *msg);

int main()
{
cout<<"\n";

int counter = 0;
struct sockaddr_in server, client1;
char msg[1000] = {' '};
char ack[3] = {'A','C','K'};
char nack[4]= {'N','A','C','K'};

serverSocket = socket(AF_INET, SOCK_STREAM, 0);
if(serverSocket<0){
cout<<"Error creating socket\n\n";
closeAll();
return 0;
}
cout<<"Socket Created\n";

server.sin_family = AF_INET;
server.sin_addr.s_addr = INADDR_ANY;
server.sin_port = htons(PORT);

check = bind(serverSocket, (struct sockaddr *) &server, sizeof(server));
if(check<0){
cout<<"Error binding the socket\n\n";
closeAll();
return 0;
}
cout<<"Binding successful\n";

cout<<"Listening..............\n" <<endl;
check = listen(serverSocket, 5);
if(check<0){
cout<<"Error listening\n\n";
closeAll();
return 0;
}

socklen_t len = sizeof(client1);
client1Socket = accept(serverSocket, (struct sockaddr *) &client1, &len);
if(client1Socket<0){
    cout<<"Error accepting the request\n\n";
    closeAll();
return 0;
}	

check = recv(client1Socket, msg, sizeof(msg), 0);
if(check<0){
cout<<"Error receiving the message\n\n";
closeAll();
return 0;
}
cout<<"Message has been received\n";

// CRC Detection
cout<<"Message received is : "<<msg <<endl <<endl; 
//Corrupt 
msg[5]='0';
if(crc(msg)){
	check = send(client1Socket, ack, sizeof(ack), 0);
	if(check<0){
	    cout<<"Error sending the message\n\n";
	    closeAll();
	    return 0;
	}    
}
else {
	check = send(client1Socket, nack, sizeof(nack), 0);
	if(check<0){
	    cout<<"Error sending the message\n\n";
	    closeAll();
	    return 0;
	}
}

closeAll();
return 0;
}


bool crc(char *message){
    char *msg1=message;

    int ctr=0; int i=0;
    char a1, a2, a3;    

    char poly[3];
    poly[0]='1'; poly[1]='1'; poly[2]='1';
	
    int len=strlen(message);
    while(i+3<=len){
	int one=0; ctr=0;
	a1=msg1[i]; a2=msg1[i+1]; a3=msg1[i+2]; 
	
//	cout<<msg1 <<endl;	
	if(msg1[i]==poly[0])
	msg1[i]='0';
	else msg1[i]='1';

	if(msg1[i+1]==poly[1])
	msg1[i+1]='0';
	else msg1[i+1]='1';

	if(msg1[i+2]==poly[2])
	msg1[i+2]='0';
	else msg1[i+2]='1';
	
	if (a1==poly[0]) ctr=1;             else return false;
	if (a2==poly[1]) ctr=2;             else one=1;
	if (a3==poly[2] && one==0) ctr=3;   else one=1;
    i=i+ctr;
//	cout<<msg1;
    }
    int ct=0;
    for(int i=0; i<strlen(msg1); i++){
	if(msg1[i]=='0'){
	    ct++;
	}
    }	
    if(ct==strlen(msg1))
    return true;
return false;
}
