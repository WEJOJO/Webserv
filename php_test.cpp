#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <string>

#define BUF_SIZE 1024

int main(int argc, char *argv[], char **env) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int pid;
    struct timeval timeout;
    fd_set reads, cpy_reads;

    socklen_t adr_sz;
    int fd_max, str_len, fd_num, i;
    char buf[BUF_SIZE];

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(8080); // htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) {
        printf("bind err");
        exit(1);
    }

    if (listen(serv_sock, 5) == -1) {
        printf("bind err");
        exit(1);
    }

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;
    argv[argc] = 0; /////
    argv = &argv[1];
	int _pipe[2];

    // char **argv = "/usr/bin/php -S 127.0.0.1:9000";
    while (1) {
        cpy_reads = reads;
        timeout.tv_sec = 1;
        timeout.tv_usec = 5000;

        if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1) {
            printf("\n=================ERR!!!===============\n");
            break;
        }
        if (fd_num == 0)
            continue;
        for (i = 0; i < fd_max + 1; i++) {
            if (FD_ISSET(i, &cpy_reads)) {
                if (i == serv_sock) {
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr,
                                       &adr_sz);
                    FD_SET(clnt_sock, &reads);
                    if (fd_max < clnt_sock)
                        fd_max = clnt_sock;
                    printf("connected client: %d \n", clnt_sock);
                } else {
					pipe(_pipe);
                    str_len = recv(i, buf, BUF_SIZE, 0);
					send(_pipe[1], buf,str_len,0);//////////////
					bzero(buf,BUF_SIZE);
                    if (str_len == 0) {
                        FD_CLR(i, &reads);
                        close(i);
						bzero(buf,BUF_SIZE);
                        printf("closed clnt %d\n", i);
                    } else {
                        // write(1, buf, sizeof(buf)); //req 출력
                        pid = fork();
                        if (pid == 0) {
							dup2(_pipe[0],0);//////////////
							close(_pipe[0]);
							dup2(_pipe[1],1);
							close(_pipe[1]);
							for (i = 3; i < fd_max + 1; i++)
								close(i);
	                        close(serv_sock);
                            chdir("/goinfre/nhwang/Webserv");
                            execve(*argv, argv, env);
							exit(1);
                        } else {
							close(_pipe[1]);
                            waitpid(pid, 0, 0);
							str_len = read(_pipe[0], buf, BUF_SIZE);
							buf[str_len] = 0;////4.24
                            ////////////////////
                            //////////////////// body를 붙혀서 던지는 게 아닌건지 확인
							std::string body;
							body = "<html><body><h1>POST\?\?</h1><p>";
							for (int j = 0; j <= str_len;j++)/////body
								body+=buf[j];
							body+="</p></body></html>";
							int body_length = body.length();
							std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: " + std::to_string(body_length) + "\r\n\r\n" + body;
                            ////////////////////
                            ////////////////////
							send(i, response.c_str(), response.length(), 0);
						   	bzero(buf,BUF_SIZE);
							close(_pipe[0]);
                        }
                    }
                }
            }
        }
    }
    close(serv_sock);
    return (0);
}