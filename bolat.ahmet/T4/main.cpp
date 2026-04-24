#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

#include "composite_shape.h"
#include "ellipse.h"
#include "rectangle.h"

void printSimpleShape(const Shape& shape) {
    Point center = shape.getCenter();
    std::cout << std::fixed << std::setprecision(2)
        << "[" << shape.getName()
        << ", (" << center.x << ", " << center.y << "), "
        << shape.getArea() << "]";
}

void printCompositeShape(const CompositeShape& composite) {
    Point center = composite.getCenter();
    const auto& shapes = composite.getShapes();

    std::cout << std::fixed << std::setprecision(2)
        << "[COMPOSITE, (" << center.x << ", " << center.y << "), "
        << composite.getArea() << ":\n";

    for (size_t i = 0; i < shapes.size(); ++i) {
        Point shapeCenter = shapes[i]->getCenter();

        std::cout << "  " << shapes[i]->getName()
            << ", (" << shapeCenter.x << ", " << shapeCenter.y << "), "
            << shapes[i]->getArea();

        if (i + 1 < shapes.size()) {
            std::cout << ",";
        }

        std::cout << "\n";
    }

    std::cout << "]";
}

void printShape(const Shape& shape) {
    const auto* composite = dynamic_cast<const CompositeShape*>(&shape);

    if (composite) {
        printCompositeShape(*composite);
    }
    else {
        printSimpleShape(shape);
    }
}

void printAllShapes(const std::vector<std::unique_ptr<Shape>>& shapes) {
    for (size_t i = 0; i < shapes.size(); ++i) {
        std::cout << "Figure " << i + 1 << ": ";
        printShape(*shapes[i]);
        std::cout << "\n";
    }
}

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Rectangle>(Point(-2.0, 1.0), Point(4.0, 5.0)));
    shapes.push_back(std::make_unique<Ellipse>(Point(7.5, -3.0), 2.5, 4.5));
    shapes.push_back(std::make_unique<Rectangle>(Point(-9.0, -4.0), Point(-3.0, 0.0)));
    shapes.push_back(std::make_unique<Ellipse>(Point(1.0, 8.0), 3.5, 2.0));
    shapes.push_back(std::make_unique<Ellipse>(Point(-6.0, 6.5), 1.5, 3.0));

    auto composite = std::make_unique<CompositeShape>();

    composite->addShape(std::make_unique<Rectangle>(Point(10.0, 10.0), Point(14.0, 13.0)));
    composite->addShape(std::make_unique<Ellipse>(Point(16.0, 12.0), 2.0, 3.0));
    composite->addShape(std::make_unique<Rectangle>(Point(13.0, 15.0), Point(18.0, 17.0)));

    shapes.push_back(std::move(composite));

    std::cout << "Before scaling:\n";
    printAllShapes(shapes);

    for (auto& shape : shapes) {
        shape->scale(2.0);
    }

    std::cout << "\nAfter scaling:\n";
    printAllShapes(shapes);

    return 0;
}