#ifndef BOW_REDIRECT_VIEW_HPP
#define BOW_REDIRECT_VIEW_HPP
#include <string>
#include <vector>
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
    class RedirectApp: public http::server_threadpool::resource
    {
    public:
        RedirectApp(void);
        virtual ~ RedirectApp();
        virtual void create(const std::string &uri,const std::string &user_uid);
        //http method
        virtual void get(const std::string &doc_root, http::server_threadpool::reply& rep);
    };
    class NaviView;
    class RedirectView: public http::server_threadpool::ReplyView
    {
    public:
        RedirectView(const string &username,const string &user_uid);
        virtual ~RedirectView();
    protected:
        virtual std::map<std::string,std::string> fillHeader(void);
        virtual int status(void);
    private:
        const string uri_;
        const string user_;
        const std::string user_uid_;
    private:
    };
}
#endif // BOW_REDIRECT_VIEW_HPP
