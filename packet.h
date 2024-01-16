#define MAX_TEXT_LENGTH 1024
#define SERVER_IPV6 "fe80::f176:f9a0:71eb:1c46%4"

struct packet {
    char text[MAX_TEXT_LENGTH];
    char sNummer[6]; //s85622
    int port = 50000;
};