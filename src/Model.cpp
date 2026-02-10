#include <alibabacloud/credentials/AuthUtil.hpp>
#include <alibabacloud/credentials/Model.hpp>

namespace AlibabaCloud {
namespace Credentials {
namespace Models {

std::string Config::getRoleSessionName() const {
  if (roleSessionName_ != nullptr) {
    return *roleSessionName_;
  }
  // Dynamic default: credentials-cpp-{timestamp}
  return AuthUtil::generateSessionName();
}

} // namespace Models
} // namespace Credentials
} // namespace AlibabaCloud
