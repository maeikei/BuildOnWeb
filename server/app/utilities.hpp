#ifndef BOW_UTILITIES_HPP
#define BOW_UTILITIES_HPP
#include <string>
#include <sstream>
#include <list>
using namespace std;

namespace BOW {
    string system_result(const string cmd);
    struct number
    {
        int i_;
        number(int i):i_(i){}
        operator string() const {
            stringstream ss;
            ss << i_;
            return ss.str();
        }
    };
} // namespace BOW

#endif // BOW_UTILITIES_HPP
