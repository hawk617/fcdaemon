/*
 * fc.cpp
 *
 *  Created on: 23.07.2013
 *      Author: Hawk
 */
#include <wait.h>
#include "fc.h"

static int wpid=0;

int FCStart()
{
	return 0;
}

void FCStop()
{
	if (wpid) kill (wpid, SIGQUIT);
}


