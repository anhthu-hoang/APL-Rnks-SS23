// gcc -Wall -o cilent cilent.c
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
    printf("Socket wurde nicht erzeugt. \n");
     exit(EXIT_FAILURE); // Fehler beim Erzeugen des Sockets
}
else  
    printf("Socket wurde ergolgreich erzeugt. \n");

// Aufbau einer TCP-Verbindung
/* Parameter
- SOCKET s: Socket-Deskriptor (Rueckgabewert von Funktion socket())
- FAR* addr: 

*/

struct sockaddr_in6 server_addr; // Verbindungsinformation fuer Server zu speichern
memset(&server_addr, 0, sizeof(server_addr)); // server_add Struktur auf 0 gesetz --> alle Felder initialisiert
server_addr.sin6_family = AF_INET6; // family IPv6 Adresse
/*
inet_pton: IP Adresse von Textformat zu binaere Form umwandeln
AF_INET6: handelt um IPv6-Adresse
SERVER_IPv6: Zeichenkette, IPv6-Adresse des Servers enthält
&(server_addr.sin6_addr): Adresse anzeigen
*/
inet_pton(AF_INET6, SERVER_IPv6, &(server_addr.sin6_addr)); 
server_addr.sin6_port = htons(Port_IPv6); // von Port_IPv6 in Netzwerk-Byte-Reihenfolge konvertiert


// Baue die TCP-Verbindung zum Server auf.
if (connect(sock,(struct sockaddr*)&server, sizeof(server)) < 0){
        printf("Connecting failed.\n"); // Fehler beim Verbindungsaufbau ...
        exit(EXIT_FAILURE);
    }
 else
    printf("Connected to Server.\n");


while (1) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);  // Das Standard-Eingabestream-File-Deskriptor (Tastatur)
        FD_SET(client_socket, &read_fds); // Der Socket-File-Deskriptor für die Verbindung zum Empfänger

        // Überwachen der Eingabestreams und der Socketverbindung
        int count = select(sock, &read_fds, NULL, NULL, NULL)
        if (count < 0) {
            perror("Fehler bei select()");
            break;
        }

        // Eingabe vom Benutzer lesen und senden
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            struct packet data;
            memset(&data, 0, sizeof(data));

            printf("Nachricht eingeben: ");
            fgets(data.text, sizeof(data.text), stdin);
            data.sNummer = sNummer;

            // Nachricht an den Empfänger senden
            if (send(sock, &data, sizeof(data), 0) == -1) {
                perror("Fehler beim Senden der Nachricht");
                break;
            }
        }

        // Nachricht vom Empfänger empfangen und anzeigen
        if (FD_ISSET(sock, &read_fds)) {
            struct packet received_data;
            memset(&received_data, 0, sizeof(received_data));

            // Nachricht vom Empfänger empfangen
            if (recv(sock, &received_data, sizeof(received_data), 0) == -1) {
                perror("Fehler beim Empfangen der Nachricht");
                break;
            }

            // Nachricht auf der Konsole ausgeben
            printf("s%d > %s", received_data.sNummer, received_data.text);
        }
    }



