#ifndef BOW_DIRECTORY_VIEW_HPP
#define BOW_DIRECTORY_VIEW_HPP
#include <string>
#include <list>
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
    class DirecoryApp: public http::server_threadpool::resource
    {
    public:
        DirecoryApp(void);
        virtual ~ DirecoryApp();
        virtual void create(const std::string &uri);
        //http method
        virtual void get(void);
        virtual void post(void);
        virtual void put(void);
        virtual void remove(void);
    };
    class SourceView;
    class DirecoryView: public SourceView
    {
    public:
        DirecoryView(const SourceView &src);
        virtual ~DirecoryView();
        virtual bool getContent(const string &doc_root,string &contents);
    private:
    };
}
#endif // BOW_DIRECTORY_VIEW_HPP
