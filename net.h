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
	void mainloop();

	void operate(int fd);

	static void sighandler(int signum);

    public:
	int start ();
	void stop ();
    private:
	int pid;
};

#endif /* NET_H_ */
