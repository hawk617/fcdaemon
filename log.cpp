/*
 * log.cpp
 *
 *  Created on: 23.07.2013
 *      Author: hawk
 */
#include <stdio.h>
#include "log.h"

static FILE* log_file=0;

void WriteLog(char* Msg)
{
	if (log_file==0) log_file=fopen(LOG_FILE, "a");
	fprintf(log_file, Msg);
}




