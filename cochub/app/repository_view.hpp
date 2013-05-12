#ifndef BOW_REPOSITORY_VIEW_HPP
#define BOW_REPOSITORY_VIEW_HPP
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
    class RepositoryApp: public http::server_threadpool::resource
    {
    public:
        RepositoryApp(void);
        virtual ~ RepositoryApp();
        virtual void create(const std::string &uri,const string& user_uid);
        //http method
        virtual void get(const std::string &doc_root, http::server_threadpool::reply& rep);
    };
    class LastPostion;
    class DirecoryView;
    class RepositoryView: public DirecoryView
    {
    public:
        RepositoryView(const string &username,const string &user_uid,const string &category,const string &repo);
        virtual ~ RepositoryView();
    private:
        const string user_;
        const string user_uid_;
        const string category_;
        const string repo_;
    private:
    };
}
#endif // BOW_REPOSITORY_VIEW_HPP
