//
// reply.hpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER3_REPLY_VIEW_HPP
#define HTTP_SERVER3_REPLY_VIEW_HPP

#include <string>
#include <vector>
#include <map>
#include <boost/asio.hpp>
#include "header.hpp"

namespace http {
namespace server_threadpool {

struct reply;

/// A reply of view to be sent to a client.
class ReplyView
{
    typedef void replace_ptr(const std::string &,std::string &);
public:
    ReplyView(void);
    virtual ~ReplyView(){}
    void responseGet(const std::string &doc_root, reply& rep);
    
    virtual void post(const std::string &data){}
    void responsePost(reply& rep);
protected:
    /// for get method
    virtual bool readBody(const std::string &doc_root,std::string &contents);
    virtual std::map<std::string,std::string> fillHeader(void);
    virtual std::map<std::string,std::string> bodyVars(void);
    virtual int status(void);
    /// for post method
    virtual bool readPostReply(std::string &contents);
    virtual std::map<std::string,std::string> fillHeaderPost(void);
    virtual std::map<std::string,std::string> bodyVarsPost(void);
    virtual int statusPost(void);
private:
    // replace commont of reply.
    void replace_basic(reply& rep);
};

} // namespace server_threadpool
} // namespace http

#endif // HTTP_SERVER3_REPLY_VIEW_HPP
