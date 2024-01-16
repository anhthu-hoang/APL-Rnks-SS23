#define MAX_TEXT_LENGTH 1024
#define SERVER_IPV6_ADDR "::1"

struct packet {
    char text[MAX_TEXT_LENGTH];
    char sNummer[6]; //s85622
    int port = 50000;
};