#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <vector>
#include "polygon.h"

void processCommands(const std::vector< Polygon >& polygons, std::istream& in,
    std::ostream& out);

#endif
