#include "reply_view.hpp"
#include "redirect_view.hpp"
#include "last_position.hpp"
using namespace BOW;


#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>


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

bool RedirectView::getContent(const string &doc_root,string &contents)
{
    string path = last_->get();
    return true;
}