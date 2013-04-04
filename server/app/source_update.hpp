#ifndef BUILD_ON_WEB_SOURCE_UPDATE_HPP
#define BUILD_ON_WEB_SOURCE_UPDATE_HPP
#include <string>
#include <list>
using namespace std;

namespace http {
namespace server_threadpool {
struct reply;
}
}

namespace BuildOnWeb {
    class BOWSoureUpdate {
    public:
        BOWSoureUpdate(const string & source,const string & path,http::server_threadpool::reply& rep);
        void run(void);
    private:
        const string & _source;
        const string & _path;
        http::server_threadpool::reply& _rep;
        const string _output;
        const list<string> _env_build_commands;
    };
}
#endif // BUILD_ON_WEB_SOURCE_UPDATE_HPP
