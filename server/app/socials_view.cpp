#include "reply_view.hpp"
#include "source_view.hpp"
#include "socials_view.hpp"
#include "last_position.hpp"
using namespace BOW;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>



#define DEBUG_PARAM
//#define DEBUG_CONTENT

SosialView::SosialView(const SourceView &src)
:SourceView(src)
,wc_temp_cmd_output_(".bow_output/branch.list")
,env_show_commands_
{
    "cd " + workspace_ + "/" + repo_ + "&& git show-branch -r > " + wc_temp_cmd_output_,
}
{
    for(auto it = env_show_commands_.begin(); it != env_show_commands_.end();it++)
    {
#ifdef DEBUG_PARAM
        std::cout << __func__ <<"system:it=<" <<  *it << ">" << endl;
#endif
        system(it->c_str());
    }
}
SosialView::~SosialView()
{
}

bool SosialView::getContent(const string &doc_root,string &contents)
{
    // Open the template file to add to contents.
    {
        std::string full_path = doc_root + "/BuildOnWebViewSosials.html";
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
    this->replace_source_path(contents);
    this->replace_loginout(contents);
    return true;
}

