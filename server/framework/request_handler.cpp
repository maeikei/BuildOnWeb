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
#include "manual_view.hpp"
#include "redirect_view.hpp"

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string_regex.hpp>

//#include <network/uri.hpp>

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
//#define DEBUG_ROUTE
    
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
// Request path must be absolute and not contain "..".
  if (request_path.empty() || request_path[0] != '/'
        || request_path.find("..") != std::string::npos)
  {
      rep = reply::stock_reply(reply::bad_request);
      return;
  }
    for(auto it= route_.begin();it != route_.end();it++)
    {
#ifdef DEBUG_ROUTE
        std::cout << "try req.uri=<" << req.uri << ">" << std::endl;
#endif
        boost::regex ex(it->first);
        if(boost::regex_match(req.uri, ex))
        {
#ifdef DEBUG_ROUTE
            std::cout << "match req.uri=<" << req.uri << ">" << std::endl;
            std::cout << "match app name =<" << typeid(it->second).name() << ">" << std::endl;
#endif
            it->second.create(req.uri,remote_);
            if( "GET" == req.method || "get" == req.method )
            {
                it->second.get(doc_root_, rep);
            }
            if( "POST" == req.method || "post" == req.method )
            {
                it->second.post(req.data, rep);
            }
            break;
        }
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
