#include "template.hpp"
using namespace BuildOnWeb;
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>

// #define DEBUG_REPLACE


BOWTemplate::BOWTemplate(string & content)
:_content(content)
,_file("temp/helloworld/main.c")
,_mode("c_cpp")
,_output("temp/helloworld/output.log")
,_extensions
{
    {".h","c_cpp"},
    {".hpp","c_cpp"},
    {".c","c_cpp"},
    {".cxx","c_cpp"},
    {".cpp","c_cpp"},
    {".cp","c_cpp"},
}
{
    
}
void BOWTemplate::replace(void)
{
// replace html template sources.
    {
        std::ifstream ifs(_file.c_str(), std::ios::in | std::ios::binary);
        char buf[512];
        string source;
        while (ifs.read(buf, sizeof(buf)).gcount() > 0) {
            source.append(buf, ifs.gcount());
        }
        boost::algorithm::replace_all(_content, "$BOW_TMPL_SOURCE$", source);
    }

// replace html template language mode.
    {
        fs::path file(_file);
        string extension = file.extension().string();
#ifdef DEBUG_REPLACE
        cout << extension << endl;
#endif
        auto it = _extensions.find(extension);
        string mode = _mode;
        if( _extensions.end() != it )
        {
            mode = it->second;
        }
        boost::algorithm::replace_all(_content, "$BOW_TMPL_TYPE$",_mode);
    }
// replace html template output.
    {
        std::ifstream ifs(_output.c_str(), std::ios::in | std::ios::binary);
        char buf[512];
        string output;
        while (ifs.read(buf, sizeof(buf)).gcount() > 0) {
            output.append(buf, ifs.gcount());
        }
        boost::algorithm::replace_all(_content, "$BOW_TMPL_OUTPUT$", output);
    }
    // replace html template path.
    boost::algorithm::replace_all(_content, "$BOW_TMPL_PATH$", _file);
}
