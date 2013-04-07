#include "reply_view.hpp"
#include "source_view.hpp"
using namespace BOW;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <fstream>

//#define DEBUG_PARAM

#define DEBUG_CONTENT

SourceView::SourceView(const string &username,const string &category,const string &repo,const list<string> &path )
:ReplyView()
,workspace_(".temp/" + username)
,git_repositories_("ssh://eikei@192.168.0.140//Volumes/DataHD/BuildOnWeb/repositories")
,env_build_commands_
{
    "mkdir -p .temp/" + workspace_,
    "git clone " + git_repositories_+ "/" + category + "/" + repo + ".git " + workspace_ + "/" + repo,
    "make exe -C " + workspace_ + "/" + repo,
}
,env_build_commands_debug_
{
    "mkdir -p .temp/" + workspace_,
    "git clone " + git_repositories_+ "/" + category + "/" + repo + ".git " + workspace_ + "/" + repo,
    "make exe -C " + workspace_ + "/" + repo,
}

{
#ifdef DEBUG_PARAM
    std::cout << __func__ <<":username=" <<  username << endl;
    std::cout << __func__ <<":category=" <<  category << endl;
    std::cout << __func__ <<":repo=" <<  repo << endl;
#endif
    const fs::path localrepo(workspace_ + "/" + repo + "/.git/");
    if( not fs::exists(localrepo) )
    {
        for(auto it = env_build_commands_.begin(); it != env_build_commands_.end();it++)
        {
            system(it->c_str());
        }        
    }
    else if ( not fs::is_directory(localrepo) )
    {
        for(auto it = env_build_commands_.begin(); it != env_build_commands_.end();it++)
        {
            system(it->c_str());
        }
    }
    else
    {
        
    }
}

bool SourceView::getContent(const string &doc_root,string &contents)
{
    // Open the file to send back.
    std::string full_path = doc_root + "/BuildOnWebViewSource.html";
#ifdef DEBUG_CONTENT
	std::cout << "full_path=" << full_path << std::endl;
#endif
    std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
    if (!is)
    {
        return false;
    }    
#ifdef DEBUG_CONTENT
	std::cout << "full_path=" << full_path << std::endl;
#endif
    // Fill out the reply to be sent to the client.
    char buf[512];
    while (is.read(buf, sizeof(buf)).gcount() > 0) {
        contents.append(buf, is.gcount());
    }
    // replace BOW var in template.
//    BuildOnWeb::BOWTemplate temp(rep.content);
//    temp.replace();
    return true;
}

