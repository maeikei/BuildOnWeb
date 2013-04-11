#include "reply.hpp"
#include "request.hpp"
#include "reply_view.hpp"
using namespace http::server_threadpool;

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>


ReplyView::ReplyView(void)
{
    
}
void ReplyView::response(const std::string &doc_root, reply& rep)
{
    if( not this->getContent(doc_root,rep.content) )
    {
        rep.content.clear();
        rep = reply::stock_reply(reply::not_found);
        return;
    }
    this->replace(rep);
    rep.status = reply::ok;
    rep.headers.resize(2);
    rep.headers[0].name = "Content-Length";
    rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
    
    rep.headers[1].name = "Content-Type";
    rep.headers[1].value = "text/html";
}
// replace commont of reply.
void ReplyView::replace(reply& rep)
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
