#include "resource.hpp"
#include "reply.hpp"
#include "reply_view.hpp"
#include "default_view.hpp"
using namespace BOW;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>

//#define DEBUG_PARAM
//#define DEBUG_CONTENT

void text2html(string &txt);

DefaultView::DefaultView(const string &uri)
:ReplyView()
{
}
DefaultView::~DefaultView()
{
}

bool DefaultView::getContent(const string &doc_root,string &contents)
{
    return true;
}



#define DEBUG_APP_PARAM
DefaultApp::DefaultApp(void)
{
}
DefaultApp::~ DefaultApp()
{
}
void DefaultApp::create(const std::string &uri,const std::string &user_uid)
{
#ifdef DEBUG_APP_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":uri=<" << uri << ">" << std::endl;
#endif
}
void DefaultApp::get(const std::string &doc_root, http::server_threadpool::reply& rep)
{
}
void DefaultApp::post(const std::string &doc_root, http::server_threadpool::reply& rep)
{
}
void DefaultApp::put(const std::string &doc_root, http::server_threadpool::reply& rep)
{
}
void DefaultApp::remove(const std::string &doc_root, http::server_threadpool::reply& rep)
{
}
