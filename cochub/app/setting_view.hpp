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
typedef std::shared_ptr<http::server_threadpool::ReplyView> ReplyViewPtr;


namespace BOW {
    class SettingApp: public http::server_threadpool::resource
    {
    public:
       SettingApp(void);
        virtual ~ SettingApp();
        virtual ReplyViewPtr create(const std::string &uri,const std::string &user_uid);
    };
    
    class GitWorker;
    class SettingView: public http::server_threadpool::ReplyView
    {
    public:
        SettingView(const string &uri,const string &username,const std::string &user_uid,const std::string &project);
        virtual ~ SettingView();
    protected:
        virtual bool readBody(const std::string &doc_root,std::string &contents);
        virtual std::map<std::string,std::string> bodyVars(void);

        virtual void post(const std::string &data);
        virtual bool readPostReply(std::string &contents);
    private:
        void create_output(std::map<std::string,std::string> &replace);
        void read_output(string &output);
        void create_loginout(std::map<std::string,std::string> &replace);
        void getbuildType(const std::string &root,std::string &contents);
    private:
        const string uri_;
        const string user_;
        const string user_uid_;
        const string project_;
        const string workspace_;
        const list<string> env_build_commands_;
    };
}
#endif // BOW_SETTING_VIEW_HPP
