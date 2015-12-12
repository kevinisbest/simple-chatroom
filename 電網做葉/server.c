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
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define PORT "14000" // �ڭ̥��b listen �� port

fd_set master;    // master file descriptor �M��
fd_set read_fds;  // �� select() �Ϊ��Ȯ� file descriptor �M��
int fdmax;        //  �̤j�� file descriptor �ƥ�

int listener;     // listening socket descriptor
int newfd;        // �s������ accept() socket descriptor
struct sockaddr_storage remoteaddr; // client address
socklen_t addrlen;
struct sigaction sa;

char buf[5000];    // �x�s client ��ƪ��w�İ�
int nbytes,numbytes;

clock_t start;
    
char remoteIP[INET6_ADDRSTRLEN];

int yes=1;        // �ѩ��U�� setsockopt() �]�w SO_REUSEADDR
int i, j, rv, k;

struct addrinfo hints, *ai, *p;

char command[10]="***";

struct chatinfo//��ѵ��c�ŧi 
{
	int NUM;//�s�� 
	char *name;
	char *said;//�x�sXXX said: 
};
struct chatinfo num[8];
int clientnum = 0;

void acceptname()//�������x�s�W�r 
{
	char *title = (char *)malloc(256);
	char *said = " said : ";
	
	if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0)
	{
		perror("recv name");
	}
	
	else
	{
		printf("%s join the conversation!\n",buf);
		
		num[clientnum].NUM = i;
		num[clientnum].name = buf;
		strcat(title,buf);
		strcat(title," said :");
		num[clientnum].said = title;
		
		clientnum = clientnum +1;
	}
}

void showchat()//��ܨöǰe��Ѥ��e 
{
	char *title = (char *)malloc(256);
	
	for(k=0;k<clientnum;k++)
	{
		if(num[k].NUM == i)
		{
			strcat(title,num[k].said);
			strcat(title,buf);
			break;
		}
	}
	
	printf("%s\n",title);
	strcpy(buf,title);
	start = clock();//�j�@�q�ɶ������~�򩹤U���� 
	while((double)( clock() - start) < 100){}
	
	// we got some data from a client
    for(j = 0; j <= fdmax; j++)
	{
        // send to everyone!
        if (FD_ISSET(j, &master))
		{
            // except the listener and ourselves
            if (j != listener && j != i)
			{
            	if (send(j, buf, nbytes, 0) == -1)
				{
                    perror("send");
                    exit(1);
                }
            }
        }
    }
}

void *get_in_addr(struct sockaddr *sa)// get sockaddr, IPv4 or IPv6
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void sigchld_handler(int s)//�Ȱ��ثe�Ҧb��process�A����child process���} 
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main(void)
{
    FD_ZERO(&master);    // �M����ӲM�� master �P temp sets 
    FD_ZERO(&read_fds);

	// ���ڭ̤@�� socket�A�åB bind ��
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	 struct stat filestat;
 FILE *fp;
	
	
	if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0)
	{
		fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
		exit(1);
	}

//---------------Socket_Bind--------------------------	
	for(p = ai; p != NULL; p = p->ai_next)
	{
    	listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listener < 0)
		{ 
			continue;
		}
		
		// �׶}"address already in use"�����~�T�� 
		if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			perror("setsockopt");
			exit(1);
		} 

		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
			close(listener);
			continue;
		}

		break;
	}

	// �Y�ڭ̶i�J�o�ӧP�_���A�h��ܧڭ� bind() ����
	if (p == NULL)
	{
		fprintf(stderr, "selectserver: failed to bind\n");
		exit(2);
	}

	freeaddrinfo(ai); //����O����Ŷ�

    //-----------------listen--------------------
    if (listen(listener, 10) == -1)
	{
        perror("listen");
        exit(3);
    }
    
   
	printf("server: waiting for connections...\n");

    //  �N listener �s�W�� master set
    FD_SET(listener, &master);

    // ����l�̤ܳj�� file descriptor
    fdmax = listener; 

    // main loop
    for(;;)
	{
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
		{
            perror("select");
            exit(4);
        }
        //�b�{�s���s�u���M��ݭnŪ�������
        for(i = 0; i <= fdmax; i++)
		{
            if (FD_ISSET(i, &read_fds))
			{ 
                if (i == listener)
				{
                    addrlen = sizeof remoteaddr;
					//Get file stat
 					if ( lstat("test.jpg", &filestat) < 0){
          				exit(1);
 						} printf("The file size is %lu\n", filestat.st_size);
  						fp = fopen("test.jpg", "rb");
  						
  						
					newfd = accept(listener,(struct sockaddr *)&remoteaddr,&addrlen);
					if (newfd == -1)
					{
                        perror("accept");
                    }
					else
					{
                        FD_SET(newfd, &master); //�s�W�� master set
                        if (newfd > fdmax)//����l�̤ܳj�� fd
						{    
                            fdmax = newfd;
                        }
                        
                        
                         //hints.ai_socktype = SOCK_DGRAM;
                        //Sending file
                        
						 while(!feof(fp)){
 							numbytes = fread(buf, sizeof(char), sizeof(buf), fp);
 							printf("fread %d bytes, ", numbytes);
 							numbytes = write(newfd, buf, numbytes);
 							printf("Sending %d bytes\n",numbytes);
 							}
 							 
                                                
                        printf("selectserver: new connection from %s on "
                            "socket %d\n",
							inet_ntop(remoteaddr.ss_family,
								get_in_addr((struct sockaddr*)&remoteaddr),
								remoteIP, INET6_ADDRSTRLEN),
							newfd);
                    }
                }
				else
				{
                    // �B�z�Ӧ� client �����
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0)
					{
                        // got error or connection closed by client
                        if (nbytes == 0)
						{
                            // connection closed
                            printf("selectserver: socket %d hung up\n", i);
                        }
						else
						{
                            perror("recv");
                        }
                        close(i); // bye!
                        
                        FD_CLR(i, &master); // �q master set ������
                        
                    }
					else
					{
                        if( strcmp(command, buf) == 0)
						{
							acceptname();
						}
						else
						{
							showchat();
                        }
                    }
                } 
            } 
        } 
    } 
    
    close(newfd);
    
    return 0;
}


