#pragma once

#include <memory>
#include <set>
#include <stdexcept>
#include <string>

using ExeptionalSyms = std::set<char>;
using ExeptionalSymsPtr = std::shared_ptr<ExeptionalSyms>;


class DefaultExceptionCheck
{
public:
    static ExeptionalSyms& getExeptionalSyms() { static ExeptionalSyms es{ 'D', 'F', 'G', 'J', 'M', 'Q', 'V' }; return es; }

    bool CheckExceptionList(char sym, char num)
    {
        bool isValidNumber = num > 0 && num <= 9;
        bool isValidSymbol = ( sym >= 'A' && sym <= 'Z' )
                && getExeptionalSyms().find(sym) == getExeptionalSyms().end();
        return isValidNumber && isValidSymbol;
    }
};

template<class ExceptionCheck = DefaultExceptionCheck>
class Group : public ExceptionCheck
{

public:
    Group()
    { reset(); }

    /**
     * @brief Group
     * @param sym значение символа
     * @param num значение номера
     * @throws std::invalid_argument в случае некорректных значений аргументов
     */
    Group(char sym, char num)
    {
        if ( !ExceptionCheck::CheckExceptionList(sym, num) )
            throw std::invalid_argument("invalid symbol argument");

        symbol = sym;
        number = num;
    }

    Group(const std::string& v)
    {
        if ( !v.empty() ) {
            if (v.size() < 2 || v.size() > 2)
                throw std::invalid_argument("invalid number argument");

            symbol = v[0];
            number = v[1]-48;
        } else {
            reset();
        }

        if ( !ExceptionCheck::CheckExceptionList(symbol, number) )
            throw std::invalid_argument("invalid symbol argument");
    }

    bool isMax() const
    {
        return IsMaxNumeric() && IsMaxSymbol();
    }

    bool TryIncrement() {
        Group tmp = *this;
        ++tmp;
        if ( HandleExeptionalSyms(tmp) ) {
            symbol = tmp.symbol;
            number = tmp.number;
            return true;
        }

        return false;
    }

    void reset()
    {
        symbol = 'A';
        number = 1;
    }

    std::string Serialize() const
    {
        return symbol+std::to_string(number);
    }

    Group& operator++()
    {
        return Inc();
    }

    Group operator++(int)
    {
        Group tmp = *this;
        Inc();
        return tmp;
    }

    Group& operator=(const Group& gr)
    {
        symbol = gr.symbol;
        number = gr.number;
        return *this;
    }

    friend bool operator!=(const Group& l, const Group& r)
    {
        return std::tie(l.symbol, l.number) != std::tie(r.symbol, r.number);
    }

protected:

    char IsMaxNumeric() const
    {
        return  number == 9;
    }

    char IsMaxSymbol() const
    {
        return  symbol == 'Z';
    }

    Group& Inc()
    {
        if ( !IsMaxNumeric() )
            ++number;
        else {
            number = 1;
            if ( !IsMaxSymbol() ) {
                ++symbol;
            }
        }

        return *this;
    }

    bool HandleExeptionalSyms(Group& src)
    {
        auto tmp = src.symbol;
        while ( !ExceptionCheck::CheckExceptionList(tmp, src.number) && tmp <= 'Z' ) {
            ++tmp;
        }

        if (tmp <= 'Z') {
            src.symbol = tmp;
            return true;
        } else {
            return false;
        }
    }

private:
    char symbol;
    char number;
};
