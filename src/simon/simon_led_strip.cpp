#include "simon_led_strip.hpp"


SimonLedStrip::SimonLedStrip(char* ip_address, int port) {
    portno = port;
    host = ip_address;
}

int SimonLedStrip::send_request(const char* raw_msg) {

    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total;
    char response[4096];

    printf("Request:\n%s\n",raw_msg);

    /* create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        return -1;
    }

    /* lookup the ip address */
    server = gethostbyname(host);
    if (server == NULL) {
        perror("ERROR, no such host");
        return -1;
    }

    printf("Server created");

    /* fill in the structure */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

    /* connect the socket */
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        return -1;
    }

    printf("Conectado con servidor");

    /* send the request */
    total = strlen(raw_msg);
    sent = 0;
    do {
        bytes = write(sockfd,raw_msg+sent,total-sent);
        if (bytes < 0) {
            perror("ERROR writing message to socket");
            return -1;
        }
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);

    close(sockfd);

    return 0;
}

void SimonLedStrip::in_game() {
    const char* in_game_msg = "POST /win&T=1&A=128&FX=68&SX=50&IX=140 HTTP/1.0\r\n\r\n";
    send_request(in_game_msg);
}

void SimonLedStrip::fail_game() {
    const char* fail_msg = "POST /win&T=1&R=255&G=0&B=0&A=128&FX=1&SX=200&IX=140 HTTP/1.0\r\n\r\n";
    send_request(fail_msg);
}

void SimonLedStrip::success_game() {
    const char* success_msg = "POST /win&T=1&R=0&G=255&B=0&A=128&FX=1&SX=200&IX=140 HTTP/1.0\r\n\r\n";
    send_request(success_msg);
}