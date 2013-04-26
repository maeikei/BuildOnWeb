// route.cpp
#include "reply_view.hpp"
#include "resource.hpp"
#include "source_view.hpp"
#include "manual_view.hpp"
#include "navi_view.hpp"
#include "socials_view.hpp"
#include "redirect_view.hpp"
#include "default_view.hpp"
#include "directory_view.hpp"
#include "repository_view.hpp"

#include "request_handler.hpp"
using namespace http::server_threadpool;
#include <string>

// route table;
static BOW::DefaultApp default_app;
static BOW::NaviApp navi;
static BOW::RepositoryApp repo;
static BOW::DirecoryApp dir;
static BOW::SourceApp source;
static BOW::ManualApp man;
static BOW::RedirectApp redirect;
static BOW::SosialApp social;

const std::map<std::string,resource&>
    request_handler::route_
    {
        {"^/users/\\w+/\\*\\*\\?\\*\\*",redirect},

        {"^/users/\\w+",navi},
        {"^/users/\\w+/",navi},
        {"^/users/\\w+/\\w+",navi},
        {"^/users/\\w+/\\w+/",navi},

        
        {"^/users/\\w+/\\w+/\\w+/edit/.*",source},
        {"/\\*\\*\\?socials\\?\\*\\*?",social},
        
        {"^/users/\\w+/\\w+/\\w+",repo},
        {"^/users/\\w+/\\w+/\\w+/",repo},
        {"^/users/\\w+/\\w+/\\w+/.*",dir},
        
        {"^/manual/.*",man},

        {".*",default_app},

    };
