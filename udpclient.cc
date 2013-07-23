#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>

int main(int argc, char **argv) {
    int sockfd, i;
    char buf[512];
    struct sockaddr_in sin;
    time_t t;

    if(argc != 2) {
	cout << "Simple client for an UDP listening daemon, v1.0" << endl << endl;
	cout << "usage:" << endl;
	cout << "\t" << argv[0] << " <number of packets>" << endl;
    } else {
	memset(&sin, 0, sizeof(sin));

	sin.sin_family = AF_INET;
	sin.sin_port = htons(1666);
	inet_pton(AF_INET, "localhost", &sin.sin_addr);

        if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) != -1) {
	    for(i = 0; i != atol(argv[1]); i++) {
		time(&t);
		sprintf(buf, "packet number %d, sent on %s", i, ctime(&t));

		if(sendto(sockfd, buf, strlen(buf)+1, 0, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
		    perror("sendto");
		}
	    }
	} else {
	    perror("socket");
	}
    }

    return 0;
}
