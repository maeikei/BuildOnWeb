#include "resource.hpp"
#include "mime_types.hpp"
#include "reply.hpp"
#include "reply_view.hpp"
#include "default_view.hpp"
using namespace BOW;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>

#define DEBUG_PARAM
//#define DEBUG_CONTENT

void text2html(string &txt);

DefaultView::DefaultView(const string &uri)
:ReplyView()
,uri_(uri)
{
}
DefaultView::~DefaultView()
{
}

bool DefaultView::readBody(const string &doc_root,string &contents)
{
    // Open the file to send back.
    std::string full_path = doc_root + uri_;
    std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
    if (!is)
    {
        return false;
    }    
#ifdef DEBUG_PATH
	std::cout << "request_path=" << request_path << std::endl;
#endif
    // Fill out the reply to be sent to the client.
    char buf[512];
    while (is.read(buf, sizeof(buf)).gcount() > 0) {
        contents.append(buf, is.gcount());
    }
    is.close();
    return true;
}

std::map<std::string,std::string> DefaultView::fillHeader(void)
{
 
    fs::path rq_path(uri_);
    std::string extension = rq_path.extension().string();
    std::map<std::string,std::string>  ret;
    ret.insert(std::pair<std::string,std::string>("Content-Type",http::server_threadpool::mime_types::extension_to_type(extension)));
    return ret;
}




#define DEBUG_APP_PARAM
DefaultApp::DefaultApp(void)
{
}
DefaultApp::~ DefaultApp()
{
}
ReplyViewPtr DefaultApp::create(const std::string &uri,const std::string &user_uid)
{
#ifdef DEBUG_APP_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":uri=<" << uri << ">" << std::endl;
#endif
    return  ReplyViewPtr(new DefaultView(uri));
}
