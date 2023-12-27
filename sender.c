// gcc -Wall -o 
#include <stdio.h>
#include <string.h>

#include <winsock2.h>   // socket() für MS Windows

/* Parameter definieren:
- af:  die Adressfamilie (d.h. die Protokollfamilie) 
- type: 

*/
SOCKET socket(int af, int type, int protocol);  // socket() für MS Windows

// Erzeuge das Socket - Verbindung über TCP/IP


sock = socket( AF_INET, SOCK_STREAM, 0 ); 
if (sock < 0) {
    SOCKET_ERROR; // Fehler beim Erzeugen des Sockets
}