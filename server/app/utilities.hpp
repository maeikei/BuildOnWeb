#ifndef BOW_UTILITIES_HPP
#define BOW_UTILITIES_HPP
#include <string>
#include <sstream>
#include <list>
using namespace std;

namespace BOW {
    string system_result(const string cmd);
    void text2html(string &txt);
    void html2text(string &txt);
    
    void parseUri(const string &uri,string &user );
    void parseUri(const string &uri,string &user,string &cate);
    void parseUri(const string &uri,string &user,string &cate,string & repo);
    void parseUri(const string &uri,string &user,string &cate,string & repo,string &type);
    void parseUri(const string &uri,string &user,string &cate,string & repo,string &type,list<string> &path );
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
