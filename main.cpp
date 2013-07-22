#include "stdafx.h"

#ifdef WIN_TEST
#include "win.h"
#endif
#ifndef WIN_TEST
#include "linux.h"
#endif



int main(int argc, char** argv)
{
#ifdef WIN_TEST
        return win_main (argc, argv);
#endif

#ifndef WIN_TEST
        return linux_main (argc, argv);
#endif
}
