// route.cpp
#include "reply_view.hpp"
#include "resource.hpp"
#include "default_view.hpp"

#include "request_handler.hpp"
using namespace http::server_threadpool;
#include <string>

// route table;
static BOW::DefaultApp default_app;


/// pay atention about the order.
/// loop will break at first match.
const std::vector<routepair>
    request_handler::route_
    {
        {".*",default_app},

    };
