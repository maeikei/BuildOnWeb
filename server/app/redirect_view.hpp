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
    }
}

namespace BOW {
    class LastPostion;
    class NaviView;
    class RedirectView: public http::server_threadpool::ReplyView
    {
    public:
        RedirectView(const string &username,const string &user_uid);
        virtual ~RedirectView();
        virtual bool getContent(const string &doc_root,string &contents);
    private:
        const string user_;
        const std::string user_uid_;
        shared_ptr<LastPostion> last_;
    private:
    };
}
#endif // BOW_REDIRECT_VIEW_HPP
