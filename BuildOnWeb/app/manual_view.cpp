#include "resource.hpp"
#include "reply_view.hpp"
#include "manual_view.hpp"
using namespace BOW;


#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>

//#define DEBUG_PARAM
//#define DEBUG_CONTENT

#define DEBUG_APP_PARAM

ManualApp::ManualApp(void)
{
}
ManualApp::~ ManualApp()
{
}
ReplyViewPtr ManualApp::create(const std::string &uri,const std::string &remote)
{
#ifdef DEBUG_APP_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":uri=<" << uri << ">" << std::endl;
#endif
    std::list<std::string> results;
    boost::split(results, uri, boost::is_any_of("/"));
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    std::string manual(results.front());
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    std::string category(results.front());
    results.pop_front();
    if(results.empty())
    {
        return;
    }
    std::string repo(results.front());
#ifdef DEBUG_APP_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":manual=<" << manual << ">" << std::endl;
	std::cout << typeid(this).name() << ":" << __func__ << ":repo=<" << repo << ">" << std::endl;
#endif
    return  ReplyViewPtr(new ManualView(category,repo));
}


ManualView::ManualView(const string &category,const string &repo)
:ReplyView()
,category_(category)
,repo_(repo)
{
}

ManualView::~ManualView()
{
}

bool ManualView::readBody(const string &doc_root,string &contents)
{
    // Open the template file to add to contents.
    {
        std::string full_path = doc_root + "/BuildOnWebViewManual.html";
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

std::map<std::string,std::string> ManualView::bodyVars(void)
{
    std::map<std::string,std::string> ret;
    // replace real manual
    ret.insert(pair<string,string>("$BOW_TMPL_REAL_MAN$","/man/" + category_ + "/" + repo_ + ".html"));
    return ret;
}

