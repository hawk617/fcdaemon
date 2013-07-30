/*
 * fcdaemon.cpp
 *
 *  Created on: 30.07.2013
 *      Author: Hawk617
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <signal.h>

#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

#include "fcdaemon.h"

using namespace std;

void fcdaemon::sighandler(int signum)
{
    waitpid(0, 0, WNOHANG);
}

void fcdaemon::daemonize() {
    int pid;
    struct sigaction sa;

    cout << "Daemonizing ourselves.. " << flush;
    pid = fork();

    switch(pid) {
	case 0:
	    setsid();
	    chdir("/");
	    close(0);
	    close(1);
	    close(2);
	    memset(&sa, 0, sizeof(sa));
	    sa.sa_handler = &sighandler;
	    sigaction(SIGCHLD, &sa, 0);
	    openlog("fcdaemon", 0, LOG_USER);
	    mainloop();
	    closelog();
	    exit(0);
	case -1:
	    cout << "fork() error" << endl;
	    break;
	default:
	    cout << "ok. PID=" << pid << endl;
	    break;
    }
}

void fcdaemon::mainloop()
{
    int sockfd, cd;
    int tfd[2];
    vector<int> fd;
    struct sockaddr_in sa;
    socklen_t n;
    if(pipe(tfd) == -1)
    {
    	syslog(LOG_ERR, "pipe failed");
    	exit(1);
    }


    sockfd = socket(PF_INET, SOCK_STREAM, 0);

    if(sockfd != -1)
    {
		memset(&sa, 0, sizeof(sa));

		sa.sin_family = AF_INET;
		sa.sin_addr.s_addr = htonl(INADDR_ANY);
		sa.sin_port = htons(port);

		if(bind(sockfd, (struct sockaddr *) &sa, sizeof(sa)) != -1)
		{
			while(1)
			{
				if(!listen(sockfd, 5))
				{
					n = sizeof(sa);
					if((cd = accept(sockfd, (struct sockaddr *) &sa, &n)) != -1)
					{
						syslog(LOG_NOTICE, "connection from %s", inet_ntoa(sa.sin_addr));

						if(!fork()) {
							operate(cd);
						}
					}
				}
			}
		}
		else
		{
			syslog(LOG_ERR, "Error bind()");

		}
    }
}

void fcdaemon::operate(int fd) {
    char c;
    bool finished;
    string cmd, answer;

    finished = false;

    answer = "the simple tcp daemon is ready\n";
    write(fd, answer.c_str(), answer.size());

    while(!finished)
    {
    	cmd = answer = "";

		while(!finished)
		{
			finished = read(fd, &c, 1) != 1;
			if(c == '\n') break;
			else if(c != '\r') cmd += c;
		}

		if(!finished && !cmd.empty())
		{
			if(cmd == "hello")
			{
				answer = "my daemonic greatings";
			}
			else if(cmd == "version")
			{
				answer = "a simple tcp daemon, version 1.0";
			}
			else if(cmd == "quit")
			{
				shutdown(fd, 2);
				close(fd);
				finished = true;
			}
		}

		if(!finished && !answer.empty())
		{
			answer += "\n";
			write(fd, answer.c_str(), answer.size());
		}
    }

    exit(0);
}

void fcdaemon::exec(bool i)
{
	if (i) daemonize();
	else mainloop ();
}



