#include "list_dir.hpp"
using namespace BuildOnWeb;
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>

// #define DEBUG_REPLACE


BOWListDir::BOWListDir(string & dir)
:_dir(dir)
{
    
}
void BOWListDir::replace(void)
{
}
