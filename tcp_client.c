#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> ////
#include <sys/socket.h>

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len = 0;
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


// test for clnt do not, server can wait till clnt req, and pop the queue
// with this case, server stdout will connected with clnt one line only, cause queue on ready to read for clnt1(first req)
////////////////////////////////////////////////////////////////////////////////////////////////
	while(read_len==read(sock,&message[idx++],1))
	{
		if (str_len == -1)
		{
			printf("read err");
			exit(1);
		}
		str_len += read_len;
	}

	printf("Msg from server %s \n", message);
////////////////////////////////////////////////////////////////////////////////////////////////


/// test for exectly finish by clnt
	// write(sock,"Hello",6);

	printf("Function read call count : %d \n",str_len);
	close(sock);
	return 0;
}

