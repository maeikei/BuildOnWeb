//
// resource.hpp
// ~~~~~~~~~~
//
//

#ifndef HTTP_SERVER3_RESOURCE_HPP
#define HTTP_SERVER3_RESOURCE_HPP

#include <string>
#include <memory>

namespace http {
namespace server_threadpool {

struct reply;
class ReplyView;

class resource
{
public:
    resource(void);
    virtual ~resource(){}
    virtual void create(const std::string &uri,const std::string &user_uid) = 0;
//http method
    virtual void get(const std::string &doc_root, reply& rep){}
    virtual void post(const std::string &data, reply& rep){}
    virtual void put(const std::string &doc_root, reply& rep){}
    virtual void remove(const std::string &doc_root, reply& rep){}
protected:
    std::shared_ptr<ReplyView> reply_;
};

} // namespace server_threadpool
} // namespace http

#endif // HTTP_SERVER3_RESOURCE_HPP
