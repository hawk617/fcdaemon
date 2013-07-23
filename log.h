/*
 * log.h
 *
 *  Created on: 23.07.2013
 *      Author: hawk
 */

#ifndef LOG_H_
#define LOG_H_

#include <string>

#define LOG_FILE "/var/log/copter_daemon.log"

using namespace std;

void WriteLog(string Msg);
void SetLogFile(const char* Filename);


#endif /* LOG_H_ */
