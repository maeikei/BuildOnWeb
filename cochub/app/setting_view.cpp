#include "resource.hpp"
#include "reply_view.hpp"
#include "git_worker.hpp"
#include "setting_view.hpp"
#include "last_position.hpp"
#include "utilities.hpp"
using namespace BOW;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>



#define DEBUG_APP_PARAM
#define DEBUG_POST_PARAM


SettingApp::SettingApp(void)
{
}
SettingApp::~ SettingApp()
{
}
ReplyViewPtr SettingApp::create(const std::string &uri,const std::string &remote)
{
#ifdef DEBUG_APP_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":uri=<" << uri << ">" << std::endl;
#endif
    std::string username;
    std::string setting;
    std::string project;
    parseUri(uri,username,setting,project);
    string use_id(username);
    if("guest"==username)
    {
        use_id += "_from_";
        use_id += boost::algorithm::replace_all_copy(remote,".","_");
    }
#ifdef DEBUG_APP_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":username=<" << username << ">" << std::endl;
	std::cout << typeid(this).name() << ":" << __func__ << ":setting=<" << setting << ">" << std::endl;
	std::cout << typeid(this).name() << ":" << __func__ << ":project=<" << project << ">" << std::endl;
#endif
    return ReplyViewPtr(new SettingView(uri,username,use_id,project));
}

//#define DEBUG_PARAM
//#define DEBUG_CONTENT


SettingView::SettingView(const string &uri,const string &username,const std::string &user_uid,const std::string &project)
:ReplyView()
,uri_(uri)
,user_(username)
,user_uid_(user_uid)
,project_(project)
,workspace_(".temp/" + user_uid_)
,env_build_commands_
 {
     "mkdir -p " + workspace_,
 }
{
#ifdef DEBUG_PARAM
    std::cout << __func__ <<":username=" <<  username << endl;
    std::cout << __func__ <<":category=" <<  category << endl;
    std::cout << __func__ <<":repo=" <<  repo << endl;
#endif
    for(const auto &cmd :env_build_commands_)
    {
        BOW::system_result(cmd);
    }
}
SettingView::~ SettingView()
{
}
    
bool SettingView::readBody(const string &doc_root,string &contents)
{
    // Open the template file to add to contents.
    std::string full_path = doc_root + "/CocHubViewSetting.html";
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
    return true;
}

std::map<std::string,std::string> SettingView::bodyVars(void)
{
    std::map<std::string,std::string> ret;

    // replace users
    ret.insert(pair<string,string>("$BOW_TMPL_USER$",user_));    
    // replace javascript $BOW_TMPL_PROJECT_NAME$
    {
        if( "+" == project_)
        {
            ret.insert(pair<string,string>("$BOW_TMPL_PROJECT_NAME$",""));
        }
        else
        {
            ret.insert(pair<string,string>("$BOW_TMPL_PROJECT_NAME$",project_));            
        }
    }

    
    // create output
    this->create_output(ret);
    // replace login/out.
    this->create_loginout(ret);
    return ret;
}

void SettingView::create_output(std::map<std::string,std::string> &replace)
{
    // replace output
    string output;
    read_output(output);
    replace.insert(pair<string,string>("$BOW_TMPL_OUTPUT$",output));
}
void SettingView::read_output(string &output)
{
#if 0
    // program run result
    {
        std::ifstream isf(output_.c_str(), std::ios::in | std::ios::binary);
        if (isf)
        {
            // Fill out the reply to be sent to the client.
            char buf[512];
            while (isf.read(buf, sizeof(buf)).gcount() > 0) {
                output.append(buf, isf.gcount());
            }
            isf.close();
        }
    }
    output += "---------build log start---------\n";
    // build result
    {
        std::ifstream isf(build_output_.c_str(), std::ios::in | std::ios::binary);
        if (isf)
        {
            // Fill out the reply to be sent to the client.
            char buf[512];
            while (isf.read(buf, sizeof(buf)).gcount() > 0) {
                output.append(buf, isf.gcount());
            }
            isf.close();
        }
    }
    output += "---------build log end----------\n";
#endif
}
    

static string strConstLogin =
    "<a href=\"/login.php\" data-method=\"post\" id=\"login\">login</a>";
static string strConstLogout =
    "<a href=\"/logout.php\" data-method=\"post\" id=\"logout\">logout</a>";
    
    

void SettingView::create_loginout(std::map<std::string,std::string> &replace)
{
    if( "guest" == user_ || user_.empty())
    {
        replace.insert(pair<string,string>("$BOW_TMPL_USER_LOGINOUT$",strConstLogin));
    }
    else
    {
        replace.insert(pair<string,string>("$BOW_TMPL_USER_LOGINOUT$",strConstLogout));
    }
}

    
void SettingView::post(const std::string &data)
{
#ifdef DEBUG_POST_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":data=<" << data << ">" << std::endl;
#endif
    string git_cmd("cd " + workspace_ + " && git clone " + data);
    string result = system_result(git_cmd);
#ifdef DEBUG_POST_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":result=<" << result << ">" << std::endl;
#endif    
}
bool SettingView::readPostReply(std::string &contents)
{
    string find_git_root("find  " + workspace_ + " -type d -name *.git");
    string gitroot = system_result(find_git_root);
    boost::algorithm::replace_all(gitroot,"/.git","");
    boost::algorithm::replace_all(gitroot,"\n","");
    boost::algorithm::replace_all(gitroot,"\r","");
#ifdef DEBUG_POST_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":gitroot=<" << gitroot << ">" << std::endl;
#endif
    string get_branches = system_result("cd " + gitroot + "&& git branch -r | grep -v \"origin/HEAD\" " );
    boost::algorithm::replace_all(get_branches,"origin/","");
    boost::algorithm::replace_all(get_branches," ","");
    boost::algorithm::replace_all(get_branches,"\n",",");
    boost::algorithm::replace_all(get_branches,"\r",",");
#ifdef DEBUG_POST_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":get_branches=<" << get_branches << ">" << std::endl;
#endif

    read_output(contents);
    return true;
}

