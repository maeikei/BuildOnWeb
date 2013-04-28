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
        struct reply;
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
        virtual void create(const std::string &uri,const std::string &user_uid);
        //http method
        virtual void get(const std::string &doc_root, http::server_threadpool::reply& rep);
    };
    
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
    class SosialView: public http::server_threadpool::ReplyView
    {
    public:
        SosialView(const string &uri,const string &username,const string &user_uid,
                    const string &category,const string &repo);
        virtual ~SosialView();
        virtual bool readBody(const std::string &doc_root,std::string &contents);
        virtual std::map<std::string,std::string> bodyVars(void);
    private:
        const string uri_;
        const string user_;
        const string user_uid_;
        const string category_;
        const string repo_;
        const string workspace_;
        
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

        void create_loginout(std::map<std::string,std::string> &replace);
    };
}

#endif // BOW_SOSIALS_VIEW_HPP
