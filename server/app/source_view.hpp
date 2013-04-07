#ifndef BOW_SOURCE_VIEW_HPP
#define BOW_SOURCE_VIEW_HPP
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
    class SourceView: public http::server_threadpool::ReplyView
    {
    public:
        SourceView(const string &username);
    private:
    };
}
#endif // BOW_SOURCE_VIEW_HPP
