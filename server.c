#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

int bind_and_receive_from_peer(int sock, struct sockaddr *local_addr, socklen_t local_addr_len);
int init_buff(uint32_t *buff, int buff_len);

int main (int argc, char **argv){

	//init part
	int sock = socket(AF_INET6,SOCK_DGRAM,0);
	if(sock == -1 ){printf("Erreur lors de la création des sockets.\n");}

	struct sockaddr_in6 peer_addr;                      // allocate the address on the stack
	memset(&peer_addr, 0, sizeof(peer_addr));           // fill the address with 0-bytes to avoid garbage-values
	peer_addr.sin6_family = AF_INET6;                   // indicate that the address is an IPv6 address
	peer_addr.sin6_port = htons(55555);                 // indicate that the programm is running on port 55555
	inet_pton(AF_INET6, "::1", &peer_addr.sin6_addr);   // indicate that the program is running on the computer identified by the ::1 IPv6 address

	//receving part
	ssize_t buff_maxsize = 16;
	
	socklen_t peer_addr_len = sizeof(peer_addr);

	ssize_t bound = bind(sock,(const struct sockaddr*)&peer_addr,peer_addr_len);
	if(bound == -1){printf("Erreur lors de la liaison\n");}
	


	//bind_and_receive_from_peer(sock,(struct sockaddr*)&peer_addr,peer_addr_len);
	while(1){
		printf("Listenning..\n");
		char buff[buff_maxsize];
		uint32_t buffNumber[buff_maxsize];

		//init buffer
		/*for(int i=0;i<buff_maxsize;i++){
		buff[i]=(uint32_t)0;
		}*/
		init_buff(buffNumber,buff_maxsize);
		//ssize_t reception = recv(sock,buff,(size_t)buff_maxsize,0); // réception string
		ssize_t reception = recv(sock,buffNumber,(size_t) buff_maxsize,0);
		//ssize_t reception = recvfrom(sock2,buff,buff_maxsize,0,(struct sockaddr*)&peer_addr,&peer_addr_len);
		if(reception == -1){printf("Erreur lors de la réception des données.\n");}

		
		uint32_t result;
		for(int i=0;i<buff_maxsize;i++){
			result = ntohl(buffNumber[i]);
			printf("%d\n", result);
		}
		
		

	}
	

	printf("Server closed.\n");


return 0;}

int init_buff(uint32_t *buff, int buff_len){
	for(int i=0;i<buff_len;i++){
		buff[i]=(uint32_t)0;
	}

	return 0;
}



#define MAX_MESSAGE_SIZE 2500
int bind_and_receive_from_peer(int sock, struct sockaddr *local_addr, socklen_t local_addr_len) {
    int err = bind(sock, local_addr, local_addr_len);  // assign our address to the socket
    if (err == -1) {
        printf("could not bind on the socket, error: %s\n", strerror(errno));
        return errno;
    }
    char buffer[MAX_MESSAGE_SIZE];  // allocate a buffer of MAX_MESSAGE_SIZE bytes on the stack
    ssize_t n_received = recv(sock, buffer, MAX_MESSAGE_SIZE, 0);   // equivalent to do: read(sock, buffer, MAX_MESSAGE_SIZE);
    if (n_received == -1) {
        printf("could not receive the message, error: %s\n", strerror(errno));
        return errno;
    }

    // let's print what we received !
    printf("received %ld bytes:\n", n_received);
    for (int i = 0 ; i < n_received ; i++) {
        printf("0x%hhx ('%c') ", buffer[i], buffer[i]);
    }
    printf("\n");
    return 0;
}