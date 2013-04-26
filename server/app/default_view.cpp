#include "resource.hpp"
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
