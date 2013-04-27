#include "utilities.hpp"
using namespace BOW;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

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
