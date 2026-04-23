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

/*
“≈—“ 1 Ч базовый рабочий
¬вод:
(:key1 10ull:key2 0b101:key3 "abc":)
(:key1 7ull:key2 0b1:key3 "x":)
(:key1 12ull:key2 0b11:key3 "test":)

ќжидаемый вывод:
(:key1 7ull:key2 0b1:key3 "x":)
(:key1 10ull:key2 0b101:key3 "abc":)
(:key1 12ull:key2 0b11:key3 "test":)
*/

/*
“≈—“ 2 Ч пол€ в разном пор€дке
¬вод:
(:key2 0b11:key1 7ull:key3 "hello":)
(:key3 "x":key1 7ull:key2 0b1:)
(:key1 10ull:key2 0b101:key3 "abc":)

ќжидаемый вывод:
(:key1 7ull:key2 0b1:key3 "x":)
(:key1 7ull:key2 0b11:key3 "hello":)
(:key1 10ull:key2 0b101:key3 "abc":)
*/

/*
“≈—“ 3 Ч сортировка по длине key3
¬вод:
(:key1 7ull:key2 0b1:key3 "abcd":)
(:key1 7ull:key2 0b1:key3 "x":)
(:key1 7ull:key2 0b1:key3 "qq":)

ќжидаемый вывод:
(:key1 7ull:key2 0b1:key3 "x":)
(:key1 7ull:key2 0b1:key3 "qq":)
(:key1 7ull:key2 0b1:key3 "abcd":)
*/

/*
“≈—“ 4 Ч пропуск неправильных строк
¬вод:
wrong line
(:key1 10ull:key2 0b101:key3 "abc":)
abc
(:key2 0b11:key1 7ull:key3 "hello":)
not valid

ќжидаемый вывод:
(:key1 7ull:key2 0b11:key3 "hello":)
(:key1 10ull:key2 0b101:key3 "abc":)
*/

/*
“≈—“ 5 Ч неправильный key1
¬вод:
(:key1 10:key2 0b101:key3 "abc":)
(:key1 7ull:key2 0b1:key3 "x":)

ќжидаемый вывод:
(:key1 7ull:key2 0b1:key3 "x":)
*/

/*
“≈—“ 6 Ч неправильный key2
¬вод:
(:key1 10ull:key2 101:key3 "abc":)
(:key1 7ull:key2 0b1:key3 "x":)

ќжидаемый вывод:
(:key1 7ull:key2 0b1:key3 "x":)
*/

/*
“≈—“ 7 Ч повтор пол€
¬вод:
(:key1 10ull:key1 7ull:key2 0b101:key3 "abc":)
(:key1 5ull:key2 0b1:key3 "ok":)

ќжидаемый вывод:
(:key1 5ull:key2 0b1:key3 "ok":)
*/

/*
“≈—“ 8 Ч пропущено поле
¬вод:
(:key1 10ull:key2 0b101:)
(:key1 5ull:key2 0b1:key3 "ok":)

ќжидаемый вывод:
(:key1 5ull:key2 0b1:key3 "ok":)
*/

/*
“≈—“ 9 Ч key3 с пробелами
¬вод:
(:key1 10ull:key2 0b101:key3 "hello world":)
(:key1 7ull:key2 0b1:key3 "x":)

ќжидаемый вывод:
(:key1 7ull:key2 0b1:key3 "x":)
(:key1 10ull:key2 0b101:key3 "hello world":)
*/

/*
“≈—“ 10 Ч нулевое бинарное значение
¬вод:
(:key1 1ull:key2 0b0:key3 "zero":)
(:key1 1ull:key2 0b1:key3 "one":)

ќжидаемый вывод:
(:key1 1ull:key2 0b0:key3 "zero":)
(:key1 1ull:key2 0b1:key3 "one":)
*/

/*
“≈—“ 11 Ч нет ни одной корректной строки
¬вод:
abc
wrong
123

ќжидаемый вывод:
ничего
*/

/*
“≈—“ 12 Ч большой смешанный тест
¬вод:
(:key1 10ull:key2 0b101:key3 "abc":)
wrong line
(:key2 0b11:key1 7ull:key3 "hello":)
(:key3 "x":key1 7ull:key2 0b1:)
(:key1 7ull:key2 0b1:key3 "abcd":)
(:key1 3ull:key2 0b111:key3 "test":)
(:key1 3ull:key2 0b10:key3 "qq":)
(:key1 3ull:key2 0b10:key3 "z":)

ќжидаемый вывод:
(:key1 3ull:key2 0b10:key3 "z":)
(:key1 3ull:key2 0b10:key3 "qq":)
(:key1 3ull:key2 0b111:key3 "test":)
(:key1 7ull:key2 0b1:key3 "x":)
(:key1 7ull:key2 0b1:key3 "abcd":)
(:key1 7ull:key2 0b11:key3 "hello":)
(:key1 10ull:key2 0b101:key3 "abc":)
*/




/*
ЅќЋ№Ўќ… ќЅў»… “≈—“
¬вод:
(:key1 10ull:key2 0b101:key3 "abc":)
(:key1 7ull:key2 0b1:key3 "x":)
(:key1 12ull:key2 0b11:key3 "test":)

(:key2 0b11:key1 7ull:key3 "hello":)
(:key3 "x":key1 7ull:key2 0b1:)
(:key1 10ull:key2 0b101:key3 "abc":)

(:key1 7ull:key2 0b1:key3 "abcd":)
(:key1 7ull:key2 0b1:key3 "x":)
(:key1 7ull:key2 0b1:key3 "qq":)

wrong line
(:key1 10ull:key2 0b101:key3 "abc":)
abc
(:key2 0b11:key1 7ull:key3 "hello":)
not valid

(:key1 10:key2 0b101:key3 "abc":)
(:key1 7ull:key2 0b1:key3 "x":)

(:key1 10ull:key2 101:key3 "abc":)
(:key1 7ull:key2 0b1:key3 "x":)

(:key1 10ull:key1 7ull:key2 0b101:key3 "abc":)
(:key1 5ull:key2 0b1:key3 "ok":)

(:key1 10ull:key2 0b101:)
(:key1 5ull:key2 0b1:key3 "ok":)

(:key1 10ull:key2 0b101:key3 "hello world":)
(:key1 7ull:key2 0b1:key3 "x":)

(:key1 1ull:key2 0b0:key3 "zero":)
(:key1 1ull:key2 0b1:key3 "one":)

abc
wrong
123

(:key1 10ull:key2 0b101:key3 "abc":)
wrong line
(:key2 0b11:key1 7ull:key3 "hello":)
(:key3 "x":key1 7ull:key2 0b1:)
(:key1 7ull:key2 0b1:key3 "abcd":)
(:key1 3ull:key2 0b111:key3 "test":)
(:key1 3ull:key2 0b10:key3 "qq":)
(:key1 3ull:key2 0b10:key3 "z":)

ќжидаемый вывод:
(:key1 1ull:key2 0b0:key3 "zero":)
(:key1 1ull:key2 0b1:key3 "one":)
(:key1 3ull:key2 0b10:key3 "z":)
(:key1 3ull:key2 0b10:key3 "qq":)
(:key1 3ull:key2 0b111:key3 "test":)
(:key1 5ull:key2 0b1:key3 "ok":)
(:key1 5ull:key2 0b1:key3 "ok":)
(:key1 7ull:key2 0b1:key3 "x":)
(:key1 7ull:key2 0b1:key3 "x":)
(:key1 7ull:key2 0b1:key3 "x":)
(:key1 7ull:key2 0b1:key3 "x":)
(:key1 7ull:key2 0b1:key3 "x":)
(:key1 7ull:key2 0b1:key3 "x":)
(:key1 7ull:key2 0b1:key3 "x":)
(:key1 7ull:key2 0b1:key3 "qq":)
(:key1 7ull:key2 0b1:key3 "abcd":)
(:key1 7ull:key2 0b1:key3 "abcd":)
(:key1 7ull:key2 0b11:key3 "hello":)
(:key1 7ull:key2 0b11:key3 "hello":)
(:key1 7ull:key2 0b11:key3 "hello":)
(:key1 10ull:key2 0b101:key3 "abc":)
(:key1 10ull:key2 0b101:key3 "abc":)
(:key1 10ull:key2 0b101:key3 "abc":)
(:key1 10ull:key2 0b101:key3 "abc":)
(:key1 10ull:key2 0b101:key3 "hello world":)
(:key1 12ull:key2 0b11:key3 "test":)
*/