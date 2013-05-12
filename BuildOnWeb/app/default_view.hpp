#ifndef BOW_DEFAULT_VIEW_HPP
#define BOW_DEFAULT_VIEW_HPP
#include <string>
#include <list>
#include <map>
#include <memory>
using namespace std;

namespace http
{
    namespace server_threadpool
    {
        struct reply;
        class ReplyView;
        class resource;
    }
}


namespace BOW {
    class DefaultApp: public http::server_threadpool::resource
    {
    public:
        DefaultApp(void);
        virtual ~ DefaultApp();
        virtual void create(const std::string &uri,const std::string &user_uid);
        //http method
        virtual void get(const std::string &doc_root, http::server_threadpool::reply& rep);
    };
    class DefaultView: public http::server_threadpool::ReplyView
    {
    public:
        DefaultView(const string &uri);
        virtual ~DefaultView();
    protected:
        virtual bool readBody(const std::string &doc_root,std::string &contents);
        virtual std::map<std::string,std::string> fillHeader(void);
   private:
        const string uri_;
    };
}
#endif // BOW_DEFAULT_VIEW_HPP
