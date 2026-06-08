#include "polygon.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>

struct DelimiterIO
{
    char expected;
};

static std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    char c = 0;
    in >> c;
    if (in && c != dest.expected)
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

bool operator==(const Point& lhs, const Point& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator==(const Polygon& lhs, const Polygon& rhs)
{
    return lhs.points == rhs.points;
}

double area(const Polygon& poly)
{
    const std::vector< Point >& p = poly.points;
    double cross = std::inner_product(p.begin(), p.end() - 1, p.begin() + 1,
        0.0, std::plus< double >(),
        [](const Point& a, const Point& b)
        {
            return static_cast< double >(a.x) * b.y
                - static_cast< double >(b.x) * a.y;
        });
    cross += static_cast< double >(p.back().x) * p.front().y
        - static_cast< double >(p.front().x) * p.back().y;
    return std::fabs(cross) / 2.0;
}

std::istream& operator>>(std::istream& in, Point& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    Point tmp{ 0, 0 };
    in >> DelimiterIO{ '(' } >> tmp.x >> DelimiterIO{ ';' }
       >> tmp.y >> DelimiterIO{ ')' };
    if (in)
    {
        dest = tmp;
    }
    return in;
}

std::istream& operator>>(std::istream& in, Polygon& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    std::string line;
    std::getline(in, line);
    std::istringstream stream(line);

    std::size_t count = 0;
    stream >> count;
    if (!stream || count < 3)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::vector< Point > tmp;
    std::copy(std::istream_iterator< Point >(stream),
        std::istream_iterator< Point >(), std::back_inserter(tmp));

    if (tmp.size() != count || !stream.eof())
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    dest.points = tmp;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Point& src)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }
    out << '(' << src.x << ';' << src.y << ')';
    return out;
}

std::ostream& operator<<(std::ostream& out, const Polygon& src)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }
    out << src.points.size();
    std::copy(src.points.cbegin(), src.points.cend(),
        std::ostream_iterator< Point >(out << ' ', " "));
    return out;
}
