#ifndef BUILD_ON_WEB_ENV_HPP
#define BUILD_ON_WEB_ENV_HPP
#include <string>
#include <list>
using namespace std;
namespace BuildOnWeb {
    class BOWEnv {
    public:
        BOWEnv(const string & www_root);
        void build_env(void);
    private:
        const string & _www_root;
        const list<string> _env_build_commands;
        const list<string> _env_build_commands_debug;
    };
}
#endif // BUILD_ON_WEB_ENV_HPP
