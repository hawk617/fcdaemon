/*
 * net.cpp
 *
 *  Created on: 23.07.2013
 *      Author: Hawk
 */
#include <wait.h>
#include "net.h"


static int npid=0;

int NetStart()
{
	return 0;
}

void NetStop()
{
	if (npid) kill (npid, SIGQUIT);
}




