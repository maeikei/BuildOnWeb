#include "source_update.hpp"
using namespace BuildOnWeb;
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <reply.hpp>

#include <iostream>
#include <fstream>
using namespace std;


// #define DEBUG_PARAM


BOWSoureUpdate::BOWSoureUpdate(const string & source,const string & path,http::server_threadpool::reply& rep)
:_source(source)
,_path(path)
,_rep(rep)
,_env_build_commands
{
    "cd temp/helloworld && git diff ",
    "cd temp/helloworld && git commit -am \"web auto modify\" ",
    "make exe -C temp/helloworld",
}
{
}
void BOWSoureUpdate::run(void)
{
#ifdef DEBUG_PARAM
    cout << "_source=<" << _source<< ">" << endl;
    cout << "_path=<" << _path<< ">" << endl;
#endif
    fs::path sourefile(_path);
    if( not fs::exists(sourefile) )
    {
        // new files.
    }
    else
    {
        std::ofstream ofs;
        ofs.open( _path );
        ofs << _source;
        ofs.close();
    }
    for(auto it = _env_build_commands.begin(); it != _env_build_commands.end();it++)
    {
        system(it->c_str());
    }
}