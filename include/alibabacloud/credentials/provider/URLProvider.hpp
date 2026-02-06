#ifndef ALIBABACLOUD_CREDENTIALS_URLPROVIDER_HPP_
#define ALIBABACLOUD_CREDENTIALS_URLPROVIDER_HPP_

#include <string>

#include <alibabacloud/credentials/Exception.hpp>
#include <alibabacloud/credentials/Constant.hpp>
#include <alibabacloud/credentials/Model.hpp>
#include <alibabacloud/credentials/provider/NeedFreshProvider.hpp>

// Disable C4251 warning for STL members in exported classes
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace AlibabaCloud {
namespace Credentials {

class ALIBABACLOUD_CREDENTIALS_EXPORT URLProvider : public NeedFreshProvider,
                    std::enable_shared_from_this<URLProvider> {
public:

  URLProvider(std::shared_ptr<Models::Config> config) : url_(config->getCredentialsURL()),
      connectTimeout_(config->hasConnectTimeout() ? config->getConnectTimeout() : 10000),
      readTimeout_(config->hasTimeout() ? config->getTimeout() : 5000) {
    credential_.setType(Constant::URL_STS);
  }

  URLProvider(const std::string &url) : url_(url) {
    if (url.empty()) {
      throw CredentialException(std::string("URL cannot be empty"));
    }
    credential_.setType(Constant::URL_STS);
  }


  virtual ~URLProvider() {}
  
  /**
   * @brief Get provider name
   */
  std::string getProviderName() const override { return Constant::URL_STS; }

protected:
  virtual bool refreshCredential() const override;

  std::string url_;
  mutable Models::CredentialModel credential_;
  int64_t connectTimeout_ = 10000;  // Connection timeout in milliseconds
  int64_t readTimeout_ = 5000;      // Read timeout in milliseconds
};

} // namespace Credentials

} // namespace AlibabaCloud

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif