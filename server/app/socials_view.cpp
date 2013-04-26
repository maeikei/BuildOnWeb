#include "resource.hpp"
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



SosialApp::SosialApp(void)
{
}
SosialApp::~ SosialApp()
{
}
void SosialApp::create(const std::string &uri,const std::string &user_uid)
{
}
void SosialApp::get(const std::string &doc_root, http::server_threadpool::reply& rep)
{
}
void SosialApp::post(const std::string &doc_root, http::server_threadpool::reply& rep)
{
}
void SosialApp::put(const std::string &doc_root, http::server_threadpool::reply& rep)
{
}
void SosialApp::remove(const std::string &doc_root, http::server_threadpool::reply& rep)
{
}

//#define DEBUG_PARAM
//#define DEBUG_BRANCH_LOG
//#define DEBUG_LOGMESH
//#define DEBUG_LOGMESH_DUMP
//#define DEBUG_LOGMESH_POSITION
//#define DEBUG_CONTENT

void text2html(string &txt);


string BOW::system_result(const string cmd)
{
//    std::cout << __func__ <<":cmd=<" <<  cmd << ">" << endl;
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


SosialView::SosialView(const SourceView &src)
:SourceView(src)
,wc_temp_cmd_output_(".bow_output/branch.list")
,left_("master")
,right_(user_uid_)
,env_show_commands_
{
 //   "cd " + workspace_ + "/" + repo_ + "&& git branch -r > " + wc_temp_cmd_output_,
}
{
    // read all remote branches.
    {
        string cmd("cd " + workspace_ + "&& git branch -r ");
        string branchs_txt = system_result(cmd.c_str());
#ifdef DEBUG_BRANCH_LOG
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
    {
        diff_ = system_result("cd " + workspace_ + "&& git diff origin/master ");
#ifdef DEBUG_PARAM
        std::cout << __func__ <<":diff_=<" <<  diff_ << ">" << endl;
#endif
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
        createAllBranchSVG(svg);
        boost::algorithm::replace_all(contents,"$BOW_TMPL_HISTORY_SVG$",svg);
    }
    // replace diff
    {
        boost::algorithm::replace_all(contents,"$BOW_TMPL_SOSIAL_LEFT$",left_);
        boost::algorithm::replace_all(contents,"$BOW_TMPL_SOSIAL_RIGHT$",right_);
        string diff_html(diff_);
        text2html(diff_html);
        boost::algorithm::replace_all(contents,"$BOW_TMPL_GIT_DIFF$",diff_html);
    }
    this->replace_source_path(contents);
    this->replace_loginout(contents);
    return true;
}


static const string strConstMasterStartNode("<circle cx=\"$cx$\" cy=\"$cy$\" r=\"5\" stroke=\"black\" stroke-width=\"2\"/>\n\
                                      <text x=\"$x$\" y=\"$y$\" font-size=\"10\" fill=\"blue\" > $txt$ </text>\n\
                                      ");
static const string strConstNormalNode("<circle cx=\"$cx$\" cy=\"$cy$\" r=\"5\" stroke=\"black\" stroke-width=\"2\"/>\n");
static const string strConstLine("<line x1=\"$x1$\" y1=\"$y1$\" x2=\"$x2$\" y2=\"$y2$\" style=\"stroke:rgb(0,0,0);stroke-width:2\"/>");
static const string strConstBranchStartNode("<a xlink:href=\"$href$\">\n\
                                            <circle cx=\"$cx$\" cy=\"$cy$\" r=\"5\" stroke=\"black\" stroke-width=\"2\"/>\n\
                                              <text x=\"$x$\" y=\"$y$\" font-size=\"10\" fill=\"blue\">$txt$</text>\n\
                                            </a>\n\
                                              ");

static const int iConstSeperateOfY = 50;
static const int iConstSeperateOfX = 80;
static const int iConstStringOffsetX = 10;
static const int iConstStringOffsetY = 4;

void SosialView::createMasterSVG(string &svg)
{
    auto it = git_log_mesh_.find("origin/master");
    if(it != git_log_mesh_.end())
    {
        for(auto  it2 =it->second.begin(),it2_pre = it->second.end();it2 != it->second.end();it2++)
        {
            if(it2_pre == it->second.end())
            {
                int startX = 20,startY =20;
                it2->x_ = startX;
                it2->y_ = startY;
                string nodesvn(strConstMasterStartNode);
                string cx = number(it2->x_);
                boost::algorithm::replace_all(nodesvn,"$cx$",cx);
                string cy = number(it2->y_);
                boost::algorithm::replace_all(nodesvn,"$cy$",cy);
                string x = number(it2->x_ + iConstStringOffsetX);
                boost::algorithm::replace_all(nodesvn,"$x$",x);
                string y = number(it2->y_ + iConstStringOffsetY);
                boost::algorithm::replace_all(nodesvn,"$y$",y);
                string txt("master");
                boost::algorithm::replace_all(nodesvn,"$txt$",txt);
                svg += nodesvn;
            }
            else
            {
                it2->x_ = it2_pre->x_;
                it2->y_ = it2_pre->y_ + iConstSeperateOfY;

                string nodesvn(strConstNormalNode);
                string cx = number(it2->x_);
                boost::algorithm::replace_all(nodesvn,"$cx$",cx);
                string cy = number(it2->y_);
                boost::algorithm::replace_all(nodesvn,"$cy$",cy);
                svg += nodesvn;
                
                string line(strConstLine);
                string x1 = number(it2_pre->x_);
                boost::algorithm::replace_all(line,"$x1$",x1);
                string y1 = number(it2_pre->y_);
                boost::algorithm::replace_all(line,"$y1$",y1);
                string x2 = number(it2->x_);
                boost::algorithm::replace_all(line,"$x2$",x2);
                string y2 = number(it2->y_);
                boost::algorithm::replace_all(line,"$y2$",y2);
                svg += line;
            }
            it2_pre = it2;
        }
    }
}

void SosialView::createBranchSVG(string &svg)
{
    int colum = 1;
    for(auto it = git_log_mesh_.begin();it != git_log_mesh_.end();it++)
    {
        if( "origin/master" == it->first)
        {
            continue;
        }
        for(auto  it2 =it->second.begin(),it2_pre = it->second.end();it2 != it->second.end();it2++)
        {
            if(it2_pre == it->second.end())
            {
                int startX = -1,startY = -1;
                this->getParentPosition(*it2,startX,startY);
#ifdef DEBUG_LOGMESH_POSITION
                std::cout << __func__ <<":startX=<" <<  startX << ">" << endl;
                std::cout << __func__ <<":startY=<" <<  startY << ">" << endl;
#endif
                int32_t key = (((int32_t)startX) << 16) + (int32_t)startY;
#ifdef DEBUG_LOGMESH_POSITION
                std::cout << __func__ <<":startX << 16=<" <<  (((int32_t)startX) << 16) << ">" << endl;
                std::cout << __func__ <<":key=<" <<  key << ">" << endl;
#endif
                auto it_pos = mesh_positions_.find(key);
                if(it_pos != mesh_positions_.end())
                {
#ifdef DEBUG_LOGMESH_POSITION
                    std::cout << __func__ <<":it_pos->second=<" <<  it_pos->second << ">" << endl;
                    std::cout << __func__ <<":(it_pos->second >> 16)=<" <<  (it_pos->second >> 16) << ">" << endl;
                    std::cout << __func__ <<":it_pos->second & 0xffff=<" <<  (it_pos->second & 0xffff) << ">" << endl;
#endif
                    it2->x_ = (it_pos->second >> 16);
                    it2->y_ = (it_pos->second & 0xffff) + iConstSeperateOfY/2;
                    int32_t value = (int32_t)(it2->x_ << 16) + (int32_t)it2->y_;
#ifdef DEBUG_LOGMESH_POSITION
                    std::cout << __func__ <<":value=<" <<  value << ">" << endl;
#endif
                    mesh_positions_.erase(it_pos);
                    mesh_positions_.insert(pair<int,int>(key,value));
                }
                else
                {
                    it2->x_ = startX + iConstSeperateOfX;
                    it2->y_ = startY + iConstSeperateOfY/2;
                    int32_t value = (int32_t)(it2->x_ << 16) + (int32_t)it2->y_;
#ifdef DEBUG_LOGMESH_POSITION
                    std::cout << __func__ <<":value=<" <<  value << ">" << endl;
#endif
                    mesh_positions_.insert(pair<int,int>(key,value));
                }
                string nodesvn(strConstBranchStartNode);
                string cx = number(it2->x_);
                boost::algorithm::replace_all(nodesvn,"$cx$",cx);
                string cy = number(it2->y_);
                boost::algorithm::replace_all(nodesvn,"$cy$",cy);
                string x = number(it2->x_ + iConstStringOffsetX);
                boost::algorithm::replace_all(nodesvn,"$x$",x);
                string y = number(it2->y_ + iConstStringOffsetY);
                boost::algorithm::replace_all(nodesvn,"$y$",y);
                string txt(it->first);
                boost::algorithm::replace_all(txt,"origin/","");
                boost::algorithm::replace_all(nodesvn,"$txt$",txt);
                string href(txt);
                boost::algorithm::replace_all(nodesvn,"$href$",href);
                svg += nodesvn;

                string line(strConstLine);
                string x1 = number(startX);
                boost::algorithm::replace_all(line,"$x1$",x1);
                string y1 = number(startY);
                boost::algorithm::replace_all(line,"$y1$",y1);
                string x2 = number(it2->x_);
                boost::algorithm::replace_all(line,"$x2$",x2);
                string y2 = number(it2->y_);
                boost::algorithm::replace_all(line,"$y2$",y2);
                svg += line;

            }
            else
            {
                it2->x_ = it2_pre->x_;
                it2->y_ = it2_pre->y_ + iConstSeperateOfY;
                
                string nodesvn(strConstNormalNode);
                string cx = number(it2->x_);
                boost::algorithm::replace_all(nodesvn,"$cx$",cx);
                string cy = number(it2->y_);
                boost::algorithm::replace_all(nodesvn,"$cy$",cy);
                svg += nodesvn;
                
                string line(strConstLine);
                string x1 = number(it2_pre->x_);
                boost::algorithm::replace_all(line,"$x1$",x1);
                string y1 = number(it2_pre->y_);
                boost::algorithm::replace_all(line,"$y1$",y1);
                string x2 = number(it2->x_);
                boost::algorithm::replace_all(line,"$x2$",x2);
                string y2 = number(it2->y_);
                boost::algorithm::replace_all(line,"$y2$",y2);
                svg += line;
            }
            it2_pre = it2;
        }
        colum++;
    }
}

void SosialView::createAllBranchSVG(string &svg)
{
    createMasterSVG(svg);
    createBranchSVG(svg);
}

        
void SosialView::getParentPosition(const GitLogMeshNote & note,int&x ,int &y)
{
    for(auto it = git_log_mesh_.begin();it != git_log_mesh_.end();it++)
    {
        for(auto  it2 =it->second.begin();it2 != it->second.end();it2++)
        {
            if( it2->hash_ == note.hash_p_)
            {
                if( -1 != it2->x_ && -1 != it2->y_)
                {
                    x = it2->x_;
                    y = it2->y_;
                    return ;
                }
                else
                {
#if 0
                    getParentPosition(*it2,x,y);
                    it2->x_ = x;
                    it2->y_ = y;
#endif
                    return;
                }
            }
        }
    }
}

void SosialView::createBranchMesh(const string &branch)
{
    list<GitLogMeshNote> branch_list;
    git_log_mesh_.insert(pair<string,GitLogMeshList>(branch,branch_list));
    string cmd("cd " + workspace_ + "&& git log --oneline --reverse --pretty=format:\"%h,%p,%cd\" " + branch);
    string branch_log = system_result(cmd.c_str());
#ifdef DEBUG_LOGMESH
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
#if 0
        for(auto it2 = it->second.begin();it2 != it->second.end();it2++)
        {
            if( it2 == it->second.begin())
            {
                continue;
            }
            if(false == isParentOfAny( it->first,*it2))
            {
                it2 = it->second.erase(it2);
            }
        }
#endif
        auto it2 = it->second.begin();
        it2++;
        while(it2 != it->second.end())
        {
            if(false == isParentOfAny( it->first,*it2))
            {
                it2 = it->second.erase(it2);
            }
            else
            {
                it2++;
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
