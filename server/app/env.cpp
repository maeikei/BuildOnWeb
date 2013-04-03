#include "env.hpp"
using namespace BuildOnWeb;
#include <iostream>
#include <cstdlib>

BOWEnv::BOWEnv(const string & www_root)
:_www_root(www_root)
,env_build_commands
{
    "rm -rf " + _www_root + "/temp",
    "mkdir -p " + _www_root + "/temp",
    "git clone ssh://eikei@192.168.0.140//Volumes/DataHD/BuildOnWeb/repositories/c_cxx/helloworld.git "
        + _www_root + "/temp/helloworld",
    "pwd",
    "make exe -C " + _www_root + "/temp/helloworld",
}
{
}
void BOWEnv::build_env(void)
{
    for(auto it = env_build_commands.begin(); it != env_build_commands.end();it++)
    {
        system(it->c_str());
    }
}
