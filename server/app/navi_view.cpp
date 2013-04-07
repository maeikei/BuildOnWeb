#include "reply_view.hpp"
#include "navi_view.hpp"
using namespace BOW;

NaviView::NaviView(const string &username)
:username_(username)
,category_("")
{
}

NaviView::NaviView(const string &username,const string &category)
:username_(username)
,category_(category)
{
    
}
