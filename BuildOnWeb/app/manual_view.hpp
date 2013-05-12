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
        struct reply;
        class ReplyView;
        class resource;
    }
}
typedef std::shared_ptr<http::server_threadpool::ReplyView> ReplyViewPtr;

namespace BOW {
    class ManualApp: public http::server_threadpool::resource
    {
    public:
        ManualApp(void);
        virtual ~ ManualApp();
        virtual ReplyViewPtr create(const std::string &uri,const std::string &user_uid);
    };

    class ManualView: public http::server_threadpool::ReplyView
    {
    public:
        ManualView(const string &category,const string &repo);
        virtual ~ManualView();
    protected:
        virtual bool readBody(const std::string &doc_root,std::string &contents);
        virtual std::map<std::string,std::string> bodyVars(void);
    private:
        const string category_;
        const string repo_;
    };
}
#endif // BOW_MANUAL_VIEW_HPP
