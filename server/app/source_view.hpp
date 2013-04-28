#ifndef BOW_SOURCE_VIEW_HPP
#define BOW_SOURCE_VIEW_HPP
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
    class SourceApp: public http::server_threadpool::resource
    {
    public:
       SourceApp(void);
        virtual ~ SourceApp();
        virtual void create(const std::string &uri,const std::string &user_uid);
        //http method
        virtual void get(const std::string &doc_root, http::server_threadpool::reply& rep);
        virtual void post(const std::string &data, http::server_threadpool::reply& rep);
    };
    
    class GitWorker;
    class SourceView: public http::server_threadpool::ReplyView
    {
    public:
        SourceView(const string &uri_,const string &username,const string &user_uid,
                   const string &category,const string &repo,const list<string> &path );
        virtual ~SourceView();
    protected:
        virtual bool readBody(const std::string &doc_root,std::string &contents);
        virtual std::map<std::string,std::string> bodyVars(void);
        virtual void post(const std::string &data);
        
        virtual bool readPostReply(std::string &contents);
    private:
        void create_source(std::map<std::string,std::string> &replace);
        void create_output(std::map<std::string,std::string> &replace);
        void read_output(string &output);
        void create_source_path(std::map<std::string,std::string> &replace);
        void create_loginout(std::map<std::string,std::string> &replace);
    private:
        const string uri_;
        const string user_;
        const string user_uid_;
        const string category_;
        const string repo_;
        const list<string> path_;
        const string workspace_;
        const string output_;
        const string build_output_;
        shared_ptr<GitWorker> git_;
        const list<string> env_build_commands_;
        const list<string> _env_commit_commands;
        const map<string,string> extensions_;
        const map<string,string> names_;
    };
}
#endif // BOW_SOURCE_VIEW_HPP
