#define MAX_TEXT_LENGTH 1024

struct packet {
    char text[MAX_TEXT_LENGTH];
    char sNummer[6]; //s85622
    int port = 50000;
};