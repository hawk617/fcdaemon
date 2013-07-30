/*
 * log.cpp
 *
 *  Created on: 23.07.2013
 *      Author: hawk
 */
#include <stdio.h>
#include <string>
#include <time.h>
#include "log.h"

using namespace std;

static FILE* log_file=0;

void WriteLog(string Msg)
{
	time_t t;
	struct tm *t_m;
	t=time(NULL);
	t_m=localtime(&t);
	//cout<<"Local time is: "<<t_m->tm_hour<<":"<<t_m->tm_min<<":"<<t_m->tm_sec;

	if (log_file==0) log_file=fopen(LOG_FILE, "a");
	fprintf(log_file, "[%2d-%2d-%4d %2d:%2d:%2d]", t_m->tm_mday, t_m->tm_mon,
			t_m->tm_year, t_m->tm_hour, t_m->tm_min, t_m->tm_sec);
	fprintf(log_file, Msg.c_str());
	delete t_m;
}

void SetLogFile(const char* Filename)
{
	log_file = fopen(Filename, "a");
}





