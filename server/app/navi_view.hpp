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
        NaviView(const string &username,const string &user_uid);
        NaviView(const string &username,const string &user_uid,const string &category);
        virtual ~NaviView();
        virtual bool getContent(const string &doc_root,string &contents);
    private:
        const string user_;
        const std::string user_uid_;
        const string category_;
        static const unsigned int iConstColNum_ = 5;
        static const map<string,vector<string>> navi_items_;
    private:
        void replace_source_path(string &contents);
        void replace_loginout(string &contents);
    };
}
#endif // BOW_NAVI_VIEW_HPP
