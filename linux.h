#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif

#include "stdafx.h"

#ifndef WIN_TEST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <execinfo.h>
#include <unistd.h>
#include <errno.h>
#include <wait.h>
#include "geometry/geometry.h"



// ����� ��� ��������� ����������� ���-�� �������� ������������
#define FD_LIMIT                        1024*10

// ��������� ��� ����� ���������� ��������
#define CHILD_NEED_WORK                 1
#define CHILD_NEED_TERMINATE    2

#define PID_FILE "/var/run/copter_daemon.pid"
#define LOG_FILE "/var/log/copter_daemon.log"

static FILE* log_file=0;

// ������� ������ ����
void WriteLog(char* Msg, ...);

// ������� �������� �������
int LoadConfig(char* FileName);

// ������� ������� �������� ������ ������
// � ������ ������ �������� � ������
int ReloadConfig();

// ������� ��� ��������� ������� � ������������ ��������
void DestroyWorkThread();

// ������� ������� �������������� ������� ������
int InitWorkThread();

// ������� ��������� ��������
static void signal_error(int sig, siginfo_t *si, void *ptr);

// ������� ��������� ������������� ���-�� ������������ ������� ����� ���� �������
int SetFdLimit(int MaxFd);

int WorkProc();

void SetPidFile(char* Filename);

void SetLogFile(char* Filename);


int MonitorProc();

int linux_main (int argc, char** argv);

#endif
