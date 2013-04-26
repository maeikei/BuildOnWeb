#ifndef BOW_DIRECTORY_VIEW_HPP
#define BOW_DIRECTORY_VIEW_HPP
#include <string>
#include <list>
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
    class DirecoryApp: public http::server_threadpool::resource
    {
    public:
        DirecoryApp(void);
        virtual ~ DirecoryApp();
        virtual void create(const std::string &uri,const std::string &user_uid);
        //http method
        virtual void get(const std::string &doc_root, http::server_threadpool::reply& rep);
        virtual void post(const std::string &doc_root, http::server_threadpool::reply& rep);
        virtual void put(const std::string &doc_root, http::server_threadpool::reply& rep);
        virtual void remove(const std::string &doc_root, http::server_threadpool::reply& rep);
    };
    class SourceView;
    class DirecoryView: public SourceView
    {
    public:
        DirecoryView(const SourceView &src);
        virtual ~DirecoryView();
        virtual bool getContent(const string &doc_root,string &contents);
    private:
    };
}
#endif // BOW_DIRECTORY_VIEW_HPP
