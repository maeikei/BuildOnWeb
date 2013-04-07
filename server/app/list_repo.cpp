#include "list_dir.hpp"
#include "list_repo.hpp"
using namespace BuildOnWeb;
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>

// #define DEBUG_REPLACE

BOWListRepo::BOWListRepo(const string & _root,const string & _lang)
:BOWListDir()
,reposlist
{
    {"c_cxx","helloworld.git"},
    {"c_cxx","helloworldcxx.git"},
}
{
    
}
list<string> BOWListRepo::getlist()
{
    list<string> ret;
    return ret;
}
