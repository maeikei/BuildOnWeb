#ifndef BUILD_ON_WEB_LIST_DIR_HPP
#define BUILD_ON_WEB_LIST_DIR_HPP
#include <string>
#include <list>
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
        BOWListDir(void);
        void response(const http::server_threadpool::request& req, http::server_threadpool::reply& rep);
        virtual list<string> getlist() = 0;
    };
}
#endif // BUILD_ON_WEB_LIST_DIR_HPP
