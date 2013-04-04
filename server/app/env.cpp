#include "env.hpp"
using namespace BuildOnWeb;
#include <iostream>
#include <cstdlib>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;


BOWEnv::BOWEnv(const string & www_root)
:_www_root(www_root)
,_env_build_commands
{
    "rm -rf " + _www_root + "/temp",
    "mkdir -p " + _www_root + "/temp",
    "git clone ssh://eikei@192.168.0.140//Volumes/DataHD/BuildOnWeb/repositories/c_cxx/helloworld.git "
        + _www_root + "/temp/helloworld",
//    "pwd",
    "make exe -C " + _www_root + "/temp/helloworld",
}
,_env_build_commands_debug
{
/*
    "rm -rf " + _www_root + "/temp",
    "mkdir -p " + _www_root + "/temp",
    "git clone file:///Users/maeikei/workspace/BuildOnWeb-dev/repositories/c_cxx/helloworld.git "
    + _www_root + "/temp/helloworld",
    "make exe -C " + _www_root + "/temp/helloworld",
 */
}
{
}
void BOWEnv::build_env(void)
{
    for(auto it = _env_build_commands.begin(); it != _env_build_commands.end();it++)
    {
        system(it->c_str());
    }
    fs::path helloworld(_www_root + "/temp/helloworld");
    if( false == fs::exists(helloworld) )
    {
        for(auto it = _env_build_commands_debug.begin(); it != _env_build_commands_debug.end();it++)
        {
            system(it->c_str());
        }        
    }
}
