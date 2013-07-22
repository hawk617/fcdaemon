<<<<<<< HEAD
#include "stdafx.h"

#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <execinfo.h>
#include <unistd.h>
#include <errno.h>
#include <wait.h>
//#include "geometry/geometry.h"



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



int main (int argc, char** argv)
{
	int status;
	int pid;

	// ���� ���������� ��������� ������ ������ ����, �� ������� ��� ������������ ������
	if (argc != 2)
	{
		printf("Usage: ./copter_daemon filename.cfg\n");
		return -1;
	}

	// ��������� ���� ������������
	status = LoadConfig(argv[1]);

	if (!status) // ���� ��������� ������ �������� �������
	{
		printf("Error: Load config failed\n");
		return -1;
	}

	// ������� �������
	pid = fork();

	if (pid == -1) // ���� �� ������� ��������� �������
	{
		// ������� �� ����� ������ � � ��������
		printf("Start Daemon Error: %s\n", strerror(errno));

		return -1;
	}
	else if (!pid) // ���� ��� �������
	{
		// ������ ��� ��� ����������� � �������� �������
		// ��������� ���������� ��� ���� ���� �� ����������� �����,
		// ����� � ��� ����� ���� �������� � ������� �������
		umask(0);

		// ������ ����� �����, ����� �� �������� �� ��������
		setsid();

		// ��������� � ������ �����, ���� �� ����� �� �������, �� ����� ���� ��������.
		// � ������� � ���������������� ������
		chdir("/");

		// ��������� ����������� �����/������/������, ��� ��� ��� ��� ������ �� �����������
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);

		// ������ ������� ����� ������������ �������� �� ���������
		status = MonitorProc();

		return status;
	}
	else // ���� ��� ��������
	{
		// �������� ������, �.�. �������� ���� ������ (������ ������) �� ���������
		return 0;
	}
}


// ������� ������ ����
void WriteLog(char* Msg, ...)
{
	if (log_file==0) log_file=fopen(LOG_FILE, "a");
	fprintf(log_file, Msg);
}

// ������� �������� �������
int LoadConfig(char* FileName)
{
	// ��� ������ ���� ��� ��� �������� �������
	return 1;
}

// ������� ������� �������� ������ ������
// � ������ ������ �������� � ������
int ReloadConfig()
{
	// ��� �������
	return 1;
}
// ������� ��� ��������� ������� � ������������ ��������
void DestroyWorkThread()
{
	// ��� ������ ���� ��� ������� ��������� ��� ������ �
	// ��������� ��������� �������
}

// ������� ������� �������������� ������� ������
int InitWorkThread()
{
	// ��� �������
	return 1;
}

// ������� ��������� ��������
static void signal_error(int sig, siginfo_t *si, void *ptr)
{
	void*  ErrorAddr;
	void*  Trace[16];
	int    x;
	int    TraceSize;
	char** Messages;

	// ������� � ��� ��� �� ������ ������
	WriteLog("[DAEMON] Signal: %s, Addr: 0x%0.16X\n", strsignal(sig), si->si_addr);


#if __WORDSIZE == 64 // ���� ���� ����� � 64 ������ ��
	// ������� ����� ���������� ������� ������� ������
	ErrorAddr = (void*)((ucontext_t*)ptr)->uc_mcontext.gregs[REG_RIP];
#else
	// ������� ����� ���������� ������� ������� ������
	ErrorAddr = (void*)((ucontext_t*)ptr)->uc_mcontext.gregs[REG_EIP];
#endif

	// ���������� backtrace ����� �������� ���� ���� �������
	TraceSize = backtrace(Trace, 16);
	Trace[1] = ErrorAddr;

	// ������� ����������� ����������
	Messages = backtrace_symbols(Trace, TraceSize);
	if (Messages)
	{
		WriteLog("== Backtrace ==\n");

		// ������� � ���
		for (x = 1; x < TraceSize; x++)
		{
			WriteLog("%s\n", Messages[x]);
		}

		WriteLog("== End Backtrace ==\n");
		free(Messages);
	}

	WriteLog("[DAEMON] Stopped\n");

	// ��������� ��� ������� ������ � ��������� ������� �� ��� ����
	DestroyWorkThread();

	// �������� ������� � ����� ��������� �����������
	exit(CHILD_NEED_WORK);
}

// ������� ��������� ������������� ���-�� ������������ ������� ����� ���� �������
int SetFdLimit(int MaxFd)
{
	struct rlimit lim;
	int           status;

	// ������� ������� ����� �� ���-�� �������� ������������
	lim.rlim_cur = MaxFd;
	// ������� ������������ ����� �� ���-�� �������� ������������
	lim.rlim_max = MaxFd;

	// ��������� ��������� ���-��
	status = setrlimit(RLIMIT_NOFILE, &lim);

	return status;
}

int WorkProc()
{
	struct sigaction sigact;
	sigset_t         sigset;
	int              signo;
	int              status;

	// ������� �� ������� � ��������� ����� ������������ ����� ���������
	// ��������� ��� ����� �������� ����������� ���������� �� �������
	sigact.sa_flags = SA_SIGINFO;
	// ������ ������� ���������� ��������
	sigact.sa_sigaction = signal_error;

	sigemptyset(&sigact.sa_mask);

	// ��������� ��� ���������� �� �������

	sigaction(SIGFPE, &sigact, 0); // ������ FPU
	sigaction(SIGILL, &sigact, 0); // ��������� ����������
	sigaction(SIGSEGV, &sigact, 0); // ������ ������� � ������
	sigaction(SIGBUS, &sigact, 0); // ������ ����, ��� ��������� � ���������� ������

	sigemptyset(&sigset);

	// ��������� ������� ������� ����� �������
	// ������ ��������� �������� �������������
	sigaddset(&sigset, SIGQUIT);

	// ������ ��� ��������� �������� ������������� � ���������
	sigaddset(&sigset, SIGINT);

	// ������ ������� ���������� ��������
	sigaddset(&sigset, SIGTERM);

	// ���������������� ������ ������� �� ����� ������������ ��� ���������� �������
	sigaddset(&sigset, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sigset, NULL);

	// ��������� ������������ ���-�� ������������ ������� ����� �������
	SetFdLimit(FD_LIMIT);

	// ������� � ���, ��� ��� ����� ���������
	WriteLog("[DAEMON] Started\n");

	// ��������� ��� ������� ������
	status = InitWorkThread();
	if (!status)
	{
		// ���� �������� ���������
		for (;;)
		{
			// ���� ��������� ���������
			sigwait(&sigset, &signo);

			// ���� �� ��������� ���������� �������
			if (signo == SIGUSR1)
			{
				// ������� ������
				status = ReloadConfig();
				if (status == 0)
				{
					WriteLog("[DAEMON] Reload config failed\n");
				}
				else
				{
					WriteLog("[DAEMON] Reload config OK\n");
				}
			}
			else // ���� �����-���� ������ ������, �� ������ �� �����
			{
				break;
			}
		}

		// ��������� ��� ������� ������ � ��������� ������� �� ��� ����
		DestroyWorkThread();
	}
	else
	{
		WriteLog("[DAEMON] Create work thread failed\n");
	}

	WriteLog("[DAEMON] Stopped\n");

	// ������ ��� �� ��������� �����������
	return CHILD_NEED_TERMINATE;
}

void SetPidFile(char* Filename)
{
	FILE* f;

	f = fopen(Filename, "w+");
	if (f)
	{
		fprintf(f, "%u", getpid());
		fclose(f);
	}
}
void SetLogFile(char* Filename)
{
	log_file = fopen(Filename, "a");
}

int MonitorProc()
{
	int       pid;
	int       status;
	int       need_start = 1;
	sigset_t  sigset;
	siginfo_t siginfo;

	// ����������� ������� ������� ����� ������������
	sigemptyset(&sigset);

	// ������ ��������� �������� �������������
	sigaddset(&sigset, SIGQUIT);

	// ������ ��� ��������� �������� ������������� � ���������
	sigaddset(&sigset, SIGINT);

	// ������ ������� ���������� ��������
	sigaddset(&sigset, SIGTERM);

	// ������ ���������� ��� ��������� ������� ��������� �������
	sigaddset(&sigset, SIGCHLD);

	// ������ ���������� ��� ��������� ������� ��������� �������
	sigaddset(&sigset, SIGCHLD);

	// ���������������� ������ ������� �� ����� ������������ ��� ���������� �������
	sigaddset(&sigset, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sigset, NULL);

	// ������ ������� ������ ���� � ����� PID'��
	SetPidFile(PID_FILE);
	SetLogFile(LOG_FILE);

	// ����������� ���� ������
	for (;;)
	{
		// ���� ���������� ������� �������
		if (need_start)
		{
			// ������ �������
			pid = fork();
		}

		need_start = 1;

		if (pid == -1) // ���� ��������� ������
		{
			// ������� � ��� ��������� �� ����
			WriteLog("[MONITOR] Fork failed (%s)\n", strerror(errno));
		}
		else if (!pid) // ���� �� �������
		{
			// ������ ��� ����������� � �������

			// �������� ������� ���������� �� ������ ������
			status = WorkProc();

			// �������� �������
			exit(status);
		}
		else // ���� �� ��������
		{
			// ������ ��� ����������� � ��������

			// ������� ����������� �������
			sigwaitinfo(&sigset, &siginfo);

			// ���� ������ ������ �� �������
			if (siginfo.si_signo == SIGCHLD)
			{
				// �������� ������ ����������
				wait(&status);

				// ����������� ������ � ���������� ���
				status = WEXITSTATUS(status);

				// ���� ������� �������� ������ � ����� ��������� � ���, ��� ��� ����� ������ ��������
				if (status == CHILD_NEED_TERMINATE)
				{
					// ������� � ��� �������� �� ����
					WriteLog("[MONITOR] Childer stopped\n");

					// ������� ����
					break;
				}
				else if (status == CHILD_NEED_WORK) // ���� ��������� ������������� �������
				{
					// ������� � ��� ������ �������
					WriteLog("[MONITOR] Childer restart\n");
				}
			}
			else if (siginfo.si_signo == SIGUSR1) // ���� ������ ������ ��� ���������� ������������� ������
			{
				kill(pid, SIGUSR1); // �������� ��� �������
				need_start = 0; // ��������� ���� ��� ��� �� ���� ��������� ������� ������
			}
			else // ���� ������ �����-���� ������ ��������� ������
			{
				// ������� � ��� ���������� � ��������� �������
				WriteLog("[MONITOR] Signal %s\n", strsignal(siginfo.si_signo));

				// ����� �������
				kill(pid, SIGTERM);
				status = 0;
				break;
			}
		}
	}

	// ������� � ���, ��� �� ������������
	WriteLog("[MONITOR] Stopped\n");

	// ������ ���� � PID'��
	unlink(PID_FILE);

	return status;
}
