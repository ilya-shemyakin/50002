#ifndef COMPOSITE_SHAPE_H
#define COMPOSITE_SHAPE_H

#include "shape.h"
#include <vector>
#include <memory>
#include <string>

class CompositeShape : public Shape {
public:
    void addShape(std::shared_ptr<Shape> shape);
    const std::vector<std::shared_ptr<Shape>>& getShapes() const;

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double coefficient) override;
    std::string getName() const override;

private:
    std::vector<std::shared_ptr<Shape>> shapes_;
};

#endif
