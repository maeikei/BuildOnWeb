//
// resource.cpp
// ~~~~~~~~~~
//

#include "resource.hpp"
#include "reply_view.hpp"
#include <vector>
#include <thread>
#include <memory>

namespace http {
namespace server_threadpool {
    resource::resource(void)
    {
    }
    void resource::get(std::shared_ptr<ReplyView> view,const std::string &doc_root, reply& rep)
    {
        view->responseGet(doc_root,rep);
    }
    void resource::post(std::shared_ptr<ReplyView> view,const std::string &data, reply& rep)
    {
        view->post(data);
        view->responsePost(rep);
    }
    void resource::put(std::shared_ptr<ReplyView> view,const std::string &doc_root, reply& rep)
    {
    }
    void resource::remove(std::shared_ptr<ReplyView> view,const std::string &doc_root, reply& rep)
    {
    }
} // namespace server_threadpool
} // namespace http
