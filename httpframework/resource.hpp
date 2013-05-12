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
    virtual std::shared_ptr<ReplyView> create(const std::string &uri,const std::string &user_uid) = 0;
//http method
    void get(std::shared_ptr<ReplyView>,const std::string &doc_root, reply& rep);
    void post(std::shared_ptr<ReplyView>,const std::string &data, reply& rep);
    void put(std::shared_ptr<ReplyView>,const std::string &doc_root, reply& rep);
    void remove(std::shared_ptr<ReplyView>,const std::string &doc_root, reply& rep);
};

} // namespace server_threadpool
} // namespace http

#endif // HTTP_SERVER3_RESOURCE_HPP
