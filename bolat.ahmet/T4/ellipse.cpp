#include "ellipse.h"

#include <stdexcept>

Ellipse::Ellipse(const Point& center, double verticalRadius, double horizontalRadius) :
    center_(center),
    verticalRadius_(verticalRadius),
    horizontalRadius_(horizontalRadius)
{
    if (verticalRadius_ <= 0.0 || horizontalRadius_ <= 0.0) {
        throw std::invalid_argument("Ellipse radii must be positive");
    }
}

double Ellipse::getArea() const {
    const double PI = 3.14159265358979323846;

    return PI * verticalRadius_ * horizontalRadius_;
}

Point Ellipse::getCenter() const {
    return center_;
}

void Ellipse::move(double dx, double dy) {
    center_.x += dx;
    center_.y += dy;
}

void Ellipse::scale(double factor) {
    if (factor <= 0.0) {
        throw std::invalid_argument("Scale factor must be positive");
    }

    verticalRadius_ *= factor;
    horizontalRadius_ *= factor;
}

std::string Ellipse::getName() const {
    return "ELLIPSE";
}

std::pair<Point, Point> Ellipse::getBounds() const {
    return std::pair<Point, Point>(
        Point(center_.x - horizontalRadius_, center_.y - verticalRadius_),
        Point(center_.x + horizontalRadius_, center_.y + verticalRadius_)
    );
}