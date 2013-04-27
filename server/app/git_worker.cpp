#include "git_worker.hpp"
#include "utilities.hpp"
using namespace BOW;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>




#define DEBUG_PARAM


GitWorker::GitWorker(const string &wc,const string & branch,const string &repo)
:workspace_(wc)
,branch_(branch)
,repo_path_(repo)
,git_repositories_("ssh://eikei@192.168.0.140//Volumes/DataHD/BuildOnWeb/repositories")
,remote_branch_commands_
{
    "rm -rf " + workspace_,
    "mkdir -p " + workspace_,
    "git clone -q " + git_repositories_+ "/" + repo_path_ + ".git " + workspace_,
    "cd " + workspace_ + "&& git branch " + branch_,
    "cd " + workspace_ + "&& git push --set-upstream origin " + branch_,
    "cd " + workspace_ + "&& git branch ",
    "cd " + workspace_ + "&& git checkout " + branch_,
    "cd " + workspace_ + "&& git branch  ",
}
{
    string gitfind = system_result("find "+ workspace_ + " -type d -name .git " );
#ifdef DEBUG_PARAM
    std::cout << __func__ <<":gitfind=<" <<  gitfind << ">" << endl;
#endif
    if(gitfind.empty())
    {
        createWorkSpace();
    }
    else
    {
        updateWorkSpace();
    }
}
GitWorker::~ GitWorker()
{
}

    
void GitWorker::createWorkSpace(void)
{
    // try clone remote branch
    system_result("mkdir -p " + workspace_);
    string cmd("git clone -q -b " + branch_ + " " + git_repositories_+ "/" + repo_path_+ ".git " + workspace_);
    string result = system_result(cmd);
    result = system_result("cd " + workspace_ + "&& git branch | grep " + branch_);
    if(false == result.empty())
    {
        return;
    }
    /// remote branch is not exist try create a remote branch 
#ifdef DEBUG_PARAM
    std::cout << __func__ <<":result=<" <<  result << ">" << endl;
#endif
    for(auto it = remote_branch_commands_.begin(); it != remote_branch_commands_.end();it++)
    {
#ifdef DEBUG_PARAM
        std::cout << __func__ <<"system:it=<" <<  *it << ">" << endl;
#endif
        system_result(*it);
    }
}

void GitWorker::updateWorkSpace(void)
{
    string pull = system_result("cd " + workspace_ + "&& git pull -q  ");
#ifdef DEBUG_PARAM
    std::cout << __func__ <<"pull=<" <<  pull << ">" << endl;
#endif
}