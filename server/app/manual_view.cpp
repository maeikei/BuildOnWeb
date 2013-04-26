#include "resource.hpp"
#include "reply_view.hpp"
#include "manual_view.hpp"
using namespace BOW;


#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>

//#define DEBUG_PARAM
//#define DEBUG_CONTENT


ManualView::ManualView(const string &repo)
:repo_(repo)
,workspace_(".temp/manual/" + repo_)
,output_(workspace_ +"/" + repo_ + ".man")
,env_build_commands_
{
    "mkdir -p " + workspace_,
}
,env_build_commands_step_
{
    "man " + repo_ + " > " + output_,
}
{
#ifdef DEBUG_PARAM
    std::cout << __func__ <<":repo_=" <<  repo_ << endl;
#endif
    for(auto it = env_build_commands_.begin(); it != env_build_commands_.end();it++)
    {
        system(it->c_str());
    }
    for(auto it = env_build_commands_step_.begin(); it != env_build_commands_step_.end();it++)
    {
        if (0 != system(it->c_str()))
        {
            break;
        }
    }
}

ManualView::~ManualView()
{
}

bool ManualView::getContent(const string &doc_root,string &contents)
{
    // Open the template file to add to contents.
    {
        std::string full_path = doc_root + "/BuildOnWebViewManual.html";
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
    // replace manual link $BOW_TMPL_SOURCE_MANUAL$
    {
        boost::algorithm::replace_all(contents,"$BOW_TMPL_REAL_MAN$","/man/"+repo_+".html");
    }
    return true;
}
