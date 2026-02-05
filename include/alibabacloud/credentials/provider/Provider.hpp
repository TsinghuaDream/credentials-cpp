#ifndef ALIBABACLOUD_CREDENTIALS_PROVIDER_HPP_
#define ALIBABACLOUD_CREDENTIALS_PROVIDER_HPP_

#include <memory>
#include <string>

#include <alibabacloud/credentials/Model.hpp>

namespace AlibabaCloud {
namespace Credentials {
class Provider {
public:
  Provider() = default;
  virtual ~Provider() {}

  virtual Models::CredentialModel &getCredential() = 0;
  virtual const Models::CredentialModel &getCredential() const = 0;
  
  /**
   * @brief Get provider name
   * @return Provider name string
   */
  virtual std::string getProviderName() const = 0;
};
} // namespace Credentials
} // namespace AlibabaCloud
#endif