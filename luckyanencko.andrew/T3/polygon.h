#ifndef POLYGON_H
#define POLYGON_H

#include <iostream>
#include <vector>

struct Point
{
    int x;
    int y;
};

struct Polygon
{
    std::vector< Point > points;
};

bool operator==(const Point& lhs, const Point& rhs);
bool operator==(const Polygon& lhs, const Polygon& rhs);

double area(const Polygon& poly);

std::istream& operator>>(std::istream& in, Point& dest);
std::istream& operator>>(std::istream& in, Polygon& dest);

std::ostream& operator<<(std::ostream& out, const Point& src);
std::ostream& operator<<(std::ostream& out, const Polygon& src);

#endif
