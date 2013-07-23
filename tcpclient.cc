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
#include <string>

int sockfd;

const string getresponse() {
    char c;
    string buf;

    while(1) {
	if(read(sockfd, &c, 1) != 1) break;
	if(c == '\n') break; else buf += c;
    }

    return buf;
}

void runcommand(const string cmd) {
    string buf;

    cout << ">> " << cmd << endl;

    /*
    *
    * Send the command to execute to the daemon.
    *
    */
    
    buf = cmd + "\n";
    write(sockfd, buf.c_str(), buf.size());

    /*
    *
    * Now let's read the answer (one line).
    *
    */

    cout << "<< " << getresponse() << endl;
}

int main(int argc, char **argv) {
    struct sockaddr_in sin;

    cout << "Simple client for a TCP listening daemon, v1.0" << endl << endl;

    memset(&sin, 0, sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_port = htons(1667);
    inet_pton(AF_INET, "localhost", &sin.sin_addr);

    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) != -1) {
	cout << "socket has been created successfully" << endl;

	if(!connect(sockfd, (struct sockaddr *) &sin, sizeof(sin))) {
	    cout << "connected to localhost:1667" << endl;

	    cout << "daemon says: " << getresponse() << endl;

	    runcommand("hello");
	    runcommand("version");
	    runcommand("quit");

	    shutdown(sockfd, 2);
	    close(sockfd);

	    cout << "connection terminated" << endl;
	} else {
	    cout << "unable to connect to localhost:1667" << endl;
	}
    } else {
        perror("socket");
    }

    return 0;
}
