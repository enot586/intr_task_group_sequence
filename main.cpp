
#include <iostream>
#include <vector>
#include <functional>
#include <stdexcept>

#include "sequence.hpp"

std::vector< std::function<bool(void)> > basis_tests = {
    []()
    {
        try{
            Sequence ss;
            ss.FromString("A1");
            ++ss;
            return ss.Serialize() == "A2";
        } catch (const std::invalid_argument& ex) {
            return false;
        }
    },
    []()
    {
        try{
            Sequence ss;
            ss.FromString("A9");
            ++ss;
            return ss.Serialize() == "B1";
        } catch (const std::invalid_argument& ex) {
            return false;
        }
    },
    []()
    {
        try{
            Sequence ss;
            ss.FromString("Z9");
            ++ss;
            return ss.Serialize() == "A1-A1";
        } catch (const std::invalid_argument& ex) {
            return false;
        }
    },
    []()
    {
        try{
            Sequence ss;
            ss.FromString("A1-A1");
            ++ss;
            return ss.Serialize() == "A1-A2";
        } catch (const std::invalid_argument& ex) {
            return false;
        }
    },
    []()
    {
        try{
            Sequence ss;
            ss.FromString("A1-Z9");
            ++ss;
            return ss.Serialize() == "A2-A1";
        } catch (const std::invalid_argument& ex) {
            return false;
        }
    },
    []()
    {
        Sequence ss;
        try {
            ss.FromString("C9-Z9");
            ++ss;
            return ss.Serialize() == "E1-A1";
        } catch (const std::invalid_argument& ex) {
            return false;
        }
    },
    []()
    {
        Sequence ss;
        try {
            ss.FromString("A1-D2");
            ++ss;
            return false;
        } catch (const std::invalid_argument& ex) {
            return true;
        }
    },
    []()
    {
        try{
            Sequence ss;
            ss.FromString("Z9-Z9-Z9-Z9");
            ++ss;
            return ss.Serialize() == "A1-A1-A1-A1-A1";
        } catch (const std::invalid_argument& ex) {
            return false;
        }
    },
    []()
    {
        try {
            Sequence ss;
            ss.FromString("Z9-Z9-A1-Z9");
            ++ss;
            return ss.Serialize() == "Z9-Z9-A2-A1";
        } catch (const std::invalid_argument& ex) {
            return false;
        }
    },
    []()
    {
        Sequence ss;
        try {
            ss.FromString("A1-Z9-Z9-Z9");
            ++ss;
            return false;
        } catch (const std::invalid_argument& ex) {
            return true;
        }
    },
    []()
    {
        Sequence ss;
        try {
            ss.FromString("A1-Z9-Z9-Z9");
            ++ss;
            return false;
        } catch (const std::invalid_argument& ex) {
            return true;
        }
    }
};

int main()
{
    std::cout << "Test start" << "\n" << std::flush;

    for (auto test:basis_tests) {
        if ( !test() ) {
            std::cout << "ERROR !" << "\n" << std::flush;
        }
    }
/*
    try {
        Sequence ss;
        ss.FromString("A1-A1-A1-A1");
        for (auto i =0; i < 600000; ++i) {
             std::cout << ss.Serialize() << "\n" << std::flush;
            ++ss;
        }
    } catch (const std::invalid_argument& ex) {
        return true;
    }
*/
    std::cout << "Test end" << "\n" << std::flush;

    return 0;
}
