#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */

class SimonLedStrip {

    public:
        SimonLedStrip(char* ip_address, int port);

        //Methods for Simon Machine
        void fail_game();
        void success_game();
        void in_game();

    private:
        int portno;
        char *host;

        int send_request(const char* raw_msg);
};