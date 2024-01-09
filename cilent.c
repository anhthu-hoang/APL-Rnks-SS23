// gcc -Wall -o cilent cilent.c
#include <stdio.h>
#include <string.h>

#include <winsock2.h>   // socket() für MS Windows
#include <winsock.h>    // connect() fuer MS Windows
#include <ws2tcpip.h>

#include "packet.h"

#define SERVER_IPv6 "mein_server_IP"
#define Port_IPv6 50000

int main(int argc, char *argv[])  //argv[1]: IPv6-Adresse, argv[2]: Port, argv[3]: sNummer
{
    int sock;
    char sendbuf[MAX_TEXT_LENGTH];
    int sendlen;
    
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <IPv6-Adresse> <Port> <sNummer>\n", argv[0]);
        exit(EXIT_FAILURE);}

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
        - FAR* addr: */

    // SENDER ADRESSE UND PORT VORBEREITEN
        struct sockaddr_in6 server_addr;
        memset(&server_addr, 0, sizeof(server_addr)); //~bzero aber neuer, um sender_add bis sizeof() mit 0 eingesetzt
        server_addr.sin6_family = AF_INET6; // family IPv6 Adresse
        inet_pton(AF_INET6, SERVER_IPv6, &(server_addr.sin6_addr)); // IPv6-Adresse aus SERVER zu konvertieren
        server_addr.sin6_port = htons(Port_IPv6); // von Port_IPv6 in Netzwerk-Byte-Reihenfolge konvertiert
        
        //sender_addr.sin6_addr = inet_addr (SERVER); Fuer IP4 ???
        //sender_addr.sin6_port = htons (PORT);

        /*
        inet_pton: IP Adresse von Textformat zu binaere Form umwandeln
        AF_INET6: handelt um IPv6-Adresse
        SERVER_IPv6: Zeichenkette, IPv6-Adresse des Servers enthält
        &(server_addr.sin6_addr): Adresse anzeigen
        */


    // Baue die TCP-Verbindung zum Server auf.
    if (connect(sock,(struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
            printf("Connecting failed.....\n"); // Fehler beim Verbindungsaufbau ...
            close(sock);
            exit(EXIT_FAILURE);
        }
    else
        printf("Connected to Server.....\n");


    packet myPacket;
    
    while (1) {
            fd_set read_fds;
            FD_ZERO(&read_fds); // Alee fds bits wird geloescht, die vorher noch nicht
            FD_SET(STDIN_FILENO, &read_fds);  // Das Standard-Eingabestream-File-Deskriptor (Tastatur)
            FD_SET(sock, &read_fds); // Der Socket-File-Deskriptor für die Verbindung zum Empfänger

            // Warten auf Eingabe von Server-Daten
            select(FD_SETSIZE, &read_fds, NULL, NULL, NULL)
            if (select(sock, &read_fds, NULL, NULL, NULL) < 0) {
                perror("Fehler bei select()");
                close(sock);
                exit(EXIT_FAILURE);
            }

            // Prufen, ob Nachricht von Konsole eingeben
            if (FD_ISSET(STDIN_FILENO, &read_fds)) {

                // Einlesen des über die Konsole angegebenen Textes
                fgets(myPacket.text, MAX_TEXT_LENGTH, stdin)

                // Pruefen, ob Nachricht an den Empfänger senden
                if (send(sock, &myPacket, sizeof(myPacket), 0) < 0) {
                    perror("Fehler beim Senden der Nachricht");
                    close(sock);
                    exit(EXIT_FAILURE);
                }
            }

            // Pruefen , ob Nachricht vom Server emfangen
            if (FD_ISSET(sock, &read_fds)) {
                // Empfangen einer Nachricht vom Kommunikationspartner
                ssize_t received_bytes = recv(sock, &myPacket, sizeof(myPacket), 0);
                if (received_bytes <= 0) {
                if (received_bytes == 0) {
                    printf("Erfolgreich verbinden\n");
                } else {
                    perror("Fehler beim Empfangen von Daten vom Server!");
                }
                close(sockfd);
                exit(EXIT_FAILURE);
            }

                // Nachricht auf der Konsole ausgeben: recv() einer Nachricht vom Kommunikationspartner
                printf("s%d > %s", myPaket.sNummer, myPaket.text);
            }
        }
    // Socket schließen
    close(sock);

    return 0;

}

