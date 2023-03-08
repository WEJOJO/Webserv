#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> ////
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[BUF_SIZE];
	int str_len,recv_len,recv_cnt;
	int idx = 0, read_len = 0;

	if (argc!=3)
	{
		printf("argc err");
		exit(1);
	}

	sock = socket(PF_INET,SOCK_STREAM,0);
	if (sock == -1)
	{
		printf("socker err");
		exit(1);
	}
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]); /////
	serv_addr.sin_port=htons(atoi(argv[2])); /////

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
	{
		printf("connect err");
		exit(1);
	}
	else
		puts("Connected ...\n");

	while(1)
	{
		fputs("Input msg (Q to quit):", stdout);
		fgets(message, BUF_SIZE, stdin);
		if (!strcmp("q\n", message) || !strcmp("Q\n",message))
			break;

		str_len = write(sock,message,strlen(message));
		recv_len = 0;
		while(recv_len < str_len)
		{
			recv_cnt = read(sock, &message[recv_len],BUF_SIZE-1); //일단 그대로  따라해보고, recv_len++;로 변경해보는 걸로...
			if (recv_cnt == -1) //
			{
				printf("err in sec while");
				exit(1);
			}
			recv_len+=recv_cnt; ////
		}
		message[recv_len] = 0; ////
		printf("msg from server is :%s \n",message);
	}
	close(sock);
	return 0;
}

