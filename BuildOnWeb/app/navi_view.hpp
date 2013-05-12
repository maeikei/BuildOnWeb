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
        struct reply;
        class ReplyView;
        class resource;
    }
}

namespace BOW {
    class NaviApp: public http::server_threadpool::resource
    {
    public:
        NaviApp(void);
        virtual ~ NaviApp();
        virtual void create(const std::string &uri,const std::string &user_uid);
        //http method
        virtual void get(const std::string &doc_root, http::server_threadpool::reply& rep);
    };

    class LastPostion;
    class NaviView: public http::server_threadpool::ReplyView
    {
    public:
        NaviView(const string &username,const string &user_uid);
        NaviView(const string &username,const string &user_uid,const string &category);
        virtual ~NaviView();
    protected:
        virtual bool readBody(const std::string &doc_root,std::string &contents);
        virtual std::map<std::string,std::string> bodyVars(void);
    private:
        const string user_;
        const string user_uid_;
        const string category_;
        
        static const unsigned int iConstColNum_ = 5;
        static const map<string,vector<string>> &navi_items_;
    private:
        void create_table(std::map<std::string,std::string> &replace);
        void create_source_path(std::map<std::string,std::string> &replace);
        void create_loginout(std::map<std::string,std::string> &replace);
    };
}
#endif // BOW_NAVI_VIEW_HPP
