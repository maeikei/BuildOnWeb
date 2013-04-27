#ifndef BOW_DIRECTORY_VIEW_HPP
#define BOW_DIRECTORY_VIEW_HPP
#include <string>
#include <list>
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
    class DirecoryApp: public http::server_threadpool::resource
    {
    public:
        DirecoryApp(void);
        virtual ~ DirecoryApp();
        virtual void create(const std::string &uri,const std::string &user_uid);
        //http method
        virtual void get(const std::string &doc_root, http::server_threadpool::reply& rep);
    };
    class SourceView;
    class DirecoryView: public http::server_threadpool::ReplyView
    {
    public:
        DirecoryView(const string &username,const string &user_uid,
                     const string &category,const string &repo);
        DirecoryView(const string &username,const string &user_uid,
                     const string &category,const string &repo,const list<string> &path);
        virtual ~DirecoryView();
    protected:
        virtual bool readBody(const std::string &doc_root,std::string &contents);
        virtual std::map<std::string,std::string> bodyVars(void);
    private:
        void create_table(std::map<std::string,std::string> &replace);
        void create_source_path(std::map<std::string,std::string> &replace);
        void create_loginout(std::map<std::string,std::string> &replace);
    private:
        const string user_;
        const string user_uid_;
        const string category_;
        const string repo_;
        const list<string> path_;
        const string workspace_;
    };
}
#endif // BOW_DIRECTORY_VIEW_HPP
