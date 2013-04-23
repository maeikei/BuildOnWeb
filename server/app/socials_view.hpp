#ifndef BOW_SOSIALS_VIEW_HPP
#define BOW_SOSIALS_VIEW_HPP
#include <string>
#include <map>
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
    struct GitLogMeshNote
    {
        string hash_;
        string hash_p_;
        string date_;
        bool operator() (const GitLogMeshNote &left) const;
    };
    typedef list<GitLogMeshNote> GitLogMeshList;
    class SosialView: public SourceView
    {
    public:
        SosialView(const SourceView &src);
        virtual ~SosialView();
        virtual bool getContent(const string &doc_root,string &contents);
    private:
        const string wc_temp_cmd_output_;
        const list<string> env_show_commands_;
        list<string> branches_;
        map<string,bool> log_check_;
        map<string,GitLogMeshList> git_log_mesh_;
        map<string,GitLogMeshList> git_log_mesh_pretty_;
    private:
        void createAllLogMesh(void);
        void createBranchMesh(const string &branch);
        void reduceLogMesh(void);
        bool isParentOfAny(const string &branch,const GitLogMeshNote & note);
        void dumpLogMesh(void);
    };
}

#endif // BOW_SOSIALS_VIEW_HPP
