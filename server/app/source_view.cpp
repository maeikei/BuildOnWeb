#include "reply_view.hpp"
#include "source_view.hpp"
#include "directory_view.hpp"
using namespace BOW;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>

#include <fstream>

//#define DEBUG_PARAM

#define DEBUG_CONTENT

SourceView::SourceView(const string &username,const string &category,const string &repo,const list<string> &path )
:ReplyView()
,user_(username)
,category_(category)
,repo_(repo)
,path_(path)
,workspace_(".temp/" + username)
,git_repositories_("ssh://eikei@192.168.0.140//Volumes/DataHD/BuildOnWeb/repositories")
,env_build_commands_
{
    "mkdir -p .temp/" + workspace_,
    "git clone " + git_repositories_+ "/" + category + "/" + repo_ + ".git " + workspace_ + "/" + repo_,
    "make exe -C " + workspace_ + "/" + repo_,
}
,env_build_commands_debug_
{
    "mkdir -p .temp/" + workspace_,
    "git clone " + git_repositories_+ "/" + category + "/" + repo_ + ".git " + workspace_ + "/" + repo_,
    "make exe -C " + workspace_ + "/" + repo_,
}
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
SourceView::~SourceView()
{
}

    
    
    
bool SourceView::getContent(const string &doc_root,string &contents)
{
    std::string source_path(workspace_ + "/" + repo_);
    for(auto it = path_.begin();it != path_.end();it++)
    {
        source_path.append( "/" + *it);
    }
#ifdef DEBUG_CONTENT
    std::cout << "source_path=" << source_path << std::endl;
#endif
    // if is a direcotry 
    fs::path src_path(source_path);
    if(fs::is_directory(src_path))
    {
        DirecoryView dir(*this);
        return dir.getContent(doc_root, contents);
    }
    // Open the template file to add to contents.
    {
        std::string full_path = doc_root + "/BuildOnWebViewSource.html";
#ifdef DEBUG_CONTENT
        std::cout << "full_path=" << full_path << std::endl;
#endif
        std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
        if (!is)
        {
            return false;
        }
        // Fill out the reply to be sent to the client.
        char buf[512];
        while (is.read(buf, sizeof(buf)).gcount() > 0) {
            contents.append(buf, is.gcount());
        }
    }
    // replace users
    {
        boost::algorithm::replace_all(contents,"$BOW_TMPL_USER$",user_);        
    }
    // replace source & file extension type
    {
        string source;
        std::ifstream isf(source_path.c_str(), std::ios::in | std::ios::binary);
        if (isf)
        {
            // Fill out the reply to be sent to the client.
            char buf[512];
            while (isf.read(buf, sizeof(buf)).gcount() > 0) {
                source.append(buf, isf.gcount());
            }            
        }
        boost::algorithm::replace_all(contents,"$BOW_TMPL_SOURCE$",source);
        
        string mode("c_cpp");
        const fs::path fs_path(source_path);
        auto it = _extensions.find( fs_path.extension().string());
        if(it != _extensions.end())
        {
            mode = it->second;
        }
        boost::algorithm::replace_all(contents,"$BOW_TMPL_TYPE$",mode);
    }
    // replace output
    {
        std::string output_path(workspace_ + "/" + repo_ +"/output.log");
#ifdef DEBUG_CONTENT
        std::cout << "output_path=" << output_path << std::endl;
#endif
        string output;
        std::ifstream isf(output_path.c_str(), std::ios::in | std::ios::binary);
        if (isf)
        {
            // Fill out the reply to be sent to the client.
            char buf[512];
            while (isf.read(buf, sizeof(buf)).gcount() > 0) {
                output.append(buf, isf.gcount());
            }
        }
        boost::algorithm::replace_all(contents,"$BOW_TMPL_OUTPUT$",output);
    }
    // replace javascript $BOW_TMPL_PATH$
    {
        std::string path("source=<" + workspace_ + "/" + repo_);
        for(auto it = path_.begin();it != path_.end();it++)
        {
            path.append( "/" + *it);
        }
        path.append(">,ws=<");
        path.append(workspace_ + "/" + repo_ + ">");
        boost::algorithm::replace_all(contents,"$BOW_TMPL_PATH$",path);
    }
    return true;
}
