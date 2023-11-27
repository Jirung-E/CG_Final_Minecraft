#ifndef __Line_h__
#define __Line_h__

#include "Point.h"
#include "Vector.h"


namespace Math {
    class Line {
    public:
        Point point;
        Vector vector;
        
    public:
        Line(Point point, Vector vector);
        Line(Vector vector);

    public:
        bool isExistPointOfContactWith(const Line& other) const;
        Point* pointOfContactWith(const Line& other) const;
        Point footOfPerpendicularFrom(const Point& point) const;
        float distanceTo(const Point& point) const;
        float distanceTo(const Line& other) const;
        bool isParallelTo(const Line& other) const;

    };
        
    float distanceBetween(const Point& point, const Line& line);
    float distanceBetween(const Line& line, const Point& point);
    float distanceBetween(const Line& line1, const Line& line2);
    bool isParallel(const Line& line1, const Line& line2);
    Point footOfPerpendicular(const Line& line, const Point& point);
        
    Point rotate(const Point& point, const Line& axis, float radian);
}

#endif