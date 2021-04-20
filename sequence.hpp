#pragma once

#include <mutex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "group.hpp"

template<int MAX_GROUPS_NUMBER = 10>
class Sequence
{
public:
    Sequence( const ExeptionalSyms& exp = { 'D', 'F', 'G', 'J', 'M', 'Q', 'V' } ) :
        exceptions( exp )
    {
        seq.reserve(MAX_GROUPS_NUMBER);
        Inc();
        current = 0;
        current_not_maximized = 0;
    }

    Sequence(const Sequence& v)
    {
        std::unique_lock lock(mx);

        current = v.current;
        current_not_maximized = v.current_not_maximized;
        seq = v.seq;
        exceptions = v.exceptions;
    }

    /**
     * @brief Устанавливаем текущий идентификатор
     * @param строковое значение целевого идентификатора
     * @throws std::invalid_argument если формат не соответствует
     */
    void FromString(const std::string& src)
    {
        std::unique_lock lock(mx);

        std::vector<Group> tmp;
        std::stringstream ss(src);

        while ( ss.good() ) {
            char symbol = ss.get();

            char number = 0;
            if ( ss.good() )
                number = ss.get() - 48;
            else
                throw std::invalid_argument("Couldn't get a numeric value");

            Group gr( symbol, number, exceptions );

            tmp.push_back(gr);

            if( tmp.size() > 2 ) {
                size_t pre_last = tmp.size()-2;
                size_t pre_pre_last = tmp.size()-3;
                if ( tmp[pre_pre_last] != Group('Z', 9) && tmp[pre_last] != Group() ) {
                    throw std::invalid_argument("invalid identificator format");
                }
            }

            auto last = ss.get();
            if(last != std::char_traits<char>::eof() && last != '-' )
                throw std::invalid_argument("invalid identificator format");
        }

        if ( !tmp.size() )
            return;

        seq = std::move(tmp);

        current = seq.size()-1;
        current_not_maximized = current;
        for (auto i = 0; i < seq.size(); ++i) {
            if ( !seq[i].isMax() ) {
                current_not_maximized = i;
                break;
            }
        }
    }

    std::string Serialize() {
        std::unique_lock lock(mx);
        if ( !seq.size() )
            return {};

        std::string result;
        auto it = seq.begin();
        result+= it->Serialize();
        ++it;

        while( it != seq.end() ) {
            result+= "-";
            result+= it->Serialize();
            ++it;
        }

        return result;
    }

    Sequence& operator++()
    {
        std::unique_lock lock(mx);
        return Inc();
    }

    Sequence operator++(int)
    {
        std::unique_lock lock(mx);
        Sequence temp = *this;
        Inc();
        return temp;
    }

    Sequence& operator=(const Sequence& v)
    {
        std::unique_lock lock(mx);
        seq = v.seq;
        current = v.current;
        current_not_maximized = v.current_not_maximized;
        exceptions = v.exceptions;

        return *this;
    }

protected:
    Sequence& Inc() {
        if ( seq.size() ) {
            if( !seq[current].isMax() ) {
                seq[current].TryIncrement();
            } else {
                size_t last = (seq.size()-1);
                if ( seq[current_not_maximized].isMax() && current_not_maximized == last ) {
                    if (seq.size() < MAX_GROUPS_NUMBER) {
                        for (auto& el : seq)
                            el.reset();

                        seq.push_back( Group(exceptions) );
                        current = seq.size()-1;
                        current_not_maximized = 0;
                    }
                } else {
                    if( seq[current_not_maximized].isMax() && current_not_maximized < last ) {
                        ++current_not_maximized;
                    }

                    if ( seq[current_not_maximized].TryIncrement() )
                        seq[current].reset();
                }
            }
        } else {
            seq.push_back( Group(exceptions) );
        }

        return *this;
    }

private:
    size_t current;
    size_t current_not_maximized;
    std::vector<Group> seq;
    ExeptionalSyms exceptions;
    std::mutex mx;
};
