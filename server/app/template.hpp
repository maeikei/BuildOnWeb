#ifndef BUILD_ON_WEB_TEMPLATE_HPP
#define BUILD_ON_WEB_TEMPLATE_HPP
#include <string>
using namespace std;
namespace BuildOnWeb {
    class BOWTemplate {
    public:
        BOWTemplate(string & content);
    private:
        string & _content;
    };
}
#endif // BUILD_ON_WEB_TEMPLATE_HPP
