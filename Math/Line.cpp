#include "Line.h"

#include "MathDefines.h"

using namespace Math;


Line::Line(Point point, Vector vector) : point { point }, vector { vector } {
    
}

Line::Line(Vector vector) : Line { Point {0, 0, 0}, vector } {
    
}


bool Line::isExistPointOfContactWith(const Line& other) const {
    if(getDistanceTo(other) == 0.0f) {
        return true;
    }
    return false;
}

Point* Line::getPointOfContactWith(const Line& other) const {
    if(isExistPointOfContactWith(other)) {
        Point H { getFootOfPerpendicularFrom(other.point) };
        float SH { distanceBetween(other.point, H) };
        float theta = angleBetween(vector, other.vector);
        if(theta == PI/2) {
            return new Point { H };
        }
        else if(theta > PI/2.0f) {
            float AH { SH * tanf(PI/2.0f - theta) };
            Vector HS { other.point - H };
            float d = other.vector * HS;
            if(d == 0) {
                return new Point { H };
			}
            else if(d > 0) {
                return new Point { H - vector.unit() * AH };
            }
            else {
                return new Point { H + vector.unit() * AH };
            }
		}
        else {
            float AH { SH * tanf(theta - PI/2.0f) };
			Vector HS { other.point - H };
			float d = other.vector * HS;
            if(d == 0) {
				return new Point { H };
            }
            else if(d > 0) {
				return new Point { H + vector.unit() * AH };
			}
            else {
				return new Point { H - vector.unit() * AH };
			}
        }
    }
    return nullptr;
}

Point Line::getFootOfPerpendicularFrom(const Point& point) const {
    return getFootOfPerpendicular(*this, point);
}

float Line::getDistanceTo(const Point& point) const {
    return getDistanceBetween(*this, point);
}

float Line::getDistanceTo(const Line& other) const {
    return getDistanceBetween(*this, other);
}

bool Line::isParallelTo(const Line& other) const {
    return isParallel(this->vector, other.vector);
}


float Line::getDistanceBetween(const Point& point, const Line& line) {
    return getDistanceBetween(line, point);
}

float Line::getDistanceBetween(const Line& line, const Point& point) {
    if(line.vector.magnitude() == 0.0f) {
        return Vector(line.point - point).magnitude();
    }
    Point H { getFootOfPerpendicular(line, point) };
    Vector AH { H - point };
    return AH.magnitude();
}

float Line::getDistanceBetween(const Line& line1, const Line& line2) {
    if(line1.point == line2.point) {
        return 0.0f;
    }
    if(isParallel(line1, line2)) {
        return getDistanceBetween(line1, line2.point);
    }
    Vector start_point_to_start_point { line2.point - line1.point };
    return abs(start_point_to_start_point * line1.vector.cross(line2.vector.unit()));
}

bool Line::isParallel(const Line& line1, const Line& line2) {
    if(line1.vector.magnitude() == 0.0f || line2.vector.magnitude() == 0.0f) {
        return true;
    }
    if(line1.vector.cross(line2.vector).magnitude() == 0.0f) {
        return true;
    }
    return false;
}

Point Line::getFootOfPerpendicular(const Line& line, const Point& point) {
    if(line.vector.magnitude() == 0.0f) {
        return line.point;
    }
    Point P { line.point };
    Vector V { line.vector };
    Point A { point };
    Vector PA { A - P};
    Vector add { V * ((PA * V) / (V*V)) };
    Point H { P + add };
    return H;
}


Point Line::rotate(const Point& point, const Line& axis, float radian) {
    Point p { axis.getFootOfPerpendicularFrom(point) };
    Vector v { point - p };
    Vector n { axis.vector.unit() };
    Vector result { (v*cos(radian)) + n*(1-cos(radian))*(v*n) + (n.cross(v) * sin(radian)) };
    return p + result;
}