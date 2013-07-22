#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "section.h"

typedef CSection<double> CDSection;
typedef CSection<float>  CFSection;
typedef CSection<int>    CISection;
typedef CSection<char>   CCSection;

typedef CLine<double> CDLine;
typedef CLine<float>  CFLine;
typedef CLine<int>    CILine;
typedef CLine<char>   CCLine;

typedef CRect<double> CDRect;
typedef CRect<float>  CFRect;
typedef CRect<int>    CIRect;
typedef CRect<char>   CCRect;

typedef CPoint<double> CDPoint;
typedef CPoint<float>  CFPoint;
typedef CPoint<int>    CIPoint;
typedef CPoint<char>   CCPoint;

#endif
