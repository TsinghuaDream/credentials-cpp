#ifndef ALIBABACLOUD_CREDENTIALS_DEFAULTPROVIDER_HPP_
#define ALIBABACLOUD_CREDENTIALS_DEFAULTPROVIDER_HPP_

#include <memory>
#include <string>

#include <alibabacloud/credentials/Exception.hpp>

#include <alibabacloud/credentials/Model.hpp>
#include <alibabacloud/credentials/provider/Provider.hpp>

namespace AlibabaCloud {
namespace Credentials {
class DefaultProvider : public Provider {
public:
  DefaultProvider();
  explicit DefaultProvider(std::shared_ptr<Models::Config> config);

  virtual ~DefaultProvider() = default;

  virtual Models::CredentialModel &getCredential() override {
    // If reuse enabled and we have a cached successful provider
    if (reuseLastProviderEnabled_ && lastSuccessfulProvider_) {
      try {
        return lastSuccessfulProvider_->getCredential();
      } catch (CredentialException&) {
        // Cache failed, continue trying all providers
        lastSuccessfulProvider_ = nullptr;
      }
    }

    for (auto &provider : providers_) {
      if (provider) {
        try {
          auto& credential = provider->getCredential();
          if (reuseLastProviderEnabled_) {
            lastSuccessfulProvider_ = provider.get();
          }
          return credential;
        } catch (CredentialException& e) {
          continue;
        }
      }
    }
    throw CredentialException(std::string("Can't get the credential."));
  }
  
  virtual const Models::CredentialModel &getCredential() const override {
    // If reuse enabled and we have a cached successful provider
    if (reuseLastProviderEnabled_ && lastSuccessfulProvider_) {
      try {
        return lastSuccessfulProvider_->getCredential();
      } catch (CredentialException&) {
        // Cache failed, continue trying all providers
        lastSuccessfulProvider_ = nullptr;
      }
    }

    for (auto &provider : providers_) {
      if (provider) {
        try {
          auto& credential = provider->getCredential();
          if (reuseLastProviderEnabled_) {
            lastSuccessfulProvider_ = provider.get();
          }
          return credential;
        } catch (CredentialException& e) {
          continue;
        }
      }
    }
    throw CredentialException(std::string("Can't get the credential."));
  }
  
  /**
   * @brief Get provider name
   */
  std::string getProviderName() const override {
    // If reuse enabled and we have a cached successful provider
    if (reuseLastProviderEnabled_ && lastSuccessfulProvider_) {
      try {
        return lastSuccessfulProvider_->getProviderName();
      } catch (CredentialException&) {
        // Cache failed, continue trying all providers
        lastSuccessfulProvider_ = nullptr;
      }
    }

    for (auto &provider : providers_) {
      if (provider) {
        try {
          auto name = provider->getProviderName();
          if (reuseLastProviderEnabled_) {
            lastSuccessfulProvider_ = provider.get();
          }
          return name;
        } catch (CredentialException& e) {
          continue;
        }
      }
    }
    throw CredentialException(std::string("Can't get the provider name."));
  }

protected:
  std::vector<std::unique_ptr<Provider>> providers_;
  bool reuseLastProviderEnabled_ = false;
  mutable Provider* lastSuccessfulProvider_ = nullptr;
};
} // namespace Credentials

} // namespace AlibabaCloud
#endif