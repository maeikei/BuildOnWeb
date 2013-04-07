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
        BOWListRepo(const string &username,const string & lang);
        virtual const list<string> &getlist(void);
    private:
        list<string> reposlist;
        const map<string,string> repos_map;
    };
}
#endif // BUILD_ON_WEB_LIST_REPO_HPP
