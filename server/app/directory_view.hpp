#ifndef BOW_DIRECTORY_VIEW_HPP
#define BOW_DIRECTORY_VIEW_HPP
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
    class DirecoryView: public http::server_threadpool::ReplyView
    {
    public:
        DirecoryView(const string &username);
    private:
    };
}
#endif // BOW_DIRECTORY_VIEW_HPP
