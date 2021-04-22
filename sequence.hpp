#pragma once

#include <mutex>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

#include "group.hpp"

using DefaultGroup = Group<DefaultExceptionCheck>;

template<class GroupType>
class Sequence
{
public:
    Sequence()
    {
        seq.reserve(max_size);
        Inc();
        current = 0;
        current_not_maximized = 0;
    }

    Sequence( std::initializer_list<GroupType> l ) :
        seq( l )
    {
        current = seq.size()-1;
        current_not_maximized = current;
        for (auto i = 0; i < seq.size(); ++i) {
            if ( !seq[i].isMax() ) {
                current_not_maximized = i;
                break;
            }
        }
    }

    Sequence(const Sequence& v)
    {
        std::unique_lock lock(mx);

        current = v.current;
        current_not_maximized = v.current_not_maximized;
        seq = v.seq;
    }

    /**
     * @brief Устанавливаем текущий идентификатор
     * @param строковое значение целевого идентификатора
     * @throws std::invalid_argument если формат не соответствует
     */
    void FromString(const std::string& src)
    {
        std::unique_lock lock(mx);

        std::vector<GroupType> tmp;
        std::stringstream ss(src);

        while ( ss.good() ) {
            char symbol = ss.get();

            char number = 0;
            if ( ss.good() )
                number = ss.get() - 48;
            else
                throw std::invalid_argument("Couldn't get a numeric value");

            GroupType gr( symbol, number );

            tmp.push_back(gr);

            if( tmp.size() > 2 ) {
                size_t pre_last = tmp.size()-2;
                size_t pre_pre_last = tmp.size()-3;
                if ( tmp[pre_pre_last] != GroupType('Z', 9) && tmp[pre_last] != GroupType() ) {
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
       auto temp(*this);
       Inc();
       return *this;
    }

    Sequence& operator=(const Sequence& v)
    {
        std::unique_lock lock(mx);
        seq = v.seq;
        current = v.current;
        current_not_maximized = v.current_not_maximized;

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
                    if (seq.size() < max_size) {
                        for (auto& el : seq)
                            el.reset();

                        seq.push_back( DefaultGroup() );
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
            seq.push_back( GroupType() );
        }

        return *this;
    }

private:
    static const size_t max_size = 10;
    size_t current;
    size_t current_not_maximized;
    std::vector<GroupType> seq;
    std::mutex mx;
};
