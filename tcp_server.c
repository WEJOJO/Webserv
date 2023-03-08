#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> ////
#include <sys/socket.h>

#define BUF_SIZE 1024

#define Q_SIZE 5

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;
	int str_len, i;

	struct sockaddr_in	serv_addr; /////
	struct sockaddr_in 	clnt_addr; /////
	socklen_t clnt_addr_size;	////

	char message[BUF_SIZE];

	if (argc!=2)
	{
		printf("error in argc!=2");
		exit(1);
	}

	serv_sock=socket(PF_INET, SOCK_STREAM, 0); /////
	if (serv_sock==-1)
	{
		printf("err sock");
		exit(1);
	}

	memset(&serv_addr,0,sizeof(serv_addr));/////
	serv_addr.sin_family=AF_INET; /////
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY); /////
	serv_addr.sin_port=htons(atoi(argv[1])); /////

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
	{
		printf("bind err");
		exit(1);
	}

	if (listen(serv_sock,Q_SIZE)==-1) ///
	{
		printf("listen err");
		exit(1);
	}

	clnt_addr_size = sizeof(clnt_addr);

	for (i = 0; i < Q_SIZE; i++)
	{
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		if (clnt_sock==-1)
		{
			printf("accept err\n");
			exit(1);
		}
		else
		{
			printf("connected with clnt\n");
		}
		while((str_len=read(clnt_sock,message,BUF_SIZE))!=0)
		{
			write(clnt_sock,message,str_len);
			message[str_len] = 0;///added for printf message
			printf("in server msg from clnt ::: %s\n",message);
		}
		close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}
