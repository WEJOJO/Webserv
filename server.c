#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> ////
#include <sys/socket.h>

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in	serv_addr; /////
	struct sockaddr_in 	clnt_addr; /////
	socklen_t clnt_addr_size;	////

	char message[] = "nhwang test";

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

	if (listen(serv_sock,5)==-1) ///
	{
		printf("listen err");
		exit(1);
	}

	clnt_addr_size = sizeof(clnt_addr_size); ///clnt addr size를 이이전전에  bind에서 사용하고 있다. 추측컨데 bind에서 사용할 때에는 저 값이 0이어야 하는 것인가? 혹은 주소를 넘기는 것이라 딱히 순서랑 상관 없는 것일지도?
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if (clnt_sock==-1)
	{
		printf("accept err");
		exit(1);
	}

	write(clnt_sock,message,sizeof(message));
	close(clnt_sock);
	close(serv_sock);
	return 0;
}