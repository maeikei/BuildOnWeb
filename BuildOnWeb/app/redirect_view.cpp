#include "resource.hpp"
#include "reply.hpp"
#include "reply_view.hpp"
#include "redirect_view.hpp"
#include "last_position.hpp"
#include "utilities.hpp"
using namespace BOW;


#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>


#define DEBUG_APP_PARAM

RedirectApp::RedirectApp(void)
{
}
RedirectApp::~ RedirectApp()
{
}
void RedirectApp::create(const std::string &uri,const std::string &remote)
{
#ifdef DEBUG_APP_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":uri=<" << uri << ">" << std::endl;
#endif
    std::string username;
    parseUri(uri,username);
    string user_uid(username);
    if("guest"==username)
    {
        user_uid += "_from_";
        user_uid += boost::algorithm::replace_all_copy(remote,".","_");
    }
    reply_ = std::shared_ptr<http::server_threadpool::ReplyView>(new RedirectView(username,user_uid));
}
void RedirectApp::get(const std::string &doc_root, http::server_threadpool::reply& rep)
{
    reply_->responseGet(doc_root,rep);
}

//#define DEBUG_CONTENT
RedirectView::RedirectView(const string &username,const string &user_uid)
:user_(username)
,user_uid_(user_uid)
{
}

RedirectView::~ RedirectView()
{
}

std::map<std::string,std::string> RedirectView::fillHeader(void)
{
    std::map<std::string,std::string>  ret;
    ret.insert(std::pair<std::string,std::string>("Content-Type","text/html"));
    string location("/users/" + user_ + "/");
    LastPostion last(user_uid_);
    location += last.get();
#ifdef DEBUG_APP_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":location=<" << location << ">" << std::endl;
#endif
    ret.insert(std::pair<std::string,std::string>("Location",location));
    return ret;
}

int RedirectView::status(void)
{
    return http::server_threadpool::reply::moved_temporarily;
}
