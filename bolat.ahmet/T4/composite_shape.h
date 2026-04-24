#ifndef COMPOSITE_SHAPE_H
#define COMPOSITE_SHAPE_H

#include "shape.h"

#include <memory>
#include <utility>
#include <vector>

class CompositeShape : public Shape {
public:
    CompositeShape() = default;
    CompositeShape(const CompositeShape&) = delete;
    CompositeShape& operator=(const CompositeShape&) = delete;
    CompositeShape(CompositeShape&&) = default;
    CompositeShape& operator=(CompositeShape&&) = default;
    ~CompositeShape() override = default;

    void addShape(std::unique_ptr<Shape> shape);
    const std::vector<std::unique_ptr<Shape>>& getShapes() const;

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double factor) override;
    std::string getName() const override;
    std::pair<Point, Point> getBounds() const override;

private:
    std::vector<std::unique_ptr<Shape>> shapes_;
};

#endif