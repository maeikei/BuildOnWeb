#ifndef BOW_SOURCE_VIEW_HPP
#define BOW_SOURCE_VIEW_HPP
#include <string>
#include <list>
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
    class SourceView: public http::server_threadpool::ReplyView
    {
    public:
        SourceView(const string &username,const string &category,const string &repo,const list<string> &path );
        virtual ~SourceView();
        virtual bool getContent(const string &doc_root,string &contents);
    protected:
        const string user_;
        const string category_;
        const string repo_;
        const list<string> &path_;
        const string workspace_;
        const string git_repositories_;
        const list<string> env_build_commands_;
        const list<string> env_build_commands_debug_;
        const map<string,string> extensions_;
        const map<string,string> names_;
    protected:
        void replace_source_path(string &contents);
        void replace_loginout(string &contents);
    };
}
#endif // BOW_SOURCE_VIEW_HPP
