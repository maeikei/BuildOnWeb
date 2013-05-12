#ifndef BOW_SETTING_VIEW_HPP
#define BOW_SETTING_VIEW_HPP
#include <string>
#include <list>
#include <map>
#include <memory>
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
    class SettingApp: public http::server_threadpool::resource
    {
    public:
       SettingApp(void);
        virtual ~ SettingApp();
        virtual void create(const std::string &uri,const std::string &user_uid);
        // http method
        virtual void get(const std::string &doc_root, http::server_threadpool::reply& rep);
        virtual void post(const std::string &data, http::server_threadpool::reply& rep);
    };
    
    class GitWorker;
    class SettingView: public http::server_threadpool::ReplyView
    {
    public:
        SettingView(const string &uri,const string &username,const std::string &user_uid);
        virtual ~ SettingView();
    protected:
        virtual bool readBody(const std::string &doc_root,std::string &contents);
        virtual std::map<std::string,std::string> bodyVars(void);

        virtual void post(const std::string &data);
        virtual bool readPostReply(std::string &contents);
    private:
        void create_source(std::map<std::string,std::string> &replace);
        void create_output(std::map<std::string,std::string> &replace);
        void read_output(string &output);
        void create_loginout(std::map<std::string,std::string> &replace);
    private:
        const string uri_;
        const string user_;
        const string user_uid_;
    };
}
#endif // BOW_SETTING_VIEW_HPP
