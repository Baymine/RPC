#include <google/protobuf/service.h>
#include <memory>
#include "tinyrpc/net/http/http_dispatcher.h"
#include "tinyrpc/net/http/http_request.h"
#include "tinyrpc/net/http/http_servlet.h"
#include "tinyrpc/comm/log.h"
#include "tinyrpc/comm/msg_req.h"


namespace tinyrpc {

void HttpDispacther::dispatch(AbstractData* data, TcpConnection* conn) {
  HttpRequest* request = dynamic_cast<HttpRequest*>(data);
  // TODO: cast may fail, the request may be a nullptr


  HttpResponse response;

  // Set message number to keep track of the request
  Coroutine::GetCurrentCoroutine()->getRunTime()->m_msg_no = MsgReqUtil::genMsgNumber();
  setCurrentRunTime(Coroutine::GetCurrentCoroutine()->getRunTime());

  InfoLog << "begin to dispatch client http request, msgno=" << Coroutine::GetCurrentCoroutine()->getRunTime()->m_msg_no;

  std::string url_path = request->m_request_path;
  if (!url_path.empty()) {
    auto it = m_servlets.find(url_path);
    // TODO: using Trie

    if (it == m_servlets.end()) {
      ErrorLog << "404, url path{ " << url_path << "}, msgno=" << Coroutine::GetCurrentCoroutine()->getRunTime()->m_msg_no;
      NotFoundHttpServlet servlet;
      Coroutine::GetCurrentCoroutine()->getRunTime()->m_interface_name = servlet.getServletName();
      servlet.setCommParam(request, &response);
      servlet.handle(request, &response);
    } else {

      Coroutine::GetCurrentCoroutine()->getRunTime()->m_interface_name = it->second->getServletName();
      it->second->setCommParam(request, &response);
      it->second->handle(request, &response);  // 这里的servlet是自定义的
    }
  }

  conn->getCodec()->encode(conn->getOutBuffer(), &response);  // 一些字符串拼接（拼接响应头）

  InfoLog << "end dispatch client http request, msgno=" << Coroutine::GetCurrentCoroutine()->getRunTime()->m_msg_no;

}

void HttpDispacther::registerServlet(const std::string& path, HttpServlet::ptr servlet) {
  auto it = m_servlets.find(path);
  if (it == m_servlets.end()) {
    DebugLog << "register servlet success to path {" << path << "}";
    m_servlets[path] = servlet;
  } else {
    ErrorLog << "failed to register, beacuse path {" << path << "} has already register sertlet";
  }
}




}