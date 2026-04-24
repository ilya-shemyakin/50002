#ifndef POINT_H
#define POINT_H

struct Point {
    double x;
    double y;

    Point() :
        x(0.0),
        y(0.0)
    {
    }

    Point(double xValue, double yValue) :
        x(xValue),
        y(yValue)
    {
    }
};

#endif