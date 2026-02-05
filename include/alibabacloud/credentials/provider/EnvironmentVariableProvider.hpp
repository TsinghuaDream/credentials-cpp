#ifndef ALIBABACLOUD_CREDENTIALS_ENVIRONMENTVARIABLEPROVIDER_HPP_
#define ALIBABACLOUD_CREDENTIALS_ENVIRONMENTVARIABLEPROVIDER_HPP_

#include <memory>

#include <alibabacloud/credentials/Exception.hpp>

#include <alibabacloud/credentials/Constant.hpp>
#include <alibabacloud/credentials/Model.hpp>
#include <alibabacloud/credentials/provider/Provider.hpp>
namespace AlibabaCloud {

namespace Credentials {

class EnvironmentVariableProvider : public Provider {
public:
  EnvironmentVariableProvider() = default;
  virtual ~EnvironmentVariableProvider() {}

  virtual Models::CredentialModel &getCredential() override {
    provider_ = createProvider();
    if (provider_ == nullptr) {
      throw CredentialException(std::string("Can't create the ProfileProvider."));
    }
    return provider_->getCredential();
  }

  virtual const Models::CredentialModel &getCredential() const override {
    provider_ = createProvider();
    if (provider_ == nullptr) {
      throw CredentialException(std::string("Can't create the ProfileProvider."));
    }
    return provider_->getCredential();
  }
  
  /**
   * @brief Get provider name
   */
  std::string getProviderName() const override {
    provider_ = createProvider();
    if (provider_ == nullptr) {
      throw CredentialException(std::string("Can't create the ProfileProvider."));
    }
    return provider_->getProviderName();
  }

protected:
  static std::unique_ptr<Provider> createProvider();

  mutable std::unique_ptr<Provider> provider_ = nullptr;
  mutable Models::CredentialModel credential_;
};

} // namespace Credentials
} // namespace AlibabaCloud

#endif
