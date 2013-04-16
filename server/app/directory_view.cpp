#include "reply_view.hpp"
#include "source_view.hpp"
#include "directory_view.hpp"
#include "last_position.hpp"
using namespace BOW;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>



//#define DEBUG_CONTENT

DirecoryView::DirecoryView(const SourceView &src)
:SourceView(src)
{
}
DirecoryView::~DirecoryView()
{
}

bool DirecoryView::getContent(const string &doc_root,string &contents)
{
    std::string dir_path(workspace_ + "/" + repo_);
    std::string relative;
    for(auto it = path_.begin();it != path_.end();it++)
    {
        relative.append( "/" + *it);
    }
    dir_path += relative;
#ifdef DEBUG_CONTENT
    std::cout << "dir_path=" << dir_path << std::endl;
#endif
    // if is a direcotry
    fs::path d_path(dir_path);
    if(not fs::is_directory(d_path))
    {
        return false;
    }
    // Open the template file to add to contents.
    {
        std::string full_path = doc_root + "/BuildOnWebViewDirectory.html";
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
    // search all files in this directory and create a table.
    {
        std::string table_trs;
        fs::directory_iterator end_iter;
        for( fs::directory_iterator dir_iter(d_path) ; dir_iter != end_iter ; ++dir_iter)
        {
#ifdef DEBUG_CONTENT
            std::cout << "dir_iter->path().string()=" << dir_iter->path().string() << std::endl;
#endif
            fs::path sub = dir_iter->path();
            std::string sub_name( sub.leaf().string());
#ifdef DEBUG_CONTENT
            std::cout << "sub_name=" << sub_name<< std::endl;
#endif
            string tr("<tr>\n");
            if(fs::is_directory(sub))
            {
                if(".git" == sub_name)
                {
                    continue;
                }
                if(".bow_output" == sub_name)
                {
                    continue;
                }
                tr += "<td class=\"icon\"><span class=\"mini-icon mini-icon-directory\"></span></td>\n";
            }
            else if(fs::is_regular_file(sub))
            {
                tr += "<td class=\"icon\"><span class=\"mini-icon mini-icon-text-file\"></span></td>\n";
            }
            else
            {
                continue;
            }
            // url tablbe
            tr +=  "<td class=\"content\">";
            tr +=  "<a href=\"/users";
            tr +=  "/" + user_;
            tr +=  "/" + category_;
            tr +=  "/" + repo_;
            if(false == relative.empty())
            {
                tr +=  "/" + relative;
            }
            tr +=  "/" + sub_name;
            tr +=  "\" ";
            tr +=  "class=\"js-directory-link js-slide-to css-truncate-target\"";
            tr +=  "title=\"";
            tr +=  sub_name;
            tr +=  "\">";
            tr +=  sub_name;
            tr += "</a></td>\n";
            tr += "</tr>\n";
            table_trs += tr;
        }
        
        boost::algorithm::replace_all(contents,"$BOW_TMPL_DIRECTORY_TABLE$",table_trs);
    }
    this->replace_source_path(contents);
    this->replace_loginout(contents);
    last_->set(repo_+ "/" + relative);
    return true;
}

