/*
 * log.cpp
 *
 *  Created on: 23.07.2013
 *      Author: hawk
 */
#include <stdio.h>
#include <string>
#include "log.h"

using namespace std;

static FILE* log_file=0;

void WriteLog(string Msg)
{
	if (log_file==0) log_file=fopen(LOG_FILE, "a");
	fprintf(log_file, Msg.c_str());
}

void SetLogFile(const char* Filename)
{
	log_file = fopen(Filename, "a");
}





