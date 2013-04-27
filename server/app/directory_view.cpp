#include "reply_view.hpp"
#include "resource.hpp"
#include "source_view.hpp"
#include "directory_view.hpp"
#include "last_position.hpp"
using namespace BOW;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>



DirecoryApp::DirecoryApp(void)
{
}
DirecoryApp::~ DirecoryApp()
{
}
void DirecoryApp::create(const std::string &uri,const std::string &user_uid)
{
}
void DirecoryApp::get(const std::string &doc_root, http::server_threadpool::reply& rep)
{
}


//#define DEBUG_CONTENT
DirecoryView::DirecoryView(const string &username,const string &user_uid,
                           const string &category,const string &repo)
:ReplyView()
,user_(username)
,user_uid_(user_uid)
,category_(category)
,repo_(repo)
,path_()
,workspace_(".temp/" + user_uid_ + "/" + category_ + "/" + repo_)
{
}

DirecoryView::DirecoryView(const string &username,const string &user_uid,
                           const string &category,const string &repo,const list<string> &path)
:ReplyView()
,user_(username)
,user_uid_(user_uid)
,category_(category)
,repo_(repo)
,path_(path)
,workspace_(".temp/" + user_uid_ + "/" + category_ + "/" + repo_)
{
}
DirecoryView::~DirecoryView()
{
}

bool DirecoryView::readBody(const string &doc_root,string &contents)
{
    std::string dir_path(workspace_);
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
        is.close();
    }
    return true;
}

std::map<std::string,std::string> DirecoryView::bodyVars(void)
{
    std::map<std::string,std::string> ret;
    // replace users
    ret.insert(pair<string,string>("$BOW_TMPL_USER$",user_));
    // replace navi path
    this->create_source_path(ret);
    // table
    this->create_table(ret);
    // replace login/out.
    this->create_loginout(ret);
    return ret;
}

void DirecoryView::create_table(std::map<std::string,std::string> &replace)
{
    std::string dir_path(workspace_);
    std::string relative;
    for(auto it = path_.begin();it != path_.end();it++)
    {
        relative.append( "/" + *it);
    }
    dir_path += relative;
    // if is a direcotry
    fs::path d_path(dir_path);
    // search all files in this directory and create a table.
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
        if(fs::is_regular_file(sub))
        {
            tr +=  "/edit";
        }
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
    replace.insert(pair<string,string>("$BOW_TMPL_DIRECTORY_TABLE$",table_trs));
}
static const string strConstSourcePath =
"<a href=\"/users/$user_$\">$user_$</a><span>/</span><a href=\"/users/$user_$/$category_$\">$category_$</a>";


void DirecoryView::create_source_path(std::map<std::string,std::string> &replace)
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
    path += "<span>/</span>";
    href += "/" + category_;
    path += "<a href=\"";
    path += href;
    path += "\">";
    path += category_;
    path += "</a>";
    // repo_
    path += "<span>/</span>";
    href += "/" + repo_;
    path += "<a href=\"";
    path += href;
    path += "\">";
    path += repo_;
    path += "</a>";
    
    // add remain paths.
    for(auto it = path_.begin();it != path_.end();it++)
    {
        path += "<span>/</span>";
        // path
        href += "/" + *it;
        path += "<a href=\"";
        path += href;
        path += "\">";
        path += *it;
        path += "</a>";
    }
    replace.insert(pair<string,string>("$BOW_TMPL_SOURCE_PATH$",path));
}

static const string strConstLogin =
"<a href=\"/login.php\" data-method=\"post\" id=\"login\">login</a>";
static const string strConstLogout =
"<a href=\"/logout.php\" data-method=\"post\" id=\"logout\">logout</a>";

void DirecoryView::create_loginout(std::map<std::string,std::string> &replace)
{
    if( "guest" == user_ || user_.empty())
    {
        replace.insert(pair<string,string>("$BOW_TMPL_USER_LOGINOUT$",strConstLogin));
    }
    else
    {
        replace.insert(pair<string,string>("$BOW_TMPL_USER_LOGINOUT$",strConstLogout));
    }
}
