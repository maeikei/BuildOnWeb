#ifndef BOW_SOURCE_VIEW_HPP
#define BOW_SOURCE_VIEW_HPP
#include <string>
#include <list>
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
        virtual bool getContent(const string &doc_root,string &contents);
    private:
        const string workspace_;
        const string git_repositories_;
        const list<string> env_build_commands_;
        const list<string> env_build_commands_debug_;
    };
}
#endif // BOW_SOURCE_VIEW_HPP
