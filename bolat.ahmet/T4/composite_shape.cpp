#include "composite_shape.h"
#include <algorithm>
#include <stdexcept>

void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    if (!shape) {
        throw std::invalid_argument("Cannot add empty shape");
    }
    if (shape->getName() == "COMPOSITE") {
        throw std::invalid_argument("CompositeShape must contain only simple shapes");
    }
    shapes_.push_back(std::move(shape));
}

const std::vector<std::unique_ptr<Shape>>& CompositeShape::getShapes() const {
    return shapes_;
}

double CompositeShape::getArea() const {
    double totalArea = 0.0;
    for (const auto& shape : shapes_) {
        totalArea += shape->getArea();
    }
    return totalArea;
}

Point CompositeShape::getCenter() const {
    auto bounds = getBounds();
    return Point(
        (bounds.first.x + bounds.second.x) / 2.0,
        (bounds.first.y + bounds.second.y) / 2.0
    );
}

void CompositeShape::move(double dx, double dy) {
    for (auto& shape : shapes_) {
        shape->move(dx, dy);
    }
}

void CompositeShape::scale(double factor) {
    if (factor <= 0.0) {
        throw std::invalid_argument("Scale factor must be positive");
    }
    if (shapes_.empty()) {
        return;
    }

    Point compositeCenter = getCenter();

    for (auto& shape : shapes_) {
        Point oldCenter = shape->getCenter();
        Point newCenter(
            compositeCenter.x + (oldCenter.x - compositeCenter.x) * factor,
            compositeCenter.y + (oldCenter.y - compositeCenter.y) * factor
        );

        shape->move(newCenter.x - oldCenter.x, newCenter.y - oldCenter.y);
        shape->scale(factor);
    }
}

std::string CompositeShape::getName() const {
    return "COMPOSITE";
}

std::pair<Point, Point> CompositeShape::getBounds() const {
    if (shapes_.empty()) {
        return { Point(), Point() };
    }

    auto firstBounds = shapes_.front()->getBounds();
    double minX = firstBounds.first.x;
    double minY = firstBounds.first.y;
    double maxX = firstBounds.second.x;
    double maxY = firstBounds.second.y;

    for (size_t i = 1; i < shapes_.size(); ++i) {
        auto bounds = shapes_[i]->getBounds();
        minX = std::min(minX, bounds.first.x);
        minY = std::min(minY, bounds.first.y);
        maxX = std::max(maxX, bounds.second.x);
        maxY = std::max(maxY, bounds.second.y);
    }

    return { Point(minX, minY), Point(maxX, maxY) };
}
