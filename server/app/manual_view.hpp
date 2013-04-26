#ifndef BOW_MANUAL_VIEW_HPP
#define BOW_MANUAL_VIEW_HPP
#include <string>
#include <list>
#include <map>
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
    class ManualApp: public http::server_threadpool::resource
    {
    public:
        ManualApp(void);
        virtual ~ ManualApp();
        virtual void create(const std::string &uri,const std::string &user_uid);
        //http method
        virtual void get(const std::string &doc_root, http::server_threadpool::reply& rep);
        virtual void post(const std::string &doc_root, http::server_threadpool::reply& rep);
        virtual void put(const std::string &doc_root, http::server_threadpool::reply& rep);
        virtual void remove(const std::string &doc_root, http::server_threadpool::reply& rep);
    };

    class ManualView: public http::server_threadpool::ReplyView
    {
    public:
        ManualView(const string &repo);
        virtual ~ManualView();
        virtual bool getContent(const string &doc_root,string &contents);
    private:
        const string repo_;
        const string workspace_;
        const string output_;
        const list<string> env_build_commands_;
        const list<string> env_build_commands_step_;
    private:
    };
}
#endif // BOW_MANUAL_VIEW_HPP
