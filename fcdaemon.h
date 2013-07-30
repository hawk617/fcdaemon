/*
 * fcdaemon.h
 *
 *  Created on: 30.07.2013
 *      Author: Hawk617
 */

#ifndef DAEMON_H_
#define DAEMON_H_

#define DEF_PORT 3333

class fcdaemon
{
protected:
	void daemonize();
	void mainloop();

	void operate(int fd);

	static void sighandler(int signum);

public:
	void exec(bool i=true);
	fcdaemon () {port=DEF_PORT;};
private:
	int port;
};




#endif /* DAEMON_H_ */
