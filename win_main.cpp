#include "win.h"

#ifdef WIN_TEST
#include "video.h"
#include "stat.h"
#include "fl.h"
#include "geometry/geometry.h"


int win_main(int argc, char* argv[])
{

	FL fl;
	fl.InitFL();
	fl.test_start();
	return 0;
}

#endif