#include "polygon.h"
#include "commands.h"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <vector>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "ERROR: filename is not provided\n";
        return 1;
    }
    std::ifstream file(argv[1]);
    if (!file)
    {
        std::cerr << "ERROR: cannot open file\n";
        return 1;
    }
    std::vector< Polygon > polygons;
    while (!file.eof())
    {
        std::copy(std::istream_iterator< Polygon >(file),
            std::istream_iterator< Polygon >(), std::back_inserter(polygons));
        if (!file.eof())
        {
            file.clear();
        }
    }
    processCommands(polygons, std::cin, std::cout);
    return 0;
}
