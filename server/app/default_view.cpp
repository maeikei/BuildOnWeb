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

DefaultApp::DefaultApp(void)
{
}
DefaultApp::~ DefaultApp()
{
}
void DefaultApp::create(const std::string &uri)
{
}
void DefaultApp::get(void)
{
}
void DefaultApp::post(void)
{
}
void DefaultApp::put(void)
{
}
void DefaultApp::remove(void)
{
}
