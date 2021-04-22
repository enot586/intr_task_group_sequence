
#include <iostream>
#include <vector>
#include <functional>
#include <stdexcept>

#include "sequence.hpp"

using Id = Sequence<DefaultGroup>;

std::vector< std::function<bool(void)> > basis_tests = {
    []()
    {
        try{
            Id ss;
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
            Id ss;
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
            Id ss;
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
            Id ss;
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
            Id ss;
            ss.FromString("A1-Z9");
            ++ss;
            return ss.Serialize() == "A2-A1";
        } catch (const std::invalid_argument& ex) {
            return false;
        }
    },
    []()
    {
        Id ss;
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
        Id ss;
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
            Id ss;
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
            Id ss;
            ss.FromString("Z9-Z9-A1-Z9");
            ++ss;
            return ss.Serialize() == "Z9-Z9-A2-A1";
        } catch (const std::invalid_argument& ex) {
            return false;
        }
    },
    []()
    {
        Id ss;
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
        Id ss;
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
        Id ss, dd;
        try {
            ss.FromString("A4");
            dd = ss;
            ss++;
            dd++;
            return dd.Serialize() == "A5" && dd.Serialize() == ss.Serialize();
        } catch (const std::invalid_argument& ex) {
            return false;
        }
    },
    []()
    {
        Id ss{ {"A1"}, {"A1"}, {"A1"}, {"A1"} };
        try {
            ++ss;
            return ss.Serialize() == "A1-A1-A1-A2";
        } catch (const std::invalid_argument& ex) {
            return false;
        }
    },
    []()
    {
        Id ss{ {'A', 1}, {'A', 1}, {'A', 1}, {'A', 1} };
        try {
            ++ss;
            return ss.Serialize() == "A1-A1-A1-A2";
        } catch (const std::invalid_argument& ex) {
            return false;
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
        Id ss;
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
