/*
 * net.cpp
 *
 *  Created on: 23.07.2013
 *      Author: Hawk
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <syslog.h>
#include <signal.h>

#include <vector>
#include <string>
#include <fstream>

#include "net.h"
#include "log.h"

void NetDaemon::sighandler(int signum) {
    waitpid(0, 0, WNOHANG);
}

int NetDaemon::start() {
    struct sigaction sa;

    pid = fork();

    switch(pid) {
	case 0:
	    memset(&sa, 0, sizeof(sa));
	    sa.sa_handler = &sighandler;
	    sigaction(SIGCHLD, &sa, 0);
	    WriteLog ("[NETDAEMON] Start listening...\n");
	    mainloop();
	    exit(0);
	case -1:
		WriteLog ("[NETDAEMON] fork() error.\n");
	    break;
	default:
		char* s = new char[100];
		sprintf (s, "[NETDAEMON] Net daemon started whith PID %d\n", pid);
		WriteLog(s);
		delete s;
	    break;
    }
    return pid;
}

void NetDaemon::mainloop() {
    int sockfd, fd;
    struct sockaddr_in sa;
    socklen_t n;

    sockfd = socket(PF_INET, SOCK_STREAM, 0);

    if(sockfd != -1) {
	memset(&sa, 0, sizeof(sa));

	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(1667);

	if(bind(sockfd, (struct sockaddr *) &sa, sizeof(sa)) != -1) {
	    while(1) {
		if(!listen(sockfd, 5)) {
		    n = sizeof(sa);
		    if((fd = accept(sockfd, (struct sockaddr *) &sa, &n)) != -1) {
			syslog(LOG_NOTICE, "connection from %s", inet_ntoa(sa.sin_addr));

			if(!fork()) {
			    operate(fd);
			}
		    }
		}
	    }
	}
    }
}

void NetDaemon::operate(int fd) {
    char c;
    bool finished;
    std::string cmd, answer;

    finished = false;

    answer = "the simple tcp daemon is ready\n";
    write(fd, answer.c_str(), answer.size());

    while(!finished) {
	cmd = answer = "";

	/*
	*
	* Read a line from the client side
	*
	*/

	while(!finished) {
	    finished = read(fd, &c, 1) != 1;

	    if(c == '\n') break; else
	    if(c != '\r') cmd += c;
	}

	/*
	*
	* If there is a command and no socket error
	* appeared in the previous cycle, try to perform
	* some processing.
	*
	*/

	if(!finished && !cmd.empty()) {
	    if(cmd == "hello") {
		answer = "my daemonic greatings";
	    } else if(cmd == "version") {
		answer = "a simple tcp daemon, version 1.0";
	    } else if(cmd == "quit") {
		shutdown(fd, 2);
		close(fd);
		finished = true;
	    }
	}

	/*
	*
	* Send an answer if we've got something to answer with.
	*
	*/

	if(!finished && !answer.empty()) {
	    answer += "\n";
	    write(fd, answer.c_str(), answer.size());
	}
    }

    exit(0);
}

void NetDaemon::kill() {

}

