#include "reply.hpp"
#include "request.hpp"
#include "reply_view.hpp"
using namespace http::server_threadpool;

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>


ReplyView::ReplyView(void)
{
    
}
void ReplyView::responseGet(const std::string &doc_root, reply& rep)
{
    if( not this->readBody(doc_root,rep.content) )
    {
        rep.content.clear();
        rep = reply::stock_reply(reply::not_found);
        return;
    }
    auto replace = this->bodyVars();
    for(auto it = replace.begin(); it !=  replace.end();it++)
    {
        boost::algorithm::replace_all(rep.content,it->first,it->second);
    }
    this->replace_basic(rep);
    rep.status = static_cast<reply::status_type>(this->status());
    
    auto header = fillHeader();
    rep.headers.resize(1 + header.size());
    rep.headers[0].name = "Content-Length";
    rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
    int i = 1;
    for(auto it = header.begin(); it !=  header.end();it++)
    {
        rep.headers[i].name  = it->first;
        rep.headers[i].value = it->second;
        i++;
    }
    
}

int ReplyView::status(void)
{
    return reply::ok;
}

std::map<std::string,std::string> ReplyView::fillHeader(void)
{
    std::map<std::string,std::string>  ret;
    ret.insert(std::pair<std::string,std::string>("Content-Type","text/html"));
    return ret;
}
bool ReplyView::readBody(const std::string &doc_root,std::string &contents)
{
    contents.clear();
    return true;
}

// replace commont of reply.
void ReplyView::replace_basic(reply& rep)
{
    // replace css paht & javascript to root path.
    {
        boost::algorithm::replace_all(rep.content,
                                      "<link rel=\"stylesheet\" href=\"css/",
                                      "<link rel=\"stylesheet\" href=\"/css/");
        boost::algorithm::replace_all(rep.content,
                                      "<script src=\"js/",
                                      "<script src=\"/js/");
        boost::algorithm::replace_all(rep.content,
                                      "<script src=\"ace/",
                                      "<script src=\"/ace/");
    }    
}

std::map<std::string,std::string> ReplyView::bodyVars(void)
{
    std::map<std::string,std::string> ret;
    return ret;
}