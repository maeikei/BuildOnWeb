#include "resource.hpp"
#include "reply_view.hpp"
#include "source_view.hpp"
#include "directory_view.hpp"
#include "repository_view.hpp"
#include "last_position.hpp"
#include "utilities.hpp"
using namespace BOW;


#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>


#define DEBUG_APP_PARAM

RepositoryApp::RepositoryApp(void)
{
}
RepositoryApp::~ RepositoryApp()
{
}
void RepositoryApp::create(const std::string &uri,const std::string &remote)
{
#ifdef DEBUG_APP_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":uri=<" << uri << ">" << std::endl;
#endif
    std::string username;
    std::string category;
    std::string repo;
    parseUri(uri,username,category,repo);
    string use_id(username);
    if("guest"==username)
    {
        use_id += "_from_";
        use_id += boost::algorithm::replace_all_copy(remote,".","_");
    }
    reply_ = std::shared_ptr<http::server_threadpool::ReplyView>(new RepositoryView(username,use_id,category,repo));
}
void RepositoryApp::get(const std::string &doc_root, http::server_threadpool::reply& rep)
{
    reply_->responseGet(doc_root,rep);
}

//#define DEBUG_CONTENT
RepositoryView::RepositoryView(const string &username,const string &user_uid,const string &category,const string &repo)
:DirecoryView(username,user_uid,category,repo)
{
}
RepositoryView::~ RepositoryView()
{
}

#if 0
bool readBody(const std::string &doc_root,std::string &contents)
{
    return DirecoryView::readBody(doc_root,contents);
}
std::map<std::string,std::string> bodyVars(void)
{
    return 
}
#endif