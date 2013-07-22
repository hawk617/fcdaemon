#ifndef VIDEO_H
#define VIDEO_H

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "geometry/geometry.h"

//using namespace std;
//using namespace cv;

using cv::Mat;

#define MAX_SECT 5000


typedef struct vConfig 
{
	double hAngle; // ���� ������ ������ �� ���������
	double vAngle; // ���� ������ �� ���������
	double dvAngle; // ������ ������ ���. ���������
	double Alt; // ������ ��� �����
	cv::Point FrameSize; // ������ ����� � �����������
	double vDist; // ���������� �� ������
	double hDist;
	int y_hor;
} vConfig;


class VideoCam
{
public:
	VideoCam();
	~VideoCam();

	VideoCam(double h_angle, double v_angle, double dv_angle=0);

	// ������������� ����������� ��������
	void InitVideo (double h_angle, double v_angle, double dv_angle=0);

	void vSetAlt (double alt);
	void vSetDVAngle (double ang);

	// video capture
	int Capture ();
	int SetVideoSource (int i);
	int SetVideoSource (char* f);

	// video capture emulation
	int Capture (Mat& image);

	Mat GetSrcFrame ();

	Mat GetCVFrame ();

	// config 
	void vSetConfig (vConfig &conf);

	// ������� ����� �������� �� ����
	CDPoint vDirectionRotate ();
	CDPoint vDirectionRotate (Mat& image, bool vis,Mat& visimg);

	// ������� ����� ������� ������
	CDPoint vDirectionAperture (Mat& image, bool vis, Mat& visimg);

	void vFilterImage ( Mat& source, Mat& dest);

private:
	vConfig config;
	cv::VideoCapture cap;
	Mat tempImg;
	bool vis;
	CDSection sect [MAX_SECT];
public:
	Mat visimg;
	Mat capt_img;
	//****************************************************************************
	//*    ������� ������ �� ����������� (���������� ������������� ����������)   *
    //****************************************************************************
	// search rotation point
	int SearchRot(void);
	// search aperture in pict
	int SearchApert(void);
	// search landing point (0-intermediate, 1-finish)
	int SearchLandingPoint(int t);
	// get reference to point, return accuracy
	float GetDirection(CDPoint & dir);
};

#endif
