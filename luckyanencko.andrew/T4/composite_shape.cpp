#include "composite_shape.h"
#include <stdexcept>

void CompositeShape::addShape(std::shared_ptr<Shape> shape) {
    if (!shape) {
        throw std::invalid_argument("Shape cannot be null");
    }
    shapes_.push_back(shape);
}

const std::vector<std::shared_ptr<Shape>>& CompositeShape::getShapes() const {
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
    if (shapes_.empty()) {
        return {0.0, 0.0};
    }
    
    Point firstCenter = shapes_[0]->getCenter();
    double minX = firstCenter.x;
    double minY = firstCenter.y;
    double maxX = firstCenter.x;
    double maxY = firstCenter.y;

    for (const auto& shape : shapes_) {
        Point p = shape->getCenter();
        if (p.x < minX) minX = p.x;
        if (p.x > maxX) maxX = p.x;
        if (p.y < minY) minY = p.y;
        if (p.y > maxY) maxY = p.y;
    }
    
    return {minX + (maxX - minX) / 2.0, minY + (maxY - minY) / 2.0};
}

void CompositeShape::move(double dx, double dy) {
    for (auto& shape : shapes_) {
        shape->move(dx, dy);
    }
}

void CompositeShape::scale(double coefficient) {
    if (coefficient <= 0) {
        throw std::invalid_argument("Invalid coefficient");
    }
    
    Point center = getCenter();
    for (auto& shape : shapes_) {
        Point shapeCenter = shape->getCenter();
        double dx = (shapeCenter.x - center.x) * (coefficient - 1.0);
        double dy = (shapeCenter.y - center.y) * (coefficient - 1.0);
        
        shape->move(dx, dy);
        shape->scale(coefficient);
    }
}

std::string CompositeShape::getName() const {
    return "COMPOSITE";
}
