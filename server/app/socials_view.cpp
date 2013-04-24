#include "reply_view.hpp"
#include "source_view.hpp"
#include "socials_view.hpp"
#include "last_position.hpp"
using namespace BOW;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <sstream>
#include <iostream>


//#define DEBUG_PARAM
//#define DEBUG_LOGMESH
#define DEBUG_LOGMESH_DUMP
//#define DEBUG_CONTENT


namespace BOW {
    string system_result(const string cmd)
    {
#ifdef DEBUG_PARAM
        std::cout << __func__ <<":cmd=<" <<  cmd << ">" << endl;
#endif
        FILE *pipe = popen(cmd.c_str(), "r");
        std::string result;
        char buf[256] = {0};
        while(!feof(pipe)) {
            if(fgets(buf, sizeof(buf), pipe) != NULL)
            {
                result += buf;
            }
        }
        pclose(pipe);
        return result;
    }
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
};



SosialView::SosialView(const SourceView &src)
:SourceView(src)
,wc_temp_cmd_output_(".bow_output/branch.list")
,env_show_commands_
{
 //   "cd " + workspace_ + "/" + repo_ + "&& git branch -r > " + wc_temp_cmd_output_,
}
{
    // read all remote branches.
    {
        string cmd("cd " + workspace_ + "&& git branch -r ");
        string branchs_txt = system_result(cmd.c_str());
#ifdef DEBUG_PARAM
        std::cout << __func__ <<":branchs_txt=<" <<  branchs_txt << ">" << endl;
#endif
        list<string> branches_temp;
        boost::split(branches_temp, branchs_txt, boost::is_any_of("\n\r"));
        for(auto it = branches_temp.begin();it != branches_temp.end();it++)
        {
            if( string::npos != it->find("origin/HEAD"))
            {
                continue;
            }
            if( string::npos != it->find("origin/master"))
            {
                continue;
            }
            boost::algorithm::replace_all(*it," ","");
            if(false == it->empty() )
            {
                branches_.push_back(*it);
            }
        }
    }
}
SosialView::~SosialView()
{
}

bool SosialView::getContent(const string &doc_root,string &contents)
{
    createAllLogMesh();
    // Open the template file to add to contents.
    {
        std::string full_path = doc_root + "/BuildOnWebViewSosials.html";
#ifdef DEBUG_CONTENT
        std::cout << "full_path=" << full_path << std::endl;
#endif
        std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
        if (!is)
        {
            return false;
        }
        // Fill out the reply to be sent to the client.
        char buf[512];
        while (is.read(buf, sizeof(buf)).gcount() > 0) {
            contents.append(buf, is.gcount());
        }
        is.close();
    }
    // replace users
    {
        boost::algorithm::replace_all(contents,"$BOW_TMPL_USER$",user_);
    }
    // replace svg
    {
        string svg;
        createBranchSVG(svg);
        boost::algorithm::replace_all(contents,"$BOW_TMPL_HISTORY_SVG$",svg);
    }
    this->replace_source_path(contents);
    this->replace_loginout(contents);
    return true;
}

static const string strConstStartNode("<circle cx=\"$cx$\" cy=\"$cy$\" r=\"5\" stroke=\"black\" stroke-width=\"2\"/>\n\
                                      <text x=\"$x$\" y=\"$y$\" font-size=\"10\" fill=\"blue\" > $txt$ </text>\n\
                                      ");
static const string strConstNormalNode("<circle cx=\"$cx$\" cy=\"$cy$\" r=\"10\" stroke=\"black\" stroke-width=\"2\"/>\n");

static const int iConstSeperateOfY = 100;
static const int iConstStringOffsetX = 10;
static const int iConstStringOffsetY = 4;
void SosialView::createBranchSVG(string &svg)
{
    auto it = git_log_mesh_.find("origin/master");
    if(it != git_log_mesh_.end())
    {
        int startX = 20,startY =20;
        int i = 1;
        for(auto  it2 =it->second.begin();it2 != it->second.end();it2++)
        {
            if(it2 == it->second.begin())
            {
                string nodesvn(strConstStartNode);
                string cx = number(startX);
                boost::algorithm::replace_all(nodesvn,"$cx$",cx);
                string cy = number(startY);
                boost::algorithm::replace_all(nodesvn,"$cy$",cy);
                string x = number(startX + iConstStringOffsetX);
                boost::algorithm::replace_all(nodesvn,"$x$",x);
                string y = number(startY + iConstStringOffsetY);
                boost::algorithm::replace_all(nodesvn,"$y$",y);
                string txt("master");
                boost::algorithm::replace_all(nodesvn,"$txt$",txt);
                svg += nodesvn;
            }
            else
            {
                string nodesvn(strConstStartNode);
                string cx = number(startX);
                boost::algorithm::replace_all(nodesvn,"$cx$",cx);
                string cy = number(startY + i* iConstSeperateOfY);
                boost::algorithm::replace_all(nodesvn,"$cy$",cy);
                svg += nodesvn;                
            }
            i++;
        }
    }
    else
    {
        return;
    }
}

void SosialView::createBranchMesh(const string &branch)
{
    list<GitLogMeshNote> branch_list;
    git_log_mesh_.insert(pair<string,GitLogMeshList>(branch,branch_list));
    string cmd("cd " + workspace_ + "&& git log --oneline --reverse --pretty=format:\"%h,%p,%cd\" " + branch);
    string branch_log = system_result(cmd.c_str());
#ifdef DEBUG_PARAM
    std::cout << __func__ <<":branch_log=<" <<  branch_log << ">" << endl;
#endif
    list<string> log_in_lines;
    boost::split(log_in_lines, branch_log, boost::is_any_of("\n\r"));
    for_each(log_in_lines.begin(), log_in_lines.end(),
             [this,branch](const string &line)
             {
#ifdef DEBUG_LOGMESH
                 cout << line << endl;
#endif
                 list<string> log_details;
                 boost::split(log_details, line, boost::is_any_of(","));
                 string hash;
                 if(false ==log_details.empty())
                 {
                     hash= log_details.front();
                     log_details.pop_front();
                 }
                 auto it = this->log_check_.find(hash);
                 string hash_parent;
                 if(false ==log_details.empty())
                 {
                     hash_parent= log_details.front();
                     log_details.pop_front();
                 }
                 string date;
                 if(false ==log_details.empty())
                 {
                     date= log_details.front();
                     log_details.pop_front();
                 }
#ifdef DEBUG_LOGMESH
                 std::cout << __func__ <<":hash=<" <<  hash << ">" << endl;
                 std::cout << __func__ <<":hash_parent=<" <<  hash_parent << ">" << endl;
                 std::cout << __func__ <<":date=<" <<  date << ">" << endl;
                 std::cout << __func__ <<":branch=<" <<  branch << ">" << endl;
#endif
                 if(it == log_check_.end())
                 {
                     GitLogMeshNote note;
                     note.hash_ = hash;
                     note.hash_p_ = hash_parent;
                     note.date_ = date;
                     git_log_mesh_[branch].push_back(note);
                     log_check_.insert(pair<string, bool>(hash,true));
                 }
             }
    );
    if( git_log_mesh_[branch].empty())
    {
        git_log_mesh_.erase(branch);
    }
}

void SosialView::createAllLogMesh(void)
{
    createBranchMesh("origin/master");
    // read all branch logs.
    for(auto it = branches_.begin();it != branches_.end();it++)
    {
        createBranchMesh(*it);
    }
//    dumpLogMesh();
    reduceLogMesh();
    dumpLogMesh();
}
void SosialView::reduceLogMesh(void)
{
    for(auto it = git_log_mesh_.begin();it != git_log_mesh_.end();it++)
    {
        for(auto it2 = it->second.begin();it2 != it->second.end();it2++)
        {
            if( it2 == it->second.begin())
            {
                continue;
            }
            if(false == isParentOfAny( it->first,*it2))
            {
                it->second.erase(it2);
            }
        }
    }
}
bool SosialView::isParentOfAny(const string &branch,const GitLogMeshNote & note)
{
#ifdef DEBUG_LOGMESH
    std::cout << __func__ <<":note.hash_=<" << note.hash_ << ">" << endl;
#endif
    for(auto it = git_log_mesh_.begin();it != git_log_mesh_.end();it++)
    {
        if(branch == it->first)
        {
#ifdef DEBUG_LOGMESH
            std::cout << __func__ <<":it->first=<" <<  it->first << ">" << endl;
#endif
            continue;
        }
        for(auto it2 = it->second.begin();it2 != it->second.end();it2++)
        {
            if(it2->hash_p_ == note.hash_)
            {
#ifdef DEBUG_LOGMESH
                std::cout << __func__ <<":it2->hash_p_=<" <<  it2->hash_p_ << ">" << endl;
                std::cout << __func__ <<":nnote.hash_=<" << note.hash_ << ">" << endl;
#endif
                return true;
            }
        }
    }
    return false;
}
void SosialView::dumpLogMesh(void)
{
#ifdef DEBUG_LOGMESH_DUMP
    for(auto it = git_log_mesh_.begin();it != git_log_mesh_.end();it++)
    {
        std::cout << __func__ <<":--------------branch=<" <<  it->first << ">----------------" << endl;
        for(auto it2 = it->second.begin();it2 != it->second.end();it2++)
        {
            std::cout << __func__ <<":hash_=<" <<  it2->hash_ << ">" << endl;
            std::cout << __func__ <<":hash_p_=<" <<  it2->hash_p_ << ">" << endl;
            std::cout << __func__ <<":date_=<" <<  it2->date_ << ">" << endl;
        }
    }
#endif
}


bool GitLogMeshNote::operator() (const GitLogMeshNote &left) const
{
    return left.hash_ == hash_;
}
