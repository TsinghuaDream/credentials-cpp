#include <darabonba/Core.hpp>

#include <alibabacloud/credentials/AuthUtil.hpp>
#include <alibabacloud/credentials/Exception.hpp>
#include <alibabacloud/credentials/provider/URLProvider.hpp>

namespace AlibabaCloud {
namespace Credentials {

URLProvider::URLProvider(std::shared_ptr<Models::Config> config,
                         StaleValueBehavior behavior,
                         std::shared_ptr<PrefetchStrategy> strategy)
    : RefreshableProvider(behavior, strategy),
      url_(config->getCredentialsURL()),
      connectTimeout_(config->hasConnectTimeout() ? config->getConnectTimeout() : 10000),
      readTimeout_(config->hasTimeout() ? config->getTimeout() : 5000) {
  if (url_.empty()) {
    throw CredentialException(std::string("URL cannot be empty."));
  }
}

URLProvider::URLProvider(const std::string &url,
                         StaleValueBehavior behavior,
                         std::shared_ptr<PrefetchStrategy> strategy)
    : RefreshableProvider(behavior, strategy),
      url_(url) {
  if (url.empty()) {
    throw CredentialException(std::string("URL cannot be empty."));
  }
}

int64_t URLProvider::getStaleTime(int64_t expiration) const {
  if (expiration <= 0) {
    return getCurrentTime() + 60 * 60;  // Default 1 hour if no expiration
  }
  return expiration - STALE_TIME_WINDOW;  // expiration - 15 minutes
}

RefreshResult URLProvider::doRefresh() const {
  // 使用 getNewRequest 创建带 User-Agent 的请求（对应 Python SDK）
  auto req = AuthUtil::getNewRequest(url_);
  // Use saved timeout configuration
  Darabonba::RuntimeOptions runtime;
  runtime.setConnectTimeout(connectTimeout_);
  runtime.setReadTimeout(readTimeout_);
  auto future = Darabonba::Core::doAction(req, runtime);
  auto resp = future.get();
  if (resp->getStatusCode() != 200) {
    throw CredentialException(Darabonba::Stream::readAsString(resp->getBody()));
  }
  const auto &result = Darabonba::Stream::readAsJSON(resp->getBody());
  if (result["Code"].get<std::string>() != "Success") {
    throw CredentialException(result.dump());
  }
  int64_t expiration = strtotime(result["Expiration"].get<std::string>());

  // Build credential model
  Models::CredentialModel cred;
  cred.setAccessKeyId(result["AccessKeyId"].get<std::string>())
      .setAccessKeySecret(result["AccessKeySecret"].get<std::string>())
      .setSecurityToken(result["SecurityToken"].get<std::string>())
      .setType(Constant::URL_STS)
      .setProviderName(getProviderName());

  // Calculate stale time (expiration - 15 minutes)
  int64_t staleTime = getStaleTime(expiration);
  // Calculate prefetch time (stale time - 180 seconds)
  int64_t prefetchTime = staleTime - PREFETCH_THRESHOLD;

  return RefreshResult(cred, staleTime, prefetchTime);
}

} // namespace Credentials
} // namespace AlibabaCloud