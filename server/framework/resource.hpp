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
    void create(const std::string &uri);
//http method
    void get(void);
    void post(void);
    void put(void);
    void remove(void);
};

} // namespace server_threadpool
} // namespace http

#endif // HTTP_SERVER3_RESOURCE_HPP
