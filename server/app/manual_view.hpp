#ifndef BOW_MANUAL_VIEW_HPP
#define BOW_MANUAL_VIEW_HPP
#include <string>
#include <list>
#include <map>
using namespace std;

namespace http
{
    namespace server_threadpool
    {
        class ReplyView;
        class resource;
    }
}

namespace BOW {
    class ManualApp: public http::server_threadpool::resource
    {
    };

    class ManualView: public http::server_threadpool::ReplyView
    {
    public:
        ManualView(const string &repo);
        virtual ~ManualView();
        virtual bool getContent(const string &doc_root,string &contents);
    private:
        const string repo_;
        const string workspace_;
        const string output_;
        const list<string> env_build_commands_;
        const list<string> env_build_commands_step_;
    private:
    };
}
#endif // BOW_MANUAL_VIEW_HPP
