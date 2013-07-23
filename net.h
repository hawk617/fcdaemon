/*
 * net.h
 *
 *  Created on: 23.07.2013
 *      Author: Hawk
 */

#ifndef NET_H_
#define NET_H_


class NetDaemon {
    protected:
	void daemonize();
	void mainloop();

	void operate(int fd);

	static void sighandler(int signum);

    public:
	int start ();
	void kill ();
    private:
	int pid;
};

#endif /* NET_H_ */
