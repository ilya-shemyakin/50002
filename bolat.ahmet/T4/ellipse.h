#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "shape.h"

#include <utility>

class Ellipse : public Shape {
public:
    Ellipse(const Point& center, double verticalRadius, double horizontalRadius);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double factor) override;
    std::string getName() const override;
    std::pair<Point, Point> getBounds() const override;

private:
    Point center_;
    double verticalRadius_;
    double horizontalRadius_;
};

#endif