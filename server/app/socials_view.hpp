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
        class resource;
    }
}


namespace BOW {
    class SosialApp: public http::server_threadpool::resource
    {
    public:
        SosialApp(void);
        virtual ~ SosialApp();
        virtual void create(const std::string &uri);
        //http method
        virtual void get(void);
        virtual void post(void);
        virtual void put(void);
        virtual void remove(void);
    };
    
    string system_result(const string cmd);
    struct number
    {
        int i_;
        number(int i):i_(i){}
        operator string() const {
            stringstream ss;
            ss << i_;
            return ss.str();
        }
    };

    class SourceView;
    struct GitLogMeshNote
    {
        string hash_;
        string hash_p_;
        string date_;
        int x_,y_;
        
        GitLogMeshNote():hash_(""),hash_p_(""),date_(""),x_(-1),y_(-1){}
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
        string left_;
        string right_;
        string diff_;
        const list<string> env_show_commands_;
        list<string> branches_;
        map<string,bool> log_check_;
        map<string,GitLogMeshList> git_log_mesh_;
        map<string,GitLogMeshList> git_log_mesh_pretty_;
        map<int32_t,int32_t> mesh_positions_;
    private:
        void createAllLogMesh(void);
        void createBranchMesh(const string &branch);
        void reduceLogMesh(void);
        bool isParentOfAny(const string &branch,const GitLogMeshNote & note);
        void dumpLogMesh(void);
        void createAllBranchSVG(string &svg);
        void createMasterSVG(string &svg);
        void createBranchSVG(string &svg);
        void getParentPosition(const GitLogMeshNote & note,int&x ,int &y);
    };
}

#endif // BOW_SOSIALS_VIEW_HPP
