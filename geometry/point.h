#ifndef POINT_H_
#define POINT_H_

#include <math.h>

template <typename T>
class CRect
{
public:
	CRect() {r=l=t=b=0;};
	CRect (T right, T left, T bottom, T top) {r=right;l=left;t=top;b=bottom;};
	T r, l, t, b;
};



template <typename T>
class CPoint
{
public:
	CPoint(void);
	~CPoint(void);
	CPoint(T, T);

	void AddPoint(CPoint<T> pt);
	T Abs(void); // длинна вектора от 0 до точки
	bool InRect (CRect<T>);
	bool InRect (T left, T right, T bottom, T top);
	CPoint& operator+=(const CPoint<T> &right) {
		x+=right.x;
		y+=right.y;
		return *this;
	};

	CPoint& operator-=(const CPoint<T> &right) {
		x-=right.x;
		y-=right.y;
		return *this;
	};

	bool operator==(const CPoint<T> &right) {
		return (x==right.x) && (y==right.y);
	};

	const CPoint<T> operator+(const CPoint<T> &right) {
		return CPoint<T>(x + right.x, y + right.y);
	};


	const CPoint<T> operator-(const CPoint<T>&right) {
		return CPoint<T>(x - right.x, y - right.y);
	};

	const CPoint<T> operator*(const T &right) {
		return CPoint<T>(x * right, y * right);
	};

	const CPoint<T> operator/(const T &right) {
		return CPoint<T>(x / right, y / right);
	};

	CPoint<T>& operator*=(const T &right) {
		x *= right;
		y *= right;
		return *this;
	};

	CPoint<T>& operator/=(const T &right) {
		x /= right;
		y /= right;
		return *this;
	};

	CPoint& operator=(const CPoint &right) {
		//проверка на самоприсваивание
        if (this == &right) {return *this;}
		x = right.x;
		y = right.y;
        return *this;
    }

	T x;
	T y;
};

template <typename T>
inline CPoint<T>::CPoint(void)
	: x(0)
	, y(0)
{
}

template <typename T>
CPoint<T>::~CPoint(void)
{
}

template <typename T>
inline CPoint<T>::CPoint(T sx, T sy)
{
	x=sx;
	y=sy;
}

template <typename T>
void CPoint<T>::AddPoint(CPoint<T> pt)
{
	x+=pt.x;
	y+=pt.y;
}

template <typename T>
T CPoint<T>::Abs(void)
{
	return sqrt (x*x+y*y);
}

template <typename T>
bool CPoint<T>::InRect (CRect<T> r)
{
	return r.l<x && x<r.r && r.b<y && y<r.t;
}

template <typename T>
bool CPoint<T>::InRect (T left, T right, T bottom, T top)
{
	return left<x && x<right && bottom<y && y<top;
}

#endif
