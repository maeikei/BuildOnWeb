#include "utilities.hpp"
using namespace BOW;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

string BOW::system_result(const string cmd)
{
//    std::cout << __func__ <<":cmd=<" <<  cmd << ">" << endl;
    FILE *pipe = popen(cmd.c_str(), "r");
    std::string result;
    char buf[256] = {0};
    while(!feof(pipe)) {
        if(fgets(buf, sizeof(buf), pipe) != NULL)
        {
            result += buf;
        }
    }
    pclose(pipe);
    return result;
}

static const map<string,string> mapConstTextHtmlPair =
{
    {"\"","&quot;"},
    {"<","&lt;"},
    {">","&gt;"},
};
void BOW::text2html(string &txt)
{
    for(auto it = mapConstTextHtmlPair.begin();it != mapConstTextHtmlPair.end();it++)
    {
        boost::algorithm::replace_all(txt,it->first,it->second);
    }
}

void BOW::html2text(string &txt)
{
    for(auto it = mapConstTextHtmlPair.begin();it != mapConstTextHtmlPair.end();it++)
    {
        boost::algorithm::replace_all(txt,it->second,it->first);
    }
}

