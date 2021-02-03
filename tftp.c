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

#include<dirent.h>
#define directory "."

#define NAME_LEN 100
#define MODE_LEN 100
#define DATA_LEN 512
#define ALARM_TIMEOUT 1
#define TIMEOUT_ATTEMPTS 5

int L_PORT;
int L_FD;
#define MSG_LEN 100
int C_PORT;
int please_wait;
int verbose;

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
	char data[DATA_LEN + 1];
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

typedef union {
	Request rrq;
	Data data;
	Ack ack;
	Error err;
}Message;


long int findSize(char file_name[]) 
{ 
	FILE* fp = fopen(file_name, "r"); 
	if (fp == NULL) 
        	return -1; 
	fseek(fp, 0L, SEEK_END); 
	long int res = ftell(fp); 
	fclose(fp); 
	return res; 
} 



void datahandler(struct sockaddr_in clientaddress, char* filename, char* mode, int num);

void sig_handler(int sign){
	please_wait = 0;
}

int ppid;

void ender(int end)
{
	if(getpid()!=ppid)
	{
		kill(getppid(),SIGINT);
		exit(0);
	}
	printf("\n\n\n=============================================================\n");
	printf("                        THANK YOU!\n");
	printf("=============================================================\n\n");
	close(L_FD);
	exit(0);
}

void main(int argc, char ** argv)
{
	printf("\n\n\n=============================================================\n");
	printf("                          WELCOME!\n");
	printf("=============================================================\n\n");
	ppid = getpid();
	please_wait = 1;
	signal(SIGALRM, sig_handler);
	signal(SIGINT, ender);

	struct sockaddr_in localbind, clientaddress;
//	struct sockaddr_in localbind;
//	struct sockaddr clientaddress;
	int run=1;


	printf("COMMAND: Initialising server...\n");
	if(argc<2) 
	{ 
		printf("COMMAND: Error: Please enter port number as CLA. Exiting....\n"); 
		raise(SIGINT);
	}

	L_PORT = atoi(argv[1]);

	if(argc>2) 
	{ 
		printf("COMMAND: Error: Invalid arguements specified. Exiting....\n"); 
		raise(SIGINT);
	}

	printf("COMMAND: Dedicating listening port at: \t\t%d\n", L_PORT);

	
	L_FD = socket(AF_INET, SOCK_DGRAM, 0);
	if(L_FD==-1) 
	{ 
		printf("LISTEN: Error initiaising listen port. Exiting....\n"); 
		raise(SIGINT);
	}
	
	int tttt = 1; 
	setsockopt(L_FD, SOL_SOCKET, SO_REUSEADDR, &tttt , sizeof(int));

	localbind.sin_family       = AF_INET;
	localbind.sin_addr.s_addr  = htonl(INADDR_ANY);
	localbind.sin_port     = htons(L_PORT);
	if (bind(L_FD, (struct sockaddr *) &localbind, sizeof(localbind)) == -1)
	{ 
		printf("LISTEN: Error binding listen port. Exiting....\n"); 
		perror("perror output - ");
		raise(SIGINT);
	}
	
	char choice[100];
	printf("COMMAND: Do you want to print the status of each transfer continuously? (Y/N)\n> ");
	
	scanf("%[^\n]s",choice); getchar();
	
	if((choice[0]=='Y')||(choice[0]=='y'))
		verbose = 1;
	else
		verbose = 0;	
	
	printf("COMMAND: Successfully initialised listen port\n");

	printf("\n======================= SERVER START ========================\n\n");
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
		printf("REQ %d: trying to send file not found error packet to client\n", num);
		sendto(sockfd, &x, sizeof(x), 0, (struct sockaddr*)&clientaddress, sizeof(struct sockaddr));

		printf("REQ %d: error packet sent to client successfully\nTERMINATING\n", num);
	}
	
	else{
				
		printf("---------------------- Request %d Start ----------------------\n\n",num,num);
		struct timeval start_time;
		gettimeofday(&start_time,NULL);
		char newfile[NAME_LEN];
		newfile[0] = '\0';
		strcat(newfile, directory);
		strcat(newfile, "/");
		strcat(newfile, filename);
		FILE* file = fopen(newfile, "r");
		
		int size_msg, fread_return;
		unsigned short int block = 1;
		Message x, y;
		int done = 0, end = 1;

		while(end){
			fread_return = fread(x.data.data, sizeof(char), DATA_LEN, file);
			if(fread_return == 0)
				end = 0;
				
			x.data.data[fread_return] = '\0';
			for(int k = 0; k< TIMEOUT_ATTEMPTS; ++k){
				// send data
				done = 0;
				
				x.data.opcode = 3;
				x.data.block = block;
				x.data.opcode = htons(x.data.opcode);
				x.data.block = htons(x.data.block);
				x.data.data[fread_return] = 0;
				sendto(sockfd, &x, sizeof(Data)-DATA_LEN-2 + fread_return, 0, (struct sockaddr*)&clientaddress, sizeof(struct sockaddr));
				
				// wait for reply for timeout duration
				alarm(ALARM_TIMEOUT);
				please_wait = 1;
				
				if(verbose==1)
					printf("REQ %d: Sent packet  for block  %d\n", num, block);
				
				while(please_wait)
					if(recvfrom(sockfd, &y, sizeof(Message), MSG_DONTWAIT, (struct sockaddr*)&clientaddress, &size_msg) != -1)
						break;
						
				y.ack.opcode = ntohs(y.ack.opcode);
				y.ack.block = ntohs(y.ack.block);
				
				please_wait = 1;

				if(y.data.opcode == 5)
					break;
				
				else if(y.data.opcode == 4 && y.ack.block == block){
					if(verbose==1)
						printf("REQ %d: Received ACK for block  %d\n", num, block);
					done = 1;
					break;
				}
				
				if(size_msg > sizeof(Message)){
					printf("REQ %d: ERROR - Wrong protocol being used by client! Cancelling transfer...\n", num, size_msg);
					printf("\n====================== Request %d Done =======================\n",num);
					printf("REQ %d: Transfer for file '%s' failed!\n",num,filename);
					printf("=============================================================\n\n");
					y.data.opcode = 6;
					break;
				}
			}
						
			if(y.data.opcode == 5){
				printf("REQ %d: ERROR - Client sending error packet -> %s\n", num,y.err.msg);
				printf("\n====================== Request %d Done =======================\n",num);
				printf("REQ %d: Transfer for file '%s' failed!\n",num,filename);
				printf("=============================================================\n\n");
				break;
			}
			
			if(done == 0){
				
				printf("REQ %d: Received ACK for block  %d\n", num, block);
				printf("REQ %d: ERROR - Unable to send block  %d ! Cancelling transfer...\n", num, block);

				printf("\n====================== Request %d Done =======================\n",num);
				printf("REQ %d: Transfer for file '%s' failed!\n",num,filename);
				printf("=============================================================\n\n");

				break;
			}
			
			++block;
			done = 0;
			
			if(fread_return < DATA_LEN){
				struct timeval result;
				gettimeofday(&result,NULL);
				result.tv_usec -= start_time.tv_usec;
				result.tv_sec  -= start_time.tv_sec;
				int sizee = findSize(filename);////////////
				printf("\n====================== Request %d Done =======================\n",num);
				printf("REQ %d: File '%s' transferred at speed -> %f Mb/s\n",num,filename,(0.000001*(float)sizee)/((float)(result.tv_sec)+((float)(result.tv_usec)/1000000)));
				printf("=============================================================\n\n");
				end = 0;
			}
		}
		
		fclose(file);
	}
	
	close(sockfd);
	exit(0);
}

