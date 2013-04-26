#include "resource.hpp"
#include "reply_view.hpp"
#include "redirect_view.hpp"
#include "last_position.hpp"
using namespace BOW;


#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>



RedirectApp::RedirectApp(void)
{
}
RedirectApp::~ RedirectApp()
{
}
void RedirectApp::create(const std::string &uri,const std::string &user_uid)
{
}
void RedirectApp::get(const std::string &doc_root, http::server_threadpool::reply& rep)
{
}
void RedirectApp::post(const std::string &doc_root, http::server_threadpool::reply& rep)
{
}
void RedirectApp::put(const std::string &doc_root, http::server_threadpool::reply& rep)
{
}
void RedirectApp::remove(const std::string &doc_root, http::server_threadpool::reply& rep)
{
}

//#define DEBUG_CONTENT
RedirectView::RedirectView(const string &username,const string &user_uid)
:user_(username)
,user_uid_(user_uid)
,last_(new LastPostion(user_uid_))
{
}

RedirectView::~RedirectView()
{
}

void RedirectView::getDist(string &dist)
{
    dist = "/users/" + user_ + "/";
    dist += last_->get();
}
