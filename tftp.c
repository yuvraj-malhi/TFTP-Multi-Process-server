#include<time.h>
#include<stdio.h>
#include<error.h>
#include<errno.h>
#include<netdb.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>

//#include <sys/errno.h>
//#include <sys/types.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <stdint.h>

#include<dirent.h>
#define directory ".."

#define NAME_LEN 100
#define MODE_LEN 100
#define DATA_LEN 512

#define L_PORT 69
#define MSG_LEN 100
int C_PORT;

typedef struct {
	unsigned short int opcode;
	char filename[NAME_LEN];
	char zero_0;
	char mode[MODE_LEN];
	char zero_1;
} Request;

typedef struct {
	unsigned short int opcode;
	unsigned short int block;
	char data[DATA_LEN];
} Data;

typedef struct {
	unsigned short int opcode;
	unsigned short int block;
} Ack;

typedef struct {
	unsigned short int opcode;
	unsigned short int ecode;	// error code
	char msg[MSG_LEN];	
} Error;

void datahandler(struct sockaddr_in clientaddress, char* filename, char* mode, int num);

void main(int argc, char ** argv)
{
	struct sockaddr_in localbind, clientaddress;
//	struct sockaddr_in localbind;
//	struct sockaddr clientaddress;
	int run=1;


	printf("COMMAND: Initialising server...\n");
	if(argc<2) 
	{ 
		printf("COMMAND: Error: Please enter port number as CLI. Exiting....\n"); 
		exit(0); 
	}


	if(argc>2) 
	{ 
		printf("COMMAND: Error: Invalid arguements specified. Exiting....\n"); 
		exit(0); 
	}

	printf("COMMAND: Dedicating listening port at: \t\t%d\n", L_PORT);

	
	int L_FD = socket(AF_INET, SOCK_DGRAM, 0);
	if(L_FD==-1) 
	{ 
		printf("LISTEN: Error initiaising listen port. Exiting....\n"); 
		exit(0); 
	}

	localbind.sin_family       = AF_INET;
	localbind.sin_addr.s_addr  = htonl(INADDR_ANY);
	localbind.sin_port     = htons(L_PORT);
	if (bind(L_FD, (struct sockaddr *) &localbind, sizeof(localbind)) == -1)
	{ 
		printf("LISTEN: Error binding listen port. Exiting....\n"); 
		perror("perror output - ");
		exit(0); 
	}
	
	
	printf("COMMAND: Successfully initialised listen port\n");

	char * tmp;
	int address_len, numbytes;
	Request * REQUEST;

	char file[NAME_LEN], mode[MODE_LEN]; short signed int opcode;

	while(1)
	{


		tmp = (char *)malloc( sizeof(Request) );
		memset(tmp, 0, sizeof(Request));
		address_len = sizeof(struct sockaddr);
		
		numbytes = recvfrom(L_FD, tmp, sizeof(Request), 0, (struct sockaddr *)&clientaddress, &address_len);
//		numbytes = recvfrom(L_FD, tmp, sizeof(Request), 0, (struct sockaddr_in *)&clientaddress, &address_len);
		if (numbytes < 0) 
		{
			free(tmp);
			continue;
		}


		REQUEST= (Request *)malloc(sizeof(Request));
		opcode=*(short signed int*)tmp;
		tmp = tmp + sizeof(REQUEST->opcode);
		strcpy(file,tmp);
		tmp = strchr(tmp, '\0')+1;		
//		tmp = tmp + sizeof(file)+1;
		printf("mode -> %s\n",tmp);
		strcpy(mode,tmp);
		
		REQUEST->opcode = ntohs(opcode);
		REQUEST->zero_0 = '\0';
		REQUEST->zero_1 = '\0';

		strcpy(REQUEST->filename, file);
		strcpy(REQUEST->mode    , mode);

		printf("LISTEN: Request recieved:\n\t\t\tIPv4   <-> %s \n\t\t\tPORT   <-> %d\n\t\t\tPACKET <-> %i bytes.\n\t\t\tMODE   <-> %s\n",inet_ntoa(clientaddress.sin_addr), ntohs(clientaddress.sin_port), numbytes,REQUEST->mode);
		if(REQUEST->opcode==1)
		printf("LISTEN: Read File request for file: %s\n",REQUEST->filename);

		else
			printf("LISTEN: Error: Invalid request\n");

		if(fork() == 0)
			datahandler(clientaddress, REQUEST->filename, REQUEST->mode, run);
		run++;
	}
	
	close(L_PORT);
}

int check_file(char* file){

	DIR* d = opendir(directory);
	struct dirent* dir;
	
	if(d){
		while((dir = readdir(d)) != NULL){
//			printf("%s\n", dir->d_name);
			if(strcmp(dir->d_name, file) == 0)
				return 0;
		}
		
		closedir(d);
	}
	
	return -1;
}

void datahandler(struct sockaddr_in clientaddress, char* filename, char* mode, int num){
	// send an error packet if you receive request from someone other than the client, with appropriate error code
	// also return an error packet if you receive duplicate acknowledgement packets
	
	// All  packets other than duplicate ACK's and those used for termination are acknowledged unless a timeout occurs
	
	// if you receive error packet, then simply terminate!! after printing the message written in the errorMsg from client
	
	// if you receive any mode other than octet then also send an error packet to the client, and terminate (for now), although this is a doubt to be asked
	
	// terminate immediately after you send the first and only error packet
	
	// when you send out the last datagram packet, wait for an ACK, if you don't receive one, then send another ACK, and another till 5 times. After that just timeout. and print, no acknowledgement of last packet received, server terminating time out

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	int check = check_file(filename);
	
	if(check == -1){
		// send error data packet, with appropriate error number	
		Error x;
		x.opcode = 5;
		x.ecode = 1;
		
		x.opcode = htons(x.opcode);
		x.ecode = htons(x.ecode);
		
		sprintf(x.msg, "No file with name %s found in %s directory\n\0", filename, directory);
		// check if this string is received correctly or not
		
		printf("SERVER #%d: trying to send file not found error packet to client\n", num);
//		sendto(sockfd, &x, sizeof(x) + sizeof(x.msg) - MSG_LEN, 0, (struct sockaddr_in*)&clientaddress, sizeof(struct sockaddr_in));
//		sendto(sockfd, &x, sizeof(x), 0, (struct sockaddr_in*)&clientaddress, sizeof(struct sockaddr_in));

		sendto(sockfd, &x, strlen(x.msg)*sizeof(char) + 5, 0, (struct sockaddr*)&clientaddress, sizeof(struct sockaddr));
		// this can cause a problem
		printf("SERVER #%d: error packet sent to client successfully\nTERMINATING\n", num);
	}
	
	else{
		//
		printf("SERVER #%d: not going to send any reply right now\n", num);
	}
	
	// close sockfd;
	// terminate the child process
	close(sockfd);
	exit(0);
}
