//
// request_handler.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER3_REQUEST_HANDLER_HPP
#define HTTP_SERVER3_REQUEST_HANDLER_HPP

#include <string>
#include <boost/noncopyable.hpp>
#include <vector>

namespace http {
namespace server_threadpool {

struct reply;
struct request;
class resource;

struct routepair
{
    std::string first;
    resource& second;
};

/// The common handler for all incoming requests.
class request_handler
  : private boost::noncopyable
{
public:
  /// Construct with a directory containing files to be served.
  explicit request_handler(const std::string& doc_root,const std::string& remote);

  /// Handle a request and produce a reply.
  void handle_request(const request& req, reply& rep);

    
    const std::string& get_doc_root() const { return doc_root_;}
    const std::string& get_remote() const { return remote_;}
private:
  /// The directory containing the files to be served.
  std::string doc_root_;
  /// client ip address 
  std::string remote_;
  /// route map to apps.
  static const std::vector<routepair> route_;

  /// Perform URL-decoding on a string. Returns false if the encoding was
  /// invalid.
  static bool url_decode(const std::string& in, std::string& out);
};

} // namespace server_threadpool
} // namespace http

#endif // HTTP_SERVERserver_threadpool_REQUEST_HANDLER_HPP
