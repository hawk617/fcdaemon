//*******************************************
//*       Flight Logic                      *
//*******************************************
#ifndef _FL_H_
#define _FL_H_


#include "video.h"
//#include "geometry/geometry.h"

class FP
{
public:
	FP() {};
	~FP() {};

	typedef bool (*fp)();
	std::vector<fp> plan;
};

class FL
{
public:
	void start (const FP &p);
	void start ();
	void test_start ();
	void InitFL();

	VideoCam cam;
	std::vector<CDPoint> wp;
private:
	bool test_rot();
	bool test_apert();
	bool test_lp_1();
	bool test_lp_2();
	bool test_nop ();
	bool nop ();
	bool alarm_land ();
	bool alarm_stop ();
	bool stop ();

};

#endif
