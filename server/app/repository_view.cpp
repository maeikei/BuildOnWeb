#include "resource.hpp"
#include "reply_view.hpp"
#include "repository_view.hpp"
#include "last_position.hpp"
using namespace BOW;


#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>



RepositoryApp::RepositoryApp(void)
{
}
RepositoryApp::~ RepositoryApp()
{
}
void RepositoryApp::create(const std::string &uri,const std::string &user_uid)
{
}
void RepositoryApp::get(const std::string &doc_root, http::server_threadpool::reply& rep)
{
}
void RepositoryApp::post(const std::string &doc_root, http::server_threadpool::reply& rep)
{
}
void RepositoryApp::put(const std::string &doc_root, http::server_threadpool::reply& rep)
{
}
void RepositoryApp::remove(const std::string &doc_root, http::server_threadpool::reply& rep)
{
}

//#define DEBUG_CONTENT
RepositoryView::RepositoryView(const string &username,const string &user_uid)
:user_(username)
,user_uid_(user_uid)
,last_(new LastPostion(user_uid_))
{
}

RepositoryView::~ RepositoryView()
{
}

void RepositoryView::getDist(string &dist)
{
    dist = "/users/" + user_ + "/";
    dist += last_->get();
}
