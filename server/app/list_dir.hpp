#ifndef BUILD_ON_WEB_LIST_DIR_HPP
#define BUILD_ON_WEB_LIST_DIR_HPP
#include <string>
#include <map>
using namespace std;

namespace http {
    namespace server_threadpool {
        struct reply;
        struct request;
    }
}

namespace BuildOnWeb {
    class BOWListDir {
    public:
        BOWListDir(string & dir);
        void response(const http::server_threadpool::request& req, http::server_threadpool::reply& rep);
    private:
        const string & _dir;
    };
}
#endif // BUILD_ON_WEB_LIST_DIR_HPP
