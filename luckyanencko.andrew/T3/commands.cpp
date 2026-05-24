#include "commands.h"

#include <algorithm>
#include <cctype>
#include <functional>
#include <iomanip>
#include <iterator>
#include <map>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class iofmtguard
{
public:
    iofmtguard(std::basic_ios< char >& s)
        : s_(s), fill_(s.fill()), precision_(s.precision()), flags_(s.flags())
    {
    }
    ~iofmtguard()
    {
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(flags_);
    }
private:
    std::basic_ios< char >& s_;
    char fill_;
    std::streamsize precision_;
    std::ios::fmtflags flags_;
};

static bool isEven(const Polygon& p)
{
    return p.points.size() % 2 == 0;
}

static bool isOdd(const Polygon& p)
{
    return p.points.size() % 2 == 1;
}

static std::size_t parseVertexCount(const std::string& s)
{
    bool digits = !s.empty() && std::all_of(s.begin(), s.end(),
        [](char c) { return std::isdigit(static_cast< unsigned char >(c)) != 0; });
    if (!digits)
    {
        throw std::logic_error("not a number");
    }
    std::size_t n = std::stoul(s);
    if (n < 3)
    {
        throw std::logic_error("too few vertices");
    }
    return n;
}

static std::function< bool(const Polygon&) > parityOrCount(const std::string& sub)
{
    if (sub == "EVEN")
    {
        return isEven;
    }
    if (sub == "ODD")
    {
        return isOdd;
    }
    std::size_t n = parseVertexCount(sub);
    return [n](const Polygon& p) { return p.points.size() == n; };
}

static double sumAreaIf(const std::vector< Polygon >& v,
    std::function< bool(const Polygon&) > pred)
{
    return std::accumulate(v.begin(), v.end(), 0.0,
        [&](double acc, const Polygon& p)
        {
            return pred(p) ? acc + area(p) : acc;
        });
}

static bool isRectangle(const Polygon& p)
{
    if (p.points.size() != 4)
    {
        return false;
    }
    const std::vector< Point >& q = p.points;
    auto edge = [](const Point& a, const Point& b)
    {
        return Point{ a.x - b.x, a.y - b.y };
    };
    auto dot = [](const Point& a, const Point& b)
    {
        return a.x * b.x + a.y * b.y;
    };
    Point e0 = edge(q[1], q[0]);
    Point e1 = edge(q[2], q[1]);
    Point e2 = edge(q[3], q[2]);
    Point e3 = edge(q[0], q[3]);
    return dot(e0, e1) == 0 && dot(e1, e2) == 0 && dot(e2, e3) == 0
        && dot(e3, e0) == 0;
}

static std::vector< Point > normalized(const Polygon& p)
{
    int minX = std::min_element(p.points.begin(), p.points.end(),
        [](const Point& a, const Point& b) { return a.x < b.x; })->x;
    int minY = std::min_element(p.points.begin(), p.points.end(),
        [](const Point& a, const Point& b) { return a.y < b.y; })->y;
    std::vector< Point > shifted;
    std::transform(p.points.begin(), p.points.end(),
        std::back_inserter(shifted), [minX, minY](const Point& q)
        {
            return Point{ q.x - minX, q.y - minY };
        });
    std::sort(shifted.begin(), shifted.end(),
        [](const Point& a, const Point& b)
        {
            return a.x < b.x || (a.x == b.x && a.y < b.y);
        });
    return shifted;
}

static void cmdArea(const std::vector< Polygon >& v, std::istream& in,
    std::ostream& out)
{
    std::string sub;
    in >> sub;
    iofmtguard guard(out);
    out << std::fixed << std::setprecision(1);
    if (sub == "MEAN")
    {
        if (v.empty())
        {
            throw std::logic_error("no polygons");
        }
        out << sumAreaIf(v, [](const Polygon&) { return true; }) / v.size() << "\n";
        return;
    }
    out << sumAreaIf(v, parityOrCount(sub)) << "\n";
}

static void cmdCount(const std::vector< Polygon >& v, std::istream& in,
    std::ostream& out)
{
    std::string sub;
    in >> sub;
    out << std::count_if(v.begin(), v.end(), parityOrCount(sub)) << "\n";
}

static const Polygon& extremeBy(const std::vector< Polygon >& v,
    const std::string& sub, bool wantMax)
{
    std::function< bool(const Polygon&, const Polygon&) > less;
    if (sub == "AREA")
    {
        less = [](const Polygon& a, const Polygon& b) { return area(a) < area(b); };
    }
    else if (sub == "VERTEXES")
    {
        less = [](const Polygon& a, const Polygon& b)
        {
            return a.points.size() < b.points.size();
        };
    }
    else
    {
        throw std::logic_error("bad parameter");
    }
    if (wantMax)
    {
        return *std::max_element(v.begin(), v.end(), less);
    }
    return *std::min_element(v.begin(), v.end(), less);
}

static void printExtreme(const std::vector< Polygon >& v, std::istream& in,
    std::ostream& out, bool wantMax)
{
    std::string sub;
    in >> sub;
    if (v.empty())
    {
        throw std::logic_error("no polygons");
    }
    const Polygon& best = extremeBy(v, sub, wantMax);
    if (sub == "AREA")
    {
        iofmtguard guard(out);
        out << std::fixed << std::setprecision(1) << area(best) << "\n";
    }
    else
    {
        out << best.points.size() << "\n";
    }
}

static void cmdRects(const std::vector< Polygon >& v, std::istream& in,
    std::ostream& out)
{
    (void)in;
    out << std::count_if(v.begin(), v.end(), isRectangle) << "\n";
}

static void cmdSame(const std::vector< Polygon >& v, std::istream& in,
    std::ostream& out)
{
    Polygon query;
    in >> query;
    if (!in)
    {
        throw std::logic_error("bad polygon");
    }
    std::vector< Point > target = normalized(query);
    out << std::count_if(v.begin(), v.end(),
        [&](const Polygon& p)
        {
            return p.points.size() == query.points.size()
                && normalized(p) == target;
        }) << "\n";
}

void processCommands(const std::vector< Polygon >& polygons, std::istream& in,
    std::ostream& out)
{
    std::map< std::string,
        std::function< void(const std::vector< Polygon >&, std::istream&,
            std::ostream&) > > cmds;
    cmds["AREA"] = cmdArea;
    cmds["COUNT"] = cmdCount;
    cmds["MAX"] = std::bind(printExtreme, std::placeholders::_1,
        std::placeholders::_2, std::placeholders::_3, true);
    cmds["MIN"] = std::bind(printExtreme, std::placeholders::_1,
        std::placeholders::_2, std::placeholders::_3, false);
    cmds["RECTS"] = cmdRects;
    cmds["SAME"] = cmdSame;

    std::string name;
    while (in >> name)
    {
        std::string rest;
        std::getline(in, rest);
        std::istringstream args(rest);
        try
        {
            cmds.at(name)(polygons, args, out);
        }
        catch (const std::exception&)
        {
            out << "<INVALID COMMAND>\n";
        }
    }
}
