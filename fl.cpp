//*******************************************
//*       Flight Logic                      *
//*******************************************
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "fl.h"

using cv::Mat;

void FL::start (const FP &p)
{

}

void FL::InitFL()
{
	cam.InitVideo(50, 30);
	if (!wp.empty()) wp.clear();
}

void FL::test_start ()
{
	test_rot();
	test_nop();
	test_apert();
	test_nop();
	test_lp_1();
	test_nop();
	test_lp_2();
}

bool FL::test_rot()
{
	printf ("[fl]: test rotate\n");
	
	cv::namedWindow("test rot original",CV_WINDOW_AUTOSIZE);
	cv::namedWindow("test rot lines",CV_WINDOW_AUTOSIZE);

	VideoCam cam;
	cam.InitVideo(M_PI*70/180, M_PI*50/180, M_PI*20/180);
	cam.SetVideoSource("test.mp4");

	CDPoint pRot;
	CDPoint pRot_tmp;
	int radius=5;

	while(1){
		cam.Capture();

		pRot_tmp=cam.vDirectionRotate();
		if (pRot_tmp.x>1 && pRot_tmp.y>1)
		{
			pRot_tmp*=0.05;
			pRot+=pRot_tmp;
			pRot/=1.05;
		}

		cv::circle (cam.visimg,cv::Point_<double>(pRot.x, pRot.y),radius,CV_RGB(255,255,250),1,8);
		cv::line (cam.visimg, cv::Point_<double>(pRot.x-radius/2, pRot.y-radius/2), cv::Point_<double>(pRot.x+radius/2, pRot.y+radius/2),CV_RGB(255,255,250),1,8);
		cv::line (cam.visimg, cv::Point_<double>(pRot.x-radius/2, pRot.y+radius/2), cv::Point_<double>(pRot.x+radius/2, pRot.y-radius/2),CV_RGB(255,255,250),1,8);

		cv::imshow ( "test rot original", cam.capt_img );
		cv::imshow ( "test rot lines", cam.visimg );


		char c = cvWaitKey(33);//33
		if (c == 27) { 
			break;
		}
		else if (c==' ')
		{
			cvWaitKey();
		}
	}

	cv::destroyWindow("test rot original");
	cv::destroyWindow("test rot lines");
	return true;
}

bool FL::test_apert()
{
	printf ("[fl]: test aperture\n");

	cv::namedWindow("test apert original",CV_WINDOW_AUTOSIZE);
	cv::namedWindow("test apert lines",CV_WINDOW_AUTOSIZE);

	VideoCam cam;
	cam.InitVideo(M_PI*70/180, M_PI*50/180, M_PI*20/180);
	cam.SetVideoSource("test.mp4");

	CDPoint pRot;
	CDPoint pRot_tmp;
	int radius=5;

	while(1){
		cam.Capture();

		//pRot_tmp=cam.vDirectionAperture();
		if (pRot_tmp.x>1 && pRot_tmp.y>1)
		{
			pRot_tmp*=0.05;
			pRot+=pRot_tmp;
			pRot/=1.05;
		}

		circle (cam.visimg,cv::Point_<double>(pRot.x, pRot.y),radius,CV_RGB(255,255,250),1,8);
		line (cam.visimg, cv::Point_<double>(pRot.x-radius/2, pRot.y-radius/2), cv::Point_<double>(pRot.x+radius/2, pRot.y+radius/2),CV_RGB(255,255,250),1,8);
		line (cam.visimg, cv::Point_<double>(pRot.x-radius/2, pRot.y+radius/2), cv::Point_<double>(pRot.x+radius/2, pRot.y-radius/2),CV_RGB(255,255,250),1,8);

		imshow ( "test apert original", cam.capt_img );
		imshow ( "test apert lines", cam.visimg );


		char c = cvWaitKey(33);//33
		if (c == 27) { 
			break;
		}
		else if (c==' ')
		{
			cvWaitKey();
		}
	}

	cv::destroyWindow("test apert original");
	cv::destroyWindow("test apert lines");
	return true;
}

bool FL::test_lp_1()
{
	printf ("[fl]: test 1 landing point\n");


	cvNamedWindow("test lp original",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("test lp lines",CV_WINDOW_AUTOSIZE);

	VideoCam cam;
	cam.InitVideo(M_PI*70/180, M_PI*50/180, M_PI*20/180);
	cam.SetVideoSource("lp.mp4");

	CDPoint pDir;
	CDPoint pDir_tmp;
	int radius=5;
	float ac=0;

	while(1){
		cam.Capture();
		cam.SearchLandingPoint(1);
		ac=cam.GetDirection(pDir_tmp);
		if (pDir_tmp.x>1 && pDir_tmp.y>1)
		{
			printf ("\t[lp1]:moment lp (%d;%d) accuracy %d, ", (int)(pDir_tmp.x), (int)(pDir_tmp.y), (int)ac);
			pDir_tmp*=ac;
			pDir*=(1-ac);
			pDir+=pDir_tmp;
			printf ("current lp (%d;%d)\n", (int)(pDir.x), (int)(pDir.y));
		}

		imshow ( "test lp original", cam.capt_img );
		imshow ( "test lp lines", cam.visimg );


		char c = cvWaitKey(33);//33
		if (c == 27) { 
			break;
		}
		else if (c==' ')
		{
			cvWaitKey();
		}
	}

	cvDestroyWindow("test lp original");
	cvDestroyWindow("test lp lines");
	return true;
}

bool FL::test_lp_2()
{
	printf ("[fl]: test 2 landing point\n");
	return true;
}

bool FL::nop ()
{
	return true;
}

bool FL::test_nop ()
{
	printf ("[fl]: nop\n");
	return true;
}

bool FL::alarm_land ()
{
	return true;
}

bool FL::alarm_stop ()
{
	return true;
}



