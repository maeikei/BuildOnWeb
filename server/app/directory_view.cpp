#include "reply_view.hpp"
#include "directory_view.hpp"
using namespace BOW;

#define DEBUG_PARAM

DirecoryView::DirecoryView(const string &username,const string &category,const string &repo,const list<string> &path )
{
#ifdef DEBUG_PARAM
    std::cout << __func__ <<":username=" <<  username << endl;
    std::cout << __func__ <<":category=" <<  category << endl;
    std::cout << __func__ <<":repo=" <<  repo << endl;
#endif
}

bool DirecoryView::getContent(const string &doc_root,string &contents)
{
    return true;
}
DirecoryView::~DirecoryView()
{
}