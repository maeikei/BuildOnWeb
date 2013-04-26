//
// resource.hpp
// ~~~~~~~~~~
//
//

#ifndef HTTP_SERVER3_RESOURCE_HPP
#define HTTP_SERVER3_RESOURCE_HPP

#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <network/uri.hpp>

namespace http {
namespace server_threadpool {

class resource
  : private boost::noncopyable
{
public:
    resource(const std::string &s);
    void get(void);
    void post(void);
    void put(void);
    void remove(void);
};

} // namespace server_threadpool
} // namespace http

#endif // HTTP_SERVER3_RESOURCE_HPP
