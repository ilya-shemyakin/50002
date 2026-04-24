#include "rectangle.h"

#include <stdexcept>

Rectangle::Rectangle(const Point& bottomLeft, const Point& topRight) :
    bottomLeft_(bottomLeft),
    topRight_(topRight)
{
    if (topRight_.x <= bottomLeft_.x || topRight_.y <= bottomLeft_.y) {
        throw std::invalid_argument("Invalid rectangle points");
    }
}

double Rectangle::getArea() const {
    return (topRight_.x - bottomLeft_.x) * (topRight_.y - bottomLeft_.y);
}

Point Rectangle::getCenter() const {
    return Point(
        (bottomLeft_.x + topRight_.x) / 2.0,
        (bottomLeft_.y + topRight_.y) / 2.0
    );
}

void Rectangle::move(double dx, double dy) {
    bottomLeft_.x += dx;
    bottomLeft_.y += dy;
    topRight_.x += dx;
    topRight_.y += dy;
}

void Rectangle::scale(double factor) {
    if (factor <= 0.0) {
        throw std::invalid_argument("Scale factor must be positive");
    }

    Point center = getCenter();

    bottomLeft_.x = center.x + (bottomLeft_.x - center.x) * factor;
    bottomLeft_.y = center.y + (bottomLeft_.y - center.y) * factor;
    topRight_.x = center.x + (topRight_.x - center.x) * factor;
    topRight_.y = center.y + (topRight_.y - center.y) * factor;
}

std::string Rectangle::getName() const {
    return "RECTANGLE";
}

std::pair<Point, Point> Rectangle::getBounds() const {
    return std::pair<Point, Point>(bottomLeft_, topRight_);
}