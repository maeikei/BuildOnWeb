//
// route.cpp
#include "request_handler.hpp"
#include <resource.hpp>
#include <string>

namespace http {
namespace server_threadpool {

class navi : public resource {
public:
    explicit navi(const std::string &uri)
    :resource(uri)
    {}
};

class source : public resource {
public:
    explicit source(const std::string &uri)
    :resource(uri)
    {}
};

// route table;
static navi a("");
static source b("");

const std::map<std::string,resource*>
    request_handler::route_
    {
        {"^/users/\\w+",&a},
        {"^/users/\\w+/",&a},
        {"^/users/\\w+/\\w+",&a},
        {"^/users/\\w+/\\w+/",&a},
        
        {"^/users/\\w+/\\w+/\\w+",&b},
        {"^/users/\\w+/\\w+/\\w+/",&b},

        {"^/users/\\w+/\\w+/\\w+",&b},
        {"^/users/\\w+/\\w+/\\w+/",&b},
    };
} // namespace server_threadpool
} // namespace http
