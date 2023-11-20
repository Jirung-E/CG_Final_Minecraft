#ifndef __Vector_h__
#define __Vector_h__

#include <ostream>

#include "Point.h"


namespace Math {
    class Vector {
    public:
        float x;
        float y;
        float z;

    public:
        Vector(float x, float y, float z = 0.0);
        Vector(const Vector& vector);
        Vector(const Point& point);
        Vector();

        Vector operator+(const Vector& vector) const;
        Vector operator-(const Vector& vector) const;
        float operator*(const Vector& vector) const;            // scalar product

        Vector operator*(float n) const;
        Vector operator/(float n) const;

        Vector operator-() const;

        Vector& operator+=(const Vector& vector);
        Vector& operator-=(const Vector& vector);
        Vector& operator*=(float n);
        Vector& operator/=(float n);

        bool operator==(const Vector& vector) const;
        bool operator!=(const Vector& vector) const;

        Vector unit() const;
        Vector cross(const Vector& vector) const;
        float magnitude() const;
    };

    float angleBetween(const Vector& vector1, const Vector& vector2);
    bool isParallel(const Vector& vector1, const Vector& vector2);
    bool isOrthogonal(const Vector& vector1, const Vector& vector2);

    std::ostream& operator<<(std::ostream& os, Vector v);

    Vector operator*(float n, const Vector& vector);

    Vector operator-(const Point& point1, const Point& point2);
    Point operator+(const Point& point, const Vector& vector);
    Point operator-(const Point& point, const Vector& vector);
    Point& operator+=(Point& point, const Vector& vector);
    Point& operator-=(Point& point, const Vector& vector);
}

#endif