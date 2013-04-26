#ifndef BUILD_ON_WEB_TEMPLATE_HPP
#define BUILD_ON_WEB_TEMPLATE_HPP
#include <string>
#include <map>
using namespace std;
namespace BuildOnWeb {
    class BOWTemplate {
    public:
        BOWTemplate(string & content);
        void replace(void);
    private:
        string & _content;
        const string _file;
        const string _mode;
        const string _output;
        const map<string,string> _extensions;
    };
}
#endif // BUILD_ON_WEB_TEMPLATE_HPP
