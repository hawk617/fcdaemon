#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <stdio.h>
#include <syslog.h>
#include <signal.h>

#include <vector>
#include <string>
#include <fstream>

#define COMMIT_LIMIT	100

class udpdaemon {
    protected:
	ofstream flog;
	vector<string> collected;

	void daemonize();
	void mainloop();
	void commit();

	static void sighandler(int signum);

    public:
	void exec();
};

void udpdaemon::sighandler(int signum) {
    waitpid(0, 0, WNOHANG);
}

void udpdaemon::daemonize() {
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

	    openlog("daemon2", 0, LOG_USER);
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

void udpdaemon::mainloop() {
    int sockfd, size;
    fd_set fds;
    struct sockaddr_in sa;
    char buf[512];

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);

    if(sockfd != -1) {
	memset(&sa, 0, sizeof(sa));
	
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(1666);

	if(bind(sockfd, (struct sockaddr *) &sa, sizeof(sa)) != -1) {
	    while(1) {
		FD_ZERO(&fds);
		FD_SET(sockfd, &fds);

		if(select(sockfd+1, &fds, 0, 0, 0) < 0) {
		    syslog(LOG_NOTICE, "select() error");
		}

    		if(FD_ISSET(sockfd, &fds)) {
		    size = recv(sockfd, (void *) buf, 512, 0);

		    if(size >= 0) {
		        buf[size] = 0;
		        collected.push_back(buf);

		        if(collected.size() >= COMMIT_LIMIT) {
		    	    commit();
			}
		    }
		}
	    }
	}
    }
}

void udpdaemon::commit() {
    ofstream f;
    vector<string>::iterator i;
    char buf[512];
    int cpid;

    switch(cpid = fork()) {
	case 0:
	    do sprintf(buf, "%s/udpdaemon.%lu", getenv("HOME"), time(0));
	        while(!access(buf, F_OK));

	    f.open(buf);

	    if(f.is_open()) {
		for(i = collected.begin(); i != collected.end(); i++)
		    f << *i << endl;

		f.close();
	    }

	    exit(0);

	case -1:
	    break;

	default:
	    collected.clear();

	    syslog(LOG_INFO, "%d packets received, forked child with PID=%lu",
		COMMIT_LIMIT, cpid);

	    break;
    }
}

void udpdaemon::exec() {
    daemonize();
}

int main() {
    udpdaemon d;
    d.exec();
    return 0;
}
