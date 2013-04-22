//
// connection.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "connection.hpp"
#include <vector>
#include <boost/bind.hpp>
#include "request_handler.hpp"

#include "thread"

//#define DEBUG_IP
#define DEBUG_READ
//#define DEBUG

namespace http {
namespace server_threadpool {

/*
connection::connection(boost::asio::io_service& io_service,
    request_handler& handler)
  : strand_(io_service)
    ,socket_(io_service)
    ,remote_adress_()
    ,request_handler_(handler)
{
}
*/

connection::connection(boost::asio::io_service& io_service,
                        const std::string& doc_root)
: strand_(io_service)
,socket_(io_service)
,remote_adress_()
,doc_root_(doc_root)
{
}

boost::asio::ip::tcp::socket& connection::socket()
{
  return socket_;
}

void connection::start()
{
  remote_adress_ = socket_.remote_endpoint().address().to_string();
#ifdef DEBUG_IP
  std::cout << "remote_adress_" << remote_adress_ << std::endl;
  std::thread::id this_id = std::this_thread::get_id();
  std::cout << "this_id" << this_id << std::endl;
#endif
  socket_.async_read_some(boost::asio::buffer(buffer_),
      strand_.wrap(
        boost::bind(&connection::handle_read, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred)));
}


void connection::handle_read(const boost::system::error_code& e,
    std::size_t bytes_transferred)
{
  if (!e)
  {
    boost::tribool result;
    boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
        request_, buffer_.data(), buffer_.data() + bytes_transferred);
#ifdef DEBUG_READ
	std::cout << "bytes_transferred=" << bytes_transferred << std::endl;
	std::string buff_str(buffer_.data(),bytes_transferred);
	std::cout << buff_str << std::endl;
	std::cout << "result" << result << std::endl;
#endif
    if (result)
    {
      /// The handler used to process the incoming request.
      request_handler request_handler_(doc_root_,remote_adress_);
      request_handler_.handle_request(request_, reply_);
      
      boost::asio::async_write(socket_, reply_.to_buffers(),
          strand_.wrap(
            boost::bind(&connection::handle_write, shared_from_this(),
              boost::asio::placeholders::error)));
    }
    else if (!result)
    {
      reply_ = reply::stock_reply(reply::bad_request);
      boost::asio::async_write(socket_, reply_.to_buffers(),
          strand_.wrap(
            boost::bind(&connection::handle_write, shared_from_this(),
              boost::asio::placeholders::error)));
    }
    else
    {
      socket_.async_read_some(boost::asio::buffer(buffer_),
          strand_.wrap(
            boost::bind(&connection::handle_read, shared_from_this(),
              boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred)));
    }
  }

  // If an error occurs then no new asynchronous operations are started. This
  // means that all shared_ptr references to the connection object will
  // disappear and the object will be destroyed automatically after this
  // handler returns. The connection class's destructor closes the socket.
}

void connection::handle_write(const boost::system::error_code& e)
{
#ifdef DEBUG
  std::cout << e.message() << std::endl;
#endif

  if (!e)
  {
    // Initiate graceful connection closure.
    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }

  // No new asynchronous operations are started. This means that all shared_ptr
  // references to the connection object will disappear and the object will be
  // destroyed automatically after this handler returns. The connection class's
  // destructor closes the socket.
}

} // namespace server_threadpool
} // namespace http
