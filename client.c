#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>



int main (int argc, char **argv){

	//init part
	int sock = socket(AF_INET6,SOCK_DGRAM,0);
	if(sock == -1 ){printf("Erreur lors de la création des sockets.\n");}

	struct sockaddr_in6 peer_addr;                      // allocate the address on the stack
	memset(&peer_addr, 0, sizeof(peer_addr));           // fill the address with 0-bytes to avoid garbage-values
	peer_addr.sin6_family = AF_INET6;                   // indicate that the address is an IPv6 address
	peer_addr.sin6_port = htons(55555);                 // indicate that the programm is running on port 55555
	inet_pton(AF_INET6, "::1", &peer_addr.sin6_addr);   // indicate that the program is running on the computer identified by the ::1 IPv6 address


	//sending part
	ssize_t sent=0;
	
	if(argc>1 && *argv[1]==*"number_mode"){
		printf("Nombre mode\n");
		
		
		uint32_t number = 10;
		int buff_size= 4;
		uint32_t buff[4];
		for(int i =0;i<buff_size;i++){
			buff[i]=htonl(number+i);
		}
		//printf("nombre envoyé sous forme de host long: %d\n", n);
		printf("nombre envoyé sous forme de network long: %d\n", number);

		sent = sendto(sock,&buff,sizeof(uint32_t)*buff_size,0,(const struct sockaddr *)&peer_addr, sizeof(peer_addr));
	}
	else if(argc>1){
		printf("%s\n",argv[1]);
		sent = sendto(sock,argv[1],strlen(argv[1]),0,(const struct sockaddr *)&peer_addr, sizeof(peer_addr));
	}
	else{
		sent = sendto(sock,"hello",strlen("hello"),0,(const struct sockaddr *)&peer_addr, sizeof(peer_addr));
		
	}
	
	if(sent == -1){printf("Erreur lors de l'envoi.\n");}
	//else {printf("Envoi réussi.\n");}
	printf("Clien closed.\n");


return 0;}