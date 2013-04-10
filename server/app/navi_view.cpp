#include "reply_view.hpp"
#include "navi_view.hpp"
using namespace BOW;


#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>


#define DEBUG_CONTENT


const map<string,vector<string>> NaviView::navi_items_ =
{
    { "c_cxx",
        {"helloworld","helloworldcxx" }
    },
}
;

NaviView::NaviView(const string &username)
:user_(username)
,category_()
{
}

NaviView::NaviView(const string &username,const string &category)
:user_(username)
,category_(category)
{
    
}
NaviView::~NaviView()
{
}

bool NaviView::getContent(const string &doc_root,string &contents)
{
    // Open the template file to add to contents.
    {
        std::string full_path = doc_root + "/BuildOnWebViewNavi.html";
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
    // replace navi path
    {
        this->replace_source_path(contents);
    }
    // search all navi items and create a table.
    {
        std::string table_navi;
        for(auto it = navi_items_.begin();it != navi_items_.end();it++ )
        {
#ifdef DEBUG_CONTENT
            std::cout << "it->first=<" << it->first << ">" << std::endl;
#endif
            // if no category is speciled,list all categories
            if(true == category_.empty())
            {
                string tr("<tr>\n");
                tr +=  "<td class=\"content\">";
                tr +=  "<a href=\"/users";
                tr +=  "/" + user_;
                tr +=  "/" + it->first;
                tr +=  "\">";
                tr +=  it->first;
                tr += "</a></td>\n";
                tr += "</tr>\n";
                
                table_navi += tr;
                continue;
            }
            // category is speciled,list all in the category
            if(category_ == it->first)
            {
                for(auto subit = it->second.begin();subit != it->second.end();subit++)
                {
                    string tr("<tr>\n");
                    tr +=  "<td class=\"content\">";
                    tr +=  "<a href=\"/users";
                    tr +=  "/" + user_;
                    tr +=  "/" + it->first;
                    tr +=  "/" + *subit;
                    tr +=  "\">";
                    tr +=  *subit;
                    tr += "</a></td>\n";
                    tr += "</tr>\n";
                    
                    table_navi += tr;
                }
                break;
            }
        }
        boost::algorithm::replace_all(contents,"$BOW_TMPL_NAVI_TABLE$",table_navi);
    }
    return true;
}

void NaviView::replace_source_path(string &contents)
{
    string href("/users");
    std::string path;
    // user_
    href += "/" + user_;
    path += "<a href=\"";
    path += href;
    path += "\">";
    path += user_;
    path += "</a>";
    // category_
    if(false == category_.empty())
    {
        path += "<span>/</span>";
        href += "/" + category_;
        path += "<a href=\"";
        path += href;
        path += "\">";
        path += category_;
        path += "</a>";
    }
    boost::algorithm::replace_all(contents,"$BOW_TMPL_NAVI_PATH$",path);
}