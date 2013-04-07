#ifndef BUILD_ON_WEB_LIST_REPO_HPP
#define BUILD_ON_WEB_LIST_REPO_HPP
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
    class BOWListRepo :public BOWListDir {
    public:
        BOWListRepo(const string & _root,const string & _lang);
        virtual list<string> getlist();
    private:
        const map<string,string> reposlist;
    };
}
#endif // BUILD_ON_WEB_LIST_REPO_HPP
