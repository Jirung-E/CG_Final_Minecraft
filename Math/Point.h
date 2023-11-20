#ifndef __Point_h__
#define __Point_h__

#include <ostream>


namespace Math {
    class Point {
    public:
        float x;
        float y;
        float z;
        float w;

    public:
        Point(float x, float y, float z = 0.0f);
        Point(const Point& point);
        Point();

        Point operator+(const Point& point) const;

        Point operator*(float n) const;
        Point operator/(float n) const;

        Point& operator+=(const Point& point);
        Point& operator*=(float n);
        Point& operator/=(float n);

        bool operator==(const Point& point) const;
        bool operator!=(const Point& point) const;
    };

    float distanceBetween(const Point& point1, const Point& point2);

    std::ostream& operator<<(std::ostream& os, Point p);
}

#endif
