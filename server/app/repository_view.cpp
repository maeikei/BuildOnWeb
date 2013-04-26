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
void RepositoryApp::create(const std::string &uri)
{
}
void RepositoryApp::get(void)
{
}
void RepositoryApp::post(void)
{
}
void RepositoryApp::put(void)
{
}
void RepositoryApp::remove(void)
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
