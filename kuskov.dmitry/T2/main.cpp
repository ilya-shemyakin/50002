#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <limits>

struct DataStruct
{
    double key1;
    unsigned long long key2;
    std::string key3;
};

struct UllIO
{
    unsigned long long& ref;
};

struct DelimiterIO
{
    char exp;
};

struct DoubleIO
{
    double& ref;
};

struct StringIO
{
    std::string& ref;
};

struct LabelIO
{
    std::string exp;
};

struct SmthIO
{
    char exp;
};

// scope guard
class iofmtguard
{
    public:
        iofmtguard(std::basic_ios<char>& s) :
            s_(s),
            width_(s.width()),
            fill_(s.fill()),
            precision_(s.precision()),
            fmt_(s.flags())
        {}

        ~iofmtguard()
        {
            s_.width(width_);
            s_.fill(fill_);
            s_.precision(precision_);
            s_.flags(fmt_);
        }
    private:
        std::basic_ios<char>& s_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios<char>::fmtflags fmt_;
};

std::istream& operator>>(std::istream& in, SmthIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
        return in;
    char c = '0';
    in >> c;
    if (in && c != dest.exp)
       in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
        return in;
    char c = '0';
    in >> c;
    if (in && (c != dest.exp))
        in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, DoubleIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
        return in;
    char c;
    bool dot{false};
    std::string num;
    while (in.get(c))
    {
        if (isdigit(c) || c == '.' || c == '-' || c == '+')
        {
            num += c;
            if (c == '.')
                dot = true;
        }
        else if (c == 'd' || c == 'D')
        {
            break;
        }
        else
        {
            in.putback(c);
            in.setstate(std::ios::failbit);
            return in;
        }
    }
    if (!dot || num.empty() || num.front() == '.' || num.back() == '.')
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    try
    {
        dest.ref = std::stod(num);
    }
    catch (...)
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, UllIO&& dest)
{
    std::istream::sentry sentry(in);
    if(!sentry)
        return in;
    char c;
    std::string num;
    std::string suf;
    while (in.get(c))
    {
        if (std::isdigit(c))
            num += c;
        else if (c == 'u' || c == 'U')
        {
            suf += c;
            suf += in.get();
            suf += in.get();
            if (suf == "ull" || suf == "ULL")
                break;
            else
            {
                in.setstate(std::ios::failbit);
                return in;
            }
        }
        else
        {
            in.putback(c);
            in.setstate(std::ios::failbit);
            return in;
        }
    }
    if (num.empty())
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    try
    {
        dest.ref = std::stoull(num);
    }
    catch (...)
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
        return in;
    return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, LabelIO& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
        return in;
    return in >> dest.exp;
}

std::istream& operator>>(std::istream& in, DataStruct& dest)
{
    std::istream::sentry sentry(in);
    if(!sentry)
        return in;
    bool check{true};
    if (!in)
        check = false;
    DataStruct input{};
    {
        using bracket = SmthIO;
        using sep = DelimiterIO;
        using label = LabelIO;
        using dbl = DoubleIO;
        using ull = UllIO;
        using str = StringIO;
        bool k1 = false;
        bool k2 = false;
        bool k3 = false;
        in >> bracket{'('};
        if (!in)
            check = false;
        in >> sep{':'};
        if (!in)
            check = false;
        while (check && in.peek() != ')')
        {
            label a{};
            in >> a;
            if (!in)
            {
                check = false;
                break;
            }
            if (a.exp == "key1")
            {
                in >> dbl{input.key1};
                k1 = true;
                if (!in)
                {
                    check = false;
                    break;
                }
            }
            else if (a.exp == "key2")
            {
                in >> ull{input.key2};
                k2 = true;
                if (!in)
                {
                    check = false;
                    break;
                }
            }
            else if (a.exp == "key3")
            {
                in >> str{input.key3};
                k3 = true;
                if (!in)
                {
                    check = false;
                    break;
                }
            }
            if (in.peek() == ':')
                in >> sep{':'};
            if (!in)
            {
                check = false;
                break;
            }
        }
        if (!k1 || !k2 || !k3)
            in.setstate(std::ios::failbit);
        if (check)
            in >> bracket{')'};
    }
    if (in)
    {
        dest = std::move(input);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& src)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
        return out;
    iofmtguard fmtguard(out);
    out << "(";
    out << ":key1 " << std::fixed << std::setprecision(1) << src.key1 << "d";
    out << ":key2 " << src.key2;
    out << ":key3 " << src.key3;
    out << ":";
    out << ")";
    return out;
}

bool cmp(const DataStruct& f, const DataStruct& ff)
{
    if(f.key1 < ff.key1)
        return true;
    else if (f.key1 == ff.key1 && f.key2 < ff.key2)
        return true;
    else if (f.key1 == ff.key1 && f.key2 == ff.key2 && f.key3.length() < ff.key3.length())
        return true;
    return false;
}

int main()
{
    std::vector<DataStruct> v;
    std::copy
    (
        std::istream_iterator<DataStruct>(iss),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(v)
    );

    std::sort
    (
        std::begin(v),
        std::end(v),
        cmp
    );

    std::cout << "Data:\n";
    std::copy
    (
        std::begin(v),
        std::end(v),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );
    return 0;
}
