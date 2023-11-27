#include "Line.h"

#include "MathDefines.h"

using namespace Math;


Line::Line(Point point, Vector vector) : point { point }, vector { vector } {
    
}

Line::Line(Vector vector) : Line { Point {0, 0, 0}, vector } {
    
}


bool Line::isExistPointOfContactWith(const Line& other) const {
    if(distanceTo(other) == 0.0f) {
        return true;
    }
    return false;
}

Point* Line::pointOfContactWith(const Line& other) const {
    if(isExistPointOfContactWith(other)) {
        Point H { footOfPerpendicularFrom(other.point) };
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

Point Line::footOfPerpendicularFrom(const Point& point) const {
    return footOfPerpendicular(*this, point);
}

float Line::distanceTo(const Point& point) const {
    return distanceBetween(*this, point);
}

float Line::distanceTo(const Line& other) const {
    return distanceBetween(*this, other);
}

bool Line::isParallelTo(const Line& other) const {
    return isParallel(this->vector, other.vector);
}


float Math::distanceBetween(const Point& point, const Line& line) {
    return distanceBetween(line, point);
}

float Math::distanceBetween(const Line& line, const Point& point) {
    if(line.vector.magnitude() == 0.0f) {
        return Vector(line.point - point).magnitude();
    }
    Point H { footOfPerpendicular(line, point) };
    Vector AH { H - point };
    return AH.magnitude();
}

float Math::distanceBetween(const Line& line1, const Line& line2) {
    if(line1.point == line2.point) {
        return 0.0f;
    }
    if(isParallel(line1, line2)) {
        return distanceBetween(line1, line2.point);
    }
    Vector start_point_to_start_point { line2.point - line1.point };
    return abs(start_point_to_start_point * line1.vector.cross(line2.vector.unit()));
}

bool Math::isParallel(const Line& line1, const Line& line2) {
    if(line1.vector.magnitude() == 0.0f || line2.vector.magnitude() == 0.0f) {
        return true;
    }
    if(line1.vector.cross(line2.vector).magnitude() == 0.0f) {
        return true;
    }
    return false;
}

Point Math::footOfPerpendicular(const Line& line, const Point& point) {
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


Point Math::rotate(const Point& point, const Line& axis, float radian) {
    Point p { axis.footOfPerpendicularFrom(point) };
    Vector v { point - p };
    Vector n { axis.vector.unit() };
    Vector result { (v*cos(radian)) + n*(1-cos(radian))*(v*n) + (n.cross(v) * sin(radian)) };
    return p + result;
}