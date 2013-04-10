#include "reply_view.hpp"
#include "navi_view.hpp"
using namespace BOW;


#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>


#define DEBUG_CONTENT


NaviView::NaviView(const string &username)
:user_(username)
,category_("")
{
}

NaviView::NaviView(const string &username,const string &category)
:user_(username)
,category_(category)
,navi_items_
 {
     {"c_cxx","helloworld"},
     {"c_cxx","helloworldcxx"},
 }
{
    
}
NaviView::~NaviView()
{
}

bool NaviView::getContent(const string &doc_root,string &contents)
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
    }
    // replace users
    {
        boost::algorithm::replace_all(contents,"$BOW_TMPL_USER$",user_);
    }
    // replace navi path
    {
        std::string path_navi;
        boost::algorithm::replace_all(contents,"$BOW_TMPL_NAVI_PATH$",path_navi);
    }
    // search all navi items and create a table.
    {
        std::string table_navi;
        boost::algorithm::replace_all(contents,"$BOW_TMPL_NAVI_TABLE$",table_navi);
    }
    return true;
}
