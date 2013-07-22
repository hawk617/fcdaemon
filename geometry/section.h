#pragma once
#include "line.h"

template <typename T>
class CSection : public CLine<T>
{
public:
	CSection(void);
	CSection(CPoint<T> pb, CPoint<T> pe);

	~CSection(void);
	T Distance(CPoint<T> p);
	CPoint<T> GetP1 () { return p1;};
	CPoint<T> GetP2 () { return p2;};
	bool InRect (CRect<T>);
	bool InRect (T left, T right, T bottom, T top);

	CSection<T>& operator=(const CSection<T>& right) { 
        if (this == &right) {return *this;}

		p1 = right.p1;
		p2 = right.p2;
		d = right.d;
		CLine<T>::operator=(CLine<T>(p1,p2));
        return *this;
    }


private:
	CPoint<T> p1, p2;
	T d;

public:
	bool InLine(CPoint<T> p);
public:
	bool IsCross(CLine<T> l);
	CPoint<T> Cross(CLine<T> l);
	bool IsCross(CSection<T> s);
	bool IsSimilar(CSection<T> s, T dist, T ang);
	CPoint<T> Cross(CSection<T> s);
	CPoint<T> DistPoint(CPoint<T> p);
	T tan(void);
	T sin(void);
	T cos(void);
};


template <typename T>
CSection<T>::CSection(void)
{
	p1=p2=CPoint<T> (0,0);
}

template <typename T>
CSection<T>::CSection(CPoint<T> pb, CPoint<T> pe):CLine<T>(pb, pe)
{
	p1=pb;
	p2=pe;
	d=sqrt (CLine<T>::GetD2());
}

template <typename T>
CSection<T>::~CSection(void)
{
}

template <typename T>
T CSection<T>::Distance(CPoint<T> p)
{
	CPoint<T> dp1, dp2;
	T d1, d2;
	dp1 = Projection (p);
	if (InLine (dp1)) return CLine<T>::Distance (p);
	dp1=p-p1;
	dp2=p-p2;
	d1=dp1.Abs();
	d2=dp2.Abs();
	return (d1>d2)?d2:d1;
}

template <typename T>
bool CSection<T>::InLine(CPoint<T> p)
{
	return	((p2.x>=p.x && p.x>=p1.x) || (p1.x>=p.x && p.x>=p2.x)) &&
		((p2.y>=p.y && p.y>=p1.y) || (p1.y>=p.y && p.y>=p2.y));
}

template <typename T>
bool CSection<T>::IsCross(CLine<T> l)
{
	if (!CLine<T>::IsCross(l)) return false;
	return InLine (CLine<T>::Cross(l));
}

template <typename T>
CPoint<T> CSection<T>::Cross(CLine<T> l)
{
	if (!IsCross(l))return CPoint<T>(0,0);
	return CLine<T>::Cross(l);
}

template <typename T>
bool CSection<T>::IsCross(CSection<T> s)
{
	if ( !IsCross( (CLine<T>)s ) ) return false;
	return s.InLine (Cross ((CLine<T>)s ));
}

template <typename T>
CPoint<T> CSection<T>::Cross(CSection<T> s)
{
	if (!IsCross (s)) return CPoint<T>(0,0);
	return CLine<T>::Cross( (CLine<T>)s );
}


template <typename T>
CPoint<T> CSection<T>::DistPoint(CPoint<T> p)
{
	CPoint<T> dp1, dp2;
	T d1, d2;
	dp1 = Projection (p);
	if (InLine (dp1)) return dp1;
	dp1=p-p1;
	dp2=p-p2;
	d1=dp1.Abs();
	d2=dp2.Abs();
	return (d1>d2)?dp2:dp1;
}


template <typename T>
T CSection<T>::tan(void)
{
	return 0;
}

template <typename T>
inline T CSection<T>::sin(void)
{
	return CLine<T>::GetA()/d;
}

template <typename T>
inline T CSection<T>::cos(void)
{
	return -CLine<T>::GetB()/d;
}

template <typename T>
inline bool CSection<T>::InRect (CRect<T> r)
{
	return p1.InRect(r) && p2.InRect(r);
}

template <typename T>
inline bool CSection<T>::InRect (T left, T right, T bottom, T top)
{
	return p1.InRect (left, right, bottom, top) && p2.InRect (left, right, bottom, top);
}

template <typename T>
inline bool CSection<T>::IsSimilar(CSection<T> s, T dist, T ang)
{
	return abs (s.cos())-abs(this->cos())<ang;
}
