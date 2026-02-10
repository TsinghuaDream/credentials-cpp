#include <memory>

#include <darabonba/Env.hpp>
#include <alibabacloud/credentials/Exception.hpp>

#include <alibabacloud/credentials/provider/AccessKeyProvider.hpp>
#include <alibabacloud/credentials/provider/EnvironmentVariableProvider.hpp>
#include <alibabacloud/credentials/provider/StsProvider.hpp>

namespace AlibabaCloud {
namespace Credentials {

std::unique_ptr<Provider> EnvironmentVariableProvider::createProvider() {
  const auto accessKeyId =
      Darabonba::Env::getEnv("ALIBABA_CLOUD_ACCESS_KEY_ID");
  const auto accessKeySecret =
      Darabonba::Env::getEnv("ALIBABA_CLOUD_ACCESS_KEY_SECRET");
  if (!accessKeyId.empty() && !accessKeySecret.empty()) {
    const auto securityToken =
        Darabonba::Env::getEnv("ALIBABA_CLOUD_SECURITY_TOKEN");
    if (securityToken.empty()) {
      return std::unique_ptr<Provider>(
          new AccessKeyProvider(accessKeyId, accessKeySecret));
    }
    return std::unique_ptr<Provider>(
        new StsProvider(accessKeyId, accessKeySecret, securityToken));
  }
  if (accessKeyId.empty()) {
    throw CredentialException(std::string("Environment variable ALIBABA_CLOUD_ACCESS_KEY_ID cannot be empty"));
  }
  if (accessKeySecret.empty()) {
    throw CredentialException(std::string("Environment variable ALIBABA_CLOUD_ACCESS_KEY_SECRET cannot be empty"));
  }
  return nullptr;
}
} // namespace Credentials
} // namespace AlibabaCloud