//
// request_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "mime_types.hpp"
#include "reply.hpp"
#include "request.hpp"

#include "resource.hpp"
#include "reply_view.hpp"
#include "navi_view.hpp"
#include "source_view.hpp"
#include "directory_view.hpp"
#include "source_update.hpp"
#include "manual_view.hpp"
#include "redirect_view.hpp"

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string_regex.hpp>

#include <network/uri.hpp>

namespace http {
namespace server_threadpool {


request_handler::request_handler(const std::string& doc_root,const std::string& remote)
  : doc_root_(doc_root)
  ,remote_(remote)
{
}

    
    
//#define DEBUG_REQ
//#define DEBUG_DATA
//#define DEBUG_PATH
//#define DEBUG_REP_DATA
//#define DEBUG_REP_HEADER
//#define DEBUG_RET
//#define DEBUG_POST
#define DEBUG_ROUTE
    
void request_handler::handle_request(const request& req, reply& rep)
{
#ifdef DEBUG_REQ
    std::cout << "req.method=" << req.method << std::endl;
    std::cout << "req.uri=" << req.uri << std::endl;
    std::cout << "req.http_version_major=" << req.http_version_major << std::endl;
    std::cout << "req.http_version_minor=" << req.http_version_minor << std::endl;
    std::cout << "req.headers------*****------" << std::endl;
    for(auto it = req.headers.begin();it != req.headers.end();it++)
    {
        std::cout << "name=<" << it->name << "> value=<"<< it->value << ">" << std::endl;
    }
    std::cout << "------*****------" << std::endl;
#endif
#ifdef DEBUG_DATA
	std::cout << "data=" << req.data << std::endl;
#endif
    for(auto it = req.headers.begin();it != req.headers.end();it++)
    {
        if("X-Real-IP" == it->name)
        {
            remote_ = it->value;
            break;
        }
    }
#ifdef DEBUG_RET
    std::cout << "remote_=" << remote_ << std::endl;
#endif
   
  // Decode url to path.
  std::string request_path;
  if (!url_decode(req.uri, request_path))
  {
    rep = reply::stock_reply(reply::bad_request);
    return;
  }
#ifdef DEBUG_PATH
	std::cout << "req.uri=" << req.uri << std::endl;
	std::cout << "request_path=" << request_path << std::endl;
#endif

    for(auto it= route_.begin();it != route_.end();it++)
    {
        boost::regex ex(it->first);
        if(boost::regex_match(req.uri, ex))
        {
#ifdef DEBUG_ROUTE
            std::cout << "match req.uri=<" << req.uri << ">" << std::endl;
#endif
            it->second.create(req.uri);
            if( "GET" == req.method || "get" == req.method )
            {
                it->second.get();
            }
            if( "POST" == req.method || "post" == req.method )
            {
                it->second.post();
            }
            break;
        }
    }
  // Request path must be absolute and not contain "..".
  if (request_path.empty() || request_path[0] != '/'
      || request_path.find("..") != std::string::npos)
  {
    rep = reply::stock_reply(reply::bad_request);
    return;
  }
  if( "GET" == req.method || "get" == req.method )
  {
      this->handle_get(req,request_path,rep);
  }
  if( "POST" == req.method || "post" == req.method )
  {
      this->handle_post(req,request_path,rep);
  }
#ifdef DEBUG_REP_DATA
    std::cout << "rep.content=" << rep.content << std::endl;
#endif
#ifdef DEBUG_REP_HEADER
    for(auto it = rep.headers.begin();it != rep.headers.end();it++)
    {
        std::cout << "it=" << it->name << " " << it->value << std::endl;
    }
#endif
}


void request_handler::handle_get(const request& req,const std::string &request_path, reply& rep)
{
#ifdef DEBUG_PATH
	std::cout << "request_path=" << request_path << std::endl;
#endif
    boost::regex ex("^/users/.*");
    boost::regex ex_manual("^/manual/.*");
    if(boost::regex_match(request_path, ex))
    {
        std::list<std::string> results;
        boost::split(results, request_path, boost::is_any_of("/"),boost::algorithm::token_compress_on);
#ifdef DEBUG_PATH
        std::cout << "match request_path=" << request_path << std::endl;
        std::cout <<"results.size()=" <<  results.size() << endl;
#endif
        results.pop_front();
        results.pop_front();
        std::string username;
        

        if(results.empty())
        {
            rep = reply::stock_reply(reply::not_found);
            return;
        }
        else
        {
            username = results.front();
            results.pop_front();
        }
        // user id infomation.
        string use_id(username);
        if("guest"==username)
        {
            use_id += "_from_";
            
            use_id += boost::algorithm::replace_all_copy(remote_,".","_");
        }
        std::string category;
        if(results.empty())
        {
            // view all repositories.
            BOW::NaviView navi(username,use_id);
            navi.response(doc_root_, rep);
            return;
        }
        else
        {
            category = results.front();
            results.pop_front();
        }
        std::string repos;
        if(results.empty())
        {
            // view all category repositories.
            if("**?**" == category)
            {
                BOW::RedirectView redirect(username,use_id);
                redirect.redirect(rep);
            }
            else
            {
                BOW::NaviView navi(username,use_id,category);
                navi.response(doc_root_, rep);
            }
            return;
        }
        else
        {
            repos = results.front();
            results.pop_front();
        }
#ifdef DEBUG_PATH
        std::cout <<"username=" <<  username << endl;
        std::cout <<"category=" <<  category << endl;
        std::cout <<"repos=" <<  repos << endl;
#endif
        BOW::SourceView source(username,use_id,category,repos,results);
        source.response(doc_root_, rep);
    }
    // deal with manual.
    else if(boost::regex_match(request_path, ex_manual))
    {
        std::list<std::string> results;
        boost::split(results, request_path, boost::is_any_of("/"),boost::algorithm::token_compress_on);
        BOW::ManualView man(results.back());
        man.response(doc_root_, rep);
    }
    // remain of files such ass css,javascript files.
    else
    {
        // Open the file to send back.
        std::string full_path = doc_root_ + request_path;
        std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
        if (!is)
        {
            rep = reply::stock_reply(reply::not_found);
            return;
        }
    
#ifdef DEBUG_PATH
	std::cout << "request_path=" << request_path << std::endl;
#endif
        // Fill out the reply to be sent to the client.
        rep.status = reply::ok;
        char buf[512];
        while (is.read(buf, sizeof(buf)).gcount() > 0) {
            rep.content.append(buf, is.gcount());
        }
        rep.headers.resize(2);
        rep.headers[0].name = "Content-Length";
        rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
    
        fs::path rq_path(request_path);
        std::string extension = rq_path.extension().string();
        rep.headers[1].name = "Content-Type";
        rep.headers[1].value = mime_types::extension_to_type(extension);
    }
}
    
    
void request_handler::handle_post(const request& req, const std::string &request_path,reply& rep)
{
#ifdef DEBUG_POST
    for( auto it = req.headers.begin(); it != req.headers.end();it++)
    {
        std::cout << "it->name=<" << it->name << ">" << "it->value<" << it->value << ">" << std::endl;
    }
#endif
    boost::regex ex("^/EventAceOnChange/.*");
    if(boost::regex_match(request_path, ex))
    {
        string postpath = boost::algorithm::replace_all_copy(request_path,"/EventAceOnChange/","");
#ifdef DEBUG_POST
        std::cout << "postpath=<" << postpath << ">" << std::endl;
#endif
        std::list<std::string> results;
        boost::split(results, postpath, boost::is_any_of("<>"));
#ifdef DEBUG_POST
        for( auto it = results.begin(); it != results.end();it++)
        {
            std::cout << "it=<" << *it << ">"  << std::endl;
        }
#endif
        results.pop_front();
        string src(results.front());
        results.pop_front();
        results.pop_front();
        string wc(results.front());
#ifdef DEBUG_POST
        std::cout << "src=<" << src << ">" << std::endl;
        std::cout << "wc=<" << wc << ">" << std::endl;
#endif
        BOW::SoureUpdate source(req.data,src,wc);
        source.response(doc_root_, rep);
    }
}
    
    
bool request_handler::url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

} // namespace server_threadpool
} // namespace http
