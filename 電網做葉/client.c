//                                 _oo8oo_
//                                o8888888o
//                                88" . "88
//                                (| -_- |)
//                                0\  =  /0
//                              ___/'==='\___
//                            .' \\|     |// '.
//                           / \\|||  :  |||// \
//                          / _||||| -:- |||||_ \
//                         |   | \\\  -  /// |   |
//                         | \_|  ''\---/''  |_/ |
//                         \  .-\__  '-'  __/-.  /
//                       ___'. .'  /--.--\  '. .'___
//                    ."" '<  '.___\_<|>_/___.'  >' "".
//                   | | :  `- \`.:`\ _ /`:.`/ -`  : | |
//                   \  \ `-.   \_ __\ /__ _/   .-` /  /
//               =====`-.____`.___ \_____/ ___.`____.-`=====
//                                 `=---=`
//
// 
//    ~~~~~~~Powered by https://github.com/ottomao/bugfreejs~~~~~~~
//
//                         �򯪫O��         �õLbug
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/poll.h>
#define PORT "14000" // Client �ҭn�s�u�� port
#define MAXDATASIZE 5000 // �ڭ̤@���i�H���쪺�̤j���ռơ]number of bytes�^

int sockfd, numbytes;
char buf[MAXDATASIZE];
char name[MAXDATASIZE];
char chat[MAXDATASIZE];
struct addrinfo hints, *servinfo, *p;
int rv;
int n=0;
int wait_time;//Receive�����ݮɶ� 
char s[INET6_ADDRSTRLEN];

pid_t pid; 
FILE *fp;
clock_t start;
char Check[10]="\n";

void *get_in_addr(struct sockaddr *sa)// ���o IPv4 �� IPv6 �� sockaddr
{
  if (sa->sa_family == AF_INET)
  {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void recv_pic()
{
	 
//Open file
               if ( (fp = fopen("get.jpg", "wb")) == NULL){
                               perror("fopen");
                               exit(1);
               }
               //Receive file from server
               while(1){
                               numbytes = read(sockfd, buf, sizeof(buf));
                               //printf("read %d bytes, ", numbytes);
                               if(numbytes == 0){
                                              break;
                               }
                               numbytes = fwrite(buf, sizeof(char), numbytes, fp);
                               //printf("fwrite %d bytes\n", numbytes);	
                               
                               n+=numbytes;
                               if(n>=8000)
                               break;
						}
	
	printf(" pic recv.\n");
	
}


void sendname()//�ǰe�W�r 

{
	if((numbytes = send(sockfd, "***", MAXDATASIZE-1, 0)) == -1)
	{
		perror("send");
		exit(1);
	}
	
	start = clock();//�j�@�q�ɶ������~�򩹤U���� 
	while((double)( clock() - start) < 100){}
	
	if((numbytes = send(sockfd, name, MAXDATASIZE-1, 0)) == -1)
	{
		perror("send");
		exit(1);
	}
	
	printf("your name : %s\n",name);
}

void Socket_Connect()// �ΰj����o���������G�A�å��s�u��ন�\�s�u��
{
	for(p = servinfo; p != NULL; p = p->ai_next)
	{
    	if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1)
		{
      		perror("client: socket");
      		continue;
    	}

   		 if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
     		close(sockfd);
      		perror("client: connect");
      		continue;
    	}

    break;
	}
	
	if (p == NULL)
	{
   		fprintf(stderr, "client: failed to connect\n");
    	exit(0);
  	}
}

void Receive()//������Ѥ��e 
{
	struct pollfd
	{
		int fd;
		short events;
		short revents;
	};
	struct pollfd ufds[2];	
	
	ufds[0].fd = sockfd;
	ufds[0].events = POLLIN;
	
	wait_time = poll(ufds,1,500);
	
	if(wait_time == -1)
	{
		perror("poll");
		exit(1);
	}
	else if(wait_time == 0){}
	else
	{
		if(ufds[0].revents & POLLIN)
		{
			recv(sockfd, buf, MAXDATASIZE-1, 0);
  			printf("%s",buf);
  			strcpy(buf,"");
		}

	}	
}

void send_chat()//�ǰe��Ѥ��e 
{
	pid = fork();
	if(pid == -1)
	{
		perror("fork");
		exit(1);
	}
	while(1 == 1)
	{
		start = clock();//�j�@�q�ɶ������~�򩹤U���� 
		while((double)( clock() - start) < 100){}
		
		if(pid==0)
		{
			while(fgets( buf, MAXDATASIZE, stdin) != NULL)
			{
				if(strcmp(Check, buf) != 0)
				{
					if((numbytes = send(sockfd, buf, MAXDATASIZE-1, 0)) == -1)
					{
						perror("send\n");
						exit(1);
					}
					break;
					strcpy(buf,"");
				}
				break;
			}
		}
		
		if(pid > 0)
		{
			Receive();
		}
	}

}

int main(int argc, char *argv[])
{
	

	if (argc != 2)
	{
    	fprintf(stderr,"usage: client hostname\n");
    	exit(1);
  	}
  	
	memset(&hints, 0, sizeof hints);
  	hints.ai_family = AF_UNSPEC;
 	hints.ai_socktype = SOCK_STREAM;
	

	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0)
	{
    	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    	return 1;
  	}
	
	Socket_Connect();
	int n;
	printf("enter 1 to recv picture , 2 to chat.\n");
	if(scanf("%d",&n)== 1)
	{
	recv_pic();
	fclose(fp);
	}
	else
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);

  	printf("client: connecting to %s\n", s);
  	
  	printf("what your name?\n");
	scanf("%s", name);
	printf("welcome to join the conversation!\n");
  	
  	freeaddrinfo(servinfo);
	
	sendname();
	
	buf[MAXDATASIZE] = '\0';
	fflush(stdin);
	buf[numbytes] = '\0';
	
	while(1)
	{
		send_chat();
	}
	
	
	
	
	buf[numbytes] = '\0';
  	printf("client: received '%s'\n",buf);
 	
 	
  	return 0;
}

