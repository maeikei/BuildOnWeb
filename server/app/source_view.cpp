#include "resource.hpp"
#include "reply_view.hpp"
#include "git_worker.hpp"
#include "source_view.hpp"
#include "last_position.hpp"
#include "utilities.hpp"
using namespace BOW;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>



//#define DEBUG_APP_PARAM


SourceApp::SourceApp(void)
{
}
SourceApp::~ SourceApp()
{
}
void SourceApp::create(const std::string &uri,const std::string &remote)
{
#ifdef DEBUG_APP_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":uri=<" << uri << ">" << std::endl;
#endif
    std::string username;
    std::string category;
    std::string repo;
    std::string type;
    list<string> path;
    parseUri(uri,username,category,repo,type,path);
    string use_id(username);
    if("guest"==username)
    {
        use_id += "_from_";
        use_id += boost::algorithm::replace_all_copy(remote,".","_");
    }
#ifdef DEBUG_APP_PARAM
	std::cout << typeid(this).name() << ":" << __func__ << ":username=<" << username << ">" << std::endl;
	std::cout << typeid(this).name() << ":" << __func__ << ":category=<" << category << ">" << std::endl;
	std::cout << typeid(this).name() << ":" << __func__ << ":repo=<" << repo << ">" << std::endl;
	std::cout << typeid(this).name() << ":" << __func__ << ":type=<" << type << ">" << std::endl;
#endif
    reply_ = std::shared_ptr<http::server_threadpool::ReplyView>(new SourceView(uri,username,use_id,category,repo,path));
}
void SourceApp::get(const std::string &doc_root, http::server_threadpool::reply& rep)
{
    reply_->responseGet(doc_root,rep);
}
void SourceApp::post(const std::string &data, http::server_threadpool::reply& rep)
{
    reply_->post(data);
    reply_->responsePost(rep);
}

//#define DEBUG_PARAM
#define DEBUG_CONTENT

const string strConstManualFormat
(
 "<a href=\"/manual/$repo_$\">$repo_$</a>"
 );
const string strConstSocialsFormat
(
 "<a href=\"/users/$user_$/$category_$/$repo_$/?socials?\">Socials</a>"
 );




SourceView::SourceView(const string &uri,const string &username,const string &user_uid,
                       const string &category,const string &repo,const list<string> &path )
:ReplyView()
,uri_(uri)
,user_(username)
,user_uid_(user_uid)
,category_(category)
,repo_(repo)
,path_(path)
,workspace_(".temp/" + user_uid_ + "/" + category_ + "/" + repo_)
,output_(workspace_ + "/.bow_output/output.log")
,build_output_(workspace_ + "/.bow_output/build.log")
,git_(new GitWorker(workspace_,user_uid_,category_+ "/" + repo_))
,env_build_commands_
{
    "rm -f " + output_ + "  " + build_output_,
    "mkdir -p " + workspace_ + "/.bow_output",
    "make exe -C " + workspace_ + " 2>&1 | tee > " + build_output_,
}
,_env_commit_commands
{
    "cd " + workspace_ + " && git commit -q -am \"web auto modify by user " + "" + "\" ",
    "cd " + workspace_ + " && git push -q --force",
}
,extensions_
{
    {".h","c_cpp"},
    {".hpp","c_cpp"},
    {".c","c_cpp"},
    {".cxx","c_cpp"},
    {".cpp","c_cpp"},
    {".cp","c_cpp"},
    {".cc","c_cpp"},
    {".cx","c_cpp"},
    
    {".rb","ruby"},
    
    {".php","php"},
    
    {".mk","makefile"},
}
,names_
{
    {"makefile","makefile"},
    {"Makefile","makefile"},
    {"GnuMakefile","makefile"},
    {"Gnumakefile","makefile"},
    {"gnumakefile","makefile"},
}

{
#ifdef DEBUG_PARAM
    std::cout << __func__ <<":username=" <<  username << endl;
    std::cout << __func__ <<":category=" <<  category << endl;
    std::cout << __func__ <<":repo=" <<  repo << endl;
#endif
    for_each(env_build_commands_.begin(),  env_build_commands_.end(),
             [](const string &cmd)
             {
                 BOW::system_result(cmd);
             }
             );
}
SourceView::~SourceView()
{
}

    
    
bool SourceView::readBody(const string &doc_root,string &contents)
{
    std::string source_path(workspace_);
    for(auto it = path_.begin();it != path_.end();it++)
    {
        source_path.append( "/" + *it);
    }
#ifdef DEBUG_CONTENT
    std::cout << "source_path=" << source_path << std::endl;
#endif
    fs::path src_path(source_path);
    // Open the template file to add to contents.
    {
        std::string full_path = doc_root + "/BuildOnWebViewSource.html";
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
    
    string lastpath(category_ + "/" +repo_ + "/edit");
    for(auto it = path_.begin();it != path_.end();it++)
    {
        lastpath.append( "/" + *it);
    }
    LastPostion last(user_uid_);
    last.set(lastpath);
    return true;
}

std::map<std::string,std::string> SourceView::bodyVars(void)
{
    std::map<std::string,std::string> ret;

    // replace users
    ret.insert(pair<string,string>("$BOW_TMPL_USER$",user_));
    
    // replace manual link $BOW_TMPL_SOURCE_SOSAILS$
    {
        std::string socials(strConstSocialsFormat);
        boost::algorithm::replace_all(socials,"$user_$",user_);
        boost::algorithm::replace_all(socials,"$category_$",category_);
        boost::algorithm::replace_all(socials,"$repo_$",repo_);
        ret.insert(pair<string,string>("$BOW_TMPL_SOURCE_SOSAILS$",socials));
    }
    
    // replace manual link $BOW_TMPL_SOURCE_MANUAL$
    {
        std::string manual(strConstManualFormat);
        boost::algorithm::replace_all(manual,"$repo_$",repo_);
        ret.insert(pair<string,string>("$BOW_TMPL_SOURCE_MANUAL$",manual));
    }
    // replace javascript $BOW_TMPL_PATH$
    {
        ret.insert(pair<string,string>("$BOW_TMPL_PATH$",uri_));
    }
   
    // create source 
    this->create_source(ret);
    // create outpu
    this->create_output(ret);
    // replace navi path
    this->create_source_path(ret);
    // replace navi path
    this->create_source_path(ret);
    // replace login/out.
    this->create_loginout(ret);
    return ret;
}

void SourceView::create_source(std::map<std::string,std::string> &replace)
{
    std::string source_path(workspace_);
    for(auto it = path_.begin();it != path_.end();it++)
    {
        source_path.append( "/" + *it);
    }
    // replace source & file extension type
    string source;
    std::ifstream isf(source_path.c_str(), std::ios::in | std::ios::binary);
    if (isf)
    {
        // Fill out the reply to be sent to the client.
        char buf[512];
        while (isf.read(buf, sizeof(buf)).gcount() > 0) {
            source.append(buf, isf.gcount());
        }
    }
    text2html(source);
    replace.insert(pair<string,string>("$BOW_TMPL_SOURCE$",source));
    string mode("c_cpp");
    const fs::path fs_path(source_path);
    {
        auto it = extensions_.find( fs_path.extension().string());
        if(it != extensions_.end())
        {
            mode = it->second;
        }
    }
    {
        auto it = names_.find( fs_path.filename().string());
        if(it != names_.end())
        {
            mode = it->second;
        }
    }
    replace.insert(pair<string,string>("$BOW_TMPL_TYPE$",mode));
}
void SourceView::create_output(std::map<std::string,std::string> &replace)
{
    // replace output
    string output;
    read_output(output);
    replace.insert(pair<string,string>("$BOW_TMPL_OUTPUT$",output));
}
void SourceView::read_output(string &output)
{
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
}
    
void SourceView::create_source_path(std::map<std::string,std::string> &replace)
{
    string href("/users");
    std::string path;
// user_
    href += "/" + user_;
    path += "<a href=\"";
    path += href;
    path += "\">";
    path += user_;
    path += "</a>";
// category_
    path += "<span>/</span>";
    href += "/" + category_;
    path += "<a href=\"";
    path += href;
    path += "\">";
    path += category_;
    path += "</a>";
// repo_
    path += "<span>/</span>";
    href += "/" + repo_;
    path += "<a href=\"";
    path += href;
    path += "\">";
    path += repo_;
    path += "</a>";
    
    string href_last(href+"/edit");
    size_t icounter = 0;
    const size_t path_size = path_.size();
// add remain paths.
    for(auto it = path_.begin();it != path_.end();it++)
    {
        path += "<span>/</span>";
        // path
        href += "/" + *it;
        href_last += "/" + *it;
        path += "<a href=\"";
        if( path_size -1 == icounter++)
        {
            path += href_last;
        }
        else
        {
            path += href;
        }
        path += "\">";
        path += *it;
        path += "</a>";        
    }
    replace.insert(pair<string,string>("$BOW_TMPL_SOURCE_PATH$",path));
}

static string strConstLogin =
    "<a href=\"/login.php\" data-method=\"post\" id=\"login\">login</a>";
static string strConstLogout =
    "<a href=\"/logout.php\" data-method=\"post\" id=\"logout\">logout</a>";
    
    

void SourceView::create_loginout(std::map<std::string,std::string> &replace)
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

    
void SourceView::post(const std::string &data)
{
    std::string source_path(workspace_);
    for(auto it = path_.begin();it != path_.end();it++)
    {
        source_path.append( "/" + *it);
    }
    fs::path sourefile(source_path);
    if( not fs::exists(sourefile) )
    {
        // new files.
    }
    else
    {
        string source_replaced(data);
        html2text(source_replaced);
        std::ofstream ofs;
        ofs.open( source_path );
        ofs << source_replaced;
        ofs.close();
    }
    for_each(_env_commit_commands.begin(),  _env_commit_commands.end(),
             [](const string &cmd)
             {
                 BOW::system_result(cmd);
             }
             );
    for_each(env_build_commands_.begin(),  env_build_commands_.end(),
             [](const string &cmd)
             {
                 BOW::system_result(cmd);
             }
             );
}
bool SourceView::readPostReply(std::string &contents)
{
    read_output(contents);
    return true;
}

