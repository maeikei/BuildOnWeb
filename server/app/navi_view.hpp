#ifndef BOW_NAVI_VIEW_HPP
#define BOW_NAVI_VIEW_HPP
#include <string>
using namespace std;

namespace BOW {
    class NaviView {
    public:
        NaviView(const string &username);
        NaviView(const string &username,const string &category);
    private:
        const string username_;
        const string category_;
    };
}
#endif // BOW_NAVI_VIEW_HPP
