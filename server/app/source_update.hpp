#ifndef BOW_SOURCE_UPDATE_HPP
#define BOW_SOURCE_UPDATE_HPP
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
    class SoureUpdate: public http::server_threadpool::ReplyView
    {
    public:
        SoureUpdate(const string & source,const string & path,const string & ws);
        virtual bool getContent(const string &doc_root,string &contents);
    private:
        const string & source_;
        const string & path_;
        const string & wc_;
        const string output_;
        const string build_output_;
        const list<string> _env_build_commands;
    };
}
#endif // BOW_SOURCE_UPDATE_HPP
