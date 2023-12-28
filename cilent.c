// gcc -Wall -o 
#include <stdio.h>
#include <string.h>

#include <winsock2.h>   // socket() für MS Windows
#include <winsock.h>    // connect() fuer MS Windows

/* Parameter definieren:
- af:  die Adressfamilie (d.h. die Protokollfamilie), AF_INET6 fuer IPv6 
- type: Socket-Typ, also hier SOCK_STREAM fuer TCP 
- protocol: 0 - Standardprotokoll, andere: IPPROTO_TCP (TCP-Protokoll)

*/
SOCKET socket(int af, int type, int protocol);  // socket() für MS Windows

// Erzeuge das Socket - Verbindung über TCP/IP
sock = socket( AF_INET6, SOCK_STREAM, 0 ); 
if (sock < 0) {
    printf("Socket wurde nicht erzeugt! \n");
    SOCKET_ERROR; // Fehler beim Erzeugen des Sockets
}
else  
    printf("Socket wurde ergolgreich erzeugt! \n");

// Aufbau einer TCP-Verbindung
int connect (
   SOCKET s,
   const struct sockaddr FAR* addr,
   int addrlen
);