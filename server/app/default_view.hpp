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
        virtual void create(const std::string &uri);
        //http method
        virtual void get(void);
        virtual void post(void);
        virtual void put(void);
        virtual void remove(void);
    };
    class DefaultView: public http::server_threadpool::ReplyView
    {
    public:
        DefaultView(const string &uri);
        virtual ~DefaultView();
        virtual bool getContent(const string &doc_root,string &contents);
    private:
    };
}
#endif // BOW_DEFAULT_VIEW_HPP
