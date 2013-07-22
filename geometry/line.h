#pragma once
#include "point.h"

template <typename T>
class CLine
{
public:
	CLine(void);
	CLine(T A, T B, T C);
	CLine(CPoint<T> p1, CPoint<T> p2);
	~CLine(void);

	T GetA () {return a;};
	T GetB () {return b;};
	T GetC () {return c;};
	T GetD2 () {return d2;};

	T GetX (T y);
	T GetY (T x);

	CLine<T>& operator=(const CLine<T>& right) {
		//проверка на самоприсваивание
        if (this == &right) {return *this;}
		a = right.a;
		b = right.b;
		c = right.c;
		d2 = right.d2;
        return *this;
    }

private:
	T a;
	T b;
	T c;
	T d2; //  d2 = a*a+b*b
public:
	// расстояние от точки до прямой
	T Distance(CPoint<T> p);
	// расчет проекции точки на прямую
	CPoint<T> Projection(CPoint<T> p);
	// возвращает точку пересечения с линией
	CPoint<T> Cross(CLine<T> l);
	// // проверка на параллельность
	bool IsCross(CLine<T> l);
};


template <typename T>
CLine<T>::CLine(void)
{
	a=b=c=d2=0;
}

template <typename T>
CLine<T>::~CLine(void)
{
}

template <typename T>
CLine<T>::CLine(T A, T B, T C)
{
	a=A;
	b=B;
	c=C;
	d2=a*a+b*b;
}

template <typename T>
CLine<T>::CLine(CPoint<T> p1, CPoint<T> p2)
{
	a=p1.y-p2.y;
	b=p2.x-p1.x;
	c=p1.x*p2.y-p2.x*p1.y;
	d2=a*a+b*b;
}

// расстояние от точки до прямой
template <typename T>
T CLine<T>::Distance(CPoint<T> p)
{
	if (a==0 && b==0) return 0; // прямая неопределена

	CPoint<T> dp;
	dp=Projection (p);
	dp-=p;
	return dp.Abs ();
}

// расчет проекции точки на прямую
template <typename T>
CPoint<T> CLine<T>::Projection(CPoint<T> p)
{
	if (a==0 && b==0) return CPoint<T>(0,0); // прямая неопределена

	return CPoint<T> ((b*b*p.x-c*a-a*b*p.y)/d2, (a*a*p.y-a*b*p.x-c*b)/d2); // вот такая закорюка

}

// возвращает точку пересечения с линией
template <typename T>
CPoint<T> CLine<T>::Cross(CLine<T> l)
{
	if (!IsCross(l)) return CPoint<T>(0,0); // если не пересеклись возвращаем что-нибудь

	T x, y;
	x=(l.GetC()*b-c*l.GetB())/(a*l.GetB()-b*l.GetA());
	y=(l.GetA()*c-a*l.GetC())/(a*l.GetB()-b*l.GetA());
	return CPoint<T> (x, y);
}

// // проверка на параллельность
template <typename T>
bool CLine<T>::IsCross(CLine<T> l)
{
	return (a*l.b-b*l.a)!=0;
}

template <typename T>
T CLine<T>::GetX (T y)
{
	return (a==0)?0:(y*b+c)/(-a);
}

template <typename T>
T CLine<T>::GetY (T x)
{
	return (b==0)?0:(x*a+c)/(-b);
}


