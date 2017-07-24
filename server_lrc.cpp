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

bool lrc(char *msg);

int main()
{
  cout<<"\n";
  char ack[3] = {'A','C','K'};
  char nack[4]= {'N','A','C','K'};
  int counter = 0;
  struct sockaddr_in server, client1;
  char msg[1000] = {' '};
 
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
    if(msg[6]=='0')msg[6]='1';
    else msg[6]='0';
  int snd;
  // LRC Detection
  if(lrc(msg)){
    snd = send(client1Socket, ack, sizeof(ack), 0);
    if(snd<0){
      cout<<"Error sending the message\n\n";
      closeAll();
      return 0;
    }    
  }
  else {
    snd = send(client1Socket, nack, sizeof(nack), 0);
    if(snd<0){
      cout<<"Error sending the message\n\n";
      closeAll();
      return 0;
    }
  }
  closeAll();
return 0;
}

bool lrc(char *msg){
  char bit1,bit2,bit3,bit4;
  int ctr=0;
  for(int i=0;i<strlen(msg);i=i+4){
    if(msg[i]=='1')
      ctr++;	
  }
  if(ctr%2==0)
    bit1='0';
  else bit1='1';
  
  ctr=0;
  for(int i=1;i<strlen(msg);i=i+4){
    if(msg[i]=='1')
      ctr++;	
  }
  if(ctr%2==0)
    bit2='0';
  else bit2='1';
  
  ctr=0;
  for(int i=2;i<strlen(msg);i=i+4){
    if(msg[i]=='1')
      ctr++;	
  }
  if(ctr%2==0)
    bit3='0';
  else bit3='1';
  
  ctr=0;
  for(int i=3;i<strlen(msg);i=i+4){
    if(msg[i]=='1')
      ctr++;	
  }
  if(ctr%2==0)
    bit4='0';
  else bit4='1';
  
  //  cout<<bit1 <<bit2 <<bit3 <<bit4 <<endl;
  
  if(bit1=='0' && bit2=='0' && bit3=='0' && bit4=='0')
    return true;
  return false;
}
