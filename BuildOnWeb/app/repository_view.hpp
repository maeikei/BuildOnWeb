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
typedef std::shared_ptr<http::server_threadpool::ReplyView> ReplyViewPtr;

namespace BOW {
    class RepositoryApp: public http::server_threadpool::resource
    {
    public:
        RepositoryApp(void);
        virtual ~ RepositoryApp();
        virtual ReplyViewPtr create(const std::string &uri,const string& user_uid);
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
