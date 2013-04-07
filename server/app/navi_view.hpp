#ifndef BOW_NAVI_VIEW_HPP
#define BOW_NAVI_VIEW_HPP
#include <string>
using namespace std;

namespace http
{
    namespace server_threadpool
    {
        class ReplyView;
    }
}

namespace BOW {
    class NaviView: public http::server_threadpool::ReplyView
    {
    public:
        NaviView(const string &username);
        NaviView(const string &username,const string &category);
    private:
        const string username_;
        const string category_;
    };
}
#endif // BOW_NAVI_VIEW_HPP
