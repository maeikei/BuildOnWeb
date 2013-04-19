#ifndef BOW_SOSIALS_VIEW_HPP
#define BOW_SOSIALS_VIEW_HPP
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
    class SourceView;
    class SosialView: public SourceView
    {
    public:
        SosialView(const SourceView &src);
        virtual ~SosialView();
        virtual bool getContent(const string &doc_root,string &contents);
    private:
        const string wc_temp_cmd_output_;
        const list<string> env_show_commands_;
        const list<string> branches_;
    };
}
#endif // BOW_SOSIALS_VIEW_HPP
