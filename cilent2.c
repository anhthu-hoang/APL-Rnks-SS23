// gcc -Wall -o client client.c
// ./client 0:0:0:0:0:0000:0000:0 50000 85622


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

int main(int argc, char *argv[])
{
	int sockfd, ret;
	struct sockaddr_in6 serv_addr;
	struct hostent *server;
	char buffer[1024];

	if (argc < 3) {
		printf("Usage: <FILE> <SERVER_IP> <PORT> <S_NUMMER>\n");
		return EXIT_FAILURE;
	}

	int portno = atoi(argv[2]);
	int snum = atoi(argv[3]);

	printf("IPv6 TCP Client started.\n");

	/* Create socket to connect to server */
	sockfd = socket(AF_INET6, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket()");
		return EXIT_FAILURE;
	}

	/* Get server IP address */
	server = gethostbyname2(argv[1], AF_INET6);
	if (server == NULL) {
		perror("gethostbyname()");
		return EXIT_FAILURE;
	}

	/* Structure IP address */
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin6_flowinfo = 0;
	serv_addr.sin6_family = AF_INET6;
	bcopy((char *)server->h_addr,
		(char *)&serv_addr.sin6_addr.s6_addr,
		server->h_length);
	serv_addr.sin6_port = htons(portno);

	/* Connect to Server */
	ret = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (ret == -1) {
		perror("connect()");
		close(sockfd);
		return EXIT_FAILURE;
	}

	printf("Server and Client are connected.\n");
	printf("Type something...\n");

	while (1)
	{
		/* Send data to server */
		bzero(buffer, 1024);
		fgets(buffer, 1024, stdin);
		ret = send(sockfd, buffer, 1024, 0);
		if (ret == -1) {
			perror("send()");
			close(sockfd);
			return EXIT_FAILURE;
		}

		/* Wait for data from server */
		ret = recv(sockfd, buffer, 1024, 0);
		if (ret == -1) {
			perror("recv()");
			close(sockfd);
			return EXIT_FAILURE;
		}
		printf("Server[s%d]: %s", snum, buffer);

		int i = strncmp("bye", buffer, 3);
		if (i == 0) break;
	}

	/* Close opening sockets */
	ret = close(sockfd);
	if (ret == -1) {
		perror("close()");
		return EXIT_FAILURE;
	}

	printf("Connection closed");

	return EXIT_SUCCESS;
}