#include "video.h"

VideoCam::VideoCam ()
{
}

VideoCam::~VideoCam ()
{
}

VideoCam::VideoCam (double h_angle, double v_angle, double dv_angle)
{

}

// ������������� ����������� ��������
void VideoCam::InitVideo (double h_angle, double v_angle, double dv_angle)
{
	config.FrameSize.x=720;
	config.FrameSize.y=480; // !!!!!!!!!!!
	config.hAngle=h_angle;
	config.vAngle=v_angle;
	config.dvAngle=dv_angle;
	config.Alt=0;
	config.vDist=config.FrameSize.x/(2*tan (v_angle));
	config.hDist=config.FrameSize.y/(2*tan (h_angle));
	config.y_hor=(int)(config.FrameSize.y/2 - config.vDist*tan (dv_angle));
}

void VideoCam::vSetAlt (double alt)
{
	config.Alt=alt;
}
void VideoCam::vSetDVAngle (double ang)
{
	config.dvAngle=ang;
	config.y_hor=(int)(config.FrameSize.x/2 - config.vDist*tan (ang));
}

// ������������ 
void VideoCam::vSetConfig (vConfig &conf)
{
	config=conf;
}

// ������ �����
int VideoCam::Capture ()
{
	Mat frame;
	cap >> frame; // get a new frame from camera

	capt_img=frame.clone();
	return 0;
}

int VideoCam::SetVideoSource(int i)
{
	cap.open(i);
	if(!cap.isOpened())  // check if we succeeded
		return -1;
	return 0;
}
int VideoCam::SetVideoSource(char* f)
{
	cap.open(f);
	if(!cap.isOpened())  // check if we succeeded
		return -1;
	return 0;
}

// �������� ������� �����
int VideoCam::Capture (Mat& image)
{
	capt_img=image.clone();
	return 0;
}

Mat VideoCam::GetSrcFrame ()
{
	return capt_img;
}

Mat VideoCam::GetCVFrame ()
{
	return capt_img;
}


CDPoint VideoCam::vDirectionRotate (Mat& image, bool vis, Mat& visimg)
{
	std::vector<cv::Vec4i> lines;

	vFilterImage (image, tempImg);

	cv::HoughLinesP( tempImg, lines, 1, CV_PI/180, 50, 50, 30 );
	int nsect=lines.size();

	for(unsigned int i = 0; i < lines.size(); i++ )
	{
		sect[i]=CDSection (CDPoint (lines[i][0], lines[i][1]), CDPoint (lines[i][2], lines[i][3]));
	}


	CDPoint min = CDPoint(0, config.FrameSize.x);
	CDSection ms, hs;
	double t_cos, t_sin, hg=config.FrameSize.x;
	for (int i=0; i<nsect; i++)
	{
		t_cos=sect[i].cos();
		t_sin=sect[i].sin();

		if (sect[i].InRect(0, config.FrameSize.y/2, 0, config.FrameSize.x))
			if(t_cos < 0 &&  0.5 < t_sin)
			{
				if (sect[i].GetP1().y<min.y) {min=sect[i].GetP1();ms=sect[i];};
				if (sect[i].GetP2().y<min.y) {min=sect[i].GetP2();ms=sect[i];};
				line( visimg, cv::Point(sect[i].GetP1().x, sect[i].GetP1().y), cv::Point(sect[i].GetP2().x, sect[i].GetP2().y), CV_RGB(0,255,0), 2, CV_AA, 0 );
				continue;
			}

			if (sect[i].InRect(0, config.FrameSize.y, config.y_hor, min.y) &&
				-0.05 <t_sin &&  t_sin< 0.05)
			{
				if ((sect[i].GetP1().y+sect[i].GetP2().y)/2<hg) {hg=(sect[i].GetP1().y+sect[i].GetP2().y)/2; hs=sect[i];};
			}
	}
	CDPoint to;
	to=(ms.GetP1().y>ms.GetP2().y)?ms.GetP2():ms.GetP1();
	to.y=(to.y+hg)/2;
	to.x=ms.GetX(to.y);

	if (vis)
	{
		cvtColor(tempImg, visimg, CV_GRAY2RGB);
		for(unsigned int i = 0; i < lines.size(); i++ )
		{
			line( visimg, cv::Point (lines[i][0], lines[i][1]), cv::Point (lines[i][2], lines[i][3]), CV_RGB(255,0,0), 2, CV_AA, 0 );
		}
		line( visimg, cv::Point(ms.GetP1().x, ms.GetP1().y), cv::Point(ms.GetP2().x, ms.GetP2().y), CV_RGB(0,255,0), 2, CV_AA, 0 );
		line( visimg, cv::Point(hs.GetP1().x, hs.GetP1().y), cv::Point(hs.GetP2().x, hs.GetP2().y), CV_RGB(200,0,150), 2, CV_AA, 0 );

		int radius=5;
		cv::circle(visimg,cv::Point(to.x, to.y),radius,CV_RGB(0,0,250),1,8);
		cv::line(visimg, cv::Point(to.x-radius/2, to.y-radius/2), cv::Point(to.x+radius/2, to.y+radius/2),CV_RGB(0,0,250),1,8);
		line(visimg, cv::Point(to.x-radius/2, to.y+radius/2), cv::Point(to.x+radius/2, to.y-radius/2),CV_RGB(0,0,250),1,8);
	}

	return to;
}

CDPoint VideoCam::vDirectionRotate ()
{
	std::vector<cv::Vec4i> lines;

	// ��������� ��������
	vFilterImage (capt_img, tempImg);

	// ���������� ������ �����
	HoughLinesP( tempImg, lines, 1, CV_PI/180, 50, 50, 30 );
	int nsect=lines.size();

	for(unsigned int i = 0; i < lines.size(); i++ )
	{
		sect[i]=CDSection (CDPoint (lines[i][0], lines[i][1]), CDPoint (lines[i][2], lines[i][3]));
	}

	CDPoint min = CDPoint(0, config.FrameSize.y);
	CDSection ms, hs;
	double t_cos, t_sin, hg=config.FrameSize.y;
	// ���������� ����� �����  ... ���� ����� � ����� �����
	for (int i=0; i<nsect; i++)
	{
		t_cos=sect[i].cos();
		t_sin=sect[i].sin();

		// ���������� ����� ������� ����� � ����
		if (sect[i].InRect(0, config.FrameSize.x/2, 0, config.FrameSize.y))
			if(t_cos < 0 &&  0.5 < t_sin)
			{
				if (sect[i].GetP1().y<min.y) {min=sect[i].GetP1();ms=sect[i];};
				if (sect[i].GetP2().y<min.y) {min=sect[i].GetP2();ms=sect[i];};
				line ( visimg, cv::Point(sect[i].GetP1().x, sect[i].GetP1().y), cv::Point(sect[i].GetP2().x, sect[i].GetP2().y), CV_RGB(0,255,0), 2, CV_AA, 0 );
				continue;
			}

			// ���������� ������� ��������� ����� � ����
			if (sect[i].InRect(0, config.FrameSize.y, config.y_hor, min.y) &&
				-0.05 <t_sin &&  t_sin< 0.05)
			{
				if ((sect[i].GetP1().y+sect[i].GetP2().y)/2<hg) {hg=(sect[i].GetP1().y+sect[i].GetP2().y)/2; hs=sect[i];};
			}
	}
	CDPoint to;
	to=(ms.GetP1().y>ms.GetP2().y)?ms.GetP2():ms.GetP1();
	to.y=(to.y+hg)/2;
	to.x=ms.GetX(to.y);

	// �� ������ ������������� ��������������� �������
	if (vis)
	{
		cvtColor(tempImg, visimg, CV_GRAY2RGB);
		for(unsigned int i = 0; i < lines.size(); i++ )
		{
			line( visimg, cv::Point (lines[i][0], lines[i][1]), cv::Point (lines[i][2], lines[i][3]), CV_RGB(255,0,0), 2, CV_AA, 0 );
		}
		line( visimg, cv::Point(ms.GetP1().x, ms.GetP1().y), cv::Point_<double>(ms.GetP2().x, ms.GetP2().y), CV_RGB(0,255,0), 2, CV_AA, 0 );
		line( visimg, cv::Point_<double>(hs.GetP1().x, hs.GetP1().y), cv::Point_<double>(hs.GetP2().x, hs.GetP2().y), CV_RGB(200,0,150), 2, CV_AA, 0 );

		int radius=5;
		circle(visimg,cv::Point_<double>(to.x, to.y),radius,CV_RGB(0,0,250),1,8);
		line(visimg, cv::Point_<double>(to.x-radius/2, to.y-radius/2), cv::Point_<double>(to.x+radius/2, to.y+radius/2),CV_RGB(0,0,250),1,8);
		line(visimg, cv::Point_<double>(to.x-radius/2, to.y+radius/2), cv::Point_<double>(to.x+radius/2, to.y-radius/2),CV_RGB(0,0,250),1,8);
	}

	return to;
}

// ������� ����� ������� ������
CDPoint VideoCam::vDirectionAperture (Mat& image, bool vis, Mat& visimg)
{
	std::vector<cv::Vec4i> lines;

	// ��������� ��������
	vFilterImage (image, tempImg);

	// ���������� ������ �����
	HoughLinesP( tempImg, lines, 1, CV_PI/180, 50, 50, 30 );
	int nsect=lines.size();

	for(unsigned int i = 0; i < lines.size(); i++ )
	{
		sect[i]=CDSection (CDPoint (lines[i][0], lines[i][1]), CDPoint (lines[i][2], lines[i][3]));
	}

	// ���������� ����� �����


	
	// ������ ���� ����
	if (vis)
	{
		for(unsigned int i = 0; i < lines.size(); i++ )
		{
			line ( visimg, cv::Point(lines[i][0],lines[i][1]), cv::Point(lines[i][2],lines[i][3]), CV_RGB(255,0,0), 2, CV_AA, 0 );
		}

	}

	return CDPoint();
}

void VideoCam::vFilterImage ( Mat& source, Mat& dest)
{
	Mat tmp=source.clone();
	Laplacian(source, tmp, 8);
	//smooth(tmp, tmp, CV_GAUSSIAN, 3, 3);
	cvtColor(tmp, dest, CV_RGB2GRAY);
	//cvAdaptiveThreshold (dst, dst, 250, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY,15, 3);
	Canny(dest, dest, 30, 150, 3);
}


// ����� ����� �������� �� �����������
int VideoCam::SearchRot(void)
{
	return 0;
}


// ����� ������� �� �����������
int VideoCam::SearchApert(void)
{
	return 0;
}


// t=1 mid landing point, t=2 start/stop landing point
int VideoCam::SearchLandingPoint(int t)
{
	vFilterImage (capt_img, tempImg);
	return 0;
}


// get reference to point, return accuracy
float VideoCam::GetDirection(CDPoint & dir)
{
	return 0;
}
