// route.cpp
#include "reply_view.hpp"
#include "resource.hpp"
#include "source_view.hpp"
#include "request_handler.hpp"
#include "manual_view.hpp"
#include "navi_view.hpp"
#include "socials_view.hpp"
#include "redirect_view.hpp"

#include <string>

namespace http {
namespace server_threadpool {

// route table;
static BOW::NaviApp navi;
static BOW::SourceApp source;
    
const std::map<std::string,resource&>
    request_handler::route_
    {
        {"^/users/\\w+",navi},
        {"^/users/\\w+/",navi},
        {"^/users/\\w+/\\w+",navi},
        {"^/users/\\w+/\\w+/",navi},
        
        {"^/users/\\w+/\\w+/\\w+",source},
        {"^/users/\\w+/\\w+/\\w+/",source},

        {"^/users/\\w+/\\w+/\\w+",source},
        {"^/users/\\w+/\\w+/\\w+/",source},

        {"^/users/\\w+/\\w+/\\w+/.*",source},

    };
} // namespace server_threadpool
} // namespace http
