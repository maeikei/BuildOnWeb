#include "resource.hpp"
#include "reply_view.hpp"
#include "navi_view.hpp"
#include "last_position.hpp"
#include "redirect_view.hpp"
using namespace BOW;


#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>



#define DEBUG_APP_PARAM
NaviApp::NaviApp(void)
{
}
NaviApp::~ NaviApp()
{
}
void NaviApp::create(const std::string &uri,const std::string &remote)
{
#ifdef DEBUG_APP_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":uri=<" << uri << ">" << std::endl;
#endif
    std::list<std::string> results;
    boost::split(results, uri, boost::is_any_of("/"));
    results.pop_front();
    results.pop_front();
    std::string username(results.front());
    results.pop_front();
    string use_id(username);
    if("guest"==username)
    {
        use_id += "_from_";
        use_id += boost::algorithm::replace_all_copy(remote,".","_");
    }
    if(results.empty())
    {
        reply_ = std::shared_ptr<http::server_threadpool::ReplyView>(new NaviView(username,use_id));
    }
    else
    {
        std::string category(results.front());
        reply_ = std::shared_ptr<http::server_threadpool::ReplyView>(new NaviView(username,use_id,category));
    }
}
void NaviApp::get(const std::string &doc_root, http::server_threadpool::reply& rep)
{
    reply_->responseGet(doc_root,rep);
}

// #define DEBUG_CONTENT
//#define DEBUG_NAVI_TREE

NaviView::NaviView(const string &username,const string &user_uid)
:user_(username)
,user_uid_(user_uid)
,category_()
{
}

NaviView::NaviView(const string &username,const string &user_uid,const string &category)
:user_(username)
,user_uid_(user_uid)
,category_(category)
{
}
NaviView::~NaviView()
{
}

bool NaviView::readTemplate(const string &doc_root,string &contents)
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
        is.close();
    }
    LastPostion last(user_uid_);
    if(category_.empty())
    {
        last.set("");
    }
    else
    {
        last.set(category_);
    }
    return true;
}

std::map<std::string,std::string> NaviView::readReplaceContents(void)
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

void NaviView::create_table(std::map<std::string,std::string> &replace)
{
    // search all navi items and create a table.
    std::string table_navi;
    unsigned int counter = iConstColNum_;
    for(auto it = navi_items_.begin();it != navi_items_.end();it++ )
    {
#ifdef DEBUG_CONTENT
        std::cout << "it->first=<" << it->first << ">" << std::endl;
#endif
        // if no category is speciled,list all categories
        if(true == category_.empty())
        {
            string tr("");
            if(iConstColNum_ == counter)
            {
                tr += "<tr>\n";
            }
            tr +=  "<td class=\"content\">";
            tr +=  "<a href=\"/users";
            tr +=  "/" + user_;
            tr +=  "/" + it->first;
            tr +=  "\">";
            tr +=  it->first;
            tr += "</a></td>\n";
            if(0 == --counter )
            {
                tr += "</tr>\n";
                counter = iConstColNum_;
            }
            table_navi += tr;
            continue;
        }
        // category is speciled,list all in the category
        if(category_ == it->first)
        {
            unsigned int counter = iConstColNum_;
            for(auto subit = it->second.begin();subit != it->second.end();subit++)
            {
                string tr("");
                if(iConstColNum_ == counter)
                {
                    tr += "<tr>\n";
                }
                tr +=  "<td class=\"content\">";
                tr +=  "<a href=\"/users";
                tr +=  "/" + user_;
                tr +=  "/" + it->first;
                tr +=  "/" + *subit;
                tr +=  "\">";
                tr +=  *subit;
                tr += "</a></td>\n";
                if(0 == --counter)
                {
                    tr += "</tr>\n";
                    counter = iConstColNum_;
                }
                table_navi += tr;
            }
            if(iConstColNum_ != counter)
            {
                table_navi += "</tr>\n";
            }
            break;
        }
    }
    if(true == category_.empty() && iConstColNum_ != counter)
    {
        table_navi += "</tr>\n";
    }
    replace.insert(pair<string,string>("$BOW_TMPL_NAVI_TABLE$",table_navi));
}

static const string strConstSourcePath =
"<a href=\"/users/$user_$\">$user_$</a>";
static const string strConstSourcePathCategory =
"<a href=\"/users/$user_$\">$user_$</a><span>/</span><a href=\"/users/$user_$/$category_$\">$category_$</a>";

void NaviView::create_source_path(std::map<std::string,std::string> &replace)
{
    std::string path(strConstSourcePath);
    // category_
    if(category_.empty())
    {
        boost::algorithm::replace_all(path,"$user_$",user_);
    }
    else
    {
        path = strConstSourcePathCategory;
        boost::algorithm::replace_all(path,"$user_$",user_);
        boost::algorithm::replace_all(path,"$category_$",category_);
        
    }
    replace.insert(pair<string,string>("$BOW_TMPL_NAVI_PATH$",path));
}


static const string strConstLogin =
"<a href=\"/login.php\" data-method=\"post\" id=\"login\">login</a>";
static const string strConstLogout =
"<a href=\"/logout.php\" data-method=\"post\" id=\"logout\">logout</a>";

void NaviView::create_loginout(std::map<std::string,std::string> &replace)
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





static const string strConstFindRepoCmdHeader("ssh eikei@192.168.0.140 ls  /Volumes/DataHD/BuildOnWeb/repositories/");
static map<string,vector<string>> mapNaviTree;

static const string strConstFindRepoCmdTemp(".temp_repo.list");

static void searchRepo(const string & category)
{
    string cmd(strConstFindRepoCmdHeader);
    cmd += category;
    cmd += "> ";
    cmd += strConstFindRepoCmdTemp;
    system(cmd.c_str());
    {
        std::ifstream isf(strConstFindRepoCmdTemp.c_str(), std::ios::in);
        if (isf)
        {
            // Fill out the reply to be sent to the client.
            vector<string> repos;
            while ( not isf.eof()) {
                string rep;
                isf >> rep;
                boost::algorithm::replace_all(rep,".git","");
                if(not rep.empty())
                {
                    repos.push_back(rep);
                }
#ifdef DEBUG_NAVI_TREE
                std::cout << "rep=<" << rep << ">" << std::endl;
#endif
            }
            mapNaviTree.insert(std::pair<string,vector<string>>(category,repos));
            isf.close();
        }
    }
    cmd = "rm -rf ";
    cmd += strConstFindRepoCmdTemp;
    system(cmd.c_str());
}
static const map<string,vector<string>> & buildNaviTree(void)
{
    searchRepo("ruby");
    searchRepo("c_cxx");
    return mapNaviTree;
}
const map<string,vector<string>> &NaviView::navi_items_ = buildNaviTree();
