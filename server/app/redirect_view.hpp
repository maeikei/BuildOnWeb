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
        class ReplyView;
        class resource;
    }
}

namespace BOW {
    class RedirectApp: public http::server_threadpool::resource
    {
    };
    class LastPostion;
    class NaviView;
    class RedirectView: public http::server_threadpool::ReplyView
    {
    public:
        RedirectView(const string &username,const string &user_uid);
        virtual ~RedirectView();
        virtual void getDist(string &dist);
    private:
        const string user_;
        const std::string user_uid_;
        shared_ptr<LastPostion> last_;
    private:
    };
}
#endif // BOW_REDIRECT_VIEW_HPP
