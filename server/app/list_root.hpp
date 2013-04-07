#ifndef BUILD_ON_WEB_LIST_ROOT_HPP
#define BUILD_ON_WEB_LIST_ROOT_HPP
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
    class BOWListDir;
}

namespace BuildOnWeb {
    class BOWListRoot :public BOWListDir {
    public:
        BOWListRoot();
        virtual const list<string> &getlist(void);
    private:
        const list<string> languagelist;
    };
}
#endif // BUILD_ON_WEB_LIST_ROOT_HPP
