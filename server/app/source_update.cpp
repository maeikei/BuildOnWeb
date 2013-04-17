#include "reply_view.hpp"
#include "source_update.hpp"
using namespace BOW;
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <reply.hpp>

#include <iostream>
#include <fstream>
using namespace std;

void text2html(string &txt);
void html2text(string &txt);


//#define DEBUG_PARAM


SoureUpdate::SoureUpdate(const string & source,const string & path,const string & ws)
:ReplyView()
,_source(source)
,_path(path)
,_wc(ws)
,_output(_wc + "/.bow_output/output.log")
,_env_build_commands
{
//    "cd " + _wc + " && git diff -q ",
    "cd " + _wc + " && git commit -q -am \"web auto modify by user " + "" + "\" ",
    "cd " + _wc + " && git push -q --force",
    "make exe -C "+ _wc ,
}
{
#ifdef DEBUG_PARAM
    cout << "_source=<" << _source<< ">" << endl;
    cout << "_path=<" << _path<< ">" << endl;
#endif
    fs::path sourefile(_path);
    if( not fs::exists(sourefile) )
    {
        // new files.
    }
    else
    {
        string source_replaced(_source);
        html2text(source_replaced);
        std::ofstream ofs;
        ofs.open( _path );
        ofs << source_replaced;
        ofs.close();
    }
    for(auto it = _env_build_commands.begin(); it != _env_build_commands.end();it++)
    {
        system(it->c_str());
    }
}


bool SoureUpdate::getContent(const string &doc_root,string &contents)
{
    // read out file .
    std::ifstream ifs(_output.c_str(), std::ios::in | std::ios::binary);
    char buf[512];
    while (ifs.read(buf, sizeof(buf)).gcount() > 0)
    {
        contents.append(buf, ifs.gcount());
    }
    return true;
}
    
    
static const map<string,string> mapConstTextHtmlPair =
{
    {"\"","&quot;"},
    {"<","&lt;"},
    {">","&gt;"},
};
void text2html(string &txt)
{
    for(auto it = mapConstTextHtmlPair.begin();it != mapConstTextHtmlPair.end();it++)
    {
        boost::algorithm::replace_all(txt,it->first,it->second);
    }
}

void html2text(string &txt)
{
    for(auto it = mapConstTextHtmlPair.begin();it != mapConstTextHtmlPair.end();it++)
    {
        boost::algorithm::replace_all(txt,it->second,it->first);
    }
}

