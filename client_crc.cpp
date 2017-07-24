#include<iostream>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/types.h>
#include<netdb.h>
using namespace std;

#define PORT 8000

int main(int argc, char *argv[]){
    int clientSocket, check;
    struct sockaddr_in server;
    struct hostent *hp;
    char msg[1000]={' '};
    char msgRecv[1000]={' '};

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket<0){
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

    check = connect(clientSocket, (struct sockaddr *) &server, sizeof(server));
    if(check<0){
        cout<<"Error connecting\n\n";
        return 0;
    }
    cout<<"Connection estd\n\n";

    msg[0]='1'; msg[1]='1'; msg[2]='0'; msg[3]='1';
    msg[4]='0'; msg[5]='0'; msg[6]='0'; msg[7]='1';

    check = send(clientSocket, msg, sizeof(msg), 0);
    if(check<0){
        cout<<"Error sending the message\n\n";
        return 0;
    }
    cout<<"Message has been sent\n";

    check = recv(clientSocket, msgRecv, sizeof(msgRecv), 0);
    if(check<0){
        cout<<"Error reading the message\n\n";
        return 0;
    }
    cout<<"Message received is : " <<msgRecv <<"\n\n";

    return 0;
}
