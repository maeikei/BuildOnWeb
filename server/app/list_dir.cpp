#include "list_dir.hpp"
using namespace BuildOnWeb;
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>

// #define DEBUG_REPLACE

#include "reply.hpp"
#include "request.hpp"

BOWListDir::BOWListDir(string & dir)
:_dir(dir)
{
    
}
void BOWListDir::response(const http::server_threadpool::request& req, http::server_threadpool::reply& rep)
{
}
