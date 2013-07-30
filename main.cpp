#include "stdafx.h"

#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif

#include "fcdaemon.h"

int main (int argc, char** argv)
{
	fcdaemon d;
	d.exec ();
}

