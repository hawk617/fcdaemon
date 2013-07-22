#ifndef VIDEO_H
#define VIDEO_H

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "geometry/geometry.h"

using namespace std;
using namespace cv;

#define MAX_SECT 5000


typedef struct vConfig 
{
	double hAngle; // угол обзора камеры по горизонту
	double vAngle; // угол обзора по вертикали
	double dvAngle; // наклон камеры отн. горизонта
	double Alt; // высота над полом
	Point FrameSize; // размер кадра с видеокамеры
	double vDist; // расстояние до экрана
	double hDist;
	int y_hor;
} vConfig;


class VideoCam
{
public:
	VideoCam();
	~VideoCam();

	VideoCam(double h_angle, double v_angle, double dv_angle=0);

	// инициализация необходимых констант
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

	// рассчет точки поворота за угол
	CDPoint vDirectionRotate ();
	CDPoint vDirectionRotate (Mat& image, bool vis,Mat& visimg);

	// рассчет точки прохода проема
	CDPoint vDirectionAperture (Mat& image, bool vis, Mat& visimg);

	void vFilterImage ( Mat& source, Mat& dest);

private:
	vConfig config;
	VideoCapture cap;
	Mat tempImg;
	bool vis;
	CDSection sect [MAX_SECT];
public:
	Mat visimg;
	Mat capt_img;
	//****************************************************************************
	//*    Функции поиска по изображению (возвращают достоверность результата)   *
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