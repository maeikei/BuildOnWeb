#ifndef BOW_NAVI_VIEW_HPP
#define BOW_NAVI_VIEW_HPP
#include <string>
#include <vector>
#include <map>
using namespace std;

namespace http
{
    namespace server_threadpool
    {
        class ReplyView;
    }
}

namespace BOW {
    class NaviView: public http::server_threadpool::ReplyView
    {
    public:
        NaviView(const string &username);
        NaviView(const string &username,const string &category);
        virtual ~NaviView();
        virtual bool getContent(const string &doc_root,string &contents);
    private:
        const string user_;
        const string category_;
        const map<string,vector<string>> navi_items_;
    private:
        void replace_source_path(string &contents);
    };
}
#endif // BOW_NAVI_VIEW_HPP
