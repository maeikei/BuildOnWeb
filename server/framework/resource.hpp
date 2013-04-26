//
// resource.hpp
// ~~~~~~~~~~
//
//

#ifndef HTTP_SERVER3_RESOURCE_HPP
#define HTTP_SERVER3_RESOURCE_HPP

#include <string>

namespace http {
namespace server_threadpool {

class resource
{
public:
    resource(void);
    virtual ~resource(){}
    virtual void create(const std::string &uri) = 0;
//http method
    virtual void get(void) = 0;
    virtual void post(void) = 0;
    virtual void put(void) = 0;
    virtual void remove(void) = 0;
};

} // namespace server_threadpool
} // namespace http

#endif // HTTP_SERVER3_RESOURCE_HPP
