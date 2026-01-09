#include <darabonba/Core.hpp>

#include <alibabacloud/credential/AuthUtil.hpp>
#include <alibabacloud/credential/provider/URLProvider.hpp>

namespace AlibabaCloud {
namespace Credential {
bool URLProvider::refreshCredential() const {
  // 使用 getNewRequest 创建带 User-Agent 的请求（对应 Python SDK）
  auto req = AuthUtil::getNewRequest(url_);
  // Use saved timeout configuration
  Darabonba::RuntimeOptions runtime;
  runtime.setConnectTimeout(connectTimeout_);
  runtime.setReadTimeout(readTimeout_);
  auto future = Darabonba::Core::doAction(req, runtime);
  auto resp = future.get();
  if (resp->getStatusCode() != 200) {
    throw Darabonba::Exception(Darabonba::Stream::readAsString(resp->getBody()));
  }
  const auto &result = Darabonba::Stream::readAsJSON(resp->getBody());
  if (result["Code"].get<std::string>() != "Success") {
    throw Darabonba::Exception(result.dump());
  }
  this->expiration_ = strtotime(result["Expiration"].get<std::string>());
  credential_.setAccessKeyId(result["AccessKeyId"].get<std::string>())
      .setAccessKeySecret(result["AccessKeySecret"].get<std::string>())
      .setSecurityToken(result["SecurityToken"].get<std::string>());
  return true;
}

} // namespace Credential
} // namespace AlibabaCloud