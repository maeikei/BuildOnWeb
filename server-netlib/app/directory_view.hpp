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
    }
}


namespace BOW {
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
