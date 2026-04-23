#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

struct DataStruct
{
    unsigned long long key1;
    unsigned long long key2;
    std::string key3;
};

void skipSpaces(const std::string& s, std::size_t& pos)
{
    while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos])))
    {
        ++pos;
    }
}

bool parseULLLit(const std::string& token, unsigned long long& value)
{
    if (token.size() < 4)
    {
        return false;
    }

    std::string suffix = token.substr(token.size() - 3);
    if (suffix != "ull" && suffix != "ULL")
    {
        return false;
    }

    std::string number = token.substr(0, token.size() - 3);
    if (number.empty())
    {
        return false;
    }

    for (std::size_t i = 0; i < number.size(); ++i)
    {
        if (!std::isdigit(static_cast<unsigned char>(number[i])))
        {
            return false;
        }
    }

    try
    {
        std::size_t used = 0;
        value = std::stoull(number, &used, 10);
        return used == number.size();
    }
    catch (...)
    {
        return false;
    }
}

bool parseULLBin(const std::string& token, unsigned long long& value)
{
    if (token.size() < 3)
    {
        return false;
    }

    if (!(token[0] == '0' && (token[1] == 'b' || token[1] == 'B')))
    {
        return false;
    }

    std::string bits = token.substr(2);
    if (bits.empty())
    {
        return false;
    }

    for (std::size_t i = 0; i < bits.size(); ++i)
    {
        if (bits[i] != '0' && bits[i] != '1')
        {
            return false;
        }
    }

    try
    {
        std::size_t used = 0;
        value = std::stoull(bits, &used, 2);
        return used == bits.size();
    }
    catch (...)
    {
        return false;
    }
}

bool parseLine(const std::string& line, DataStruct& data)
{
    std::size_t pos = 0;
    skipSpaces(line, pos);

    if (pos >= line.size() || line[pos] != '(')
    {
        return false;
    }
    ++pos;

    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;

    for (int i = 0; i < 3; ++i)
    {
        if (pos >= line.size() || line[pos] != ':')
        {
            return false;
        }
        ++pos;

        if (line.compare(pos, 4, "key1") == 0)
        {
            if (hasKey1)
            {
                return false;
            }
            pos += 4;

            if (pos >= line.size() || line[pos] != ' ')
            {
                return false;
            }
            ++pos;

            std::size_t start = pos;
            while (pos < line.size() && line[pos] != ':')
            {
                ++pos;
            }

            if (pos == start || pos >= line.size())
            {
                return false;
            }

            std::string token = line.substr(start, pos - start);
            if (!parseULLLit(token, data.key1))
            {
                return false;
            }

            hasKey1 = true;
        }
        else if (line.compare(pos, 4, "key2") == 0)
        {
            if (hasKey2)
            {
                return false;
            }
            pos += 4;

            if (pos >= line.size() || line[pos] != ' ')
            {
                return false;
            }
            ++pos;

            std::size_t start = pos;
            while (pos < line.size() && line[pos] != ':')
            {
                ++pos;
            }

            if (pos == start || pos >= line.size())
            {
                return false;
            }

            std::string token = line.substr(start, pos - start);
            if (!parseULLBin(token, data.key2))
            {
                return false;
            }

            hasKey2 = true;
        }
        else if (line.compare(pos, 4, "key3") == 0)
        {
            if (hasKey3)
            {
                return false;
            }
            pos += 4;

            if (pos >= line.size() || line[pos] != ' ')
            {
                return false;
            }
            ++pos;

            if (pos >= line.size() || line[pos] != '"')
            {
                return false;
            }
            ++pos;

            std::size_t start = pos;
            while (pos < line.size() && line[pos] != '"')
            {
                ++pos;
            }

            if (pos >= line.size())
            {
                return false;
            }

            data.key3 = line.substr(start, pos - start);
            ++pos;

            if (pos >= line.size() || line[pos] != ':')
            {
                return false;
            }

            hasKey3 = true;
        }
        else
        {
            return false;
        }
    }

    if (pos >= line.size() || line[pos] != ':')
    {
        return false;
    }
    ++pos;

    if (pos >= line.size() || line[pos] != ')')
    {
        return false;
    }
    ++pos;

    skipSpaces(line, pos);

    return pos == line.size() && hasKey1 && hasKey2 && hasKey3;
}

std::string toBinary(unsigned long long value)
{
    if (value == 0)
    {
        return "0";
    }

    std::string result;
    while (value > 0)
    {
        result.insert(result.begin(), static_cast<char>('0' + value % 2));
        value /= 2;
    }
    return result;
}

std::istream& operator>>(std::istream& in, DataStruct& data)
{
    std::string line;

    while (std::getline(in, line))
    {
        DataStruct temp;
        if (parseLine(line, temp))
        {
            data = temp;
            return in;
        }
    }

    in.setstate(std::ios::failbit);
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data)
{
    out << "(:key1 " << data.key1 << "ull";
    out << ":key2 0b" << toBinary(data.key2);
    out << ":key3 \"" << data.key3 << "\":)";
    return out;
}

bool compareData(const DataStruct& a, const DataStruct& b)
{
    if (a.key1 != b.key1)
    {
        return a.key1 < b.key1;
    }
    if (a.key2 != b.key2)
    {
        return a.key2 < b.key2;
    }
    return a.key3.size() < b.key3.size();
}

int main()
{
    std::vector<DataStruct> data;
    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    std::sort(data.begin(), data.end(), compareData);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}