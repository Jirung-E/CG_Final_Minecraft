#include "Point.h"

#include <cmath>

using namespace Math;


Point::Point(float x, float y, float z) : x { x }, y { y }, z { z }, w { 1 } {

}

Point::Point(const Point& point) : Point { point.x, point.y, point.z } {

}

Point::Point() : Point { 0, 0, 0 } {

}


Point Point::operator+(const Point& point) const {
    return Point { x + point.x, y + point.y, z + point.z };
}


Point Point::operator*(float n) const {
    return Point { x*n, y*n, z*n };
}

Point Point::operator/(float n) const {
    return Point { x/n, y/n, z/n };
}


Point& Point::operator+=(const Point& point) {
    x += point.x;
    y += point.y;
    z += point.z;
    return *this;
}

Point& Point::operator*=(float n) {
    x *= n;
    y *= n;
    z *= n;
    return *this;
}

Point& Point::operator/=(float n) {
    x /= n;
    y /= n;
    z /= n;
    return *this;
}


bool Point::operator==(const Point& point) const {
    if(x == point.x && y == point.y && z == point.z) {
        return true;
    }
    return false;
}

bool Point::operator!=(const Point& point) const {
    return !(*this == point);
}


float Math::distanceBetween(const Point& point1, const Point& point2) {
    return (float)sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2) + pow(point1.z - point2.z, 2));
}


std::ostream& Math::operator<<(std::ostream& os, Point p) {
    os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    return os;
}
