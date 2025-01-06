#include<sys/types.h> 

#include<netinet/in.h> 

#include<netdb.h> 

int main() 

{ 

//variable to store the socket_id. 

int serversocket,port; 

//variable to store the network addresses. 

struct sockaddr_in serveraddr,clientaddr; 

//variable to store the address length. 

socklen_t len; 

char message[50]; 

//socket creation. 

serversocket=socket(AF_INET,SOCK_DGRAM,0); 

//steps involved in defining the serveraddress. 

bzero((char*)&serveraddr,sizeof(serveraddr)); 

serveraddr.sin_family=AF_INET; 

printf("Enter the port number "); 

scanf("%d",&port); 

serveraddr.sin_port=htons(port); 

serveraddr.sin_addr.s_addr=INADDR_ANY;
