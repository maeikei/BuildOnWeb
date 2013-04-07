#include "list_dir.hpp"
#include "list_repo.hpp"
using namespace BuildOnWeb;
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>

// #define DEBUG_REPLACE

BOWListRepo::BOWListRepo(const string &username,const string & lang)
:BOWListDir()
,reposlist{}
,repos_map
{
    {"c_cxx","helloworld.git"},
    {"c_cxx","helloworldcxx.git"},
}
{
    for(auto it = repos_map.begin();it != repos_map.end();it++)
    {
        if( lang == it->first)
        {
            reposlist.push_back (it->second);
        }
    }
}
const list<string>& BOWListRepo::getlist()
{
    return reposlist;
}
