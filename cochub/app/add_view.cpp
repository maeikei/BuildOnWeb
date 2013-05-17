#include "resource.hpp"
#include "reply_view.hpp"
#include "git_worker.hpp"
#include "add_view.hpp"
#include "last_position.hpp"
#include "utilities.hpp"
using namespace BOW;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
namespace pt = boost::property_tree;
#include <boost/algorithm/string.hpp>
namespace alg = boost::algorithm;

#include <fstream>
#include <iostream>



//#define DEBUG_APP_PARAM
#define DEBUG_POST_PARAM
#define DEBUG_POST_BUILD_TYPE

AddApp::AddApp(void)
{
}
AddApp::~ AddApp()
{
}
ReplyViewPtr AddApp::create(const std::string &uri,const std::string &remote)
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
        use_id += alg::replace_all_copy(remote,".","_");
    }
#ifdef DEBUG_APP_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":username=<" << username << ">" << std::endl;
	std::cout << typeid(this).name() << ":" << __func__ << ":setting=<" << setting << ">" << std::endl;
	std::cout << typeid(this).name() << ":" << __func__ << ":project=<" << project << ">" << std::endl;
#endif
    return ReplyViewPtr(new AddView(uri,username,use_id,project));
}

//#define DEBUG_PARAM
//#define DEBUG_CONTENT


AddView::AddView(const string &uri,const string &username,const std::string &user_uid,const std::string &project)
:ReplyView()
,uri_(uri)
,user_(username)
,user_uid_(user_uid)
,project_(project)
,workspace_(".temp/new_project/" + user_uid_)
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
AddView::~ AddView()
{
}
    
bool AddView::readBody(const string &doc_root,string &contents)
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

std::map<std::string,std::string> AddView::bodyVars(void)
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

void AddView::create_output(std::map<std::string,std::string> &replace)
{
    // replace output
    string output;
    read_output(output);
    replace.insert(pair<string,string>("$BOW_TMPL_OUTPUT$",output));
}
void AddView::read_output(string &output)
{
}
    

static const string strConstLogin =
    "<a href=\"/login.php\" data-method=\"post\" id=\"login\">login</a>";
static const string strConstLogout =
    "<a href=\"/logout.php\" data-method=\"post\" id=\"logout\">logout</a>";
    
    

void AddView::create_loginout(std::map<std::string,std::string> &replace)
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

    
void AddView::post(const std::string &data)
{
#ifdef DEBUG_POST_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":data=<" << data << ">" << std::endl;
#endif
    std::stringstream ss;
    ss << data;
    pt::ptree ptree;
    pt::read_json(ss,ptree);
/// try to checkout git
    auto repo = ptree.get_child("git").data();
#ifdef DEBUG_POST_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":repo=<" << repo << ">" << std::endl;
#endif
    if(false == repo.empty())
    {
        string git_cmd("cd " + workspace_ + " && git clone " + repo + " 2>&1");
        string result = system_result(git_cmd);
#ifdef DEBUG_POST_PARAM
        std::cout << typeid(this).name() << ":" << __func__ << ":result=<" << result << ">" << std::endl;
#endif
        output_ += result;
        alg::replace_all(output_,"\n","\\n");
        alg::replace_all(output_,"\r","\\r");
    }
    {        
        string find_git_root("find  " + workspace_ + " -type d -name *.git");
#ifdef DEBUG_POST_PARAM
        std::cout << typeid(this).name() << ":" << __func__ << ":find_git_root=<" << find_git_root << ">" << std::endl;
#endif
        gitroot_ = system_result(find_git_root);
        alg::replace_all(gitroot_,"/.git","");
        alg::replace_all(gitroot_,"\n","");
        alg::replace_all(gitroot_,"\r","");
    }
#ifdef DEBUG_POST_PARAM
    std::cout << typeid(this).name() << ":" << __func__ << ":gitroot_=<" << gitroot_ << ">" << std::endl;
#endif
    if(false == gitroot_.empty())
    {
        fs::path gitrootpath(gitroot_);
        projectname_= gitrootpath.stem().string();

        string get_branches = system_result("cd " + gitroot_ + "&& git branch -r | grep -v \"origin/HEAD\" " );
        alg::replace_all(get_branches,"origin/","");
        alg::replace_all(get_branches," ","");
        alg::replace_all(get_branches,"\n",",");
        alg::replace_all(get_branches,"\r",",");
        boost::split(gitbranches_,get_branches,boost::is_any_of(","));
        gitbranches_.remove("");
        

    
        string cmakefile = system_result("cd " + gitroot_ + "&& find . -type f -name CMakeLists.txt");
#ifdef DEBUG_POST_BUILD_TYPE
        std::cout << typeid(this).name() << ":" << __func__ << ":cmakefile=<" << cmakefile << ">" << std::endl;
#endif
        if(false == cmakefile.empty())
        {
            buildType_.push_back("CMake");
        }
        string makefile = system_result("cd " + gitroot_ + "&& find . -type f -name Makefile");
#ifdef DEBUG_POST_BUILD_TYPE
        std::cout << typeid(this).name() << ":" << __func__ << ":makefile=<" << makefile << ">" << std::endl;
        if(false == makefile.empty())
        {
            buildType_.push_back("Makefile");
        }
#endif
        string gnumakefile = system_result("cd " + gitroot_ + "&& find . -type f -name GNUMakefile");
#ifdef DEBUG_POST_BUILD_TYPE
        std::cout << typeid(this).name() << ":" << __func__ << ":gnumakefile=<" << gnumakefile << ">" << std::endl;
#endif
        if(false == gnumakefile.empty())
        {
            buildType_.push_back("Makefile");
        }
        string configure = system_result("cd " + gitroot_ + "&& find . -type f -name configure");
#ifdef DEBUG_POST_BUILD_TYPE
        std::cout << typeid(this).name() << ":" << __func__ << ":configure=<" << configure << ">" << std::endl;
#endif
        if(false == configure.empty())
        {
            buildType_.push_back("configure");
        }
        buildType_.unique();
    }
}

bool AddView::readPostReply(std::string &contents)
{
    
    contents = "{";

    if(false == gitbranches_.empty())
    {
        std::string get_branches("\"branches\":[\"");
        get_branches += boost::algorithm::join(gitbranches_, "\",\"");
        get_branches += "\"]";
        contents += get_branches;
    }
    
    /// copy&paste this one
    if(false == projectname_.empty())
    {
        contents += ",\"name\":\"" + projectname_ + "\"";
    }
    /// copy&paste this one
    if(false == output_.empty())
    {
        contents += ",\"output\":\"" + output_ + "\"";
    }
    
    /// last one
    if(false == buildType_.empty())
    {
        string buildType;
        buildType += ",\"buildtype\":[\"";
        buildType += boost::algorithm::join(buildType_, "\",\"");
        buildType += "\"]";
        contents += buildType;
    }
    contents += "}";
    read_output(contents);
    return true;
}

