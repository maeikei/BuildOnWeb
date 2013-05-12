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
//    std::cout << __func__ <<":result=<" <<  result << ">" << endl;
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

#define DEBUG_URI

void BOW::parseUri(const string &uri,string &user )
{
    std::list<std::string> results;
    boost::split(results, uri, boost::is_any_of("/"));
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    user = results.front();
}
void BOW::parseUri(const string &uri,string &user,string &cate)
{
    std::list<std::string> results;
    boost::split(results, uri, boost::is_any_of("/"));
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    user = results.front();
    results.pop_front();    
    if(results.empty())
    {
        return;
    }
    cate = results.front();
}
void BOW::parseUri(const string &uri,string &user,string &cate,string & repo)
{
    std::list<std::string> results;
    boost::split(results, uri, boost::is_any_of("/"));
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    user = results.front();
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    cate = results.front();
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    repo = results.front();
}

void BOW::parseUri(const string &uri,string &user,string &cate,string & repo,list<string> &path)
{
    std::list<std::string> results;
    boost::split(results, uri, boost::is_any_of("/"));
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    user = results.front();
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    cate = results.front();
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    repo = results.front();
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    path = results;
}


void BOW::parseUri(const string &uri,string &user,string &cate,string & repo,string &type)
{
    std::list<std::string> results;
    boost::split(results, uri, boost::is_any_of("/"));
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    user = results.front();
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    cate = results.front();
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    repo = results.front();
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    type = results.front();
}
void BOW::parseUri(const string &uri,string &user,string &cate,string & repo,string &type,list<string> &path )
{
    std::list<std::string> results;
    boost::split(results, uri, boost::is_any_of("/"));
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    user = results.front();
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    cate = results.front();
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    repo = results.front();
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    type = results.front();
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    path = results;
}

