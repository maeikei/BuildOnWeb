#ifndef BOW_GIT_WORKER_HPP
#define BOW_GIT_WORKER_HPP
#include <string>
#include <list>
using namespace std;



namespace BOW {
    class GitWorker
    {
    public:
        GitWorker(const string &wc,const string & branch,const string &repo);
        virtual ~ GitWorker();
    private:
        void createWorkSpace(void);
        void updateWorkSpace(void);
    private:
        const string workspace_;
        const string branch_;
        const string repo_path_;
        const string git_repositories_;
        const list<string> remote_branch_commands_;
    };
}
#endif // BOW_GIT_WORKER_HPP
