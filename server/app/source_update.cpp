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
,source_(source)
,path_(path)
,wc_(ws)
,output_(wc_ + "/.bow_output/output.log")
,build_output_(wc_ + "/.bow_output/build.log")
,_env_build_commands
{
    "rm -f " + output_ + "  " + build_output_,
//    "cd " + _wc + " && git diff -q ",
    "cd " + wc_ + " && git commit -q -am \"web auto modify by user " + "" + "\" ",
    "cd " + wc_ + " && git push -q --force",
    "make exe -C "+ wc_ +" 2>&1 | tee > " + build_output_,
}
{
#ifdef DEBUG_PARAM
    cout << "_source=<" << _source<< ">" << endl;
    cout << "_path=<" << _path<< ">" << endl;
#endif
    fs::path sourefile(path_);
    if( not fs::exists(sourefile) )
    {
        // new files.
    }
    else
    {
        string source_replaced(source_);
        html2text(source_replaced);
        std::ofstream ofs;
        ofs.open( path_ );
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
    // program run result
    {
        std::ifstream isf(output_.c_str(), std::ios::in | std::ios::binary);
        if (isf)
        {
            // Fill out the reply to be sent to the client.
            char buf[512];
            while (isf.read(buf, sizeof(buf)).gcount() > 0) {
                contents.append(buf, isf.gcount());
            }
            isf.close();
        }
    }
    contents += "---------build log start---------\n";
    // build result
    {
        std::ifstream isf(build_output_.c_str(), std::ios::in | std::ios::binary);
        if (isf)
        {
            // Fill out the reply to be sent to the client.
            char buf[512];
            while (isf.read(buf, sizeof(buf)).gcount() > 0) {
                contents.append(buf, isf.gcount());
            }
            isf.close();
        }
    }
    contents += "---------build log end----------\n";
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

