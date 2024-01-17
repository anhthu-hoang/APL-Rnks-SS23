// gcc -Wall -o server server.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>
#include <arpa/inet.h>


int main(int argc, char* argv[])
{
    int sockfd, newsockfd;
    int ret;
    char buffer [1024];
    socklen_t clilen;
    struct sockaddr_in6 serv_addr, cli_addr;
    char client_addr_ipv6[100];
    
    if(argc !=3){
        printf("Usage: <FILE> <PORT> <SNUMMER>\n");
        return EXIT_FAILURE;
    }

    int portno = atoi(argv[1]);
    int snum = atoi(argv[2]);
    printf("IPv6 TCP Server started.\n");


    // Socket erstellen
    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if(sockfd == -1){
        perror("socket");
        return EXIT_FAILURE;
    }


    // Empfänger-Addresse vorbereiten
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin6_flowinfo = 0;
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_addr = in6addr_any;
    serv_addr.sin6_port = htons(portno);


    // Socket an Port binden
    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1){
        perror("bind");
        close(sockfd);
        return EXIT_FAILURE;
    }


    // Auf eingehende Verbindungen hören
    if(listen(sockfd,5) == -1){
        perror("listen");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("Waiting for connection...\n");


    // Verbindung akzeptieren
    newsockfd = accept(sockfd, (struct sockaddr *) & cli_addr, & clilen);
    if(newsockfd == -1){
        perror("accept");
        close(sockfd);
        return EXIT_FAILURE;
    }

    inet_ntop(AF_INET6,  &(cli_addr.sin6_addr),client_addr_ipv6,100);

    printf("New connection from: %s\n", client_addr_ipv6);
    printf("Server and Client are connected.\n");

    while(1)
    {
        // Lesen Daten von Server
        bzero(buffer,1024);
        if(recv(newsockfd,buffer,1024,0) == -1){
            perror("recv");
            close(newsockfd);
            exit(1);
        }

        printf("Client[s%d]: %s", snum,buffer);


        // Antwort senden
        bzero(buffer,1024);
        fgets(buffer,1024,stdin);
        if(send(newsockfd,buffer,1024,0) == -1) {
            perror("send");
            close(newsockfd);
            exit(1);
        }


        // Verbindung schließen
        int i = strncmp("Bye!",buffer,3);
        if(i == 0) break;
    }

    // Socket schließen
    close(newsockfd);
    close(sockfd);

    return 0;
}
