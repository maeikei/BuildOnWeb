#ifndef BUILD_ON_WEB_LIST_DIR_HPP
#define BUILD_ON_WEB_LIST_DIR_HPP
#include <string>
#include <map>
using namespace std;
namespace BuildOnWeb {
    class BOWListDir {
    public:
        BOWListDir(string & dir);
        void replace(void);
    private:
        string & _dir;
    };
}
#endif // BUILD_ON_WEB_LIST_DIR_HPP
