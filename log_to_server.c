#include "iwl_connector.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/socket.h>
#include <linux/netlink.h>
#include <linux/connector.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/errno.h>

#define MAX_PAYLOAD 2048
#define SLOW_MSG_CNT 1
#define SERV_PORT 8090
#define SERV_IP "127.0.0.1"

int serv_port = -1;
char serv_ip[16];
int sock_fd = -1;
int sock_cli_fd = -1;

void check_usage(int argc, char** argv);
void caught_signal(int sig);
void exit_program(int code);
void exit_program_err(int code, char* func);
int Socket(int, int, int);
void Inet_pton(int , const char *, void *);
void Connect(int, struct sockaddr_in * , int);

int main(int argc, char** argv)
{
	struct sockaddr_nl proc_addr, kern_addr;
	struct cn_msg *cmsg;
	char buf[10000];
	int ret;
	unsigned short l, l2;
	int count = 0;
	struct sockaddr_in servaddr;

	check_usage(argc, argv);
	printf("to connect %s with port %d\n", serv_ip, serv_port);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(serv_port);
	Inet_pton(AF_INET, serv_ip, &servaddr.sin_addr);
	sock_cli_fd = Socket(AF_INET, SOCK_STREAM, 0);
	Connect(sock_cli_fd, &servaddr, sizeof(servaddr));

	sock_fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
	if (sock_fd == -1)
		exit_program_err(-1, "socket");

	memset(&proc_addr, 0, sizeof(struct sockaddr_nl));
	proc_addr.nl_family = AF_NETLINK;
	proc_addr.nl_pid = getpid();
	proc_addr.nl_groups = CN_IDX_IWLAGN;
	memset(&kern_addr, 0, sizeof(struct sockaddr_nl));
	kern_addr.nl_family = AF_NETLINK;
	kern_addr.nl_pid = 0;
	kern_addr.nl_groups = CN_IDX_IWLAGN;

	if (bind(sock_fd, (struct sockaddr *)&proc_addr, sizeof(struct sockaddr_nl)) == -1)
		exit_program_err(-1, "bind");

	{
		int on = proc_addr.nl_groups;
		ret = setsockopt(sock_fd, 270, NETLINK_ADD_MEMBERSHIP, &on, sizeof(on));
		if (ret)
			exit_program_err(-1, "setsockopt");
	}

	signal(SIGINT, caught_signal);

	while (1)
	{


		ret = recv(sock_fd, buf, sizeof(buf), 0);

		if (ret == -1)
			exit_program_err(-1, "recv");

		cmsg = NLMSG_DATA(buf);
		if (count % SLOW_MSG_CNT == 0)
			printf("%d received %d bytes: id: %d val: %d seq: %d clen: %d\n", count,cmsg->len, cmsg->id.idx, cmsg->id.val, cmsg->seq, cmsg->len);


		l = (unsigned short) cmsg->len;
		l2 = htons(l);
		if( write(sock_cli_fd, &l2, sizeof(unsigned short)) < 0)
			exit_program_err(-1, "write");
		if (write(sock_cli_fd, cmsg->data, l) < 0)
			exit_program_err(-1, "write");
		if (count % 100 == 0)
			printf("wrote %d bytes [msgcnt=%u]\n", ret, count);
		++count;
	}
	exit_program(0);
	return 0;
}

void check_usage(int argc, char** argv)
{	
	if (argc == 1)
	{
		serv_port = SERV_PORT;
		strcpy(serv_ip, SERV_IP);
	}
	else if(argc == 2)
	{
		serv_port = SERV_PORT;
		strcpy(serv_ip, argv[1]);
	}
	else if(argc == 3)
	{
		serv_port = atoi(argv[2]);
		strcpy(serv_ip, argv[1]);
	}
	else
	{
		fprintf(stderr, "Usage: %s <Server IP Address> <Server IP Port> OR %s <Server IP Address>\n", argv[0], argv[0]);
		exit_program(1);
	}
}

int Socket (int family, int type, int protocol)
{
	int n;
	if ( (n=socket(family, type, protocol)) < 0){
		fprintf(stderr, "socket error\n");
		exit_program(1);
	}
	return n;
}

void Inet_pton(int family, const char * ip, void * addptr)
{
	int n;
	if ( (n = inet_pton(family, ip, addptr)) <=0)
	{
		if(n == 0)
			fprintf(stdin, "inet_pton format error : %s\n", ip);
		else
			fprintf(stdin, "inet_pton error : %s\n", ip);
		exit_program(1);
	}
}

void Connect(int fd, struct sockaddr_in * sa, int size)
{
	if ( connect(fd, (struct sockaddr *)sa, size) < 0 )
	{
		fprintf(stderr, "connect error\n");
		exit_program(1);
	}
}

void caught_signal(int sig)
{
	fprintf(stderr, "Caught signal %d\n", sig);
	exit_program(0);
}

void exit_program(int code)
{
	if (sock_fd != -1)
	{
		close(sock_fd);
		sock_fd = -1;
	}
	if(sock_cli_fd != -1)
	{
		close(sock_cli_fd);
		sock_cli_fd = -1;
	}
	exit(code);
}

void exit_program_err(int code, char* func)
{
	perror(func);
	exit_program(code);
}
