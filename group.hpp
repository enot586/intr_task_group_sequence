#pragma once

#include <string>
#include <stdexcept>
#include <set>
#include <memory>

using ExeptionalSyms = std::set<char>;
using ExeptionalSymsPtr = std::shared_ptr<ExeptionalSyms>;

struct Group
{

public:
    Group(ExeptionalSymsPtr exp={})  :
        symbol('A'), number(1), expSym(exp)
    { }

    /**
     * @brief Group
     * @param sym значение символа
     * @param num значение номера
     * @throws std::invalid_argument в случае некорректных значений аргументов
     */
    Group(char sym, char num, ExeptionalSymsPtr exp={}) :
        expSym(exp)
    {
        if ( exp && exp->find(sym) != exp->end() )
            throw std::invalid_argument("invalid symbol argument");

        if( sym >= 'A' && sym <= 'Z' )
            symbol = sym;
        else
            throw std::invalid_argument("invalid symbol argument");

        if(num > 0 && num <= 9)
            number = num;
        else
            throw std::invalid_argument("invalid number argument");
    }

    bool isMax() const
    {
        return IsMaxNumeric() && IsMaxSymbol();
    }

    bool TryIncrement() {
        Group tmp = *this;
        ++tmp;
        if ( HandleExptionalSyms(tmp) ) {
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

    bool HandleExptionalSyms(Group& src)
    {
        auto tmp = src.symbol;
        while (expSym && expSym->find( tmp ) != expSym->end() && tmp <= 'Z' ) {
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
    std::shared_ptr<ExeptionalSyms> expSym;
};
