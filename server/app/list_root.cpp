#include "list_dir.hpp"
#include "list_root.hpp"
using namespace BuildOnWeb;
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>

// #define DEBUG_REPLACE

BOWListRoot::BOWListRoot(void)
:BOWListDir()
,languagelist
{
    "c_cxx",
}
{
    
}
const list<string> &BOWListRoot::getlist(void)
{
    return languagelist;
}
