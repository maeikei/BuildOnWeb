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
#include <boost/asio.hpp>
#include "header.hpp"

namespace http {
namespace server_threadpool {

struct reply;
/// A reply of view to be sent to a client.
class ReplyView
{
public:
    ReplyView(void);
    virtual ~ReplyView(){}

    void response(const std::string &doc_root, reply& rep);
    virtual bool getContent(const std::string &doc_root,std::string &contents){ return true;};
    void redirect(reply& rep);
    virtual void getDist(std::string &dist){};
protected:
private:
    // replace commont of reply.
    void replace(reply& rep);    
};

} // namespace server_threadpool
} // namespace http

#endif // HTTP_SERVER3_REPLY_VIEW_HPP
