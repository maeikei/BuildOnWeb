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

struct reply ;
struct request;
/// A reply of view to be sent to a client.
class ReplyView
{
public:
    ReplyView(void);
    void response(const request& req, reply& rep);
private:
    
};

} // namespace server_threadpool
} // namespace http

#endif // HTTP_SERVER3_REPLY_VIEW_HPP
