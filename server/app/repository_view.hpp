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
        virtual void create(const std::string &uri);
        //http method
        virtual void get(void);
        virtual void post(void);
        virtual void put(void);
        virtual void remove(void);
    };
    class LastPostion;
    class RepositoryView: public http::server_threadpool::ReplyView
    {
    public:
        RepositoryView(const string &username,const string &user_uid);
        virtual ~ RepositoryView();
        virtual void getDist(string &dist);
    private:
        const string user_;
        const std::string user_uid_;
        shared_ptr<LastPostion> last_;
    private:
    };
}
#endif // BOW_REPOSITORY_VIEW_HPP
